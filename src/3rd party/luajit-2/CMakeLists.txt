# LuaJIT 2.0.4 vendored - Linux bootstrap build.
# Replicates gen_luajit_artifacts.bat: minilua -> dynasm -> buildvm -> generated
# headers + lj_vm.S, then compiles the amalgamated core.

set(LJ_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LJ_DYNASM ${CMAKE_CURRENT_SOURCE_DIR}/dynasm/dynasm.lua)
set(LJ_GEN ${CMAKE_CURRENT_BINARY_DIR}/gen)
file(MAKE_DIRECTORY ${LJ_GEN} ${LJ_GEN}/jit)

set(LJ_LIB_SRCS
    ${LJ_SRC}/lib_base.c ${LJ_SRC}/lib_math.c ${LJ_SRC}/lib_bit.c
    ${LJ_SRC}/lib_string.c ${LJ_SRC}/lib_table.c ${LJ_SRC}/lib_io.c
    ${LJ_SRC}/lib_os.c ${LJ_SRC}/lib_package.c ${LJ_SRC}/lib_debug.c
    ${LJ_SRC}/lib_jit.c ${LJ_SRC}/lib_ffi.c)

# 1. minilua host tool
add_executable(minilua ${LJ_SRC}/host/minilua.c)
target_link_libraries(minilua m)
set(MINILUA $<TARGET_FILE:minilua>)

# 2. dynasm -> buildvm_arch.h (x64 POSIX: P64, JIT, FFI; no WIN)
add_custom_command(
    OUTPUT ${LJ_GEN}/buildvm_arch.h
    COMMAND ${MINILUA} ${LJ_DYNASM} -LN -D JIT -D FFI -D P64
            -o ${LJ_GEN}/buildvm_arch.h ${LJ_SRC}/vm_x86.dasc
    DEPENDS minilua ${LJ_SRC}/vm_x86.dasc ${LJ_DYNASM})

# 3. buildvm host tool
add_executable(buildvm
    ${LJ_SRC}/host/buildvm.c ${LJ_SRC}/host/buildvm_asm.c
    ${LJ_SRC}/host/buildvm_peobj.c ${LJ_SRC}/host/buildvm_lib.c
    ${LJ_SRC}/host/buildvm_fold.c ${LJ_GEN}/buildvm_arch.h)
target_include_directories(buildvm PRIVATE ${LJ_SRC} ${LJ_SRC}/../dynasm ${LJ_GEN})
target_link_libraries(buildvm m)
set(BUILDVM $<TARGET_FILE:buildvm>)

# 4. generated artifacts
set(LJ_GEN_HDRS ${LJ_GEN}/lj_bcdef.h ${LJ_GEN}/lj_ffdef.h ${LJ_GEN}/lj_libdef.h
    ${LJ_GEN}/lj_recdef.h ${LJ_GEN}/lj_folddef.h)

add_custom_command(
    OUTPUT ${LJ_GEN}/lj_vm.S ${LJ_GEN_HDRS} ${LJ_GEN}/jit/vmdef.lua
    COMMAND ${BUILDVM} -m elfasm -o ${LJ_GEN}/lj_vm.S
    COMMAND ${BUILDVM} -m bcdef -o ${LJ_GEN}/lj_bcdef.h ${LJ_LIB_SRCS}
    COMMAND ${BUILDVM} -m ffdef -o ${LJ_GEN}/lj_ffdef.h ${LJ_LIB_SRCS}
    COMMAND ${BUILDVM} -m libdef -o ${LJ_GEN}/lj_libdef.h ${LJ_LIB_SRCS}
    COMMAND ${BUILDVM} -m recdef -o ${LJ_GEN}/lj_recdef.h ${LJ_LIB_SRCS}
    COMMAND ${BUILDVM} -m vmdef -o ${LJ_GEN}/jit/vmdef.lua ${LJ_LIB_SRCS}
    COMMAND ${BUILDVM} -m folddef -o ${LJ_GEN}/lj_folddef.h ${LJ_SRC}/lj_opt_fold.c
    DEPENDS buildvm ${LJ_LIB_SRCS} ${LJ_SRC}/lj_opt_fold.c)

# 5. library: amalgamated core + VM asm
add_library(luajit STATIC ${LJ_SRC}/ljamalg.c ${LJ_GEN}/lj_vm.S ${LJ_GEN_HDRS})
target_include_directories(luajit PUBLIC ${LJ_SRC} PRIVATE ${LJ_GEN})
target_compile_definitions(luajit PRIVATE LUAJIT_OS=LUAJIT_OS_LINUX)
set_target_properties(luajit PROPERTIES POSITION_INDEPENDENT_CODE ON)
