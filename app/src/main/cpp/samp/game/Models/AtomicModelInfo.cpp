//
// Created by x1y2z on 31.07.2023.
//

#include "AtomicModelInfo.h"
#include "../vendor/armhook/patch.h"

void SetAtomicModelInfoFlags(CAtomicModelInfo* modelInfo, uint32 dwFlags) {
    CHook::CallFunction<void>(g_libGTASA + 0x55478C, modelInfo, dwFlags);
}