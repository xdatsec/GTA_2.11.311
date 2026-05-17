//
// Created by x1y2z on 15.11.2023.
//

#include "Renderer.h"
#include "../vendor/armhook/patch.h"

void CRenderer::InjectHooks() {
    CHook::Write(g_libGTASA + 0x838338, &ms_bRenderOutsideTunnels);
    CHook::Write(g_libGTASA + 0x838348, &m_loadingPriority);

    CHook::Write(g_libGTASA + 0x83A488, &ms_aVisibleEntityPtrs);
    CHook::Write(g_libGTASA + 0x83A438, &ms_nNoOfVisibleEntities);
}
