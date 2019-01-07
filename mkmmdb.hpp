// Part of Measurement Kit <https://measurement-kit.github.io/>.
// Measurement Kit is free software under the BSD license. See AUTHORS
// and LICENSE for more information on the copying conditions.
#ifndef MEASUREMENT_KIT_MKMMDB_HPP
#define MEASUREMENT_KIT_MKMMDB_HPP

#include <memory>
#include <string>
#include <vector>

namespace mk {
namespace mmdb {

/// Handle is a MMDB database handle.
class Handle {
 public:
  /// Handle creates and empty database handle.
  Handle() noexcept;

  /// Handle is the deleted copy constructor.
  Handle(const Handle &) noexcept = delete;

  /// operator= is the deleted copy assignment.
  Handle &operator=(const Handle &) noexcept = delete;

  /// Handle is the deleted move constructor.
  Handle(Handle &&) noexcept = delete;

  /// operator= is the deleted move assignment.
  Handle &operator=(Handle &&) noexcept = delete;

  /// ~Handle is the destructor. If a call to open was successful, this
  /// function will close the internal database handle.
  ~Handle() noexcept;

  /// open opens the database at @p path. Returns true on success and false
  /// on failure. In the latter case, @p logs contains error logs. Calling
  /// open after a successful call to open will not leak resources, because
  /// the internal database handle will be closed first.
  bool open(const std::string &path, std::vector<std::string> &logs) noexcept;

  /// lookup_cc looks the country code (CC) bound to @p ip with the internal
  /// database handle. On success, true is returned and @p cc will contain
  /// the country code. On failure, false is returned and @p logs will contain
  /// an explaination of the error that occurred.
  bool lookup_cc(const std::string &ip, std::string &cc,
                 std::vector<std::string> &logs) noexcept;

  /// lookup_asn is like lookup_cc except that it looks up the autonomous
  /// system number (ASN), which will be saved into the @p cc string.
  bool lookup_asn(const std::string &ip, std::string &asn,
                  std::vector<std::string> &logs) noexcept;

  /// lookup_asn is like lookup_cc except that it looks up the autonomous
  /// system organization anem, which will be saved into @p org.
  bool lookup_org(const std::string &ip, std::string &org,
                  std::vector<std::string> &logs) noexcept;

 private:
  // Impl contains the implementation internals.
  class Impl;

  // impl is a unique pointer to the private implementation.
  std::unique_ptr<Impl> impl;
};

}  // namespace mmdb
}  // namespace mk

// By default the implementation is not included. You can force it being
// included by providing the following definition to the compiler.
//
// If you're just into understanding the API, you can stop reading here.
#ifdef MKMMDB_INLINE_IMPL

#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <netdb.h>
#endif

#include <assert.h>

#include <functional>
#include <sstream>

#include <maxminddb.h>

#include "mkmock.hpp"

namespace mk {
namespace mmdb {

// MMDB_s_deleter is a deleter for a MMDB_s pointer.
struct MMDB_s_deleter {
  void operator()(MMDB_s *p) {
    MMDB_close(p);
    delete p;
  }
};

// MMDB_s_uptr is a unique pointer to MMDB_s.
using MMDB_s_uptr = std::unique_ptr<MMDB_s, MMDB_s_deleter>;

// Handle::Impl contains Handle's internals.
class Handle::Impl {
 public:
  // db is a unique pointer to the real database instance.
  MMDB_s_uptr db;

