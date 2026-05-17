//
// Created by x1y2z on 11.04.2023.
//

#include "RenderWare.h"
#include "game/common.h"
#include "../vendor/armhook/patch.h"

RpClump* RpClumpForAllAtomics(RpClump* clump, RpAtomicCallBack callback, void* data) {
    if(clump)
        return CHook::CallFunction<RpClump*>(g_libGTASA + 0x746E20, clump, callback, data);
}

RpGeometry* RpGeometryForAllMaterials(RpGeometry* geometry, RpMaterialCallBack fpCallBack, void* data) {
    return CHook::CallFunction<RpGeometry*>(g_libGTASA + 0x749D7C, geometry, fpCallBack, data);
}

RwBool RpClumpDestroy(RpClump* clump) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x7478C4, clump);
}

RpClump* RpClumpRender(RpClump* clump) {
    return CHook::CallFunction<RpClump*>("_Z13RpClumpRenderP7RpClump", clump);
}

RpLight* RpLightCreate(RwInt32 type) {
    return CHook::CallFunction<RpLight*>(g_libGTASA + 0x74B110, type);
}

RwBool RpLightDestroy(RpLight* light) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x74B2C4, light);
}

RpWorld* RpWorldCreate(RwBBox* boundingBox) {
    return CHook::CallFunction<RpWorld*>(g_libGTASA + 0x7537C8, boundingBox);
}

RpWorld* RpWorldAddCamera(RpWorld* world, RwCamera* camera) {
    return CHook::CallFunction<RpWorld*>(g_libGTASA + 0x754A30, world, camera);
}

RpLight* RpLightSetColor(RpLight* light, const RwRGBAReal* color) {
    return CHook::CallFunction<RpLight*>(g_libGTASA + 0x74A96C, light, color);
}

RpAtomic* AtomicDefaultRenderCallBack(RpAtomic* atomic) {
    return CHook::CallFunction<RpAtomic*>(g_libGTASA + 0x7468FC, atomic);
}

RpWorld* RpWorldAddLight(RpWorld* world, RpLight* light) {
    return CHook::CallFunction<RpWorld*>(g_libGTASA + 0x7557EC, world, light);
}

RpWorld* RpWorldRemoveLight(RpWorld* world, RpLight* light) {
    return CHook::CallFunction<RpWorld*>(g_libGTASA + 0x755858, world, light);
}

RwBool RpAtomicDestroy(RpAtomic* atomic) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x74731C, atomic);
}

void RpClumpGtaCancelStream() {
    CHook::CallFunction<void>(g_libGTASA + 0x5FF320);
}