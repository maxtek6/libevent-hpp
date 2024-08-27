# Libevent-HPP

Modern C++ bindings for Libevent.

## Requirements

+ C++17
+ CMake
+ Libevent libraries and headers

## Build

There are several cmake flags avaialble:

+ `EVENTHPP_VENDORED`: Build using a locally built version of Libevent. This 
is especially useful for systems where Libevent is not readily available through a package manager or a newer version is required.

+ `EVENTHPP_TESTS`: Build unit test suite. This is not required, but is useful
for verifying that everything works as intended in a given environment.

+ `EVENTHPP_TESTS`: Build test suite with code coverage. This is not useful
in most cases.