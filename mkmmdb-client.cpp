#include <string.h>

#include <iostream>
#include <ios>

#include "mkmmdb.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#endif  // __clang__
#include "argh.h"
#ifdef __clang__
#pragma clang diagnostic pop
#endif  // __clang__

// LCOV_EXCL_START
static void usage() {
  std::clog << "Usage: mkmmdb-client [-k] [--asn-db <path>] "
            << "[--country-db <path>] <ip>" << std::endl;
}
// LCOV_EXCL_STOP

int main(int, char **argv) {
  std::string asn_db = "asn.mmdb";
  std::string country_db = "country.mmdb";
  bool keep_going = false;
  std::string ip;
  {
    argh::parser cmdline;
    cmdline.add_param("asn-db");
    cmdline.add_param("country-db");
    cmdline.parse(argv);
    for (auto &flag : cmdline.flags()) {
      if (flag == "k") {
        keep_going = true;
      } else {
        // LCOV_EXCL_START
        std::clog << "fatal: unrecognized flag: " << flag << std::endl;
        usage();
        exit(EXIT_FAILURE);
        // LCOV_EXCL_STOP
      }
    }
    for (auto &param : cmdline.params()) {
      if (param.first == "asn-db") {
        asn_db = param.second;
      } else if (param.first == "country-db") {
        country_db = param.second;
      } else {
        // LCOV_EXCL_START
        std::clog << "fatal: unrecognized param: " << param.first << std::endl;
        usage();
        exit(EXIT_FAILURE);
        // LCOV_EXCL_STOP
      }
    }
    auto sz = cmdline.pos_args().size();
    if (sz != 2) {
      // LCOV_EXCL_START
      usage();
      exit(EXIT_FAILURE);
      // LCOV_EXCL_STOP
    }
    ip = cmdline.pos_args()[1];
  }
  std::clog << std::boolalpha;
#define LOOKUP(Database, Feature)                          \
  do {                                                     \
    std::vector<std::string> logs;                         \
    mk::mmdb::Handle db;                                   \
    std::string value;                                     \
    bool ok = db.open(Database, logs);                     \
    std::clog << "open: " << ok << std::endl;              \
    ok = ok || keep_going;                                 \
    if (ok) {                                              \
      ok = db.lookup_##Feature(ip, value, logs);           \
      std::clog << "lookup: " << ok << std::endl;          \
      ok = ok || keep_going;                               \
      std::clog << #Feature << ": " << value << std::endl; \
    }                                                      \
    for (const std::string &s : logs) std::clog << s;      \
    if (!ok) exit(EXIT_FAILURE);                           \
  } while (0)
  LOOKUP(country_db, cc);
  LOOKUP(asn_db, asn);
  LOOKUP(asn_db, org);
}
