add_library(xrcpupipe STATIC
    PLC.cpp
    StdAfx.cpp
    ttapi.cpp
    xrCPU_Pipe.cpp
    xrSkin2W.cpp
    xrSkin2W_SSE.cpp
    xrSkin2W_thread.cpp
)
target_include_directories(xrcpupipe PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/compat"
    "${CMAKE_SOURCE_DIR}/src/xrCore"
    "${CMAKE_SOURCE_DIR}/src/xrCPU_Pipe"
    "${CMAKE_SOURCE_DIR}/src/3rd party"
    "${CMAKE_SOURCE_DIR}/src/3rd party/fast_dynamic_cast"
)
