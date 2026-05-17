//
// Created by kuzia15 on 14.07.2025.
//

#include "MobileSettings.h"
#include "patch.h"

void CMobileSettings::InjectHooks() {
    CHook::Write(g_libGTASA + 0x8373D0, &ms_MobileSettings);
}