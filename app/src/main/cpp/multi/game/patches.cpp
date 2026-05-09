//
// Created by Dionisio on 5/9/2026.
//
#include "../main.h"
#include "../game/game.h"
#include "../vendor/armhook/patch.h"

extern CGame* pGame;

void ApplyGlobalPatches()
{
    CHook::NOP(g_libGTASA + 0x705514, 1);

    CHook::WriteMemory(g_libGTASA + 0x597CBC, (uintptr_t)"\x34\x00\x80\x52", 4);

    CHook::RET("_ZN10CSkidmarks6UpdateEv"); // CSkidmarks::Update
    CHook::RET("_ZN10CSkidmarks6RenderEv"); // CSkidmarks::Render

    CHook::RET("_ZN14CLoadingScreen15DisplayPCScreenEv"); // Loading screen
    CHook::RET("_ZN8CCarCtrl18GenerateRandomCarsEv"); // CCarCtrl::GenerateRandomCars(void)

    CHook::RET("_ZN10CGameLogic43SetPlayerWantedLevelForForbiddenTerritoriesEb"); // CGameLogic::SetPlayerWantedLevelForForbiddenTerritories

    CHook::RET("_ZN7CWanted14ReportCrimeNowE10eCrimeTypeRK7CVectorb"); // CWanted::ReportCrimeNow


    CHook::RET("_ZN10CPedGroups7ProcessEv"); // CPedGroups::Process
    CHook::RET("_ZN21CPedGroupIntelligence7ProcessEv"); // CPedGroupIntelligence::Process
    CHook::RET("_ZN19CPedGroupMembership9SetLeaderEP4CPed"); // CPedGroupMembership::SetLeader
    CHook::RET("_ZN21CPedGroupIntelligence5FlushEv"); // CPedGroupIntelligence::Flush
    CHook::RET("_ZN10CPlayerPed14AnnoyPlayerPedEb"); // CPedSamp::AnnoyPlayerPed
    CHook::RET("_ZN11CPopulation15AddToPopulationEffff");    // CPopulation::AddToPopulation

    CHook::RET("_ZN23CAEPedSpeechAudioEntity11AddSayEventEisjfhhh"); // CPed::Say
    CHook::RET("_ZN7CEntity23PreRenderForGlassWindowEv"); // CEntity::PreRenderForGlassWindow

    CHook::RET("_ZN8CMirrors16RenderReflBufferEb"); // CMirrors::RenderReflBuffer
    CHook::RET("_ZN4CHud23DrawBustedWastedMessageEv"); // CHud::DrawBustedWastedMessage // ПОТРАЧЕНО

   CHook::RET("_ZN4CHud14SetHelpMessageEPKcPsbbbj"); // CHud::SetHelpMessage
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

   CHook::RET("_ZN13CCarEnterExit17SetPedInCarDirectEP4CPedP8CVehicleib");	// CCarEnterExit::SetPedInCarDirect
   CHook::RET("_ZN6CRadar10DrawLegendEiii"); // CRadar::DrawLgegend
   CHook::RET("_ZN6CRadar19AddBlipToLegendListEhi"); // CRadar::AddBlipToLegendList

   CHook::RET("_ZN11CAutomobile35CustomCarPlate_BeforeRenderingStartEP17CVehicleModelInfo"); // CAutomobile::CustomCarPlate_BeforeRenderingStart
   CHook::RET("_ZN11CAutomobile33CustomCarPlate_AfterRenderingStopEP17CVehicleModelInfo"); // CAutomobile::CustomCarPlate_AfterRenderingStop

   CHook::RET("_ZN30CWidgetRegionSteeringSelection4DrawEv");
   CHook::RET("_ZN17CVehicleRecording4LoadEP8RwStreamii");
   CHook::RET("_Z16SaveGameForPause10eSaveTypesPc");
   CHook::RET("_ZN17CVehicleModelInfo17SetCarCustomPlateEv");

   CHook::RET("_ZN10CPlayerPed22FindWeaponLockOnTargetEv");// CPed::FindWeaponLockOnTarget
   CHook::RET("_ZN10CPlayerPed26FindNextWeaponLockOnTargetEP7CEntityb"); // CPed::FindNextWeaponLockOnTarget
   CHook::RET("_ZN4CPed21SetWeaponLockOnTargetEP7CEntity"); // CPed::SetWeaponLockOnTarget
}