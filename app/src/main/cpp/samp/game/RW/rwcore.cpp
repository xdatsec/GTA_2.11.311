//
// Created by x1y2z on 20.04.2023.
//
#include "rwcore.h"
#include "rpworld.h"
#include "../vendor/armhook/patch.h"
#include <assert.h>

RwFrame* RwFrameUpdateObjects(RwFrame* frame) {
    return CHook::CallFunction<RwFrame*>(g_libGTASA + 0x7681B0, frame);
}

RwTexture* RwTextureCreate(RwRaster* raster) {
    return CHook::CallFunction<RwTexture*>(g_libGTASA + 0x76CAC4, raster);
}

RwCamera* RwCameraCreate() {
    return CHook::CallFunction<RwCamera*>(g_libGTASA + 0x765B90);
}

RwFrame* RwFrameCreate() {
    return CHook::CallFunction<RwFrame*>(g_libGTASA + 0x7683E4);
}

RwCamera* RwCameraClear(RwCamera* camera, RwRGBA* colour, RwInt32 clearMode) {
    return CHook::CallFunction<RwCamera*>(g_libGTASA + 0x26D1E8, camera, colour, clearMode);
}

RwCamera* RwCameraSetNearClipPlane(RwCamera* camera, RwReal nearClip) {
    return CHook::CallFunction<RwCamera*>(g_libGTASA + 0x7656B8, camera, nearClip);
}

RwCamera* RwCameraSetFarClipPlane(RwCamera* camera, RwReal farClip) {
    return CHook::CallFunction<RwCamera*>(g_libGTASA + 0x765764, camera, farClip);
}

RwFrame* RwFrameTranslate(RwFrame* frame, const RwV3d* v, RwOpCombineType combine) {
    return CHook::CallFunction<RwFrame*>(g_libGTASA + 0x768AA0, frame, v, combine);
}

RwFrame* RwFrameRotate(RwFrame* frame, const RwV3d* axis, RwReal angle, RwOpCombineType combine) {
    return CHook::CallFunction<RwFrame*>(g_libGTASA + 0x768C44, frame, axis, angle, combine);
}

RwCamera* RwCameraSetViewWindow(RwCamera* camera, const RwV2d* viewWindow) {
    return CHook::CallFunction<RwCamera*>(g_libGTASA + 0x765A90, camera, viewWindow);
}

RwCamera* RwCameraSetProjection(RwCamera* camera, RwCameraProjection projection) {
    return CHook::CallFunction<RwCamera*>(g_libGTASA + 0x765980, camera, projection);
}

void _rwObjectHasFrameSetFrame(void *object, RwFrame *frame) {
    CHook::CallFunction<void>(g_libGTASA + 0x76E828, object, frame);
}

RwMatrix* RwFrameGetLTM(RwFrame* frame) {
    return CHook::CallFunction<RwMatrix*>(g_libGTASA + 0x768858, frame);
}

RwCamera* RwCameraEndUpdate(RwCamera* camera) {
    return CHook::CallFunction<RwCamera*>(g_libGTASA + 0x765670, camera);
}

RwBool RwIm3DEnd() {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x7563EC);
}

RwBool RwIm3DRenderPrimitive(RwPrimitiveType primType) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x7565E0, primType);
}

RwBool RwIm3DRenderIndexedPrimitive(RwPrimitiveType primType, RwImVertexIndex* indices, RwInt32 numIndices) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x756438, primType, indices, numIndices);
}

void* RwIm3DTransform(RwIm3DVertex* pVerts, RwUInt32 numVerts, RwMatrix* ltm, RwUInt32 flags) {
    return CHook::CallFunction<void*>(g_libGTASA + 0x7562EC, pVerts, numVerts, ltm, flags);
}

RwTexture* RwTextureRead(const char* name, const char* maskName) {
    return CHook::CallFunction<RwTexture*>(g_libGTASA + 0x76CE70, name, maskName);
}

RwFrame* RwFrameForAllObjects(RwFrame* frame, RwObjectCallBack callBack, void* data) {
    return CHook::CallFunction<RwFrame*>(g_libGTASA + 0x768E10, frame, callBack, data);
}

RwBool RwFrameDestroy(RwFrame* frame) {
    assert(frame);
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x768758, frame);
}

RwTexture* RwTextureSetRaster(RwTexture* texture, RwRaster* raster) {
    return CHook::CallFunction<RwTexture*>(g_libGTASA + 0x76C774, texture, raster);
}

RwBool RwCameraDestroy(RwCamera* camera) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x765B34, camera);
}

RwBool RwIm3DRenderLine(RwInt32 vert1, RwInt32 vert2) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x7568B0, vert1, vert2);
}

RwTexture* RwTextureSetName(RwTexture* texture, const RwChar* name) {
    return CHook::CallFunction<RwTexture*>(g_libGTASA + 0x76CB54, texture, name);
}

RwFrame* RwFrameOrthoNormalize(RwFrame* frame) {
    return CHook::CallFunction<RwFrame*>(g_libGTASA + 0x768D80, frame);
}

RwBool RwTextureSetFindCallBack(RwTextureCallBackFind callBack) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x76C600, callBack);
}

RwBool RwTextureSetReadCallBack(RwTextureCallBackRead callBack) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x76C648, callBack);
}