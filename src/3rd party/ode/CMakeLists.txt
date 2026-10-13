# ODE physics library (vendored)
add_library(ode STATIC
    "ode/src/Bounder33.cpp"
    "ode/src/StepJointInternal.cpp"
    "ode/src/array.cpp"
    "ode/src/collision_kernel.cpp"
    "ode/src/collision_quadtreespace.cpp"
    "ode/src/collision_space.cpp"
    "ode/src/collision_std.cpp"
    "ode/src/collision_transform.cpp"
    "ode/src/collision_util.cpp"
    "ode/src/error.cpp"
    "ode/src/export-dif.cpp"
    "ode/src/fastdot.c"
    "ode/src/fastldlt.c"
    "ode/src/fastlsolve.c"
    "ode/src/fastltsolve.c"
    "ode/src/joint.cpp"
    "ode/src/lcp.cpp"
    "ode/src/mass.cpp"
    "ode/src/mat.cpp"
    "ode/src/matrix.cpp"
    "ode/src/memory.cpp"
    "ode/src/misc.cpp"
    "ode/src/obstack.cpp"
    "ode/src/ode.cpp"
    "ode/src/odemath.cpp"
    "ode/src/quickstep.cpp"
    "ode/src/rotation.cpp"
    "ode/src/step.cpp"
    "ode/src/stepfast.cpp"
    "ode/src/testing.cpp"
    "ode/src/timer.cpp"
    "ode/src/util.cpp"
)
target_include_directories(ode PUBLIC
    "${CMAKE_SOURCE_DIR}/src/3rd party/ode/include"
    "${CMAKE_SOURCE_DIR}/src/3rd party/ode/ode/src"
    "${CMAKE_SOURCE_DIR}/src/3rd party/ode/config"
)
target_compile_definitions(ode PUBLIC
    dSINGLE
    "__forceinline=inline __attribute__((always_inline))"
)
target_compile_options(ode PRIVATE -w)
