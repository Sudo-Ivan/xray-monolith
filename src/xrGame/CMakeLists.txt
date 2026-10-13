# xrGame - game logic module
file(GLOB_RECURSE XRGAME_SOURCES CONFIGURE_DEPENDS "*.cpp" "*.c" "*.cxx")
file(GLOB XRSERVERENTITIES_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/xrServerEntities/*.cpp" "${CMAKE_SOURCE_DIR}/src/xrServerEntities/*.c")
# lua_studio.cpp is the Lua Studio debugger plugin; the cs SDK headers have no Linux branch
list(FILTER XRSERVERENTITIES_SOURCES EXCLUDE REGEX "(lua_studio|doug_lea_memory_allocator)\.cpp$|doug_lea_memory_allocator\.c$")

# files marked ExcludedFromBuild in xrGame.vcxproj
list(FILTER XRGAME_SOURCES EXCLUDE REGEX
    "(ai_rat_fsm|debug_make_final_test|DynamicHeightMap|static_cast_checked_test)\\.cpp$")

add_library(xrgame STATIC ${XRGAME_SOURCES} ${XRSERVERENTITIES_SOURCES})
target_include_directories(xrgame BEFORE PUBLIC "${CMAKE_SOURCE_DIR}/src/compat")
target_include_directories(xrgame PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/xrCore"
    "${CMAKE_SOURCE_DIR}/src/xrServerEntities"
    "${CMAKE_SOURCE_DIR}/src/xrEngine"
    "${CMAKE_SOURCE_DIR}/src/xrSound"
    "${CMAKE_SOURCE_DIR}/src/xrPhysics"
    "${CMAKE_SOURCE_DIR}/src/xrCDB"
    "${CMAKE_SOURCE_DIR}/src/Include"
    "${CMAKE_SOURCE_DIR}/src/Include/xrAPI"
    "${CMAKE_SOURCE_DIR}/src/Include/xrRender"
    "${CMAKE_SOURCE_DIR}/src/3rd party"
    "${CMAKE_SOURCE_DIR}/src/3rd party/luajit-2/src"
    "${CMAKE_SOURCE_DIR}/src/3rd party/luabind"
    "${CMAKE_SOURCE_DIR}/src/3rd party/imgui"
    "${CMAKE_SOURCE_DIR}/sdk/include"
    ${SDL2_INCLUDE_DIRS}
)
target_compile_definitions(xrgame PRIVATE XRGAME_EXPORTS USE_LUAJIT_ONE dSINGLE)
target_link_libraries(xrgame PUBLIC xrengine xrcore xrphysics xrsound xrcdb xrnetserver xrxmlparser luabind luajit ${SDL2_LIBRARIES})
set_target_properties(xrgame PROPERTIES POSITION_INDEPENDENT_CODE ON)
