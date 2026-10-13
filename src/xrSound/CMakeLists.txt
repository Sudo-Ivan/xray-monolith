# xrSound - OpenAL sound renderer
set(XRSOUND_SOURCES
    OpenALDeviceList.cpp
    SoundRender_Cache.cpp
    SoundRender_Core.cpp
    SoundRender_CoreA.cpp
    SoundRender_Core_Processor.cpp
    SoundRender_Core_SourceManager.cpp
    SoundRender_Core_StartStop.cpp
    SoundRender_Emitter.cpp
    SoundRender_Emitter_FSM.cpp
    SoundRender_Emitter_StartStop.cpp
    SoundRender_Emitter_streamer.cpp
    SoundRender_Environment.cpp
    SoundRender_Source.cpp
    SoundRender_Source_loader.cpp
    SoundRender_Target.cpp
    SoundRender_TargetA.cpp
    sound.cpp
    stdafx.cpp
)

if(WIN32)
list(APPEND XRSOUND_SOURCES
    guids.cpp
    NotificationClient.cpp
)
endif()

add_library(xrsound STATIC ${XRSOUND_SOURCES})
target_include_directories(xrsound PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/xrSound"
)
target_link_libraries(xrsound PUBLIC xrcore xrcdb)
