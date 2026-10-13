# lua-extensions - LuaSocket + extra libs loaded into xrGame's script VM
file(GLOB LUAEXT_SOURCES "*.cpp" "*.c")
list(FILTER LUAEXT_SOURCES EXCLUDE REGEX "(keyboard|Logger)\.cpp$")
file(GLOB LUASOCKET_SOURCES "luasocket/*.c")
list(FILTER LUASOCKET_SOURCES EXCLUDE REGEX "wsocket\.c$")
# unix-domain transport sources are optional; keep core POSIX socket only
list(FILTER LUASOCKET_SOURCES EXCLUDE REGEX "unix(dgram|stream)?\.c$")

add_library(luaextensions STATIC ${LUAEXT_SOURCES} ${LUASOCKET_SOURCES})
target_include_directories(luaextensions BEFORE PUBLIC "${CMAKE_SOURCE_DIR}/src/compat")
target_include_directories(luaextensions PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/luasocket"
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/xrCore"
    "${CMAKE_SOURCE_DIR}/src/3rd party"
    "${CMAKE_SOURCE_DIR}/src/3rd party/luajit-2/src"
    "${CMAKE_SOURCE_DIR}/src/3rd party/luabind"
    "${CMAKE_SOURCE_DIR}/sdk/include"
)
target_link_libraries(luaextensions PUBLIC luajit xrcore)
