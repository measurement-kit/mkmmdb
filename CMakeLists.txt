# Autogenerated by `mkbuild`; DO NOT EDIT!

cmake_minimum_required(VERSION 3.12.0)
project("mkmmdb")

include(CheckIncludeFileCXX)
include(CheckLibraryExists)
include(CheckCXXCompilerFlag)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

list(APPEND CMAKE_REQUIRED_LIBRARIES Threads::Threads)

if(("${WIN32}"))
  list(APPEND CMAKE_REQUIRED_LIBRARIES ws2_32 crypt32)
endif()

enable_testing()

if(("${WIN32}"))
  if(("${CMAKE_SIZEOF_VOID_P}" EQUAL 4))
    SET(MK_WIN32_ARCH "x86")
  elseif(("${CMAKE_SIZEOF_VOID_P}" EQUAL 8))
    SET(MK_WIN32_ARCH "x64")
  else()
    message(FATAL_ERROR "Neither 32 not 64 bit")
  endif()
endif()
if((${APPLE}))
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/include")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/local/lib")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -L/usr/local/lib")
endif()

include(CheckFunctionExists)
include(CheckSymbolExists)

#
# argh.h
#

message(STATUS "mkdirAll: ${CMAKE_BINARY_DIR}/.mkbuild/include")
execute_process(COMMAND
  ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/.mkbuild/include"
  RESULT_VARIABLE FAILURE_1352)
if("${FAILURE_1352}")
  message(FATAL_ERROR "${FAILURE_1352}")
endif()
message(STATUS "download: https://raw.githubusercontent.com/adishavit/argh/v1.3.1/argh.h")
file(DOWNLOAD https://raw.githubusercontent.com/adishavit/argh/v1.3.1/argh.h
  "${CMAKE_BINARY_DIR}/.mkbuild/include/argh.h"
  EXPECTED_HASH SHA256=ddb7dfc18dcf90149735b76fb2cff101067453a1df1943a6911233cb7085980c
  TLS_VERIFY ON)
LIST(APPEND CMAKE_REQUIRED_INCLUDES "${CMAKE_BINARY_DIR}/.mkbuild/include")
CHECK_INCLUDE_FILE_CXX("argh.h" MK_HAVE_HEADER_1852)
if(NOT ("${MK_HAVE_HEADER_1852}"))
  message(FATAL_ERROR "cannot find: argh.h")
endif()

#
# catch.hpp
#

message(STATUS "mkdirAll: ${CMAKE_BINARY_DIR}/.mkbuild/include")
execute_process(COMMAND
  ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/.mkbuild/include"
  RESULT_VARIABLE FAILURE_2176)
if("${FAILURE_2176}")
  message(FATAL_ERROR "${FAILURE_2176}")
endif()
message(STATUS "download: https://github.com/catchorg/Catch2/releases/download/v2.11.1/catch.hpp")
file(DOWNLOAD https://github.com/catchorg/Catch2/releases/download/v2.11.1/catch.hpp
  "${CMAKE_BINARY_DIR}/.mkbuild/include/catch.hpp"
  EXPECTED_HASH SHA256=914690be7714fed1f228679a0e379b4e1f6d7d66f88ac20335b7853ff1a8bc55
  TLS_VERIFY ON)
LIST(APPEND CMAKE_REQUIRED_INCLUDES "${CMAKE_BINARY_DIR}/.mkbuild/include")
CHECK_INCLUDE_FILE_CXX("catch.hpp" MK_HAVE_HEADER_2695)
if(NOT ("${MK_HAVE_HEADER_2695}"))
  message(FATAL_ERROR "cannot find: catch.hpp")
endif()
CHECK_INCLUDE_FILE_CXX("maxminddb.h" MK_HAVE_HEADER_2842)
if(NOT ("${MK_HAVE_HEADER_2842}"))
  message(FATAL_ERROR "cannot find: maxminddb.h")
endif()
CHECK_LIBRARY_EXISTS("maxminddb" "MMDB_open" "" MK_HAVE_LIB_2993)
if(NOT ("${MK_HAVE_LIB_2993}"))
  message(FATAL_ERROR "cannot find: maxminddb")
endif()
LIST(APPEND CMAKE_REQUIRED_LIBRARIES "maxminddb")

