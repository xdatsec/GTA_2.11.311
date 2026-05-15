#include "../main.h"
#include "game.h"
#include "../vendor/armhook/patch.h"

void ApplyGlobalPatches();
void InstallHooks();
void ApplySAMPPatchesInGame();

bool bUsedPlayerSlots[PLAYER_PED_SLOTS];

extern char* WORLD_PLAYERS;

uint16_t *szGameTextMessage;

inline int FindFirstFreePlayerPedSlot()
{
    uint8_t x = 2;
    while (x != PLAYER_PED_SLOTS) {
        if (!bUsedPlayerSlots[x]) return x;
        x++;
    }

    return 0;
}

CGame::CGame()
{
    m_pGameCamera = new CCamera();
    m_pGamePlayer = nullptr;
    m_bCheckpointsEnabled = false;
    m_bRaceCheckpointsEnabled = false;
    m_dwRaceCheckpointHandle = 0;

    m_bClockEnabled = false;
    m_bInputEnable = true;

    memset(bUsedPlayerSlots, 0, sizeof(bUsedPlayerSlots));
    memset(m_bPreloadedVehicleModels, 0, sizeof(m_bPreloadedVehicleModels));
}

CGame::~CGame()
{

}

void CGame::StartGame()
{
    FLog("Starting game..");

    // OnNewGameCheck
    //(( void (*)())(g_libGTASA + (VER_x32 ? 0x002A7270 + 1 : 0x365EA0)))();

    //*(int*)(g_libGTASA + 0xA987C8) = 8;
    //*(char*)(g_libGTASA + 0x96B514) = 0;
    //*(short*)(g_libGTASA + 0x6E00C0) = 0;
    //*(int*)(g_libGTASA + 0x6E0098) = 0;
    //*(char*)(g_libGTASA + 0x6E00D9) = 0;
    GameAimSyncInit();
    InstallHooks();
    ApplyGlobalPatches();
}

void InstallSAMPHooks();
void InstallWidgetHooks();

