#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <maxminddb.h>

// By setting MKMOCK_HOOK_ENABLE and including mkmock.hpp we cause mkcurl.hpp
// to compile with mocking enabled so that we can run unit tests.
#define MKMOCK_HOOK_ENABLE
#include "mkmock.hpp"

MKMOCK_DEFINE_HOOK(mmdb_open, int);
MKMOCK_DEFINE_HOOK(MMDB_lookup_string_gai_error, int);
MKMOCK_DEFINE_HOOK(MMDB_lookup_string_mmdb_error, int);
MKMOCK_DEFINE_HOOK(MMDB_lookup_string_record, MMDB_lookup_result_s);
MKMOCK_DEFINE_HOOK(MMDB_get_value_registered_country_iso_code_error, int);
MKMOCK_DEFINE_HOOK(
    MMDB_get_value_registered_country_iso_code_data, MMDB_entry_data_s);
MKMOCK_DEFINE_HOOK(MMDB_get_value_autonomous_system_number_error, int);
MKMOCK_DEFINE_HOOK(
    MMDB_get_value_autonomous_system_number_data, MMDB_entry_data_s);
MKMOCK_DEFINE_HOOK(MMDB_get_value_autonomous_system_organization_error, int);
MKMOCK_DEFINE_HOOK(
    MMDB_get_value_autonomous_system_organization_data, MMDB_entry_data_s);

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

TEST_CASE("When MMDB_get_value fails with mmdb_error for lookup_cc") {
  MKMOCK_WITH_ENABLED_HOOK(
      MMDB_get_value_registered_country_iso_code_error,
      MMDB_CORRUPT_SEARCH_TREE_ERROR,
      {
        std::vector<std::string> logs;
        mk::mmdb::Handle handle;
        std::string cc;
        REQUIRE(handle.open("country.mmdb", logs) == true);
        REQUIRE(handle.lookup_cc("8.8.8.8", cc, logs) == false);
      });
}

TEST_CASE("When MMDB_get_value fails with mmdb_error for lookup_asn") {
  MKMOCK_WITH_ENABLED_HOOK(
      MMDB_get_value_autonomous_system_number_error,
      MMDB_CORRUPT_SEARCH_TREE_ERROR,
      {
        std::vector<std::string> logs;
        mk::mmdb::Handle handle;
        std::string asn;
        REQUIRE(handle.open("asn.mmdb", logs) == true);
        REQUIRE(handle.lookup_asn("8.8.8.8", asn, logs) == false);
      });
}

TEST_CASE("When MMDB_get_value fails with mmdb_error for lookup_org") {
  MKMOCK_WITH_ENABLED_HOOK(
      MMDB_get_value_autonomous_system_organization_error,
      MMDB_CORRUPT_SEARCH_TREE_ERROR,
      {
        std::vector<std::string> logs;
        mk::mmdb::Handle handle;
        std::string org;
        REQUIRE(handle.open("asn.mmdb", logs) == true);
        REQUIRE(handle.lookup_org("8.8.8.8", org, logs) == false);
      });
}
