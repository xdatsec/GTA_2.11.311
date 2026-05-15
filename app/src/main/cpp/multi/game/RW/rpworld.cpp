//
// Created by x1y2z on 11.04.2023.
//

#include "RenderWare.h"
#include "game/common.h"
#include "../vendor/armhook/patch.h"

RpClump* RpClumpForAllAtomics(RpClump* clump, RpAtomicCallBack callback, void* data) {
    if(clump)
        return CHook::CallFunction<RpClump*>(g_libGTASA + 0x736B98, clump, callback, data);
}

RpGeometry* RpGeometryForAllMaterials(RpGeometry* geometry, RpMaterialCallBack fpCallBack, void* data) {
    return CHook::CallFunction<RpGeometry*>(g_libGTASA + 0x739B44, geometry, fpCallBack, data);
}
RpGeometry* RpGeometryForAllMaterials2(RpGeometry* geometry, RpMaterialCallBack fpCallBack, void* data) {
    return CHook::CallFunction<RpGeometry*>(g_libGTASA + 0x739B44, geometry, fpCallBack, data);
}
RwBool RpClumpDestroy(RpClump* clump) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x73765C, clump);
}

RpClump* RpClumpRender(RpClump* clump) {
    return CHook::CallFunction<RpClump*>("_Z13RpClumpRenderP7RpClump", clump);
}

RpLight* RpLightCreate(RwInt32 type) {
    return CHook::CallFunction<RpLight*>(g_libGTASA + 0x73ADB4, type);
}

RwBool RpLightDestroy(RpLight* light) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x73AF6C, light);
}

RpWorld* RpWorldCreate(RwBBox* boundingBox) {
    return CHook::CallFunction<RpWorld*>(g_libGTASA + 0x743EE8, boundingBox);
}

RpWorld* RpWorldAddCamera(RpWorld* world, RwCamera* camera) {
    return CHook::CallFunction<RpWorld*>(g_libGTASA + 0x7450BC, world, camera);
}

RpLight* RpLightSetColor(RpLight* light, const RwRGBAReal* color) {
    return CHook::CallFunction<RpLight*>(g_libGTASA + 0x73A614, light, color);
}

RpAtomic* AtomicDefaultRenderCallBack(RpAtomic* atomic) {
    return CHook::CallFunction<RpAtomic*>(g_libGTASA + 0x736658, atomic);
}

RpWorld* RpWorldAddLight(RpWorld* world, RpLight* light) {
    return CHook::CallFunction<RpWorld*>(g_libGTASA + 0x745E54, world, light);
}

RpWorld* RpWorldRemoveLight(RpWorld* world, RpLight* light) {
    return CHook::CallFunction<RpWorld*>(g_libGTASA + 0x745EC0, world, light);
}

RwBool RpAtomicDestroy(RpAtomic* atomic) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x7370AC, atomic);
}

void RpClumpGtaCancelStream() {
    CHook::CallFunction<void>(g_libGTASA + 0x5E1754);
}