#
# generic-assets-20190520205742.tar.gz
#

message(STATUS "mkdirAll: ${CMAKE_BINARY_DIR}/.mkbuild/download")
execute_process(COMMAND
  ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/.mkbuild/download"
  RESULT_VARIABLE FAILURE_3409)
if("${FAILURE_3409}")
  message(FATAL_ERROR "${FAILURE_3409}")
endif()
message(STATUS "download: https://github.com/measurement-kit/generic-assets/releases/download/20190520205742/generic-assets-20190520205742.tar.gz")
file(DOWNLOAD https://github.com/measurement-kit/generic-assets/releases/download/20190520205742/generic-assets-20190520205742.tar.gz
  "${CMAKE_BINARY_DIR}/.mkbuild/download/generic-assets-20190520205742.tar.gz"
  EXPECTED_HASH SHA256=70d590c20b2ed31fd43cc63709b267672fecfeac7e908d11e845664ddd43b04f
  TLS_VERIFY ON)
message(STATUS "Extract: ${CMAKE_BINARY_DIR}/.mkbuild/download/generic-assets-20190520205742.tar.gz")
execute_process(COMMAND
  ${CMAKE_COMMAND} -E tar xf "${CMAKE_BINARY_DIR}/.mkbuild/download/generic-assets-20190520205742.tar.gz"
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/.mkbuild/download"
  RESULT_VARIABLE FAILURE_4270)
if("${FAILURE_4270}")
  message(FATAL_ERROR "${FAILURE_4270}")
endif()

#
# mkmock.hpp
#

message(STATUS "mkdirAll: ${CMAKE_BINARY_DIR}/.mkbuild/include")
execute_process(COMMAND
  ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/.mkbuild/include"
  RESULT_VARIABLE FAILURE_4557)
if("${FAILURE_4557}")
  message(FATAL_ERROR "${FAILURE_4557}")
endif()
message(STATUS "download: https://raw.githubusercontent.com/measurement-kit/mkmock/v0.2.0/mkmock.hpp")
file(DOWNLOAD https://raw.githubusercontent.com/measurement-kit/mkmock/v0.2.0/mkmock.hpp
  "${CMAKE_BINARY_DIR}/.mkbuild/include/mkmock.hpp"
  EXPECTED_HASH SHA256=f07bc063a2e64484482f986501003e45ead653ea3f53fadbdb45c17a51d916d2
  TLS_VERIFY ON)
LIST(APPEND CMAKE_REQUIRED_INCLUDES "${CMAKE_BINARY_DIR}/.mkbuild/include")
CHECK_INCLUDE_FILE_CXX("mkmock.hpp" MK_HAVE_HEADER_5085)
if(NOT ("${MK_HAVE_HEADER_5085}"))
  message(FATAL_ERROR "cannot find: mkmock.hpp")
endif()

#
# Set restrictive compiler flags
#

