# xrRenderNull - no-op renderer for headless bring-up
set(NULLRENDER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/xrRenderNull.cpp"
    "${CMAKE_SOURCE_DIR}/src/Layers/xrAPI/xrAPI.cpp"
)

add_library(xrrendernull STATIC ${NULLRENDER_SOURCES})
target_include_directories(xrrendernull BEFORE PUBLIC "${CMAKE_SOURCE_DIR}/src/compat")
target_include_directories(xrrendernull PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/xrEngine"
    "${CMAKE_SOURCE_DIR}/src/Include"
    "${CMAKE_SOURCE_DIR}/src/Include/xrAPI"
    "${CMAKE_SOURCE_DIR}/src/Include/xrRender"
    "${CMAKE_SOURCE_DIR}/src/3rd party"
    "${CMAKE_SOURCE_DIR}/sdk/include"
    "${CMAKE_SOURCE_DIR}/src/Layers/xrRender"
    "${CMAKE_SOURCE_DIR}/src/3rd party/imgui"
)
target_compile_definitions(xrrendernull PRIVATE ENGINE_BUILD STATIC_RENDERER_NULL)
target_link_libraries(xrrendernull PUBLIC xrengine xrcore ${SDL2_LIBRARIES})
