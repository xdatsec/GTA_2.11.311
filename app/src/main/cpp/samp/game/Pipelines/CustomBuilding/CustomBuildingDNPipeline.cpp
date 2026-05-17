//
// Created by x1y2z on 25.11.2023.
//

#include "CustomBuildingDNPipeline.h"
#include "../vendor/armhook/patch.h"

void CCustomBuildingDNPipeline::InjectHooks() {
    CHook::Write(g_libGTASA + 0x8375E0, &m_fDNBalanceParam);
}
