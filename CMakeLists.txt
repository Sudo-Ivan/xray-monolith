cmake_minimum_required(VERSION 3.20)
project(xray-monolith LANGUAGES C CXX ASM)

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
        -fno-operator-names -Wno-template-body   # code uses 'or'/'and'/'not' as identifiers
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
find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(SDL2 sdl2)
endif()

add_subdirectory(src/xrCore)

add_subdirectory(src/xrXMLParser)
add_subdirectory(src/xrCDB)
add_subdirectory(src/xrNetServer)
add_subdirectory(src/xrSound)
add_subdirectory("src/3rd party/ode")
add_subdirectory(src/xrPhysics)
add_subdirectory(src/xrEngine)
add_subdirectory(src/Layers/xrRenderNull)
add_subdirectory("src/3rd party/imgui")
add_subdirectory("src/3rd party/luajit-2")
add_subdirectory("src/3rd party/luabind")
add_subdirectory("src/3rd party/crypto")
add_subdirectory("src/3rd party/lua-extensions")
add_subdirectory(src/xrGame)


enable_testing()

option(XRCORE_BUILD_SMOKE_TEST "Build the xrCore smoke test executable" ON)
if(XRCORE_BUILD_SMOKE_TEST)
    add_subdirectory(tests)
endif()

if(NOT WIN32)
    add_executable(xray src/xrEngine/x_ray.cpp src/Layers/reshade_stub.cpp src/Layers/cpupipe_stub.cpp src/compat/icu_shim.cpp)
    target_link_libraries(xray PRIVATE xrgame xrcrypto luaextensions xrrendernull xrengine xrphysics xrsound xrnetserver xrcdb xrxmlparser xrcore luabind luajit imgui ${SDL2_LIBRARIES} openal vorbisfile vorbis ogg icuuc dl pthread m)
endif()
