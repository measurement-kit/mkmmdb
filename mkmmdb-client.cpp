#include <string.h>

#include <iostream>
#include <ios>

#include "mkmmdb.hpp"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::clog << "Usage: mkmmdb-client <ip>" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string ip = argv[1];
  std::clog << std::boolalpha;
  {
    std::vector<std::string> logs;
    mk::mmdb::Handle db;
    std::string cc;
    std::clog << "open: " << db.open("country.mmdb", logs) << std::endl;
    std::clog << "lookup_cc: " << db.lookup_cc(ip, cc, logs) << std::endl;
    std::clog << "cc: " << cc << std::endl;
    std::clog << "=== BEGIN LOGS === " << std::endl;
    for (const std::string &s : logs) { std::clog << s; }
    std::clog << "=== END LOGS === " << std::endl;
  }
  {
    std::vector<std::string> logs;
    mk::mmdb::Handle db;
    std::string asn, org;
    std::clog << "open: " << db.open("asn.mmdb", logs) << std::endl;
    std::clog << "lookup_asn: " << db.lookup_asn(ip, asn, logs) << std::endl;
    std::clog << "lookup_org: " << db.lookup_org(ip, org, logs) << std::endl;
    std::clog << "asn: " << asn << std::endl;
    std::clog << "org: " << org << std::endl;
    std::clog << "=== BEGIN LOGS === " << std::endl;
    for (const std::string &s : logs) { std::clog << s; }
    std::clog << "=== END LOGS === " << std::endl;
  }
}
