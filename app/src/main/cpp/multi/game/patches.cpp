//
// Created by Dionisio on 5/9/2026.
//
#include "../main.h"
#include "../game/game.h"
#include "../vendor/armhook/patch.h"
#include "vehicleColoursTable.h"
#include "../settings.h"
#include "net/netgame.h"

extern CGame* pGame;


void ApplySAMPPatchesInGame()
{
    FLog("Installing patches (ingame)..");

    /* Ð Ð°Ð·Ð±Ð»Ð¾ÐºÐ¸Ñ€Ð¾Ð²ÐºÐ° ÐºÐ°Ñ€Ñ‚Ñ‹ */
    // CTheZones::ZonesVisited[100]
    memset((void*)(g_libGTASA + 0xA406BA), 1, 100);
    // CTheZones::ZonesRevealed
    *(uint32_t*)(g_libGTASA + 0xA40720) = 100;

    CHook::WriteMemory(g_libGTASA + 0x597CC0, (uintptr_t)"\x34\x00\x80\x52", 4);

    // radar draw blips
    CHook::NOP(g_libGTASA + 0x4FEF70, 2);
    CHook::NOP(g_libGTASA + 0x4FFD50, 2);

    CHook::RET("_ZN4CPed31RemoveWeaponWhenEnteringVehicleEi"); // CPed::RemoveWeaponWhenEnteringVehicle



    // Aggressively RETing automobile flying
    CHook::RET(g_libGTASA+0x693314); // CAutomobile::ProcessFlyingCarStuff
//	CHook::WriteMemory(g_libGTASA + 0x00341F84, (uintptr_t)"\x00\xF0\x21\xBE", 4);

    // no vehicle audio processing
    CHook::NOP(g_libGTASA + 0x69525C, 1); // CAutomobile
    CHook::NOP(g_libGTASA + 0x6A462C, 1); // CBike
    CHook::NOP(g_libGTASA + 0x6AF470, 1); // CBoat
    CHook::NOP(g_libGTASA + 0x6C23C4, 1); // CTrain

    // Disable in-game radio
    CHook::RET("_ZN20CAERadioTrackManager7ServiceEi");

    // ÐºÐ°Ñ€Ñ‚Ð° Ð² Ð¼ÐµÐ½ÑŽ
    CHook::NOP(g_libGTASA + 0x70C708, 2); // Ñ‚ÐµÐºÑ�Ñ‚ Ð»ÐµÐ³ÐµÐ½Ð´Ñ‹ ÐºÐ°Ñ€Ñ‚Ñ‹
    CHook::NOP(g_libGTASA + 0x70C718, 2); // Ð·Ð½Ð°Ñ‡ÐºÐ¸ Ð»ÐµÐ³ÐµÐ½Ð´Ñ‹
    CHook::NOP(g_libGTASA + 0x70C14C, 2); // Ð½Ð°Ð·Ð²Ð°Ð½Ð¸Ðµ Ð¼ÐµÑ�Ñ‚Ð½Ð¾Ñ�Ñ‚Ð¸
}
inline void* TryLoadAlphaFunc(const char* szlib)
{
    void* lib = dlopen(szlib, RTLD_LAZY);
    if(!lib) return NULL;

    void* sym = dlsym(lib, "glAlphaFuncQCOM");
    if(!sym) sym = dlsym(lib, "glAlphaFunc");
    return sym;
}
int32_t CWorld__FindPlayerSlotWithPedPointer(PED_TYPE* pPlayersPed)
{
    // ÐžÐ¿Ñ‚Ð¸Ð¼Ð¸Ð·Ð°Ñ†Ð¸Ñ� - Ð±Ñ‹Ñ�Ñ‚Ñ€Ñ‹Ð¹ Ð¿Ð¾Ð¸Ñ�Ðº
    static PED_TYPE* lastPed = nullptr;
    static int32_t lastIndex = -1;

    if(lastPed == pPlayersPed && lastIndex != -1)
        return lastIndex;

    for(int i = 0; i < MAX_PLAYERS; ++i)
    {
        if(Players[i].m_pPed == pPlayersPed)
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
    v1 = Players;

    return v1;

}
void ApplyPatches_level0()
{
    FLog("ApplyPatches_level0");

    /*
     mission in 2.11
    CHook::Write32(g_libGTASA + 0x278E50, 0x8B090108);
    CHook::Write32(g_libGTASA + 0x278E60, 0x8B090108);
    CHook::Write32(g_libGTASA + 0x278E70, 0xAA0903E9);
     */
    FLog("ApplyPatches_level1");
    CHook::Write(g_libGTASA + 0x821CC8, &Players);

    CHook::Redirect("_ZN6CWorld28FindPlayerSlotWithPedPointerEPv", &CWorld__FindPlayerSlotWithPedPointer);
    CHook::Redirect("_ZN10CPlayerPed29GetPlayerInfoForThisPlayerPedEv", &GetPlayerInfoForThisPlayerPed);
    //CHook::Redirect(g_libGTASA + 0x501C68, &ret);

    // CVehicleModelInfo::ms_vehicleColourTable

    void* sym;
    if((sym = TryLoadAlphaFunc("libGLESv2.so"))       || (sym = TryLoadAlphaFunc("libGLESv3.so"))    ||
       (sym = TryLoadAlphaFunc("libGLESv2_mtk.so"))   || (sym = TryLoadAlphaFunc("libGLES_mali.so")) ||
       (sym = TryLoadAlphaFunc("libGLES_android.so")) || (sym = TryLoadAlphaFunc("libGLES.so")))
    {
        *(void**)(g_libGTASA + 0xCFA580) = sym;
    }


    CHook::WriteMemory(g_libGTASA + 0x765524, (uintptr_t)"\x22\x00\x80\x52", 4);

    FLog("ApplyPatches_level2");

    CHook::NOP(g_libGTASA + 0x707BDC, 2);

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
    CHook::RET("_ZN17CEntryExitManager22PostEntryExitsCreationEv");

    CHook::RET("_ZN10CSkidmarks6UpdateEv"); // CSkidmarks::Update
    CHook::RET("_ZN10CSkidmarks6RenderEv"); // CSkidmarks::Render
    FLog("ApplyPatches_level3");
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

    // CHook::RET("_ZN11CPlayerInfo14LoadPlayerSkinEv");
    //CHook::RET("_ZN11CPopulation10InitialiseEv");


    // fix skin vertices a lot, and it caused caching in RenderQueue.
   // CHook::WriteMemory(g_libGTASA + 0x77D068, (uintptr_t)"\x15\x80\xA0\x52", 4);
    FLog("ApplyPatches_level4");
}

void ApplyGlobalPatches()
{

    FLog("Installing patches..");

    CHook::NOP(g_libGTASA + 0x705514, 1);

    // vehicle reflection wrong color
    CHook::NOP(g_libGTASA + 0x5D5C94, 1);

    CHook::RET("_ZN17CVehicleModelInfo17SetCarCustomPlateEv"); // default plate
    FLog("Installing patches.. 1");
    CHook::RET("_Z16SaveGameForPause10eSaveTypesPc"); // Ð½Ðµ Ñ�Ð¾Ñ…Ñ€Ð°Ð½Ñ�Ñ‚ÑŒ Ð¿Ñ€Ð¸ Ñ�Ð²Ð¾Ñ€Ð°Ñ‡Ð¸Ð²Ð°Ð½Ð¸Ð¸. Ñ‡ÐµÑ€Ð½Ñ‹Ð¹ Ñ�ÐºÑ€Ð°Ð½

/*
 * to ne added later
    // Ñ‡ÐµÑ€Ð½Ñ‹Ðµ Ð·Ð½Ð°Ñ‡ÐºÐ¸
    CHook::WriteMemory(g_libGTASA + 0x52737C, (uintptr_t)"\x1E\x00\x00\x14", 4); // B 0x5273F4
    CHook::WriteMemory(g_libGTASA + 0x5273F4, (uintptr_t)"\xE1\x03\x14\x2A", 4); // mov w1, w20

    // CRadar::DrawEntityBlip (translate color)
    CHook::WriteMemory(g_libGTASA + 0x5258D8, (uintptr_t)"\x22\x00\x00\x14", 4); // B 0x525960
    CHook::WriteMemory(g_libGTASA + 0x525960, (uintptr_t)"\xE1\x03\x16\x2A", 4); // mov w1, W22

    // CRadar::DrawCoordBlip (translate color)
    CHook::WriteMemory(g_libGTASA + 0x524F58, (uintptr_t)"\xCC\xFF\xFF\x17", 4); // B 0x524E88
    CHook::WriteMemory(g_libGTASA + 0x524E88, (uintptr_t)"\xE1\x03\x16\x2A", 4); // mov w1, W22
    CHook::WriteMemory(g_libGTASA + 0x002AB5C6, (uintptr_t)"\x00\x21", 2);
    */
    // crash legend
    CHook::NOP(g_libGTASA + 0x70C690, 1);


    //ApplyShadowPatch();

    //CDebugInfo::ApplyDebugPatches();
    FLog("Installing patches.. 2");
    //colorfix
    CHook::UnFuck(g_libGTASA + 0x822280);
    *(uintptr_t*)(g_libGTASA + 0x822280) = (uintptr_t)VehicleColoursTableRGBA;

    CHook::RET("_ZN12CAudioEngine16StartLoadingTuneEv"); // Ð·Ð²ÑƒÐº Ð·Ð°Ð³Ñ€ÑƒÐ·Ð¾Ñ‡Ð½Ð¾Ð³Ð¾ Ñ�ÐºÑ€Ð°Ð½Ð°

    // DefaultPCSaveFileName
    char* DefaultPCSaveFileName = (char*)(g_libGTASA + 0x86A350);
    memcpy(DefaultPCSaveFileName, "GTASAMP", 8);

    CHook::NOP(g_libGTASA + 0x47A2D8, 1);  // CCoronas::RenderSunReflection crash
    CHook::NOP(g_libGTASA + 0x6CAA00, 1);  // Ð½Ðµ Ð´Ð°Ð²Ð°Ñ‚ÑŒ Ð³Ð°Ð½ Ð¿Ñ€Ð¸ Ð²Ñ‹Ñ…Ð¾Ð´Ðµ Ð¸Ð· Ñ‚Ð°Ñ‡ÐºÐ¸   ( ÐºÐ»ÑŽÑˆÐºÐ°, Ð´Ñ€Ð¾Ð±Ð¾Ð²Ð¸Ðº and etc )
    CHook::NOP(g_libGTASA + 0x6CA9B4, 1);  // Ð½Ðµ Ð´Ð°Ð²Ð°Ñ‚ÑŒ Ð³Ð°Ð½ Ð¿Ñ€Ð¸ Ð²Ñ‹Ñ…Ð¾Ð´Ðµ Ð¸Ð· Ñ‚Ð°Ñ‡ÐºÐ¸  ( ÐºÐ»ÑŽÑˆÐºÐ°, Ð´Ñ€Ð¾Ð±Ð¾Ð²Ð¸Ðº and etc )


    CHook::RET("_ZN17CVehicleRecording4LoadEP8RwStreamii"); // CVehicleRecording::Load

    CHook::RET("_ZN18CMotionBlurStreaks6UpdateEv");
    CHook::RET("_ZN7CCamera16RenderMotionBlurEv");

    CHook::RET("_ZN11CPlayerInfo17FindObjectToStealEP4CPed"); // Crash
    CHook::RET("_ZN26CAEGlobalWeaponAudioEntity21ServiceAmbientGunFireEv");	// CAEGlobalWeaponAudioEntity::ServiceAmbientGunFire
    CHook::RET("_ZN30CWidgetRegionSteeringSelection4DrawEv"); // CWidgetRegionSteeringSelection::Draw
    CHook::RET("_ZN23CTaskSimplePlayerOnFoot18PlayIdleAnimationsEP10CPlayerPed");	// CTaskSimplePlayerOnFoot::PlayIdleAnimations
    FLog("Installing patches.. 3");
    // CPlayerPed::ProcessAnimGroups
    // CHook::NOP(g_libGTASA + 0x5C3970, 2);

    CHook::RET("_ZN13CCarEnterExit17SetPedInCarDirectEP4CPedP8CVehicleib");	// CCarEnterExit::SetPedInCarDirect
    CHook::RET("_ZN6CRadar10DrawLegendEiii"); // CRadar::DrawLgegend
    CHook::RET("_ZN6CRadar19AddBlipToLegendListEhi"); // CRadar::AddBlipToLegendList

    CHook::RET("_ZN11CAutomobile35CustomCarPlate_BeforeRenderingStartEP17CVehicleModelInfo"); // CAutomobile::CustomCarPlate_BeforeRenderingStart
    CHook::RET("_ZN11CAutomobile33CustomCarPlate_AfterRenderingStopEP17CVehicleModelInfo"); // CAutomobile::CustomCarPlate_AfterRenderingStop
    //CHook::RET("_ZN7CCamera8CamShakeEffff"); // CCamera::CamShake
    CHook::RET("_ZN7CEntity23PreRenderForGlassWindowEv"); // CEntity::PreRenderForGlassWindow
    CHook::RET("_ZN8CMirrors16RenderReflBufferEb"); // CMirrors::RenderReflBuffer
    CHook::RET("_ZN4CHud23DrawBustedWastedMessageEv"); // CHud::DrawBustedWastedMessage // ÐŸÐžÐ¢Ð Ð�Ð§Ð•Ð�Ðž
    CHook::RET(g_libGTASA+0x4F5744); // CHud::SetHelpMessage
    CHook::RET("_ZN4CHud24SetHelpMessageStatUpdateEhtff"); // CHud::SetHelpMessageStatUpdate
    CHook::RET("_ZN6CCheat16ProcessCheatMenuEv"); // CCheat::ProcessCheatMenu
    CHook::RET("_ZN6CCheat13ProcessCheatsEv"); // CCheat::ProcessCheats
    FLog("Installing patches.. 4");
    CHook::RET("_ZN6CCheat16AddToCheatStringEc"); // CCheat::AddToCheatString
    CHook::RET("_ZN6CCheat12WeaponCheat1Ev"); // CCheat::WeaponCheat1
    CHook::RET("_ZN6CCheat12WeaponCheat2Ev"); // CCheat::WeaponCheat2
    CHook::RET("_ZN6CCheat12WeaponCheat3Ev"); // CCheat::WeaponCheat3
    CHook::RET("_ZN6CCheat12WeaponCheat4Ev"); // CCheat::WeaponCheat4
    CHook::RET("_ZN8CGarages14TriggerMessageEPcsts"); // CGarages::TriggerMessage
    FLog("Installing patches.. 5");
    CHook::RET("_ZN11CPopulation6AddPedE8ePedTypejRK7CVectorb"); // CPopulation::AddPed
    CHook::RET("_ZN6CPlane27DoPlaneGenerationAndRemovalEv"); // CPlane::DoPlaneGenerationAndRemoval

    CHook::RET("_ZN10CEntryExit19GenerateAmbientPedsERK7CVector"); // CEntryExit::GenerateAmbientPeds
    CHook::RET("_ZN8CCarCtrl31GenerateOneEmergencyServicesCarEj7CVector"); // CCarCtrl::GenerateOneEmergencyServicesCar
    CHook::RET("_ZN11CPopulation17AddPedAtAttractorEiP9C2dEffect7CVectorP7CEntityi"); // CPopulation::AddPedAtAttractor crash. wtf stuff?
    FLog("Installing patches.. 6");
    CHook::RET("_ZN7CDarkel26RegisterCarBlownUpByPlayerEP8CVehiclei"); // CDarkel__RegisterCarBlownUpByPlayer_hook
    CHook::RET("_ZN7CDarkel25ResetModelsKilledByPlayerEi"); // CDarkel__ResetModelsKilledByPlayer_hook
    CHook::RET("_ZN7CDarkel25QueryModelsKilledByPlayerEii"); // CDarkel__QueryModelsKilledByPlayer_hook
    CHook::RET("_ZN7CDarkel27FindTotalPedsKilledByPlayerEi"); // CDarkel__FindTotalPedsKilledByPlayer_hook
    CHook::RET("_ZN7CDarkel20RegisterKillByPlayerEPK4CPed11eWeaponTypebi"); // CDarkel__RegisterKillByPlayer_hook

    CHook::NOP(g_libGTASA + 0x538C44, 1);	// CStreaming::ms_memoryAvailable = (int)v24
    FLog("Installing patches.. 7");

    CHook::NOP(g_libGTASA + 0x59A424, 1);  // CCamera::ClearPlayerWeaponMode from CPlayerPed::ClearWeaponTarget
    CHook::WriteMemory(g_libGTASA + 0x477DF4, "\x1F\x0D\x00\x71", 4); // RE3: Fix R* optimization that prevents peds to spawn



    // openglSkinAllInOneAtomicInstanceCB
    //got missing
    //CHook::Write32(g_libGTASA + 0x25C278, ARMv8::MOVBits::Create(1, 27, false));
    // CHook::NOP(g_libGTASA + 0x25C28C, 1);
    // CHook::Write32(g_libGTASA + 0x25C290, ARMv8::MOVBits::Create(1, 27, false));
    FLog("Installing patches.. 7");
    CHook::RET("_ZN10CPlayerPed14AnnoyPlayerPedEb"); // CPedSamp::AnnoyPlayerPed
    CHook::RET("_ZN11CPopulation15AddToPopulationEffff");    // CPopulation::AddToPopulation

    CHook::RET("_ZN23CAEPedSpeechAudioEntity11AddSayEventEisjfhhh"); // CPed::Say
    FLog("Installing patches.. 8");
    CHook::RET("_ZN10CPedGroups7ProcessEv"); // CPedGroups::Process
    CHook::RET("_ZN21CPedGroupIntelligence7ProcessEv"); // CPedGroupIntelligence::Process
    CHook::RET("_ZN19CPedGroupMembership9SetLeaderEP4CPed"); // CPedGroupMembership::SetLeader
    CHook::RET("_ZN21CPedGroupIntelligence5FlushEv"); // CPedGroupIntelligence::Flush
    FLog("Installing patches.. 9");
    //CHook::RET("_ZN22CRealTimeShadowManager4InitEv"); // CRealTimeShadowManager::Init
    //   CHook::RET("_ZN22CRealTimeShadowManager6UpdateEv"); // CRealTimeShadowManager::Update

    //  CHook::RET("_ZN22CRealTimeShadowManager20ReturnRealTimeShadowEP15CRealTimeShadow"); // CRealTimeShadowManager::ReturnRealTimeShadow from ~CPhysical
    //  CHook::RET("_ZN8CShadows19RenderStaticShadowsEb"); // CShadows::RenderStaticShadows

    CHook::RET("_ZN8CMirrors16BeforeMainRenderEv"); // CMirrors::BeforeMainRender(void)
    CHook::RET("_ZN8CMirrors17RenderReflectionsEv"); // CMirrors::RenderReflections(void)
    FLog("Installing patches.. 10");
    CHook::RET("_ZN8CCarCtrl18GenerateRandomCarsEv"); // CCarCtrl::GenerateRandomCars(void)

    CHook::RET("_ZN10CGameLogic43SetPlayerWantedLevelForForbiddenTerritoriesEb"); // CGameLogic::SetPlayerWantedLevelForForbiddenTerritories

    CHook::RET("_ZN7CWanted14ReportCrimeNowE10eCrimeTypeRK7CVectorb"); // CWanted::ReportCrimeNow

    CHook::RET("_ZN8CCarCtrl18GenerateRandomCarsEv");


    FLog("Installing patches.. 11");
    CHook::NOP(g_libGTASA+0x477CD8, 1); // NOP calling CCutsceneMgr::Update from CGame::Process

    // Disable camera jump-cut after respawning
    CHook::NOP(g_libGTASA+0x3D8220, 1); // NOP calling CCamera::RestoreWithJumpCut from CGameLogic::RestorePlayerStuffDuringResurrection
    CHook::NOP(g_libGTASA + 0x4D6F74, 9); //PED DUMMY

    CHook::NOP(g_libGTASA + 0x6E537C, 1); // camshake
    CHook::NOP(g_libGTASA + 0x6E5350, 1); // camshake
    FLog("Installing patches.. 12");

    //CHook::Write32(g_libGTASA + 0x6B52E0, ARMv8::MOVBits::Create(2, 1, false));

    //riffle
    CHook::NOP(g_libGTASA + 0x675888, 2);
    CHook::Write32(g_libGTASA + 0x675E14, ARMv8::CMPBits::Create(0xFF, 28, false));

    /*
     * some jpatches do we really need this?
    CHook::Write32(g_libGTASA + 0x51D9D8, 0x39403102);
    CHook::Write32(g_libGTASA + 0x51D9DC, 0x39403503);
    CHook::Write32(g_libGTASA + 0x51D9E0, 0x35000061);
    CHook::Write32(g_libGTASA + 0x51D9E4, 0x35000042);
    //shadow in all surface
    CHook::Write32(g_libGTASA + 0x6DF67C, 0x52800033);
     */
    FLog("Installing patches.. 13");
}