void CGame::Initialize()
{
    FLog("CGame initializing..");
    ApplySAMPPatchesInGame();
    GameResetRadarColors();

    szGameTextMessage = new uint16_t[1076];
}
// 0.3.7
void CGame::SetMaxStats()
{
    // CCheat::VehicleSkillsCheat
    ((void(*)(void))(g_libGTASA + /*0x2BAED0*/0x3CDC74))();
    // CCheat::WeaponSkillsCheat
    ((void(*)(void))(g_libGTASA + /*0x2BAE68*/0x3CDBD4))();
    // CStats::SetStatValue
    CHook::RET(g_libGTASA + 0x49D218);
}
// 0.3.7
void CGame::ToggleThePassingOfTime(bool bOnOff)
{
    if (bOnOff)
    {
        //i dont know what bytecode to put here
       // ARMHook::writeMemory(g_libGTASA + /*0x38C154*/0x3E3378, (uintptr_t)"\xD0\xB5", 2);
        this->m_bClockEnabled = true;
    }
    else
    {
        //i dont know what bytecode to put here
        //ARMHook::makeRET(g_libGTASA + /*0x38C154*/0x3E3378);
        this->m_bClockEnabled = false;
    }
}
// 0.3.7
void CGame::EnableClock(bool bEnable)
{

}
// 0.3.7
void CGame::EnableZoneNames(bool bEnable)
{
    ScriptCommand(&enable_zone_names, bEnable);
}
// 0.3.7
void CGame::SetWorldTime(int iHour, int iMinute)
{
    *(uint8_t*)(g_libGTASA +  0x9E036F) = (uint8_t)iMinute;
    *(uint8_t*)(g_libGTASA + 0x9E036E) = (uint8_t)iHour;
    ScriptCommand(&set_current_time, iHour, iMinute);
}
// 0.3.7
void CGame::GetWorldTime(int *iHour, int *iMinute)
{
    *iMinute = *(uint8_t*)(g_libGTASA + /*0x8B18A4*/0x9E036F);
    *iHour = *(uint8_t*)(g_libGTASA + /*0x8B18A5*/0x9E036E);
}
// 0.3.7
void CGame::PreloadObjectsAnims()
{
    //HAS BEEN TURN OF IN 2.10 DO WE REALLY NEED THIS?
    /*
    // keep the throwable weapon models loaded
    if(!IsModelLoaded(WEAPON_MODEL_TEARGAS)) RequestModel(WEAPON_MODEL_TEARGAS);
    if(!IsModelLoaded(WEAPON_MODEL_GRENADE)) RequestModel(WEAPON_MODEL_GRENADE);
    if(!IsModelLoaded(WEAPON_MODEL_MOLOTOV)) RequestModel(WEAPON_MODEL_MOLOTOV);

    // special action object
    if(!IsModelLoaded(330)) RequestModel(330);
    if(!IsModelLoaded(OBJECT_PARACHUTE)) RequestModel(OBJECT_PARACHUTE);
    if(!IsModelLoaded(OBJECT_CJ_CIGGY)) RequestModel(OBJECT_CJ_CIGGY);
    if(!IsModelLoaded(OBJECT_DYN_BEER_1)) RequestModel(OBJECT_DYN_BEER_1);
    if(!IsModelLoaded(OBJECT_CJ_BEER_B_2)) RequestModel(OBJECT_CJ_BEER_B_2);
    if(!IsModelLoaded(OBJECT_CJ_PINT_GLASS)) RequestModel(OBJECT_CJ_PINT_GLASS);
    if(!IsModelLoaded(18631)) RequestModel(18631);

    // special action anim
    if(IsAnimationLoaded("PARACHUTE") == 0) RequestAnimation("PARACHUTE");
    if(IsAnimationLoaded("PAULNMAC") == 0) RequestAnimation("PAULNMAC");
    if(IsAnimationLoaded("BAR") == 0) RequestAnimation("BAR");
    if(IsAnimationLoaded("SMOKING") == 0) RequestAnimation("SMOKING");
    if(IsAnimationLoaded("DANCING") == 0) RequestAnimation("DANCING");
    if(IsAnimationLoaded("GFUNK") == 0) RequestAnimation("GFUNK");
    if(IsAnimationLoaded("RUNNINGMAN") == 0) RequestAnimation("RUNNINGMAN");
    if(IsAnimationLoaded("STRIP") == 0) RequestAnimation("STRIP");
    if(IsAnimationLoaded("WOP") == 0) RequestAnimation("WOP");
     */
}
// 0.3.7
void CGame::SetWorldWeather(int byteWeatherID)
{
    CHook::CallFunction<void>(g_libGTASA + 0x5DE9D0, byteWeatherID);

    if(!m_bClockEnabled)
    {
        *(uint16_t*)(g_libGTASA + 0xCADD02) = byteWeatherID;
        *(uint16_t*)(g_libGTASA + 0xCADD00) = byteWeatherID;
    }
}
// 0.3.7
void CGame::DisplayHUD(bool bDisp)
{
    if (bDisp)
    {
        *(uint8_t*)(g_libGTASA + 0x9D7D58) = 1;
        *(uint8_t*)(g_libGTASA + 0xA4C794) = 0;
    }
    else
    {
        *(uint8_t*)(g_libGTASA + 0x9D7D58) = 0;
        *(uint8_t*)(g_libGTASA + 0xA4C794) = 1;
    }
}
// 0.3.7
uint8_t CGame::GetActiveInterior()
{
    uint32_t dwRet;
    ScriptCommand(&get_active_interior, &dwRet);
    return (uint8_t)dwRet;
}

const char* CGame::GetDataDirectory()
{
    return "/storage/emulated/0/Android/media/com.rockstargames.gtasa/";
}
// 0.3.7
void CGame::UpdateCheckpoints()
{
    if (m_bCheckpointsEnabled)
    {
        CPlayerPed* pPlayerPed = this->FindPlayerPed();
        if (pPlayerPed)
        {
            ScriptCommand(&is_actor_near_point_3d, pPlayerPed->m_dwGTAId,
                          m_vecCheckpointPos.X, m_vecCheckpointPos.Y, m_vecCheckpointPos.Z,
                          m_vecCheckpointExtent.X, m_vecCheckpointExtent.Y, m_vecCheckpointExtent.Z, 1);

            if (!m_dwCheckpointMarker)
            {
                m_dwCheckpointMarker = CreateRadarMarkerIcon(0, m_vecCheckpointPos.X,
                                                             m_vecCheckpointPos.Y, m_vecCheckpointPos.Z, 1005, 0);
            }
        }
    }
    else if (m_dwCheckpointMarker)
    {
        DisableMarker(m_dwCheckpointMarker);
        m_dwCheckpointMarker = 0;
    }

    if (m_bRaceCheckpointsEnabled)
    {
        CPlayerPed* pPlayerPed = this->FindPlayerPed();
        if (pPlayerPed)
        {
            if (!m_dwRaceCheckpointMarker)
            {
                m_dwRaceCheckpointMarker = CreateRadarMarkerIcon(0, m_vecRaceCheckpointPos.X,
                                                                 m_vecRaceCheckpointPos.Y, m_vecRaceCheckpointPos.Z, 1005, 0);
            }
        }
    }
    else if (m_dwRaceCheckpointMarker)
    {
        DisableMarker(m_dwRaceCheckpointMarker);
        DisableRaceCheckpoint();
        m_dwRaceCheckpointMarker = 0;
    }
}
// 0.3.7
uint8_t CGame::GetPedSlotsUsed()
{
    uint8_t count = 0;
    for (int i = 2; i < PLAYER_PED_SLOTS; i++)
    {
        if (bUsedPlayerSlots[i])
            count++;
    }

    return count;
}

