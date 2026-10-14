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

## xrGame milestone status

The real `src/xrGame` module now compiles and links on Linux and the
`xray` binary runs it: `SDL_VIDEODRIVER=dummy ./build/xray -fsltx
tests/fixtures/fsgame.ltx` boots through real `CGamePersistent` creation
and enters `CGamePersistent::OnFrame` inside `CRenderDevice::Run`.

- `libxrgame.a` is ~112MB covering the vcxproj source set (minus the three
  files upstream marks `ExcludedFromBuild` and `lua_studio.cpp`, which
  needs the cs SDK debugger backend that has no Linux platform branch).
- `xrServerEntities`, vendored `luajit-2` (amalgamated ljamalg.c plus the
  minilua/dynasm/buildvm bootstrap), `luabind`, `crypto` (ported to the
  OpenSSL 3.x API), and `lua-extensions` are linked in.
- The null renderer registers the renderer cvars game code queries
  (r2_dof family) at module attach, matching the real renderer entry
  points.
- VFS keeps backslash-internal naming; `xr_conv_path` now resolves
  component case-insensitively on literal-miss because the VFS lowercases
  registered names while gamedata files keep mixed case on disk.
- `tests/fixtures/` carries the minimal gamedata the boot path touches:
  fsgame.ltx, system.ltx/game.ltx sections, environment + weathers +
  sun_positions, 24 synthesized .efd eval functions, a silent rain.ogg,
  a font atlas ini, and the UI xmls loaded during OnAppStart.
- The fixture set now boots clean: the intro_logo tutorial completes,
  the loop idles stable (0% CPU at 90s) and SIGTERM runs the graceful
  shutdown path. Remaining informational lines: loose gamemtl.xr falls
  back to materials/materials.ltx. Real gamedata is still needed
  before any level or gameplay path works.
- `_REG_INFO.Prio` is `u32`: on LP64 an `int` priority of -1 never
  compared equal to the `0xfffffffful` invalid sentinel, so removed
  registrator entries kept receiving frame callbacks after the object
  was freed. Windows only worked because `unsigned long` is 32-bit.
- SDL window visibility/focus events feed `CRenderDevice::OnWM_Activate`
  so `b_is_Active` tracks the window under headless drivers.
- The null renderer returns real selected mode dimensions from
  `Create()` and its imgui backend builds the font atlas once fonts
  exist, which keeps `ide::OnFrame` safe.
- `CMainMenu::ReloadUI` falls back to a native `CUIDialogWndEx` when the
  `MAIN_MNU` clsid has no script-registered class, and
  `CUISequencer::OnRender` no longer dereferences an empty item list.

## Real gamedata milestone (Anomaly 1.5.3)

Running against the real Anomaly 1.5.3 install (see
`base/anomaly` / fsgame.ltx), the engine boots the real xrGame module
end to end: the db0 archives decompress, `system.ltx` and the config
chain load, all script class registrators run (object_factory register
for every clsid and server class), the real console cvar set applies,
fonts/shader-bus/renderer attach, the intro logo tutorial completes,
and the loop holds at the main menu idle until SIGTERM teardown.

Three port bugs the real data exposed and fixed:

- `luabind` now builds with `LUABIND_NO_EXCEPTIONS`, matching every
  upstream vcxproj config. Without it the member-dispatch `o.call` body
  in `class_rep::function_dispatcher` was inside the `#else` of
  `#ifndef LUABIND_NO_EXCEPTIONS` and compiled out entirely, so every
  bound member call fell through to `lua_error` with the raw argument
  stack still on top. The engine relies on the callback error model
  (`set_error_callback`/`set_pcall_callback`).
- `xr_fpclass` returned `_FPCLASS_PD` for positive-normal floats, so
  `_valid` rejected every positive float (sun positions, etc). Added
  `_FPCLASS_PN` (0x0200) and corrected the default case; also removed
  a stale conflicting `_fpclass`/`_FPCLASS_*` shim block.
- `CScriptEngine::lua_pcall_failed` returned `LUA_ERRRUN` (2) as the
  Lua message-handler result count, making the VM pop two slots and
  underflow the frame inside `lj_vm_return`. Now returns 1 (the error
  object) as the handler contract requires.
- LZO moved from the vendored minilzo (which corrupted match copies
  under GCC 16) to the system `lzo2` decompression, fixing db archive
  reads (BADINI spam and truncated config lines).

Remaining benign warnings on real data: `r__clear_models_on_unload`
is an Anomaly cvar this engine rev does not register, and
`misc/smart_terrain_masks.ltx` is optional script data absent from the
base install.