macro(MKSetRestrictiveCompilerFlags)
  if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR
     ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
    set(MK_COMMON_FLAGS "${MK_COMMON_FLAGS} -Werror")
    # https://www.owasp.org/index.php/C-Based_Toolchain_Hardening_Cheat_Sheet
    set(MK_COMMON_FLAGS "${MK_COMMON_FLAGS} -Wall")
    set(MK_COMMON_FLAGS "${MK_COMMON_FLAGS} -Wextra")
    set(MK_COMMON_FLAGS "${MK_COMMON_FLAGS} -Wconversion")
    set(MK_COMMON_FLAGS "${MK_COMMON_FLAGS} -Wcast-align")
    set(MK_COMMON_FLAGS "${MK_COMMON_FLAGS} -Wformat=2")
    set(MK_COMMON_FLAGS "${MK_COMMON_FLAGS} -Wformat-security")
    set(MK_COMMON_FLAGS "${MK_COMMON_FLAGS} -fno-common")
    # Some options are only supported by GCC when we're compiling C code:
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
      set(MK_COMMON_FLAGS "${MK_COMMON_FLAGS} -Wmissing-prototypes")
      set(MK_COMMON_FLAGS "${MK_COMMON_FLAGS} -Wstrict-prototypes")
    else()
      set(MK_C_FLAGS "${MK_C_FLAGS} -Wmissing-prototypes")
      set(MK_C_FLAGS "${MK_C_FLAGS} -Wstrict-prototypes")
    endif()
    set(MK_COMMON_FLAGS "${MK_COMMON_FLAGS} -Wmissing-declarations")
    set(MK_COMMON_FLAGS "${MK_COMMON_FLAGS} -Wstrict-overflow")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
      set(MK_COMMON_FLAGS "${MK_COMMON_FLAGS} -Wtrampolines")
    endif()
    set(MK_CXX_FLAGS "${MK_CXX_FLAGS} -Woverloaded-virtual")
    set(MK_CXX_FLAGS "${MK_CXX_FLAGS} -Wreorder")
    set(MK_CXX_FLAGS "${MK_CXX_FLAGS} -Wsign-promo")
    set(MK_CXX_FLAGS "${MK_CXX_FLAGS} -Wnon-virtual-dtor")
    set(MK_COMMON_FLAGS "${MK_COMMON_FLAGS} -fstack-protector-all")
    if(NOT "${APPLE}" AND NOT "${MINGW}")
      set(MK_LD_FLAGS "${MK_LD_FLAGS} -Wl,-z,noexecstack")
      set(MK_LD_FLAGS "${MK_LD_FLAGS} -Wl,-z,now")
      set(MK_LD_FLAGS "${MK_LD_FLAGS} -Wl,-z,relro")
      set(MK_LD_FLAGS "${MK_LD_FLAGS} -Wl,-z,nodlopen")
      set(MK_LD_FLAGS "${MK_LD_FLAGS} -Wl,-z,nodump")
    elseif(("${MINGW}"))
      set(MK_LD_FLAGS "${MK_LD_FLAGS} -static")
    endif()
    add_definitions(-D_FORTIFY_SOURCES=2)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # TODO(bassosimone): add support for /Wall and /analyze
    set(MK_COMMON_FLAGS "${MK_COMMON_FLAGS} /WX /W4 /EHs")
    set(MK_LD_FLAGS "${MK_LD_FLAGS} /WX")
  else()
    message(FATAL_ERROR "Compiler not supported: ${CMAKE_CXX_COMPILER_ID}")
  endif()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MK_COMMON_FLAGS} ${MK_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MK_COMMON_FLAGS} ${MK_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MK_LD_FLAGS}")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${MK_LD_FLAGS}")
  if("${WIN32}")
    add_definitions(-D_WIN32_WINNT=0x0600) # for NI_NUMERICSERV and WSAPoll
  endif()
endmacro()

MKSetRestrictiveCompilerFlags()

#
# Prepare for compiling targets
#

add_definitions(${CMAKE_REQUIRED_DEFINITIONS})
include_directories(${CMAKE_REQUIRED_INCLUDES})

#
# mkmmdb
#

add_library(
  mkmmdb
  mkmmdb.cpp
)
target_link_libraries(
  mkmmdb
  ${CMAKE_REQUIRED_LIBRARIES}
)

#
# mkmmdb-client
#

add_executable(
  mkmmdb-client
  mkmmdb-client.cpp
)
target_link_libraries(
  mkmmdb-client
  mkmmdb
  ${CMAKE_REQUIRED_LIBRARIES}
)

#
# tests
#

add_executable(
  tests
  tests.cpp
)
target_link_libraries(
  tests
  ${CMAKE_REQUIRED_LIBRARIES}
)

#
# test: basic_test
#

add_test(
  NAME basic_test COMMAND mkmmdb-client 8.8.8.8
)

#
# test: invalid_ip_test
#

add_test(
  NAME invalid_ip_test COMMAND mkmmdb-client -k 8.8.8.x
)

#
# test: mocked_tests
#

add_test(
  NAME mocked_tests COMMAND tests
)

#
# test: name_not_found_test
#

add_test(
  NAME name_not_found_test COMMAND mkmmdb-client -k 127.0.0.1
)

#
# test: nonexistent_asn_db_test
#

add_test(
  NAME nonexistent_asn_db_test COMMAND mkmmdb-client -k --asn-db non-existent.mmdb 8.8.8.8
)

#
# test: nonexistent_country_db_test
#

add_test(
  NAME nonexistent_country_db_test COMMAND mkmmdb-client -k --country-db non-existent.mmdb 8.8.8.8
)
