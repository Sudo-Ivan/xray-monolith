add_executable(xrcore_smoke smoke.cpp)
target_link_libraries(xrcore_smoke PRIVATE xrcore)
add_test(NAME xrcore_smoke COMMAND xrcore_smoke)
