#include "../main.h"
#include "../game/game.h"
#include "../vendor/armhook/patch.h"
#include "vehicleColoursTable.h"
#include "../settings.h"

///skip for now later if above is already converted
extern CSettings* pSettings;

VehicleAudioPropertiesStruct VehicleAudioProperties[20000];
#include "game.h"
#include "World.h"
#include "net/netgame.h"

extern CGame* pGame;

void ApplyFPSPatch(uint8_t fps)
{
    uint8_t targetFPS = 120;


    CHook::WriteMemory(g_libGTASA + 0x3685B8 + 1, &targetFPS, 1);
    CHook::WriteMemory(g_libGTASA + 0x368644 + 1, &targetFPS, 1);
    CHook::WriteMemory(g_libGTASA + 0x368924 + 1, &targetFPS, 1);

    FLog("New fps limit = %d", targetFPS);
}

void DisableAutoAim()
{
    CHook::RET("_ZN10CPlayerPed22FindWeaponLockOnTargetEv"); // CPed::FindWeaponLockOnTarget
    CHook::RET("_ZN10CPlayerPed26FindNextWeaponLockOnTargetEP7CEntityb"); // CPed::FindNextWeaponLockOnTarget
    CHook::RET("_ZN4CPed21SetWeaponLockOnTargetEP7CEntity"); // CPed::SetWeaponLockOnTarget
}

void ApplySAMPPatchesInGame()
{
    FLog("Installing patches (ingame)..");

    /* Разблокировка карты */
    // CTheZones::ZonesVisited[100]
    memset((void*)(g_libGTASA + 0xA59D7A), 1, 100);
    // CTheZones::ZonesRevealed
    *(uint32_t*)(g_libGTASA + 0xA59DE0) = 100;

    // CPlayerPed::CPlayerPed task fix
    CHook::WriteMemory(g_libGTASA + 0x5B5510, (uintptr_t)"\x34\x00\x80\x52", 4);

    // radar draw blips
    CHook::NOP(g_libGTASA + (VER_x32 ? 0x0043FE5A : 0x51A314), 2);
    CHook::NOP(g_libGTASA + (VER_x32 ? 0x004409AE : 0x51B11C), 2);

    CHook::RET("_ZN4CPed31RemoveWeaponWhenEnteringVehicleEi"); // CPed::RemoveWeaponWhenEnteringVehicle

//	CHook::WriteMemory(g_libGTASA + 0x00341F84, (uintptr_t)"\x00\xF0\x21\xBE", 4);

    // no vehicle audio processing
    CHook::NOP(g_libGTASA + 0x6A7834, 2);
    CHook::NOP(g_libGTASA + 0x6B69B8, 2);
    CHook::NOP(g_libGTASA + 0x6C183C, 2);

    // Disable in-game radio
    CHook::RET("_ZN20CAERadioTrackManager7ServiceEi");

    // карта в меню
    //CHook::NOP(g_libGTASA + 0x71DBB8, 2; // текст легенды карты
    //CHook::NOP(g_libGTASA + ( 0x71DBC8, 2); // значки легенды
   // CHook::NOP(g_libGTASA + : 0x71D59C, 2); // название местности
}

