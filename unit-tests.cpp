#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <maxminddb.h>

// By setting MKMOCK_HOOK_ENABLE and including mkmock.hpp we cause mkcurl.hpp
// to compile with mocking enabled so that we can run unit tests.
#define MKMOCK_HOOK_ENABLE
#include "mkmock.hpp"

MKMOCK_DEFINE_HOOK(MMDB_lookup_string_mmdb_error, int);
MKMOCK_DEFINE_HOOK(finish_lookup_cc_check, bool);
MKMOCK_DEFINE_HOOK(finish_lookup_asn_check, bool);
MKMOCK_DEFINE_HOOK(finish_lookup_org_check, bool);

// Include mkmmdb implementation
// -----------------------------

#define MKMMDB_INLINE_IMPL
#include "mkmmdb.hpp"

TEST_CASE("When MMDB_lookup_string fails with mmdb_error") {
  MKMOCK_WITH_ENABLED_HOOK(
      MMDB_lookup_string_mmdb_error, MMDB_CORRUPT_SEARCH_TREE_ERROR,
      {
        std::vector<std::string> logs;
        mk::mmdb::Handle handle;
        std::string asn;
        REQUIRE(handle.open("asn.mmdb", logs) == true);
        REQUIRE(handle.lookup_asn("8.8.8.8", asn, logs) == false);
      });
}

TEST_CASE("When MMDB_get_value fails") {
  std::vector<std::string> logs;
  MMDB_entry_data_s data{};
  REQUIRE(mk::mmdb::MMDB_get_value_check(
              MMDB_CORRUPT_SEARCH_TREE_ERROR, data, MMDB_DATA_TYPE_UTF8_STRING,
              logs) == false);
}

TEST_CASE("When MMDB_get_value does not contain data") {
  std::vector<std::string> logs;
  MMDB_entry_data_s data{};
  REQUIRE(mk::mmdb::MMDB_get_value_check(
              MMDB_SUCCESS, data, MMDB_DATA_TYPE_UTF8_STRING, logs) == false);
}

TEST_CASE("When MMDB_get_value does not contain the right type") {
  std::vector<std::string> logs;
  MMDB_entry_data_s data{};
  data.has_data = true;
  data.type = MMDB_DATA_TYPE_UINT32;
  REQUIRE(mk::mmdb::MMDB_get_value_check(
              MMDB_SUCCESS, data, MMDB_DATA_TYPE_UTF8_STRING, logs) == false);
}

TEST_CASE("When lookup_cc fails because MMDB provides us a bad value") {
  MKMOCK_WITH_ENABLED_HOOK(
      finish_lookup_cc_check, false,
      {
        std::vector<std::string> logs;
        mk::mmdb::Handle handle;
        std::string cc;
        REQUIRE(handle.open("country.mmdb", logs) == true);
        REQUIRE(handle.lookup_cc("8.8.8.8", cc, logs) == false);
      });
}

TEST_CASE("When lookup_asn fails because MMDB provides us a bad value") {
  MKMOCK_WITH_ENABLED_HOOK(
      finish_lookup_asn_check, false,
      {
        std::vector<std::string> logs;
        mk::mmdb::Handle handle;
        std::string asn;
        REQUIRE(handle.open("asn.mmdb", logs) == true);
        REQUIRE(handle.lookup_asn("8.8.8.8", asn, logs) == false);
      });
}

TEST_CASE("When lookup_org fails because MMDB provides us a bad value") {
  MKMOCK_WITH_ENABLED_HOOK(
      finish_lookup_org_check, false,
      {
        std::vector<std::string> logs;
        mk::mmdb::Handle handle;
        std::string org;
        REQUIRE(handle.open("asn.mmdb", logs) == true);
        REQUIRE(handle.lookup_org("8.8.8.8", org, logs) == false);
      });
}
