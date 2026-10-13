# xray-monolith Linux port

Goal: run STALKER Anomaly + GAMMA natively on Linux, no Wine.

Strategy: keep the monolith engine and its D3D11 renderer, port only the
Windows platform layer. D3D calls are handled at runtime by DXVK Native
(libdxvk_d3d11.so, no Wine). OpenXRay xray-16 is the reference
implementation for every platform piece. There is no shared git history
between the two trees; porting is manual grafting, not merging.

Reference remote: `openxray` (https://github.com/OpenXRay/xray-16, branch dev).
Compare files with `git diff HEAD openxray/dev -- <path>`.

## Milestone 1: xrCore builds and runs on Linux

xrCore is the foundation everything links against: VFS (LocatorAPI),
containers, math, logging, threads, compression, crypto, debug.

Direct windows.h users in src/xrCore (7 of 187 files):

| File | Win32 dependency | OpenXRay replacement |
|---|---|---|
| LocatorAPI.cpp | file API, MMF, FindFirstFile | openxray src/xrCore/LocatorAPI.cpp (POSIX port) |
| xrCore_platform.h | platform defines | openxray platform headers |
| blackbox/* | Bugslayer stack walking, dbghelp | openxray src/xrCore/Debug/StackTrace.cpp |
| PPMdType.h | typedefs only | trivial |
| rt_lzo_dll.ch, rt_miniacc.h | lzo loader glue | use system lzo / bundled build |

Missing pieces to graft from OpenXRay:

- ModuleLookup (.so loading, replaces LoadLibrary)
- Threading/ (Task, Event, Lock: monolith has its own Lock.cpp, keep API)
- Debug/ (StackTrace, xrDebug without MessageBox)
- CMakeLists.txt for xrCore (openxray has one at src/xrCore/CMakeLists.txt)

Steps:

1. Top-level CMakeLists.txt + src/xrCore/CMakeLists.txt, build xrCore as a
   static lib with GCC. Stub out what does not port yet.
2. Port xrCore_platform.h: defines for Linux (paths, cdecl, types).
3. Replace blackbox/ usage with OpenXRay Debug/ implementation or a stub
   (backtrace via execinfo is fine for M1).
4. LocatorAPI: graft OpenXRay POSIX file layer (case-insensitive lookup
   is handled there already).
5. Module loading: graft ModuleLookup for xrEngine/xrGame .so loading.
6. Sanity check: a small test exe that mounts a gamedata dir via
   LocatorAPI and lists files.

M1 exit criteria: `cmake --build` produces libxrcore.a on Linux and the
test binary reads a real Anomaly gamedata folder.

### Status

Done so far:

- `cmake -B build -G Ninja` plus `cmake --build` produces
  `src/xrCore/libxrcore.a` with GCC on x86_64.
- `tests/xrcore_smoke` links against the library, mounts a gamedata
  directory via LocatorAPI (flTargetFolderOnly), and lists the files the
  VFS registered. Verified against tests/fixtures/gamedata in ctest and
  the real Anomaly gamedata tree.
- VFS path handling: internal names keep backslash separators; the POSIX
  file layer translates to slashes in xr_conv_path at every syscall
  boundary. Recursive CRITICAL_SECTION semantics preserved via
  PTHREAD_MUTEX_RECURSIVE.
- Platform layer: OpenXRay `src/Common/Platform.hpp` and `PlatformLinux.inl`
  vendored under `src/Common`, selected from `xrCore_platform.h` on
  non-Windows. The inl was extended with the Win32 file API surface the
  engine uses (CreateFile, file mapping over mmap, file attributes,
  _findfirst family over glob, Interlocked ops, x87 control word helpers).
- `src/compat/` holds thin shims for MSVC headers (windows.h, io.h,
  direct.h, intrin.h, hash_map/hash_set, ppl/concurrent_* backed by
  std::thread and std containers).
- Windows-only sources excluded or stubbed: blackbox, FileSystem.cpp
  (replaced by portable FileSystem_linux.cpp), xrDebugNew.cpp StackWalker
  path (xrDebug.cpp used instead, dialog/dump paths compiled out),
  MSVC inline assembly replaced with memcpy/memset and GCC asm.
- GitHub Actions workflow .github/workflows/linux-ci.yml builds the gcc
  Release and Debug matrix and runs ctest. Clang leg is present but
  non-blocking until the port reaches parity.

Not done yet:

- TBB is not used; PPL shim provides task_group/parallel_for over
  std::thread.
- Crash handling has no backtrace; stack walking port is still open.
- File notifications in LocatorAPI_Notifications are stubbed.

## Later milestones

- M2: xrEngine + SDL2 window/input (openxray src/xrEngine is the template),
  xrXMLParser, xrSound (OpenAL already in tree).
- M3: xrGame + xrScriptEngine (LuaJIT is in tree, GC32 vs GC64 TBD),
  launch Anomaly main menu with a null/software renderer.
- M4: xrRenderPC_R4 on DXVK Native (SDL2 WSI backend), HLSL shaders
  unchanged. Validate against SSS/GAMMA shaderpack.
- M5: full GAMMA validation: 3DSS PiP scopes, DXML, modded-exe script
  callbacks used by the mod list.
- M6: packaging: gamma-launcher + linux-mod-organizer integration so a
  native build drops into the existing Linux install flow.

## Non-goals

- Renderer rewrite to OpenGL/Vulkan. DXVK Native covers D3D8-11.
- Multiplayer/GameSpy.
- Keeping the Windows build working on the linux branch during early
  milestones. Fix divergence before merge.
