//
// Created by x1y2z on 01.07.2023.
//

#include "ColStore.h"
#include "../vendor/armhook/patch.h"
#include "game/constants.h"

void CColStore::RequestCollision(const CVector *pos, int32 areaCode) {

}

void CColStore::RemoveCol(int32 colSlot)
{
   CHook::CallFunction<void>(g_libGTASA + 0x3C1E4C, colSlot);
}

void CColStore::AddCollisionNeededAtPosn(const CVector *pos) {
   CHook::CallFunction<void>(g_libGTASA + 0x3C2120, pos);
}

void CColStore::LoadCollision(CVector pos, bool bIgnorePlayerVeh)
{
   CHook::CallFunction<void>(g_libGTASA + 0x3C2340, pos, bIgnorePlayerVeh);
}

void CColStore::EnsureCollisionIsInMemory(const CVector* pos)
{
   CHook::CallFunction<void>(g_libGTASA + 0x3C26B8, pos);
}

void CColStore::Initialise()
{
    CHook::CallFunction<void>(g_libGTASA + 0x3C1828);
}