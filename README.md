# Measurement Kit MMDB wrappers

[![GitHub license](https://img.shields.io/github/license/measurement-kit/mkmmdb.svg)](https://raw.githubusercontent.com/measurement-kit/mkmmdb/master/LICENSE) [![Github Releases](https://img.shields.io/github/release/measurement-kit/mkmmdb.svg)](https://github.com/measurement-kit/mkmmdb/releases) [![Build Status](https://img.shields.io/travis/measurement-kit/mkmmdb/master.svg?label=travis)](https://travis-ci.org/measurement-kit/mkmmdb) [![codecov](https://codecov.io/gh/measurement-kit/mkmmdb/branch/master/graph/badge.svg)](https://codecov.io/gh/measurement-kit/mkmmdb) [![Build status](https://img.shields.io/appveyor/ci/bassosimone/mkmmdb/master.svg?label=appveyor)](https://ci.appveyor.com/project/bassosimone/mkmmdb/branch/master)

Measurement Kit MMDB wrappers. Because this library is a basic building
block for Measurement Kit, we don't provide any API guarantees (i.e.
we'll never tag and release `v1.0.0`).

## Regenerating build files

Possibly edit `MKBuild.yaml`, then run:

```
go get -v github.com/measurement-kit/mkbuild
mkbuild
```

## Building

```
mkdir build
cd build
cmake -GNinja ..
cmake --build .
ctest -a -j8 --output-on-failure
```

## Testing with docker

```
./docker.sh <build-type>
```
