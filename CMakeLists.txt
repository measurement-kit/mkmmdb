cmake_minimum_required(VERSION 3.1.0)
project(mkmmdb LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
include(MkUtils)

# Download dependencies
# ---------------------

MkDownloadMMDBDatabases()
MkDownloadMeasurementKitMkMock()
MkDownloadAdishavitArgh()

if("${MSVC}")
  MkDownloadMeasurementKitPrebuiltWindowsLibmaxminddb()
  list(APPEND CMAKE_REQUIRED_INCLUDES "${MK_WINDOWS_LIBMAXMINDDB_INCLUDE_PATH}")
  list(APPEND CMAKE_LIBRARY_PATH "${MK_WINDOWS_LIBMAXMINDDB_LIBRARY_PATH}")
endif()

# Checks
# ------

include(CheckIncludeFiles)

check_include_files(maxminddb.h MKMMDB_HAVE_MAXMINDDB_H)
find_library(LIBMAXMINDDB_LIBRARY maxminddb)
if (("${MKMMDB_HAVE_MAXMINDDB_H}" STREQUAL "") OR
    ("${LIBMAXMINDDB_LIBRARY}" STREQUAL "LIBMAXMINDDB_LIBRARY-NOTFOUND"))
  message(FATAL_ERROR "Cannot find libmaxminddb")
endif()
list(APPEND MKMMDB_LIBS "${LIBMAXMINDDB_LIBRARY}")

# Compiler flags
# --------------

MkSetCompilerFlags()

# Library and binary
# ------------------

set(MKMMDB_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_REQUIRED_INCLUDES})

include_directories(${MKMMDB_INCLUDES})

if("${WIN32}" OR "${MINGW}")
  list(APPEND MKMMDB_LIBS "ws2_32")
  if ("${MINGW}")
      list(APPEND MKMMDB_LIBS -static-libgcc -static-libstdc++)
  endif()
endif()
list(APPEND MKMMDB_LIBS Threads::Threads)
link_libraries("${MKMMDB_LIBS}")

add_library(mkmmdb mkmmdb.cpp)
add_executable(mkmmdb-client mkmmdb-client.cpp)
target_link_libraries(mkmmdb-client mkmmdb)

# Testing
# -------

set(BUILD_TESTING "ON" CACHE BOOL "Whether to build tests")
if(${BUILD_TESTING})
  enable_testing()
  add_test(NAME basic_test COMMAND mkmmdb-client 8.8.8.8)
  add_test(NAME name_not_found_test COMMAND mkmmdb-client -k 127.0.0.1)
  add_test(NAME invalid_ip_test COMMAND mkmmdb-client -k 8.8.8.x)
  add_test(NAME nonexistent_db COMMAND mkmmdb-client -k
           --asn-db non-existent.mmdb 8.8.8.8)
endif()