void CGame::PlaySound(int iSound, float fX, float fY, float fZ)
{
    ScriptCommand(&play_sound, fX, fY, fZ, iSound);
}
// 0.3.7
void CGame::RefreshStreamingAt(float x, float y)
{
    ScriptCommand(&refresh_streaming_at, x, y);
}
// 0.3.7
void CGame::DisableTrainTraffic()
{
    ScriptCommand(&enable_train_traffic, 0);
}
// 0.3.7
void CGame::UpdateGlobalTimer(uint32_t dwTimer)
{
    if (!m_bClockEnabled)
    {
        *(uint32_t*)(g_libGTASA + 0xA00C7C) = dwTimer & 0x3FFFFFFF;
    }
}
// 0.3.7
void CGame::SetGravity(float fGravity)
{
    CHook::CallFunction<void>(g_libGTASA + 0x484080, fGravity);
}

bool CGame::IsGamePaused()
{
    return *(bool *)(g_libGTASA + 0xA00CB8);
}


bool CGame::IsGameLoaded()
{
    return true;
}

void CGame::DrawGangZone(float fPos[], uint32_t dwColor, uint32_t dwUnk)
{
    // CRadar::DrawAreaOnRadar
    CHook::CallFunction<void>(g_libGTASA + 0x502C8C, fPos, &dwColor, dwUnk);
}
// 0.3.7
uint32_t CGame::CreatePickup(int iModel, int iType, float x, float y, float z, int *pdwIndex)
{
    if (!IsValidModel(iModel)) {
        iModel = 18631;
    }

    if (!IsModelLoaded(iModel))
    {
        RequestModel(iModel);
        LoadRequestedModels();
        while (!IsModelLoaded(iModel)) sleep(1);
    }

    uint32_t hnd;
    ScriptCommand(&create_pickup, iModel, iType, x, y, z, &hnd);
    int offset = 32 * (hnd & 0xFFFF);
    if (offset) offset /= 32;
    if (pdwIndex) {
        *pdwIndex = offset;
    }

    return hnd;
}
// 0.3.7
bool CGame::IsModelLoaded(int iModel)
{
    if (iModel > 20000 || iModel < 0) {
        return true;
    }
    else {
        return ScriptCommand(&is_model_available, iModel);
    }
}
// 0.3.7
void CGame::RequestModel(uint16_t iModelId, uint8_t iLoadingStream)
{
    // CStreaming::RequestModel
    (( void (*)(int32_t, int32_t))(g_libGTASA+0x39CDCC))(iModelId, iLoadingStream);
    //ScriptCommand(&request_model, iModelID);
}
// 0.3.7
void CGame::LoadRequestedModels()
{
    ScriptCommand(&load_requested_models);
}
// 0.3.7
void CGame::RemoveModel(int iModel, bool bFromStreaming)
{
    if (iModel >= 0 && iModel < 20000)
    {
        if (bFromStreaming)
        {
            if(ScriptCommand(&is_model_available, iModel))
                // CStreaming::RemoveModel
                ((void(*)(int))(g_libGTASA + 0x39A354))(iModel);
        }
        else
        {
            if (ScriptCommand(&is_model_available, iModel))
                ScriptCommand(&release_model, iModel);
        }
    }
}
// 0.3.7 (ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ 2 ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ 0.3DL)
CObject* CGame::NewObject(int iModel, VECTOR vecPos, VECTOR vecRot, float fDrawDistance)
{
    CObject *pObjectNew = new CObject(iModel, vecPos, vecRot, fDrawDistance, 0);
    return pObjectNew;
}
// 0.3.7 (ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ bIsNPC)
CPlayerPed* CGame::NewPlayer(int iSkin, float fX, float fY, float fZ, float fRotation, bool unk, bool bIsNPC)
{
    uint8_t bytePedSlot = FindFirstFreePlayerPedSlot();
    if (!bytePedSlot) return nullptr;

    CPlayerPed* pPed = new CPlayerPed(bytePedSlot, iSkin, fX, fY, fZ, fRotation);
    if (pPed && pPed->m_pPed) {
        bUsedPlayerSlots[bytePedSlot] = true;
    }

    return pPed;
}
// 0.3.7
bool CGame::RemovePlayer(CPlayerPed* pPed)
{
    if (!pPed) return false;

    delete pPed;
    bUsedPlayerSlots[pPed->m_bytePlayerNumber] = false;
    return true;
}
// 0.3.7
void CGame::DisableMarker(uint32_t dwMarker)
{
    ScriptCommand(&disable_marker, dwMarker);
}
// 0.3.7
uintptr CGame::CreateRadarMarkerIcon(int iMarkerType, float fX, float fY, float fZ, int iColor, int iStyle)
{
    uintptr dwMarkerID = 0;

    if(iStyle == 1)
        ScriptCommand(&create_marker_icon, fX, fY, fZ, iMarkerType, &dwMarkerID);
    else if(iStyle == 2)
        ScriptCommand(&create_radar_marker_icon, fX, fY, fZ, iMarkerType, &dwMarkerID);
    else if(iStyle == 3)
        ScriptCommand(&create_icon_marker_sphere, fX, fY, fZ, iMarkerType, &dwMarkerID);
    else
        ScriptCommand(&create_radar_marker_without_sphere, fX, fY, fZ, iMarkerType, &dwMarkerID);

    if(iMarkerType == 0)
    {
        if(iColor >= 1004)
        {
            ScriptCommand(&set_marker_color, dwMarkerID, iColor);
            ScriptCommand(&show_on_radar, dwMarkerID, 3);
        }
        else
        {
            ScriptCommand(&set_marker_color, dwMarkerID, iColor);
            ScriptCommand(&show_on_radar, dwMarkerID, 2);
        }
    }

    return dwMarkerID;
}// 0.3.7
bool CGame::IsAnimationLoaded(const char* szAnimLib)
{
    return ScriptCommand(&is_animation_loaded, szAnimLib);
}
// 0.3.7
void CGame::RequestAnimation(const char* szAnimLib)
{
    ScriptCommand(&request_animation, szAnimLib);
}
// 0.3.7
float CGame::FindGroundZForCoord(float fX, float fY)
{
    /*float fPosZ;
    ScriptCommand(&get_ground_z, fX, fY, fZ, &fPosZ);
    return fPosZ;*/
    return (( float (*)(float, float))(g_libGTASA+0x4B336C))(fX, fY);
}
// 0.3.7
void CGame::DisableAutoAim()
{
    //ARMHook::makeRET(g_libGTASA + 0x4C6CF4); // CPlayerPed::FindWeaponLockOnTarget
    //ARMHook::makeRET(g_libGTASA + 0x4C7CDC); // CPlayerPed::FindNextWeaponLockOnTarget

    // CPed::SetWeaponLockOnTarget
    CHook::RET(g_libGTASA + 0x572FB0/*0x438DB4*/);
}

