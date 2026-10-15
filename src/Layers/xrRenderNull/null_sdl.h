#pragma once

// SDL-backed software visual path for the null renderer. Draws the
// engine UI layer (textured quads, lines, scissor) through SDL_Renderer
// so a real window shows the menu, console, and debug UI instead of a
// discarded frame.

class IUIRender;
class IUIShader;
class IFontRender;

IUIRender* xray_null_sdl_uirender();
IFontRender* xray_null_sdl_fontrender();
void xray_null_sdl_fontrender_destroy(IFontRender* f);
IUIShader* xray_null_sdl_uishader_create();
void xray_null_sdl_uishader_destroy(IUIShader* sh);

void xray_null_sdl_frame_begin();
void xray_null_sdl_frame_end();
