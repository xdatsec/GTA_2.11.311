#include "../../main.h"
#include "RenderWare.h"
#include "../vendor/armhook/patch.h"

// ========================================================
// STATIC GRAPHICS CONFIG (NO JAVA CONTROL ANYMORE)
// ========================================================

static struct GraphicsConfig {
    bool bloomEnabled = true;
    float bloomIntensity = 0.5f;

    bool hdrEnabled = false;

    float saturation = 1.0f;
    float contrast = 1.0f;
    float brightness = 1.0f;

    int preset = 1;
} g_Config;

struct Light {
    float x, y, z;
    int r, g, b;
    float intensity;
    float radius;
    bool active;
};

static Light g_Lights[64];
static int g_LightCount = 0;

static RwRaster* g_TempRaster = nullptr;
static RwRaster* g_BloomRaster = nullptr;
static bool g_Initialized = false;

// ========================================================
// COLOR CORRECTION
// ========================================================

void ApplyColorCorrection(RwRaster* raster) {
    if (!raster) return;

    RwUInt8* pixels = RwRasterLock(raster, 0, rwRASTERLOCKREADWRITE);
    if (!pixels) return;

    int width = RwRasterGetWidth(raster);
    int height = RwRasterGetHeight(raster);

    for (int i = 0; i < width * height * 4; i += 4) {
        float r = pixels[i] / 255.0f;
        float g = pixels[i+1] / 255.0f;
        float b = pixels[i+2] / 255.0f;

        r *= g_Config.brightness;
        g *= g_Config.brightness;
        b *= g_Config.brightness;

        r = (r - 0.5f) * g_Config.contrast + 0.5f;
        g = (g - 0.5f) * g_Config.contrast + 0.5f;
        b = (b - 0.5f) * g_Config.contrast + 0.5f;

        float gray = 0.299f*r + 0.587f*g + 0.114f*b;

        r = gray + g_Config.saturation*(r - gray);
        g = gray + g_Config.saturation*(g - gray);
        b = gray + g_Config.saturation*(b - gray);

        r = std::clamp(r, 0.0f, 1.0f);
        g = std::clamp(g, 0.0f, 1.0f);
        b = std::clamp(b, 0.0f, 1.0f);

        pixels[i]   = (RwUInt8)(r * 255);
        pixels[i+1] = (RwUInt8)(g * 255);
        pixels[i+2] = (RwUInt8)(b * 255);
    }

    RwRasterUnlock(raster);
}

// ========================================================
// BLOOM
// ========================================================

void ApplyBlur(RwRaster* src, RwRaster* dst, int kernelSize);
void ApplyBloom(RwRaster* raster);

// ========================================================
// HDR
// ========================================================

void ApplyHDR(RwRaster* raster) {
    if (!raster) return;

    RwUInt8* pixels = RwRasterLock(raster, 0, rwRASTERLOCKREADWRITE);
    if (!pixels) return;

    int w = RwRasterGetWidth(raster);
    int h = RwRasterGetHeight(raster);

    for (int i = 0; i < w*h*4; i += 4) {
        float r = pixels[i]   / 255.0f;
        float g = pixels[i+1] / 255.0f;
        float b = pixels[i+2] / 255.0f;

        r = r / (1.0f + r);
        g = g / (1.0f + g);
        b = b / (1.0f + b);

        pixels[i]   = (RwUInt8)(r * 255);
        pixels[i+1] = (RwUInt8)(g * 255);
        pixels[i+2] = (RwUInt8)(b * 255);
    }

    RwRasterUnlock(raster);
}

// ========================================================
// PROCESS
// ========================================================

// ========================================================
// HOOK
// ========================================================

RwCamera* (*RwCameraShowRaster_Original)(RwCamera*, void*, RwUInt32) = nullptr;

RwCamera* RwCameraShowRaster_Hook(RwCamera* cam, void* dev, RwUInt32 flags)
{
    if (g_Initialized && cam && cam->frameBuffer)
    {
        RwRaster* raster = cam->frameBuffer;

        //if (g_Config.bloomEnabled)
         //   ApplyBloom(raster);

        if (g_Config.hdrEnabled)
            ApplyHDR(raster);

        if (g_Config.saturation != 1.0f ||
            g_Config.contrast   != 1.0f ||
            g_Config.brightness != 1.0f)
            ApplyColorCorrection(raster);
    }

    return RwCameraShowRaster_Original(cam, dev, flags);
}


// ========================================================
// INITIALIZE GRAPHICS SYSTEM  (NO JAVA CONTROL)
// ========================================================

void InitializeGraphicsSystem() {
    if (g_Initialized) return;

    FLog("Initializing Graphics System...");

    if (RsGlobal) {
        int w = RsGlobal->maximumWidth;
        int h = RsGlobal->maximumHeight;

        g_TempRaster  = RwRasterCreate(w, h,     32, rwRASTERTYPECAMERATEXTURE);
        g_BloomRaster = RwRasterCreate(w/2, h/2, 32, rwRASTERTYPECAMERATEXTURE);
    }

    RwCameraShowRaster_Original = RwCameraShowRaster;
    RwCameraShowRaster = RwCameraShowRaster_Hook;


    g_Initialized = true;

    FLog("Graphics System initialized OK");
}

