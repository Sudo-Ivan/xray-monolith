add_library(xrcore STATIC)

target_include_directories(xrcore PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/xrCore"
    "${CMAKE_SOURCE_DIR}/src/compat"
    "${CMAKE_SOURCE_DIR}/src/3rd party"
    "${CMAKE_SOURCE_DIR}/src/3rd party/fastdelegate"
    "${CMAKE_SOURCE_DIR}/src/3rd party/robin_hood"
    "${CMAKE_SOURCE_DIR}/src/3rd party/tbb"
    "${CMAKE_SOURCE_DIR}/src/3rd party/DXERR"
)

target_compile_definitions(xrcore PRIVATE
    PURE_ALLOC
)

target_link_libraries(xrcore PUBLIC Threads::Threads ${CMAKE_DL_LIBS})

if(WIN32)
    target_sources(xrcore PRIVATE
        blackbox/BlackBoxUI.cpp
        blackbox/BSUFunctions.cpp
        blackbox/CrashHandler.cpp
        blackbox/DiagAssert.cpp
        blackbox/GetLoadedModules.cpp
        blackbox/IsNT.cpp
        blackbox/NT4ProcessInfo.cpp
        blackbox/stdafx_.cpp
        blackbox/TLHELPProcessInfo.cpp
        FileSystem.cpp
        FileSystem_borland.cpp
        xrMemory_subst_borland.cpp
        )
else()
    target_sources(xrcore PRIVATE
        FileSystem_linux.cpp
        stubs_linux.cpp
        )
endif()

target_sources(xrcore PRIVATE
    clsid.cpp
    cpuid.cpp
    crc32.cpp
    dump_string.cpp
    ShaderSourceCRC.cpp
    file_stream_reader.cpp
    FS.cpp
    FTimer.cpp
    LocatorAPI.cpp
    LocatorAPI_auth.cpp
    LocatorAPI_defs.cpp
    LocatorAPI_Notifications.cpp
    Lock.cpp
    log.cpp
    LzHuf.cpp
    lzo_compressor.cpp
    memory_allocation_stats.cpp
    memory_monitor.cpp
    memory_usage.cpp
    mezz_stringbuffer.cpp
    Model.cpp
    NET_utils.cpp
    os_clipboard.cpp
    ppmd_compressor.cpp
    rt_compressor.cpp
    rt_compressor9.cpp
    rt_lzo1x_1.cpp
    rt_lzo1x_9x.cpp
    rt_lzo1x_d1.cpp
    rt_lzo1x_d2.cpp
    rt_lzo1x_d3.cpp
    rt_lzo_init.cpp
    ScopeLock.cpp
    stdafx.cpp
    stream_reader.cpp
    string_concatenations.cpp
    xrCore.cpp
    xrDebug.cpp
    xrMemory.cpp
    xrMemory_align.cpp
    xrMemory_debug.cpp
    xrMemory_POOL.cpp
    xrMemory_pso_Copy.cpp
    xrMemory_pso_Fill.cpp
    xrMemory_pso_Fill32.cpp
    xrMemory_subst_msvc.cpp
    xrsharedmem.cpp
    xrstring.cpp
    xrSyncronize.cpp
    Xr_ini.cpp
    xr_shared.cpp
    xr_trims.cpp
    _compressed_normal.cpp
    _math.cpp
    _sphere.cpp
    _std_extensions.cpp
)
