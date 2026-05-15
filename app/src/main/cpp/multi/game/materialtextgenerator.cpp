#include "../main.h"
#include "game.h"
#include "../gui/gui.h"
#include "RW/RenderWare.h"

extern UI* pUI;

MaterialTextGenerator::MaterialTextGenerator()
{
    m_camera = 0;
    m_frame = 0;

    SetUpScene();
}

void MaterialTextGenerator::SetUpScene()
{
    // RwCameraCreate
    m_camera = ((uintptr_t(*)())(g_libGTASA + 0x756184))();
    // RwFrameCreate
    m_frame = ((uintptr_t(*)())(g_libGTASA + 0x758A7C))();

    if (!m_camera) return;
    if (!m_frame) return;

    // RwObjectHasFrameSetFrame
    ((void(*)(uintptr_t, uintptr_t))(g_libGTASA + 0x75F178))(m_camera, m_frame);
    // RwCameraSetFarClipPlane
    ((void(*)(uintptr_t, float))(g_libGTASA + 0x755DF8))(m_camera, 300.0f);
    // RwCameraSetNearClipPlane
    ((void(*)(uintptr_t, float))(g_libGTASA + 0x755D48))(m_camera, 0.01f);
    // RwCameraSetViewWindow
    float view[2] = { 0.5f, 0.5f };
    ((void(*)(uintptr_t, float*))(g_libGTASA + 0x756088))(m_camera, view);

    // RwCameraSetProjection
    ((void(*)(uintptr_t, int))(g_libGTASA + 0x755F78))(m_camera, 1);

    // RpWorldAddCamera
    uintptr_t pRwWorld = *(uintptr_t*)(g_libGTASA + 0xCB7118);
    if (pRwWorld) {
        ((void(*)(uintptr_t, uintptr_t))(g_libGTASA + 0x7450BC))(pRwWorld, m_camera);
    }
}

void MaterialTextGenerator::Render(const char* text, const ImVec2& size, int font_size, bool bold,
                                   const ImColor& font_color, const ImColor& background_color, int text_align)
{
    if (pUI) {
        ImGui::NewFrame();

        auto info = Encoding::cp2utf(text).c_str();

        pUI->renderer()->drawRect(ImVec2(0.0f, 0.0f), size, background_color, true);
        ImVec2 text_size = pUI->renderer()->calculateTextSize(info, font_size);

        ImVec2 text_pos;
        switch (text_align)
        {
            case 0: // LEFT
                text_pos = ImVec2(0.0f, 0.0f);
                break;
            case 1: // CENTER
                text_pos = ImVec2((size.x - text_size.x) / 2, (size.y - text_size.y) / 2);
                break;
            case 2: // RIGHT
                text_pos = ImVec2(size.x - text_size.x, 0.0f);
                break;
        }

        pUI->renderer()->drawText(text_pos, font_color, info, false, font_size);

        ImGui::Render();
        pUI->renderDrawData(ImGui::GetDrawData());
    }
}

const ImColor fix_color(uint32_t color)
{
    unsigned char a = (int)((color & 0xFF000000) >> 24);
    unsigned char r = (int)((color & 0x00FF0000) >> 16);
    unsigned char g = (int)((color & 0x0000FF00) >> 8);
    unsigned char b = (int)((color & 0x000000FF));

    //FLog("RGBA: 0x%X, 0x%X, 0x%X, 0x%X", r, g, b, a);
    ImColor col = ImColor(r, g, b, a);
    //FLog("color: %f, %f, %f, %f", col.Value.x, col.Value.y, col.Value.z, col.Value.w);
    return col;
}

uintptr_t MaterialTextGenerator::Generate(const char* text, int size, int font_size, bool bold, uint32_t font_color, uint32_t background_color, int alignment)
{
    int width, height;
    GetMaterialSize(size, &width, &height);

    // RwRasterCreate
    uintptr_t raster = (uintptr_t)RwRasterCreate(width, height, 32, rwRASTERFORMAT8888 | rwRASTERTYPECAMERATEXTURE);
    // RwTextureCreate
    uintptr_t bufferTexture = ((uintptr_t(*)(uintptr_t))(g_libGTASA + 0x75D2E4))(raster);

    if (raster && bufferTexture)
    {
        *(uintptr_t*)(m_camera + 0x80) = raster;

        // CVisibilityPlugins::SetRenderWareCamera
        ((void(*)(uintptr_t))(g_libGTASA + 0x5E7D50))(m_camera);

        // RwCameraClear
        ((void(*)(uintptr_t, uint32_t*, int))(g_libGTASA + 0x755F14))(m_camera, &background_color, 3);

        RwCameraBeginUpdate((RwCamera*)m_camera);
        // DefinedState2d
        ((void(*) (void))(g_libGTASA + 0x5E18D4))();

        Render(text, ImVec2(width, height), font_size, bold, fix_color(font_color), fix_color(background_color), alignment);

        RwCameraEndUpdate((RwCamera*)m_camera);
    }

    return bufferTexture;
}

void MaterialTextGenerator::GetMaterialSize(int size, int* x, int* y)
{
    int sizes[14][2] = {
            { 32, 32 } , { 64, 32 }, { 64, 64 }, { 128, 32 }, { 128, 64 }, { 128,128 }, { 256, 32 },
            { 256, 64 } , { 256, 128 } , { 256, 256 } , { 512, 64 } , { 512,128 } , { 512,256 } , { 512,512 }
    };

    int del = (size / 10) - 1;
    *x = sizes[del][0];
    *y = sizes[del][1];
}