  // lookup contains code common to all lookup methods. @param ip is the IP
  // address to lookup for. @param logs is the place where to add logs. @param
  // fun is the function containing code specific to a given lookup (e.g.
  // ASN). Returns true on success and false on failure.
  bool lookup(
      const std::string &ip, std::vector<std::string> &logs,
      std::function<bool(MMDB_entry_s *)> fun);
};

Handle::Handle() noexcept { impl.reset(new Handle::Impl); }

Handle::~Handle() noexcept {}

// MKMMDB_LOG appends a line made of @p Statements to @p Logs.
#define MKMMDB_LOG(Logs, Statements) \
  do {                               \
    std::stringstream ss;            \
    ss << Statements << std::endl;   \
    Logs.push_back(ss.str());        \
  } while (0)

bool Handle::open(
    const std::string &path, std::vector<std::string> &logs) noexcept {
  impl->db.reset(new MMDB_s);  // This close preexisting databases (if any)
  int ret = MMDB_open(path.c_str(), MMDB_MODE_MMAP, impl->db.get());
  MKMOCK_HOOK(mmdb_open, ret);
  if (ret != 0) {
    MKMMDB_LOG(logs, "mmdb_open('" << path << "'): " << MMDB_strerror(ret));
    impl->db.reset();  // Close the just openned handle
    return false;
  }
  return true;
}

bool Handle::Impl::lookup(
    const std::string &ip, std::vector<std::string> &logs,
    std::function<bool(MMDB_entry_s *)> fun) {
  if (db == nullptr) {
    MKMMDB_LOG(logs, "Cannot lookup: the database is not open.");
    return false;
  }
  auto gai_error = 0;
  auto mmdb_error = 0;
  auto record = MMDB_lookup_string(db.get(), ip.c_str(),
                                   &gai_error, &mmdb_error);
  MKMOCK_HOOK(MMDB_lookup_string_gai_error, gai_error);
  MKMOCK_HOOK(MMDB_lookup_string_mmdb_error, mmdb_error);
  MKMOCK_HOOK(MMDB_lookup_string_record, record);
  if (gai_error != 0) {
    MKMMDB_LOG(logs, "MMDB_lookup_string: " << gai_strerror(gai_error));
    return false;
  }
  if (mmdb_error != 0) {
    MKMMDB_LOG(logs, "MMDB_lookup_string: " << MMDB_strerror(mmdb_error));
    return false;
  }
  if (!record.found_entry) {
    MKMMDB_LOG(logs, "MMDB_lookup_string: " << ip << " not found.");
    return false;
  }
  return fun(&record.entry);  // OK to throw (and abort) if function not set
}

bool Handle::lookup_cc(const std::string &ip, std::string &cc,
                       std::vector<std::string> &logs) noexcept {
  return impl->lookup(
      ip, logs, [&](MMDB_entry_s *entry) {
        assert(!!entry);
        MMDB_entry_data_s data{};
        auto mmdb_error = MMDB_get_value(
            entry, &data, "registered_country", "iso_code", nullptr);
        MKMOCK_HOOK(MMDB_get_value_registered_country_iso_code, mmdb_error);
        if (mmdb_error != 0) {
          MKMMDB_LOG(logs, "MMDB_get_value: " << MMDB_strerror(mmdb_error));
          return false;
        }
        if (!data.has_data) {
          MKMMDB_LOG(logs, "MMDB_get_value: no data for entry.");
          return false;
        }
        if (data.type != MMDB_DATA_TYPE_UTF8_STRING) {
          MKMMDB_LOG(logs, "MMDB_get_value: unexpected data type.");
          return false;
        }
        cc = std::string{data.utf8_string, data.data_size};
        return true;
      });
}

bool Handle::lookup_asn(const std::string &ip, std::string &asn,
                        std::vector<std::string> &logs) noexcept {
  return impl->lookup(
      ip, logs, [&](MMDB_entry_s *entry) {
        assert(!!entry);
        MMDB_entry_data_s data{};
        auto mmdb_error = MMDB_get_value(
            entry, &data, "autonomous_system_number", nullptr);
        MKMOCK_HOOK(MMDB_get_value_autonomous_system_number, mmdb_error);
        if (mmdb_error != 0) {
          MKMMDB_LOG(logs, "MMDB_get_value: " << MMDB_strerror(mmdb_error));
          return false;
        }
        if (!data.has_data) {
          MKMMDB_LOG(logs, "MMDB_get_value: no data for entry.");
          return false;
        }
        if (data.type != MMDB_DATA_TYPE_UINT32) {
          MKMMDB_LOG(logs, "MMDB_get_value: unexpected data type.");
          return false;
        }
        asn = std::to_string(data.uint32);
        return true;
      });
}

bool Handle::lookup_org(const std::string &ip, std::string &org,
                        std::vector<std::string> &logs) noexcept {
  return impl->lookup(
      ip, logs, [&](MMDB_entry_s *entry) {
        assert(!!entry);
        MMDB_entry_data_s data{};
        auto mmdb_error = MMDB_get_value(
            entry, &data, "autonomous_system_organization", nullptr);
        MKMOCK_HOOK(
            MMDB_get_value_autonomous_system_organization, mmdb_error);
        if (mmdb_error != 0) {
          MKMMDB_LOG(logs, "MMDB_get_value: " << MMDB_strerror(mmdb_error));
          return false;
        }
        if (!data.has_data) {
          MKMMDB_LOG(logs, "MMDB_get_value: no data for entry.");
          return false;
        }
        if (data.type != MMDB_DATA_TYPE_UTF8_STRING) {
          MKMMDB_LOG(logs, "MMDB_get_value: unexpected data type.");
          return false;
        }
        org = std::string{data.utf8_string, data.data_size};
        return true;
      });
}

}  // namespace mmdb
}  // namespace mk
#endif  // MKMMDB_INLINE_IMPL
#endif  // MEASUREMENT_KIT_MKMMDB_HPP