int32_t CWorld__FindPlayerSlotWithPedPointer(CPedGTA* pPlayersPed)
{
    // Оптимизация - быстрый поиск
    static CPedGTA* lastPed = nullptr;
    static int32_t lastIndex = -1;

    if(lastPed == pPlayersPed && lastIndex != -1)
        return lastIndex;

    for(int i = 0; i < MAX_PLAYERS; ++i)
    {
        if(CWorld::Players[i].m_pPed == pPlayersPed)
        {
            lastPed = pPlayersPed;
            lastIndex = i;
            return i;
        }
    }

    lastPed = nullptr;
    lastIndex = -1;
    return -1;
}
CPlayerInfoGta * GetPlayerInfoForThisPlayerPed(CPedGTA *thiz)
{
    CPlayerInfoGta *v1; // x8
    v1 = CWorld::Players;

    return v1;

}
void ApplyPatches_level0()
{
    FLog("ApplyPatches_level0");

    CHook::Write(g_libGTASA + 0x837358, &CWorld::Players);

    CHook::Redirect("_ZN6CWorld28FindPlayerSlotWithPedPointerEPv", &CWorld__FindPlayerSlotWithPedPointer);
    CHook::Redirect("_ZN10CPlayerPed29GetPlayerInfoForThisPlayerPedEv", &GetPlayerInfoForThisPlayerPed);


    CHook::WriteMemory(g_libGTASA + 0x77584C, (uintptr_t)"\x22\x00\x80\x52", 4);


/*
#if VER_x32
    CHook::WriteMemory(g_libGTASA + 0x002D9724, (uintptr_t)0x6061F44F, 4);
#endif */

    DisableAutoAim();

    //CHook::RET("_ZN6CTrain10InitTrainsEv"); // CTrain::InitTrains

    //CHook::RET("_ZN8CClothes4InitEv"); // CClothes::Init()/
    //CHook::RET("_ZN8CClothes13RebuildPlayerEP10CPlayerPedb"); // CClothes::RebuildPlayer/

    CHook::RET("_ZNK35CPedGroupDefaultTaskAllocatorRandom20AllocateDefaultTasksEP9CPedGroupP4CPed"); // AllocateDefaultTasks
    CHook::RET("_ZN6CGlass4InitEv"); // CGlass::Init
    CHook::RET("_ZN8CGarages17Init_AfterRestartEv"); // CGarages::Init_AfterRestart
    CHook::RET("_ZN6CGangs10InitialiseEv"); // CGangs::Initialise
    CHook::RET("_ZN5CHeli9InitHelisEv"); // CHeli::InitHelis(void)
    CHook::RET("_ZN11CFileLoader10LoadPickupEPKc"); // CFileLoader::LoadPickup
    CHook::RET("_ZN14CLoadingScreen15DisplayPCScreenEv"); // Loading screen

    // entryexit
    //CHook::RET("_ZN17CEntryExitManager4InitEv");
    //CHook::RET("_ZN17CEntryExitManager22PostEntryExitsCreationEv");

    CHook::RET("_ZN10CSkidmarks6UpdateEv"); // CSkidmarks::Update
    CHook::RET("_ZN10CSkidmarks6RenderEv"); // CSkidmarks::Render

    //CHook::RET("_ZN14SurfaceInfos_c17CreatesWheelGrassEj"); // SurfaceInfos_c::CreatesWheelGrass
    //CHook::RET("_ZN14SurfaceInfos_c18CreatesWheelGravelEj"); // SurfaceInfos_c::CreatesWheelGravel
    //CHook::RET("_ZN14SurfaceInfos_c15CreatesWheelMudEj"); // SurfaceInfos_c::CreatesWheelMud
    //CHook::RET("_ZN14SurfaceInfos_c16CreatesWheelDustEj"); // SurfaceInfos_c::CreatesWheelDust
    //CHook::RET("_ZN14SurfaceInfos_c16CreatesWheelSandEj"); // SurfaceInfos_c::CreatesWheelSand
    //CHook::RET("_ZN14SurfaceInfos_c17CreatesWheelSprayEj"); // SurfaceInfos_c::CreatesWheelSpray

    //CHook::RET("_ZN4Fx_c13AddWheelGrassEP8CVehicle7CVectorhf"); // Fx_c::AddWheelGrass
    //CHook::RET("_ZN4Fx_c14AddWheelGravelEP8CVehicle7CVectorhf"); // Fx_c::AddWheelGravel
    //CHook::RET("_ZN4Fx_c11AddWheelMudEP8CVehicle7CVectorhf"); // Fx_c::AddWheelMud
    //CHook::RET("_ZN4Fx_c12AddWheelDustEP8CVehicle7CVectorhf"); // Fx_c::AddWheelDust
    //CHook::RET("_ZN4Fx_c12AddWheelSandEP8CVehicle7CVectorhf"); // Fx_c::AddWheelSand
    //CHook::RET("_ZN4Fx_c13AddWheelSprayEP8CVehicle7CVectorhhf"); // Fx_c::AddWheelSpray

    CHook::RET("_ZN11CPlayerInfo14LoadPlayerSkinEv");
    CHook::RET("_ZN11CPopulation10InitialiseEv");

    // fix skin vertices a lot, and it caused caching in RenderQueue.
    //missing renderqueu have been reworked
    //CHook::WriteMemory(g_libGTASA + 0x266FC8, (uintptr_t)"\x15\x80\xA0\x52", 4);
}

