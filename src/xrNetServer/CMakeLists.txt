# xrNetServer - network message layer
set(XRNETSERVER_SOURCES
    ip_filter.cpp
    NET_AuthCheck.cpp
    NET_Common.cpp
    NET_Compressor.cpp
    NET_Log.cpp
    stdafx.cpp
)

if(WIN32)
list(APPEND XRNETSERVER_SOURCES
    NET_Client.cpp
    NET_Server.cpp
)
else()
list(APPEND XRNETSERVER_SOURCES
    linux/NET_Client.cpp
    linux/NET_Server.cpp
)
endif()

add_library(xrnetserver STATIC ${XRNETSERVER_SOURCES})
target_include_directories(xrnetserver PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/xrNetServer"
)
target_link_libraries(xrnetserver PUBLIC xrcore)
