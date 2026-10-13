# luabind vendored - Linux build against vendored luajit-2
file(GLOB LUABIND_SOURCES CONFIGURE_DEPENDS luabind/src/*.cpp)
list(APPEND LUABIND_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/luabind_memory.cpp)

add_library(luabind STATIC ${LUABIND_SOURCES})
target_include_directories(luabind PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/luabind/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../luajit-2/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../sdk/include)
target_compile_definitions(luabind PUBLIC
    USE_LUAJIT_ONE __cdecl= PRIVATE LUABIND_BUILDING)
target_link_libraries(luabind PUBLIC luajit)
set_target_properties(luabind PROPERTIES POSITION_INDEPENDENT_CODE ON)