// 0.3.7
void CGame::EnabledAutoAim()
{
    CHook::RET(g_libGTASA + 0x59BC48); // CPlayerPed::FindWeaponLockOnTarget
    CHook::RET(g_libGTASA + 0x59CF04); // CPlayerPed::FindNextWeaponLockOnTarget
}
// 0.3.7
CVehicle* CGame::NewVehicle(int iVehicleType, float fX, float fY, float fZ, float fRotation, bool bAddSiren)
{
    bool bPreloaded = false;
    if (m_bPreloadedVehicleModels[iVehicleType - 400] == true) {
        bPreloaded = true;
    }

    CVehicle* pNewVehicle = new CVehicle(iVehicleType, fX, fY, fZ, fRotation, bPreloaded, bAddSiren);

    return pNewVehicle;
}
// 0.3.7
void CGame::SetCheckpointInformation(VECTOR* vecPos, VECTOR* vecSize)
{
    m_vecCheckpointPos.X = vecPos->X;
    m_vecCheckpointPos.Y = vecPos->Y;
    m_vecCheckpointPos.Z = vecPos->Z;

    m_vecCheckpointExtent.X = vecSize->X;
    m_vecCheckpointExtent.Y = vecSize->Y;
    m_vecCheckpointExtent.Z = vecSize->Z;

    if (m_dwCheckpointMarker)
    {
        DisableMarker(m_dwCheckpointMarker);
        m_dwCheckpointMarker = 0;

        m_dwCheckpointMarker = CreateRadarMarkerIcon(0,
                                                     m_vecCheckpointPos.X,
                                                     m_vecCheckpointPos.Y,
                                                     m_vecCheckpointPos.Z,
                                                     1005, 0);
    }
}
// 0.3.7
void CGame::SetRaceCheckpointInformation(uint8_t byteType, VECTOR* vecPos, VECTOR* vecNextPos, float fRadius)
{
    m_vecRaceCheckpointPos.X = vecPos->X;
    m_vecRaceCheckpointPos.Y = vecPos->Y;
    m_vecRaceCheckpointPos.Z = vecPos->Z;

    m_vecRaceCheckpointNextPos.X = vecNextPos->X;
    m_vecRaceCheckpointNextPos.Y = vecNextPos->Y;
    m_vecRaceCheckpointNextPos.Z = vecNextPos->Z;

    m_byteRaceType = byteType;
    m_fRaceCheckpointRadius = fRadius;

    if (m_dwRaceCheckpointMarker)
    {
        DisableMarker(m_dwRaceCheckpointMarker);

        m_dwRaceCheckpointMarker = CreateRadarMarkerIcon(0,
                                                         m_vecRaceCheckpointPos.X,
                                                         m_vecRaceCheckpointPos.Y,
                                                         m_vecRaceCheckpointPos.Z,
                                                         1005,
                                                         0);
    }

    MakeRaceCheckpoint();
}
// 0.3.7
void CGame::MakeRaceCheckpoint()
{
    DisableRaceCheckpoint();

    ScriptCommand(&create_racing_checkpoint, (int)m_byteRaceType,
                  m_vecRaceCheckpointPos.X, m_vecRaceCheckpointPos.Y, m_vecRaceCheckpointPos.Z,
                  m_vecRaceCheckpointNextPos.X, m_vecRaceCheckpointNextPos.Y, m_vecRaceCheckpointNextPos.Z,
                  m_fRaceCheckpointRadius, &m_dwRaceCheckpointHandle);

    m_bRaceCheckpointsEnabled = true;
}
// 0.3.7
void CGame::DisableRaceCheckpoint()
{
    if (m_dwRaceCheckpointHandle)
    {
        ScriptCommand(&destroy_racing_checkpoint, m_dwRaceCheckpointHandle);
        m_dwRaceCheckpointHandle = 0;
    }

    m_bRaceCheckpointsEnabled = false;
}

