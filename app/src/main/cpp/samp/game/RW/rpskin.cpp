//
// Created by x1y2z on 20.04.2023.
//

#include "RenderWare.h"
#include "../vendor/armhook/patch.h"

RpSkin* RpSkinGeometryGetSkin(RpGeometry* geometry) {
    return CHook::CallFunction<RpSkin*>(g_libGTASA + 0x25DEA0, geometry);
}

RpHAnimHierarchy* RpSkinAtomicGetHAnimHierarchy(const RpAtomic* atomic) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x784CEC, atomic);
}
