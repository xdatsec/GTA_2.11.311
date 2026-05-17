//
// Created by Gor on 1/4/2025.
//

#include "COcclusion.h"
#include "patch.h"

void COcclusion::InjectHooks()
{
    CHook::Write(g_libGTASA+0x83C410, &aOccluders); // 0xA41140 0xCE3EE8 old
    CHook::Write(g_libGTASA+0x83C3D8, &NumOccludersOnMap); // 0xA45790 0xCE8538 old
}