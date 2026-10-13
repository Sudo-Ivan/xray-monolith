# crypto - DSA/SHA wrappers used by xrGame config signing
add_library(xrcrypto STATIC
    crypto.cpp
    xr_dsa.cpp
    xr_sha.cpp
)
target_include_directories(xrcrypto PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/compat"
    "${CMAKE_SOURCE_DIR}/src/xrCore"
)
target_compile_definitions(xrcrypto PRIVATE CRYPTO_EXPORTS)
target_link_libraries(xrcrypto PUBLIC xrcore ssl crypto)
