#include <string.h>

#include <iostream>
#include <ios>

#include "mkmmdb.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::clog << "Usage: mkmmdb-client <ip>" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string ip = argv[1];
  {
    mkmmdb_uptr db{mkmmdb_open("country.mmdb")};
    if (!mkmmdb_good(db.get())) {
      std::clog << "Cannot open country.mmdb" << std::endl;
      // FALLTHROUGH
    }
    std::string s = mkmmdb_lookup_cc(db.get(), ip.c_str());
    std::clog << "=== BEGIN LOGS === " << std::endl;
    std::clog << mkmmdb_get_last_lookup_logs(db.get());
    std::clog << "=== END LOGS === " << std::endl;
    std::clog << "CC: " << s << std::endl;
  }
  {
    mkmmdb_uptr db{mkmmdb_open("asn.mmdb")};
    if (!mkmmdb_good(db.get())) {
      std::clog << "Cannot open asn.mmdb" << std::endl;
      // FALLTHROUGH
    }
    {
      std::string s = mkmmdb_lookup_org(db.get(), ip.c_str());
      std::clog << "=== BEGIN LOGS === " << std::endl;
      std::clog << mkmmdb_get_last_lookup_logs(db.get());
      std::clog << "=== END LOGS === " << std::endl;
      std::clog << "ORG: " << s << std::endl;
    }
    {
      int64_t v = mkmmdb_lookup_asn(db.get(), ip.c_str());
      std::clog << "=== BEGIN LOGS === " << std::endl;
      std::clog << mkmmdb_get_last_lookup_logs(db.get());
      std::clog << "=== END LOGS === " << std::endl;
      std::clog << "ASN: " << v << std::endl;
    }
  }
}
