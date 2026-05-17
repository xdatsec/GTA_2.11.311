//
// Created by x1y2z on 13.05.2023.
//

#include "World.h"
#include "../vendor/armhook/patch.h"
#include "main.h"
#include "game/game.h"
#include "net/netgame.h"

//CRepeatSector CWorld::ms_aRepeatSectors[MAX_REPEAT_SECTORS_Y][MAX_REPEAT_SECTORS_X];
//CPtrListDoubleLink CWorld::ms_listMovingEntityPtrs;

CSector* GetSector(int32 x, int32 y) {
    static CSector(&ms_aSectors)[MAX_SECTORS_Y][MAX_SECTORS_X] = *(CSector(*)[MAX_SECTORS_Y][MAX_SECTORS_X])(g_libGTASA + 0xA1A908);

    const auto x1 = std::clamp<int32>(x, 0, MAX_SECTORS_X - 1);
    const auto y1 = std::clamp<int32>(y, 0, MAX_SECTORS_Y - 1);

    return &ms_aSectors[y1][x1];
}

CRepeatSector* GetRepeatSector(int32 x, int32 y) {
    static CRepeatSector(&ms_aRepeatSectors)[MAX_REPEAT_SECTORS_Y][MAX_REPEAT_SECTORS_X] = *(CRepeatSector(*)[MAX_REPEAT_SECTORS_Y][MAX_REPEAT_SECTORS_X])(g_libGTASA + 0xA52D08);

    return &ms_aRepeatSectors[y % MAX_REPEAT_SECTORS_Y][x % MAX_REPEAT_SECTORS_X];
}

void CWorld::InjectHooks() {

    CHook::InstallPLT(g_libGTASA + 0x8466D0, &ProcessPedsAfterPreRender);
}

bool CWorld::ProcessLineOfSight(const CVector* origin, const CVector* target, CColPoint* outColPoint, CEntityGTA** outEntity, bool buildings, bool vehicles, bool peds, bool objects, bool dummies, bool doSeeThroughCheck, bool doCameraIgnoreCheck, bool doShootThroughCheck) {
    // assert(!origin.HasNanOrInf() && !target.HasNanOrInf()); // We're getting random nan/inf's from somewhere, so let's try to root cause it...
    return CHook::CallFunction<bool>(g_libGTASA + 0x4C94BC, origin, target, outColPoint, outEntity, buildings, vehicles, peds, objects, dummies, doSeeThroughCheck, doCameraIgnoreCheck, doShootThroughCheck);
}
float CWorld::FindGroundZForCoord(float x, float y) {
    return CHook::CallFunction<float>(g_libGTASA + 0x4D0480, x, y);
}

bool CWorld::GetIsLineOfSightClear(const CVector& origin, const CVector& target, bool buildings, bool vehicles, bool peds, bool objects, bool dummies, bool doSeeThroughCheck, bool doCameraIgnoreCheck) {
    return CHook::CallFunction<bool>(g_libGTASA + 0x4C7E48, &origin, &target, buildings, vehicles, peds, objects, dummies, doSeeThroughCheck, doCameraIgnoreCheck);
}

void CWorld::Add(CEntityGTA *entity) {
    ((void(*)(CEntityGTA*))(g_libGTASA + 0x4C7DB8))(entity);
}

void CWorld::Remove(CEntityGTA *entity) {
    ((void (*)(CEntityGTA*))(g_libGTASA + 0x4C7B8C)) (entity);
}

extern CNetGame *pNetGame;
void CWorld::ProcessPedsAfterPreRender() {
    if (CTimer::bSkipProcessThisFrame)
        return;

    if (pNetGame)
    {
        CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
        if (pPlayerPool)
        {
            pPlayerPool->ProcessAttachedObjects();
        }
    }
}
