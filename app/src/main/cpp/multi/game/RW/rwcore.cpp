#include "rwcore.h"
#include "rpworld.h"
#include "../vendor/armhook/patch.h"
#include <assert.h>

RwCamera* RwCameraEndUpdate(RwCamera* camera) {
    return CHook::CallFunction<RwCamera*>(g_libGTASA + 0x755D04, camera);
}

RwFrame* RwFrameForAllObjects(RwFrame* frame, RwObjectCallBack callBack, void* data) {
    return CHook::CallFunction<RwFrame*>(g_libGTASA + 0x759490, frame, callBack, data);
}

RwFrame* RwFrameUpdateObjects(RwFrame* frame) {
    return CHook::CallFunction<RwFrame*>(g_libGTASA + 0x758848, frame);
}

RwTexture* RwTextureCreate(RwRaster* raster) {
    return CHook::CallFunction<RwTexture*>(g_libGTASA + 0x75D2E4, raster);
}

RwCamera* RwCameraCreate() {
    return CHook::CallFunction<RwCamera*>(g_libGTASA + 0x756184);
}

RwCamera* RwCameraCreate2(RwInt32 width, RwInt32 height, RwBool zBuffer) {
    return CHook::CallFunction<RwCamera*>(g_libGTASA + 0x53EBFC, width,height, zBuffer);
}


RwFrame* RwFrameCreate() {
    return CHook::CallFunction<RwFrame*>(g_libGTASA + 0x758A7C);
}

RwCamera* RwCameraClear(RwCamera* camera, RwRGBA* colour, RwInt32 clearMode) {
    return CHook::CallFunction<RwCamera*>(g_libGTASA + 0x755F14, camera, colour, clearMode);
}

RwCamera* RwCameraSetNearClipPlane(RwCamera* camera, RwReal nearClip) {
    return CHook::CallFunction<RwCamera*>(g_libGTASA + 0x755D48, camera, nearClip);
}

RwCamera* RwCameraSetFarClipPlane(RwCamera* camera, RwReal farClip) {
    return CHook::CallFunction<RwCamera*>(g_libGTASA + 0x755DF8, camera, farClip);
}

RwFrame* RwFrameTranslate(RwFrame* frame, const RwV3d* v, RwOpCombineType combine) {
    return CHook::CallFunction<RwFrame*>(g_libGTASA + 0x759120, frame, v, combine);
}

RwFrame* RwFrameRotate(RwFrame* frame, const RwV3d* axis, RwReal angle, RwOpCombineType combine) {
    return CHook::CallFunction<RwFrame*>(g_libGTASA + 0x7592C4, frame, axis, angle, combine);
}

RwCamera* RwCameraSetViewWindow(RwCamera* camera, const RwV2d* viewWindow) {
    return CHook::CallFunction<RwCamera*>(g_libGTASA + 0x756088, camera, viewWindow);
}

RwCamera* RwCameraSetProjection(RwCamera* camera, RwCameraProjection projection) {
    return CHook::CallFunction<RwCamera*>(g_libGTASA + 0x755F78, camera, projection);
}

void _rwObjectHasFrameSetFrame(void *object, RwFrame *frame) {
    CHook::CallFunction<void>(g_libGTASA + 0x75F178, object, frame);
}

RwMatrix* RwFrameGetLTM(RwFrame* frame) {
    return CHook::CallFunction<RwMatrix*>(g_libGTASA + 0x758EE0, frame);
}


RwBool RwIm3DEnd() {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x7468C0);
}

RwBool RwIm3DRenderPrimitive(RwPrimitiveType primType) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x746AB4, primType);
}

RwBool RwIm3DRenderIndexedPrimitive(RwPrimitiveType primType, RwImVertexIndex* indices, RwInt32 numIndices) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x746910, primType, indices, numIndices);
}

void* RwIm3DTransform(RwIm3DVertex* pVerts, RwUInt32 numVerts, RwMatrix* ltm, RwUInt32 flags) {
    return CHook::CallFunction<void*>(g_libGTASA + 0x7467B8, pVerts, numVerts, ltm, flags);
}

RwTexture* RwTextureRead(const char* name, const char* maskName) {
    return CHook::CallFunction<RwTexture*>(g_libGTASA + 0x75D68C, name, maskName);
}



RwBool RwFrameDestroy(RwFrame* frame) {
    assert(frame);
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x758DE0, frame);
}

RwTexture* RwTextureSetRaster(RwTexture* texture, RwRaster* raster) {
    return CHook::CallFunction<RwTexture*>(g_libGTASA + 0x758DE0, texture, raster);
}

RwBool RwCameraDestroy(RwCamera* camera) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x756128, camera);
}

RwBool RwIm3DRenderLine(RwInt32 vert1, RwInt32 vert2) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x746D80, vert1, vert2);
}

RwTexture* RwTextureSetName(RwTexture* texture, const RwChar* name) {
    return CHook::CallFunction<RwTexture*>(g_libGTASA + 0x75D374, texture, name);
}

RwFrame* RwFrameOrthoNormalize(RwFrame* frame) {
    return CHook::CallFunction<RwFrame*>(g_libGTASA + 0x759400, frame);
}

RwBool RwTextureSetFindCallBack(RwTextureCallBackFind callBack) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x75CE1C, callBack);
}

RwBool RwTextureSetReadCallBack(RwTextureCallBackRead callBack) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x75CE64, callBack);
}