void ApplyGlobalPatches()
{
    FLog("Installing patches..");

    CHook::NOP(g_libGTASA + 0x716AD0, 1);

    // vehicle reflection wrong color
    CHook::NOP(g_libGTASA + 0x5F3B14, 1);

    CHook::RET("_ZN17CVehicleModelInfo17SetCarCustomPlateEv"); // default plate

    CHook::RET("_Z16SaveGameForPause10eSaveTypesPc"); // не сохранять при сворачивании. черный экран

    // черные значки
   // CHook::WriteMemory(g_libGTASA + 0x51C8B0, (uintptr_t)"\x1E\x00\x00\x14", 4); // B 0x5273F4
  //  CHook::WriteMemory(g_libGTASA + 0x51C974, (uintptr_t)"\xE1\x03\x14\x2A", 4); // mov w1, w20

    //missing n 2.11.311
    // CRadar::DrawEntityBlip (translate color)
    //CHook::WriteMemory(g_libGTASA + 0x5258D8, (uintptr_t)"\x22\x00\x00\x14", 4); // B 0x525960
    //CHook::WriteMemory(g_libGTASA + 0x525960, (uintptr_t)"\xE1\x03\x16\x2A", 4); // mov w1, W22

    // CRadar::DrawCoordBlip (translate color)
   // CHook::WriteMemory(g_libGTASA + 0x524F58, (uintptr_t)"\xCC\xFF\xFF\x17", 4); // B 0x524E88
    //CHook::WriteMemory(g_libGTASA + 0x524E88, (uintptr_t)"\xE1\x03\x16\x2A", 4); // mov w1, W22
    ////CHook::WriteMemory(g_libGTASA + 0x002AB5C6, (uintptr_t)"\x00\x21", 2);

    // crash legend
    CHook::NOP(g_libGTASA + 0x71DB60, 1);


    //ApplyShadowPatch();

    //CDebugInfo::ApplyDebugPatches();

    CHook::RET("_ZN12CAudioEngine16StartLoadingTuneEv"); // звук загрузочного экрана

    // DefaultPCSaveFileName
    char* DefaultPCSaveFileName = (char*)(g_libGTASA + 0x8839B0);
    memcpy(DefaultPCSaveFileName, "GTASAMP", 8);


    CHook::NOP(g_libGTASA + 0x498718, 1);  // CCoronas::RenderSunReflection crash
    CHook::NOP(g_libGTASA + 0x6DC14C, 1);  // не давать ган при выходе из тачки   ( клюшка, дробовик and etc )
    CHook::NOP(g_libGTASA + 0x6DC100, 1);  // не давать ган при выходе из тачки  ( клюшка, дробовик and etc )


    CHook::RET("_ZN17CVehicleRecording4LoadEP8RwStreamii"); // CVehicleRecording::Load

    CHook::RET("_ZN18CMotionBlurStreaks6UpdateEv");
    CHook::RET("_ZN7CCamera16RenderMotionBlurEv");

    CHook::RET("_ZN11CPlayerInfo17FindObjectToStealEP4CPed"); // Crash
    CHook::RET("_ZN26CAEGlobalWeaponAudioEntity21ServiceAmbientGunFireEv");	// CAEGlobalWeaponAudioEntity::ServiceAmbientGunFire
    CHook::RET("_ZN30CWidgetRegionSteeringSelection4DrawEv"); // CWidgetRegionSteeringSelection::Draw
    CHook::RET("_ZN23CTaskSimplePlayerOnFoot18PlayIdleAnimationsEP10CPlayerPed");	// CTaskSimplePlayerOnFoot::PlayIdleAnimations
    CHook::RET("_ZN13CCarEnterExit17SetPedInCarDirectEP4CPedP8CVehicleib");	// CCarEnterExit::SetPedInCarDirect
    CHook::RET("_ZN6CRadar10DrawLegendEiii"); // CRadar::DrawLgegend
    CHook::RET("_ZN6CRadar19AddBlipToLegendListEhi"); // CRadar::AddBlipToLegendList

    CHook::RET("_ZN11CAutomobile35CustomCarPlate_BeforeRenderingStartEP17CVehicleModelInfo"); // CAutomobile::CustomCarPlate_BeforeRenderingStart
    CHook::RET("_ZN11CAutomobile33CustomCarPlate_AfterRenderingStopEP17CVehicleModelInfo"); // CAutomobile::CustomCarPlate_AfterRenderingStop
    CHook::RET("_ZN7CCamera8CamShakeEffff"); // CCamera::CamShake
    CHook::RET("_ZN7CEntity23PreRenderForGlassWindowEv"); // CEntity::PreRenderForGlassWindow
    CHook::RET("_ZN8CMirrors16RenderReflBufferEb"); // CMirrors::RenderReflBuffer
    CHook::RET("_ZN4CHud23DrawBustedWastedMessageEv"); // CHud::DrawBustedWastedMessage // ПОТРАЧЕНО
    CHook::RET(g_libGTASA+0x510A94); // CHud::SetHelpMessage
    CHook::RET("_ZN4CHud24SetHelpMessageStatUpdateEhtff"); // CHud::SetHelpMessageStatUpdate
    CHook::RET("_ZN6CCheat16ProcessCheatMenuEv"); // CCheat::ProcessCheatMenu
    CHook::RET("_ZN6CCheat13ProcessCheatsEv"); // CCheat::ProcessCheats
    CHook::RET("_ZN6CCheat16AddToCheatStringEc"); // CCheat::AddToCheatString
    CHook::RET("_ZN6CCheat12WeaponCheat1Ev"); // CCheat::WeaponCheat1
    CHook::RET("_ZN6CCheat12WeaponCheat2Ev"); // CCheat::WeaponCheat2
    CHook::RET("_ZN6CCheat12WeaponCheat3Ev"); // CCheat::WeaponCheat3
    CHook::RET("_ZN6CCheat12WeaponCheat4Ev"); // CCheat::WeaponCheat4
    CHook::RET("_ZN8CGarages14TriggerMessageEPcsts"); // CGarages::TriggerMessage

    CHook::RET("_ZN11CPopulation6AddPedE8ePedTypejRK7CVectorb"); // CPopulation::AddPed
    CHook::RET("_ZN6CPlane27DoPlaneGenerationAndRemovalEv"); // CPlane::DoPlaneGenerationAndRemoval

    CHook::RET("_ZN10CEntryExit19GenerateAmbientPedsERK7CVector"); // CEntryExit::GenerateAmbientPeds
    CHook::RET("_ZN8CCarCtrl31GenerateOneEmergencyServicesCarEj7CVector"); // CCarCtrl::GenerateOneEmergencyServicesCar
    CHook::RET("_ZN11CPopulation17AddPedAtAttractorEiP9C2dEffect7CVectorP7CEntityi"); // CPopulation::AddPedAtAttractor crash. wtf stuff?

    CHook::RET("_ZN7CDarkel26RegisterCarBlownUpByPlayerEP8CVehiclei"); // CDarkel__RegisterCarBlownUpByPlayer_hook
    CHook::RET("_ZN7CDarkel25ResetModelsKilledByPlayerEi"); // CDarkel__ResetModelsKilledByPlayer_hook
    CHook::RET("_ZN7CDarkel25QueryModelsKilledByPlayerEii"); // CDarkel__QueryModelsKilledByPlayer_hook
    CHook::RET("_ZN7CDarkel27FindTotalPedsKilledByPlayerEi"); // CDarkel__FindTotalPedsKilledByPlayer_hook
    CHook::RET("_ZN7CDarkel20RegisterKillByPlayerEPK4CPed11eWeaponTypebi"); // CDarkel__RegisterKillByPlayer_hook

    CHook::NOP(g_libGTASA + 0x555A08, 1);	// CStreaming::ms_memoryAvailable = (int)v24


   // CHook::NOP(g_libGTASA + 0x5B7C20, 1);  // CCamera::ClearPlayerWeaponMode from CPlayerPed::ClearWeaponTarget
    CHook::WriteMemory(g_libGTASA + 0x496200, "\x1F\x0D\x00\x71", 4); // RE3: Fix R* optimization that prevents peds to spawn

/*
 * //uncomplete
 * main address in 2.11.32 of openglaallinoneatomic is 0x7857F4
#if !VER_x32
    // openglSkinAllInOneAtomicInstanceCB
    CHook::Write32(g_libGTASA + 0x785A28, ARMv8::MOVBits::Create(1, 27, false));
    CHook::NOP(g_libGTASA + 0x25C28C, 1);
    CHook::Write32(g_libGTASA + 0x25C290, ARMv8::MOVBits::Create(1, 27, false));
#else
    CHook::WriteMemory(g_libGTASA + 0x1C8064, (uintptr_t)"\x01", 1);
    CHook::WriteMemory(g_libGTASA + 0x1C8082, (uintptr_t)"\x01", 1);
#endif

 */
    CHook::RET("_ZN10CPlayerPed14AnnoyPlayerPedEb"); // CPedSamp::AnnoyPlayerPed
    CHook::RET("_ZN11CPopulation15AddToPopulationEffff");    // CPopulation::AddToPopulation

    CHook::RET("_ZN23CAEPedSpeechAudioEntity11AddSayEventEisjfhhh"); // CPed::Say

    CHook::RET("_ZN10CPedGroups7ProcessEv"); // CPedGroups::Process
    CHook::RET("_ZN21CPedGroupIntelligence7ProcessEv"); // CPedGroupIntelligence::Process
    CHook::RET("_ZN19CPedGroupMembership9SetLeaderEP4CPed"); // CPedGroupMembership::SetLeader
    CHook::RET("_ZN21CPedGroupIntelligence5FlushEv"); // CPedGroupIntelligence::Flush

    CHook::RET("_ZN22CRealTimeShadowManager4InitEv"); // CRealTimeShadowManager::Init
    CHook::RET("_ZN22CRealTimeShadowManager6UpdateEv"); // CRealTimeShadowManager::Update

    CHook::RET("_ZN22CRealTimeShadowManager20ReturnRealTimeShadowEP15CRealTimeShadow"); // CRealTimeShadowManager::ReturnRealTimeShadow from ~CPhysical
    CHook::RET("_ZN8CShadows19RenderStaticShadowsEb"); // CShadows::RenderStaticShadows

    CHook::RET("_ZN8CMirrors16BeforeMainRenderEv"); // CMirrors::BeforeMainRender(void)
    CHook::RET("_ZN8CMirrors17RenderReflectionsEv"); // CMirrors::RenderReflections(void)

    CHook::RET("_ZN8CCarCtrl18GenerateRandomCarsEv"); // CCarCtrl::GenerateRandomCars(void)

    CHook::RET("_ZN10CGameLogic43SetPlayerWantedLevelForForbiddenTerritoriesEb"); // CGameLogic::SetPlayerWantedLevelForForbiddenTerritories

    CHook::RET("_ZN7CWanted14ReportCrimeNowE10eCrimeTypeRK7CVectorb"); // CWanted::ReportCrimeNow

    CHook::RET("_ZN8CCarCtrl18GenerateRandomCarsEv");
}