void CGame::SetWantedLevel(uint8_t level)
{
    //ARMHook::writeMemory(g_libGTASA+0x2BDF6E, (uintptr_t)&level, 1);
}

void CGame::EnableStuntBonus(bool bEnable)
{

}
// 0.3.7
void CGame::DisplayGameText(const char* szStr, int iTime, int iSize)
{
    if (iSize > 200) return;

    ScriptCommand(&text_clear_all);
    CFont::AsciiToGxtChar(szStr, szGameTextMessage);

    (( void (*)(uint16_t*, int, int))(g_libGTASA + 0x68C27C))(szGameTextMessage, iTime, iSize);

}
// 0.3.7
void CGame::AddToLocalMoney(int iAmmount)
{
    ScriptCommand(&add_to_player_money, 0, iAmmount);
}
// 0.3.7
void CGame::ResetLocalMoney()
{
    int iMoney = GetLocalMoney();
    if (!iMoney) return;

    if (iMoney < 0)
        AddToLocalMoney(abs(iMoney));
    else
        AddToLocalMoney(-(iMoney));
}
// 0.3.7
int CGame::GetLocalMoney()
{
    return *(int*)(Players + 0xF4);
}
// 0.3.7
void CGame::DisableEnterExits()
{

}

void CGame::ToggleCJWalk(bool bUseCJWalk)
{
    CHook::NOP(g_libGTASA + 0x59ABE4, 2);
}


// 0.3.7


