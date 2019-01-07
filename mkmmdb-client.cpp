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
  {
    std::vector<std::string> logs;
    mk::mmdb::Handle db;
    std::string cc;
    bool ok = db.open(country_db, logs);
    std::clog << "open: " << ok << std::endl;
    if (ok || keep_going) {
      ok = db.lookup_cc(ip, cc, logs);
      std::clog << "lookup_cc: " << ok << std::endl;
      std::clog << "cc: " << cc << std::endl;
    }
    std::clog << "=== BEGIN LOGS === " << std::endl;
    for (const std::string &s : logs) {
      std::clog << s;
    }
    std::clog << "=== END LOGS === " << std::endl;
    if (!ok && !keep_going) exit(EXIT_FAILURE);
  }
  {
    std::vector<std::string> logs;
    mk::mmdb::Handle db;
    std::string asn, org;
    bool ok = db.open(asn_db, logs);
    std::clog << "open: " << ok << std::endl;
    if (ok || keep_going) {
      ok = db.lookup_asn(ip, asn, logs);
      std::clog << "lookup_asn: " << ok << std::endl;
      std::clog << "asn: " << asn << std::endl;
    }
    if (ok || keep_going) {
      ok = db.lookup_org(ip, org, logs);
      std::clog << "lookup_org: " << ok << std::endl;
      std::clog << "org: " << org << std::endl;
    }
    std::clog << "=== BEGIN LOGS === " << std::endl;
    for (const std::string &s : logs) {
      std::clog << s;
    }
    std::clog << "=== END LOGS === " << std::endl;
    if (!ok && !keep_going) exit(EXIT_FAILURE);
  }
}
