//
// Created by x1y2z on 25.04.2024.
//

#include "CustomBuildingRenderer.h"
#include "../vendor/armhook/patch.h"

bool CCustomBuildingRenderer::Initialise() {
    return CHook::CallFunction<bool>("_ZN23CCustomBuildingRenderer10InitialiseEv");
}

void CCustomBuildingRenderer::Update() {
    CHook::CallFunction<void>(g_libGTASA + 0x35DF68);
}
