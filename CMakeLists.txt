cmake_minimum_required(VERSION 3.20)
project(xray-monolith LANGUAGES C CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Legacy MSVC-era codebase: keep the standard low while porting
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -fpermissive          # downgrade legacy code errors to warnings
        -fms-extensions       # unnamed struct/union members (Fvector etc.)
        -fno-operator-names   # code uses 'or'/'and'/'not' as identifiers
        -fno-strict-aliasing  # pervasive type punning
        -Wno-unknown-pragmas  # #pragma warning / #pragma hdrstop
        -Wno-write-strings
        -Wno-narrowing
        -Wno-multichar
        -msse4.2
    )
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wno-deprecated>)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)
endif()

find_package(Threads REQUIRED)

add_subdirectory(src/xrCore)

add_subdirectory(src/xrXMLParser)
add_subdirectory(src/xrCDB)
add_subdirectory(src/xrNetServer)
add_subdirectory(src/xrSound)
add_subdirectory("src/3rd party/ode")
add_subdirectory(src/xrPhysics)

enable_testing()

option(XRCORE_BUILD_SMOKE_TEST "Build the xrCore smoke test executable" ON)
if(XRCORE_BUILD_SMOKE_TEST)
    add_subdirectory(tests)
endif()
