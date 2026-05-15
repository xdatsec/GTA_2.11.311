#include "../main.h"
#include "game.h"
#include "../net/netgame.h"
#include "../gui/gui.h"
#include "../vendor/armhook/patch.h"

extern UI* pUI;
extern CGame* pGame;
extern CNetGame* pNetGame;


uint8_t byteInternalPlayer = 0;
PED_TYPE* dwCurPlayerActor = 0;
uint8_t byteCurPlayer = 0;

PAD_KEYS LocalPlayerKeys;
PAD_KEYS RemotePlayerKeys[PLAYER_PED_SLOTS];

uint16_t(*CPad__GetPedWalkLeftRight)(uintptr_t thiz);
uint16_t CPad__GetPedWalkLeftRight_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        // Remote player
        uint16_t dwResult = RemotePlayerKeys[byteCurPlayer].wKeyLR;
        if ((dwResult == 0xFF80 || dwResult == 0x80) &&
            RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_WALK])
        {
            dwResult = 0x20;
        }
        return dwResult;
    }
    else
    {
        // Local player
        LocalPlayerKeys.wKeyLR = CPad__GetPedWalkLeftRight(thiz);
        return LocalPlayerKeys.wKeyLR;
    }
}

uint16_t(*CPad__GetPedWalkUpDown)(uintptr_t thiz);
uint16_t CPad__GetPedWalkUpDown_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        // Remote player
        uint16_t dwResult = RemotePlayerKeys[byteCurPlayer].wKeyUD;
        if ((dwResult == 0xFF80 || dwResult == 0x80) &&
            RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_WALK])
        {
            dwResult = 0x20;
        }
        return dwResult;
    }
    else
    {
        // Local player
        LocalPlayerKeys.wKeyUD = CPad__GetPedWalkUpDown(thiz);
        return LocalPlayerKeys.wKeyUD;
    }
}

uint32_t(*CPad__GetSprint)(uintptr_t thiz, uint32_t unk);
uint32_t CPad__GetSprint_hook(uintptr_t thiz, uint32_t unk)
{
    if (*pbyteCurrentPlayer)
    {
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_SPRINT];
    }
    else
    {
        LocalPlayerKeys.bKeys[ePadKeys::KEY_SPRINT] = CPad__GetSprint(thiz, unk);
        return LocalPlayerKeys.bKeys[ePadKeys::KEY_SPRINT];
    }
}

uint32_t(*CPad__JumpJustDown)(uintptr_t thiz);
uint32_t CPad__JumpJustDown_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        if (!RemotePlayerKeys[byteCurPlayer].bIgnoreJump &&
            RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_JUMP] &&
            !RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_HANDBRAKE])
        {
            RemotePlayerKeys[byteCurPlayer].bIgnoreJump = true;
            return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_JUMP];
        }

        return 0;
    }
    else
    {
        LocalPlayerKeys.bKeys[ePadKeys::KEY_JUMP] = CPad__JumpJustDown(thiz);
        return LocalPlayerKeys.bKeys[ePadKeys::KEY_JUMP];
    }
}

uint32_t(*CPad__GetJump)(uintptr_t thiz);
uint32_t CPad__GetJump_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        if (RemotePlayerKeys[byteCurPlayer].bIgnoreJump) return 0;
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_JUMP];
    }
    else
    {
        LocalPlayerKeys.bKeys[ePadKeys::KEY_JUMP] = CPad__JumpJustDown(thiz);
        return LocalPlayerKeys.bKeys[ePadKeys::KEY_JUMP];
    }
}

uint32_t(*CPad__GetAutoClimb)(uintptr_t thiz);
uint32_t CPad__GetAutoClimb_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_JUMP];
    }
    else
    {
        LocalPlayerKeys.bKeys[ePadKeys::KEY_JUMP] = CPad__GetAutoClimb(thiz);
        return LocalPlayerKeys.bKeys[ePadKeys::KEY_JUMP];
    }
}

uint32_t(*CPad__GetAbortClimb)(uintptr_t thiz);
uint32_t CPad__GetAbortClimb_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_SECONDARY_ATTACK];
    }
    else
    {
        LocalPlayerKeys.bKeys[ePadKeys::KEY_SECONDARY_ATTACK] = CPad__GetAutoClimb(thiz);
        return LocalPlayerKeys.bKeys[ePadKeys::KEY_SECONDARY_ATTACK];
    }
}

uint32_t(*CPad__DiveJustDown)();
uint32_t CPad__DiveJustDown_hook()
{
    if (*pbyteCurrentPlayer)
    {
        // remote player
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_FIRE];
    }
    else
    {
        LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE] = CPad__DiveJustDown();
        return LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE];
    }
}

uint32_t(*CPad__SwimJumpJustDown)(uintptr_t thiz);
uint32_t CPad__SwimJumpJustDown_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_JUMP];
    }
    else
    {
        LocalPlayerKeys.bKeys[ePadKeys::KEY_JUMP] = CPad__SwimJumpJustDown(thiz);
        return LocalPlayerKeys.bKeys[ePadKeys::KEY_JUMP];
    }
}

uint32_t(*CPad__DuckJustDown)(uintptr_t thiz, int unk);
uint32_t CPad__DuckJustDown_hook(uintptr_t thiz, int unk)
{
    if (*pbyteCurrentPlayer)
    {
        return 0;
    }
    else
    {
        return CPad__DuckJustDown(thiz, unk);
    }
}

uint32_t(*CPad__MeleeAttackJustDown)(uintptr_t thiz);
uint32_t CPad__MeleeAttackJustDown_hook(uintptr_t thiz)
{
    /*
        0 - ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
        1 - ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ (ï¿½ï¿½ï¿½)
        2 - ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ (ï¿½ï¿½ï¿½ + F)
    */

    if (*pbyteCurrentPlayer)
    {
        if (RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_HANDBRAKE] &&
            RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_SECONDARY_ATTACK])
            return 2;

        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_FIRE];
    }
    else
    {
        uint32_t dwResult = CPad__MeleeAttackJustDown(thiz);
        //LocalPlayerKeys.bKeys[ePadKeys::KEY_HANDBRAKE] = true;

        //if(dwResult == 2)
        //{
        //	LocalPlayerKeys.bKeys[ePadKeys::KEY_SECONDARY_ATTACK] = true;
        //}
        //else if(dwResult == 1)
        //{
        LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE] = dwResult;
        //	LocalPlayerKeys.bKeys[ePadKeys::KEY_HANDBRAKE] = false;
        //}

        return dwResult;
    }
}

uint32_t(*CPad__GetBlock)(uintptr_t thiz);
uint32_t CPad__GetBlock_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        if (RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_JUMP] &&
            RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_HANDBRAKE])
            return 1;

        return 0;
    }
    else
    {
        return CPad__GetBlock(thiz);
    }
}

int16_t(*CPad__GetSteeringLeftRight)(uintptr_t thiz);
int16_t CPad__GetSteeringLeftRight_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        // remote player
        return (int16_t)RemotePlayerKeys[byteCurPlayer].wKeyLR;
    }
    else
    {
        // local player
        LocalPlayerKeys.wKeyLR = CPad__GetSteeringLeftRight(thiz);
        return LocalPlayerKeys.wKeyLR;
    }
}

uint16_t(*CPad__GetSteeringUpDown)(uintptr_t thiz);
uint16_t CPad__GetSteeringUpDown_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        // remote player
        return RemotePlayerKeys[byteCurPlayer].wKeyUD;
    }
    else
    {
        // local player
        LocalPlayerKeys.wKeyUD = CPad__GetSteeringUpDown(thiz);
        return LocalPlayerKeys.wKeyUD;
    }
}

uint16_t(*CPad__GetAccelerate)(uintptr_t thiz);
uint16_t CPad__GetAccelerate_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        // remote player
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_SPRINT] ? 0xFF : 0x00;
    }
    else
    {
        // local player
        CPlayerPed* pPlayerPed = pGame->FindPlayerPed();
        if (pPlayerPed)
        {
            if (!pPlayerPed->IsInVehicle() || pPlayerPed->IsAPassenger())
                return 0;
        }

        // local player
        uint16_t wAccelerate = CPad__GetAccelerate(thiz);
        LocalPlayerKeys.bKeys[ePadKeys::KEY_SPRINT] = wAccelerate;
        if (wAccelerate == 0xFF)
        {
            if (pPlayerPed)
            {
                VEHICLE_TYPE* pGtaVehicle = pPlayerPed->GetGtaVehicle();
                if (pGtaVehicle)
                {
                    if (pGtaVehicle->dwFlags.bEngineOn == 0)
                    {
                        wAccelerate = 0;
                    }
                }
            }
        }

        return wAccelerate;
    }
}

uint16_t(*CPad__GetBrake)(uintptr_t thiz);
uint16_t CPad__GetBrake_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        // remote player
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_JUMP] ? 0xFF : 0x00;
    }
    else
    {
        CPlayerPed* pPlayerPed = pGame->FindPlayerPed();
        if (pPlayerPed)
        {
            if (!pPlayerPed->IsInVehicle() || pPlayerPed->IsAPassenger())
                return 0;
        }

        // local player
        uint16_t wBrake = CPad__GetBrake(thiz);
        LocalPlayerKeys.bKeys[ePadKeys::KEY_JUMP] = wBrake;
        if (wBrake == 0xFF)
        {
            if (pPlayerPed)
            {
                VEHICLE_TYPE* pGtaVehicle = pPlayerPed->GetGtaVehicle();
                if (pGtaVehicle)
                {
                    if (pGtaVehicle->dwFlags.bEngineOn == 0)
                    {
                        wBrake = 0;
                    }
                }
            }
        }
        return wBrake;
    }
}

uint32_t(*CPad__GetHandBrake)(uintptr_t thiz);
uint32_t CPad__GetHandBrake_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        // remote player
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_HANDBRAKE] ? 0xFF : 0x00;
    }
    else
    {
        // local player
        uint32_t handBrake = CPad__GetHandBrake(thiz);
        LocalPlayerKeys.bKeys[ePadKeys::KEY_HANDBRAKE] = handBrake;
        return handBrake;
    }
}

uint32_t(*CPad__GetHorn)(uintptr_t thiz);
uint32_t CPad__GetHorn_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        // remote player
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_CROUCH];
    }
    else
    {
        // local player
        uint32_t horn = CPad__GetHorn(thiz);
        //Log("horn: %d", horn);
        LocalPlayerKeys.bKeys[ePadKeys::KEY_CROUCH] = CPad__GetHorn(thiz);
        return LocalPlayerKeys.bKeys[ePadKeys::KEY_CROUCH];
    }
}

/*extern bool g_bLockEnterVehicleWidget;
extern bool g_bForceEnterVehicle;
uint32_t(*CPad__ExitVehicleJustDown)(uintptr_t thiz, int a2, uintptr_t vehicle, int a4, uintptr_t vec);
uint32_t CPad__ExitVehicleJustDown_hook(uintptr_t thiz, int a2, uintptr_t vehicle, int a4, uintptr_t vec)
{
	int result = CPad__ExitVehicleJustDown(thiz, a2, vehicle, a4, vec);

	if (g_bForceEnterVehicle)
	{
		g_bForceEnterVehicle = false;
		return true;
	}

	if (g_bLockEnterVehicleWidget) return false;

	return result;
}
*/

uint32_t(*CPad__ExitVehicleJustDown)(uintptr_t thiz, int a2, uintptr_t vehicle, int a4, uintptr_t vec);
uint32_t CPad__ExitVehicleJustDown_hook(uintptr_t thiz, int a2, uintptr_t vehicle, int a4, uintptr_t vec)
{
    static uint32_t dwPassengerEnterExit = GetTickCount();

    if (GetTickCount() - dwPassengerEnterExit < 1000)
        return 0;

    if (pNetGame)
    {
        CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
        if (pPlayerPool)
        {
            CLocalPlayer* pLocalPlayer = pPlayerPool->GetLocalPlayer();
            if (pLocalPlayer) {
                if (pLocalPlayer->HandlePassengerEntry())
                {
                    dwPassengerEnterExit = GetTickCount();
                    return 0;
                }
            }
        }
    }

    return CPad__ExitVehicleJustDown(thiz, a2, vehicle, a4, vec);
}

uint32_t(*CPad__GetExitVehicle)(uintptr_t thiz);
uint32_t CPad__GetExitVehicle_hook(uintptr_t thiz)
{
    return 0;
}


/* Weapons */

bool (*CPad__GetEnterTargeting)(uintptr_t thiz);
bool CPad__GetEnterTargeting_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_HANDBRAKE];
    }
    else
    {
        uint8_t old = *pbyteCurrentPlayer;
        *pbyteCurrentPlayer = byteCurPlayer;
        uintptr_t result = CPad__GetEnterTargeting(thiz);
        LocalPlayerKeys.bKeys[ePadKeys::KEY_HANDBRAKE] = result;
        *pbyteCurrentPlayer = old;
        return result;
    }
}

bool bWeaponClicked;
extern "C" {
JNIEXPORT void JNICALL Java_com_sampmobile_game_main_SAMP_changeGun(JNIEnv *pEnv, jobject thiz) {
    if(!pGame->FindPlayerPed()) return;

    if(!bWeaponClicked) {
        bWeaponClicked = true;
    }
    else {
        bWeaponClicked = false;
    }
}
}

uint32_t (*CPad__CycleWeaponRightJustDown)(uintptr_t thiz);
uint32_t CPad__CycleWeaponRightJustDown_hook(uintptr_t thiz)
{
    if(dwCurPlayerActor && (byteCurPlayer != 0)) return 0;

    if(!bWeaponClicked) {
        //Log("bWeaponClicked: %d", bWeaponClicked);
        //Log("bWeaponClicked: %d", bWeaponClicked);
        return 0;
    }
    else {
        bWeaponClicked = false;
        return 1;
    }
    return CPad__CycleWeaponRightJustDown(thiz);
}

uint32_t(*CPad__CycleWeaponLeftJustDown)(uintptr_t thiz);
uint32_t CPad__CycleWeaponLeftJustDown_hook(uintptr_t thiz)
{
    if (*pbyteCurrentPlayer)
    {
        return 0;
    }
    else
    {
        return CPad__CycleWeaponLeftJustDown(thiz);
    }
}

bool (*CPad__GetWeapon)(uintptr_t thiz, PED_TYPE* pPed);
bool CPad__GetWeapon_hook(uintptr_t thiz, PED_TYPE* pPed)
{
    if (*pbyteCurrentPlayer)
    {
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_FIRE];
    }
    else
    {
        LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE] = CPad__GetWeapon(thiz, pPed);
        return LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE];
    }
}

uint32_t(*CCamera_IsTargetingActive)(uintptr_t thiz, PED_TYPE* pPed);
uint32_t CCamera_IsTargetingActive_hook(uintptr_t thiz, PED_TYPE* pPed)
{
    if (pPed != GamePool_FindPlayerPed())
    {
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_HANDBRAKE] ? 1 : 0;
    }
    else
    {
        /* CCamera::IsTargetingActive */
        uint32_t bIsTargeting = ((uint32_t (*)(uintptr_t))(g_libGTASA + 0x3D9F04 + 1))(g_libGTASA + 0x951FA8);
        LocalPlayerKeys.bKeys[ePadKeys::KEY_HANDBRAKE] = bIsTargeting;
        return bIsTargeting;
    }
}

uint32_t(*CPad__GetDisplayVitalStats)(uint32_t thiz);
uint32_t CPad__GetDisplayVitalStats_hook(uint32_t thiz)
{
    uint32_t result = CPad__GetDisplayVitalStats(thiz);

    if (pUI) {
        if (result) pUI->playertablist()->show();
    }

    return 0;
}

uint32_t(*CPad__GetLookBehindForPed)(uint32_t thiz);
uint32_t CPad__GetLookBehindForPed_hook(uint32_t thiz)
{
    uint32_t result = CPad__GetLookBehindForPed(thiz);

    VoiceButton* vbutton = pUI->voicebutton();
    if (vbutton->countdown > 50) return 0;

    //return 0;
}

int (*CPad__GetNitroFired)(uintptr_t thiz);
int CPad__GetNitroFired_hook(uintptr_t thiz)
{
    if(*pbyteCurrentPlayer)
    {
        if(RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_FIRE])
            return 1;
    }
    else
    {
        LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE] = CPad__GetNitroFired(thiz);
        return LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE];
    }
}

uint32_t (*CPad__GetLookLeft)(uintptr_t thiz);
uint32_t CPad__GetLookLeft_hook(uintptr_t thiz)
{
    if(*pbyteCurrentPlayer)
    {
        if(RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_FIRE])
            return 1;
    }
    else
    {
        LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE] = CPad__GetLookLeft(thiz);
        return LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE];
    }
}

uint32_t (*CPad__GetLookRight)(uintptr_t thiz);
uint32_t CPad__GetLookRight_hook(uintptr_t thiz)
{
    if(*pbyteCurrentPlayer)
    {
        if(RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_FIRE])
            return 1;
    }
    else
    {
        LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE] = CPad__GetLookRight(thiz);
        return LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE];
    }
}

uint16_t(*CPad__GetCarGunLeftRight)(unsigned int a1, int a2, int a3);
uint16_t CPad__GetCarGunLeftRight_hook(unsigned int a1, int a2, int a3)
{
    if (*pbyteCurrentPlayer)
    {
        // Remote player
        uint16_t dwResult = RemotePlayerKeys[byteCurPlayer].wKeyLR;
        if (RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_FIRE])
        {
            dwResult = 0xFFFFFF80;
        }
        return dwResult;
    }
    else
    {
        // Local player
        uint16_t dwResult = CPad__GetCarGunLeftRight(a1, a2, a3);

        if ( dwResult == 0x80 )
        {
            LocalPlayerKeys.wKeyLR = 1;
            dwResult = 0x80;
        }
        else if ( dwResult == 0xFFFFFF80 )
        {
            LocalPlayerKeys.wKeyLR = 1;
            dwResult = 0xFFFFFF80;
        }
        else
        {
            LocalPlayerKeys.wKeyLR = 0;
        }

        return dwResult;
    }
}

uint16_t(*CPad__GetCarGunUpDown)(unsigned int a1, int a2, void *a3, float a4, int a5);
uint16_t CPad__GetCarGunUpDown_hook(unsigned int a1, int a2, void *a3, float a4, int a5)
{
    if (*pbyteCurrentPlayer)
    {
        // Remote player
        uint16_t dwResult = RemotePlayerKeys[byteCurPlayer].wKeyUD;
        if (RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_FIRE])
        {
            dwResult = 0xFFFFFF80;
        }
        return dwResult;
    }
    else
    {
        // Local player
        uint16_t dwResult = CPad__GetCarGunUpDown(a1, a2, a3, a4, a5);

        if ( dwResult == 0x80 )
        {
            LocalPlayerKeys.wKeyUD = 1;
            dwResult = 0x80;
        }
        else if ( dwResult == 0xFFFFFF80 )
        {
            LocalPlayerKeys.wKeyUD = 1;
            dwResult = 0xFFFFFF80;
        }
        else
        {
            LocalPlayerKeys.wKeyUD = 0;
        }

        return dwResult;
    }
}

uint32_t (*CPad__GetCarGunFired)(uintptr_t thiz);
uint32_t CPad__GetCarGunFired_hook(uintptr_t thiz)
{
    if(*pbyteCurrentPlayer)
    {
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_FIRE];
    }
    else
    {
        LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE] = CPad__GetCarGunFired(thiz);
        return LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE];
    }
}

bool (*CPad__GetTurretRight)(uintptr_t *thiz);
bool CPad__GetTurretRight_hook(uintptr_t *thiz)
{
    if(*pbyteCurrentPlayer)
    {
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_LOOK_RIGHT];
    }
    else
    {
        LocalPlayerKeys.bKeys[ePadKeys::KEY_LOOK_RIGHT] = CPad__GetTurretRight(thiz);
        return LocalPlayerKeys.bKeys[ePadKeys::KEY_LOOK_RIGHT];
    }
}

bool (*CPad__GetTurretLeft)(uintptr_t *thiz);
bool CPad__GetTurretLeft_hook(uintptr_t *thiz)
{
    if(*pbyteCurrentPlayer)
    {
        return RemotePlayerKeys[byteCurPlayer].bKeys[ePadKeys::KEY_LOOK_LEFT];
    }
    else
    {
        LocalPlayerKeys.bKeys[ePadKeys::KEY_LOOK_LEFT] = CPad__GetTurretLeft(thiz);
        return LocalPlayerKeys.bKeys[ePadKeys::KEY_LOOK_LEFT];
    }
}
uint32_t TaskUseGun(uintptr_t thiz, uintptr_t pPed)
{
    dwCurPlayerActor = reinterpret_cast<PED_TYPE *>(pPed);
    byteInternalPlayer = *pbyteCurrentPlayer;
    byteCurPlayer = FindPlayerNumFromPedPtr(reinterpret_cast<PED_TYPE *>(pPed));
    uint32_t result = 0;

    if (dwCurPlayerActor && byteCurPlayer && byteInternalPlayer == 0)
    {
        uint8_t byteSavedCameraMode = *pbyteCameraMode;
        *pbyteCameraMode = GameGetPlayerCameraMode(byteCurPlayer);

        uint16_t wSavedCameraMode2 = *wCameraMode2;
        *wCameraMode2 = GameGetPlayerCameraMode(byteCurPlayer);
        if (*wCameraMode2 == 4) {
            *wCameraMode2 = 0;
        }

        GameStoreLocalPlayerCameraExtZoomAndAspect();
        GameSetRemotePlayerCameraExtZoomAndAspect(byteCurPlayer);

        GameStoreLocalPlayerAim();
        GameSetRemotePlayerAim(byteCurPlayer);

        GameStoreLocalPlayerSkills();
        GameSetRemotePlayerSkills(byteCurPlayer);

        *pbyteCurrentPlayer = byteCurPlayer;


        result = ((uint32_t(*)(uintptr_t, uintptr_t))(g_libGTASA +  0x5F9450))(thiz,
                                                                               reinterpret_cast<uintptr_t>(pPed));

        GameSetLocalPlayerSkills();

        *pbyteCameraMode = byteSavedCameraMode;
        *wCameraMode2 = wSavedCameraMode2;

        GameSetLocalPlayerCameraExtZoomAndAspect();

        *pbyteCurrentPlayer = 0;

        GameSetLocalPlayerAim();
    }
    else
    {
        result = ((uint32_t(*)(uintptr_t, uintptr_t))(g_libGTASA +  0x5F9450))(thiz,
                                                                               reinterpret_cast<uintptr_t>(pPed));

    }

    return result;
}
uint32_t (*CPed__GetWeaponSkill)(PED_TYPE * ped);
uint32_t CPed__GetWeaponSkill_hook(PED_TYPE * ped)
{
    PED_TYPE * thiz = (PED_TYPE *)ped;
    bool bWeaponSkillStored = false;

    dwCurPlayerActor = ped;
    byteInternalPlayer = *pbyteCurrentPlayer;
    byteCurPlayer = FindPlayerNumFromPedPtr(dwCurPlayerActor);

    if(dwCurPlayerActor && byteCurPlayer != 0 && byteInternalPlayer == 0)
    {
        GameStoreLocalPlayerSkills();
        GameSetRemotePlayerSkills(byteCurPlayer);
        bWeaponSkillStored = true;
    }
    // CPed::GetWeaponSkill
    uint32_t result = (( uint32_t (*)(PED_TYPE *, uint32_t))(g_libGTASA+0x56F72C))(ped, ped->WeaponSlots[thiz->byteCurWeaponSlot].dwType);

    if(bWeaponSkillStored)
    {
        GameSetLocalPlayerSkills();
        bWeaponSkillStored = false;
    }
    return result;
}
void (*CPed__ProcessControl)(uintptr_t *  thiz);
void CPed__ProcessControl_hook(uintptr_t *  thiz)
{
    dwCurPlayerActor = reinterpret_cast<PED_TYPE *>(thiz);
    byteInternalPlayer = *pbyteCurrentPlayer;
    byteCurPlayer = FindPlayerNumFromPedPtr(reinterpret_cast<PED_TYPE *>(thiz));

    if (dwCurPlayerActor && byteCurPlayer && byteInternalPlayer == 0)
    {

        uint8_t byteSavedCameraMode = *pbyteCameraMode;
        *pbyteCameraMode = GameGetPlayerCameraMode(byteCurPlayer);

        GameStoreLocalPlayerAim();
        GameSetRemotePlayerAim(byteCurPlayer);

        GameStoreLocalPlayerSkills();
        GameSetRemotePlayerSkills(byteCurPlayer);

        GameStoreLocalPlayerCameraExtZoomAndAspect();
        GameSetRemotePlayerCameraExtZoomAndAspect(byteCurPlayer);

        uint16_t wSavedCameraMode2 = *wCameraMode2;
        *wCameraMode2 = GameGetPlayerCameraMode(byteCurPlayer);
        if (*wCameraMode2 == 4) {
            *wCameraMode2 = 0;
        }

        CHook::NOP(g_libGTASA + 0x56C2F8, 2);

        *pbyteCurrentPlayer = byteCurPlayer;

        CPed__ProcessControl(thiz);


       CHook::WriteMemory(g_libGTASA + 0x56C2F8, "\x7A\xFB\xFF\x97", 4);


        *pbyteCameraMode = byteSavedCameraMode;
        *wCameraMode2 = wSavedCameraMode2;

        GameSetLocalPlayerSkills();

        *pbyteCurrentPlayer = 0;
        *pbyteCameraMode = byteSavedCameraMode;
        GameSetLocalPlayerCameraExtZoomAndAspect();
        GameSetLocalPlayerAim();
        GameSetLocalPlayerAim();
    }
    else
    {

        CPed__ProcessControl(thiz);
    }

}
uint32_t CPad__TaskProcess(uintptr_t thiz, PED_TYPE * ped, int unk, int unk1)
{
    dwCurPlayerActor = ped;
    byteCurPlayer = FindPlayerNumFromPedPtr(dwCurPlayerActor);
    uint8_t old = *pbyteCurrentPlayer;
    *pbyteCurrentPlayer = byteCurPlayer;

    uint32_t result =  ((uint32_t(*)(uintptr_t, PED_TYPE *, int, int))(g_libGTASA + 0x6782F4))(thiz, ped, unk, unk1);
    *pbyteCurrentPlayer = old;
    return result;
}
float fSavedBikeLean;
float dwSavedBikeUnk;
MATRIX4X4 *matSavedMatrix;
VECTOR vecSavedMoveSpeed;
void AllVehicles__ProcessControl_hook(uintptr_t thiz)
{
    VEHICLE_TYPE* pVehicle = (VEHICLE_TYPE*)thiz;
    uintptr_t this_vtable = pVehicle->entity.vtable;
    this_vtable -= g_libGTASA;

    uintptr_t call_addr = 0;

    switch (this_vtable)
    {
        // CAutomobile
        case /*0x5CC9F0*/0x81A960:
            call_addr =/* 0x4E314C*/0x6951E0;
            break;

            // CBoat
        case /*0x5CCD48*/0x81B110:
            call_addr = /*0x4F7408*/0x6AF440;
            break;

            // CBike
        case /*0x5CCB18*/0x81AC80:
            call_addr = /*0x4EE790*/0x6A45CC;
            break;

            // CPlane
        case /*0x5CD0B0*/0x81B820:
            call_addr = /*0x5031E8*/0x6BA1FC;
            break;

            // CHeli
        case /*0x5CCE60*/0x81B350:
            call_addr = /*0x4FE62C*/0x6B54C8;
            break;

            // CBmx
        case /*0x5CCC30*/0x81AEC8:
            call_addr = /*0x4F3CE8*/0x6ABFA4;
            break;

            // CMonsterTruck
        case /*0x5CCF88*/0x81B5B8:
            call_addr = /*0x500A34*/0x6B8F98;
            break;

            // CQuadBike
        case /*0x5CD1D8*/0x81BA88:
            call_addr = /*0x505840*/0x6BF210;
            break;

            // CTrain
        case /*0x5CD428*/0x81BF58:
            call_addr = /*0x50AB24*/0x6C2374;
            break;

            // CTrailer
        case 0x81BCF0:
            call_addr = 0x6C0280;
            break;
    }

    byteInternalPlayer = *pbyteCurrentPlayer;

    if (pVehicle->pDriver && pVehicle->pDriver->dwPedType == 0 &&
        pVehicle->pDriver != GamePool_FindPlayerPed() && byteInternalPlayer == 0) // CWorld::PlayerInFocus
    {
        byteCurPlayer = FindPlayerNumFromPedPtr(pVehicle->pDriver);

        // save the internal cammode, apply the context.
        uint8_t byteSavedCameraMode = *pbyteCameraMode;
        *pbyteCameraMode = GameGetPlayerCameraMode(byteCurPlayer);

        // save the second internal cammode, apply the context.
        uint8_t usSavedCameraMode2 = *wCameraMode2;
        *wCameraMode2 = GameGetPlayerCameraMode(byteCurPlayer);
        if(*wCameraMode2 == 4) *wCameraMode2 = 0;

        // aim switching.
        GameStoreLocalPlayerAim();
        GameSetRemotePlayerAim(byteCurPlayer);

        if (pVehicle && pVehicle->pDriver && pVehicle->pDriver->dwPedType == 0 &&
            GamePool_FindPlayerPed() == pVehicle->pDriver)
        {
            if (pVehicle->byteFlags & 0x10)
            {
                pVehicle->entity.nControlFlags &= 0xDF;
            }
            else
            {
                if(call_addr == 0x6B54C8)
                    pVehicle->entity.nControlFlags |= 0x20;
            }
        }

        // bike lean
        if(call_addr == 0x6A45CC || call_addr == 0xABFA4)
        {
            fSavedBikeLean = pVehicle->fBikeLean;
            dwSavedBikeUnk = pVehicle->dwBikeUnk;
        }

        //CWorld::PlayerInFocus
        *pbyteCurrentPlayer = 0;

        pVehicle->pDriver->dwPedType = 4;
        uint8_t byteSavedControlFlags = pVehicle->entity.nControlFlags;
        pVehicle->entity.nControlFlags = 0x1A; // fix helicopter sound bug

        //CAEVehicleAudioEntity::Service
        ((void (*)(uintptr_t))(g_libGTASA + /*0x364B64*/0x38355C))(thiz + /*0x138*/0x198);
        pVehicle->entity.nControlFlags = byteSavedControlFlags;
        pVehicle->pDriver->dwPedType = 0;

        *pbyteCurrentPlayer = byteCurPlayer;

        // matrix pos Z and vecmovespeed Z
        if(call_addr == 0x6AF440 || call_addr == 0x6ABFA4 || call_addr == 0x6B54C8)
        {
            matSavedMatrix = pVehicle->entity.mat;
            vecSavedMoveSpeed = pVehicle->entity.vecMoveSpeed;
        }

        // VEHTYPE::ProcessControl()
        ((void (*)(VEHICLE_TYPE*))(g_libGTASA + call_addr))(pVehicle);

        // restore matrix pos Z and vecmovespeed Z
        if(call_addr == 0x6AF440 || call_addr == 0x6ABFA4 || call_addr == 0x6B54C8)
        {
            pVehicle->entity.vecMoveSpeed.Z = vecSavedMoveSpeed.Z;
            pVehicle->entity.mat->pos.Z = matSavedMatrix->pos.Z;
        }

        // restore bike lean.
        if(call_addr == 0x6A45CC || call_addr == 0xABFA4)
        {
            pVehicle->fBikeLean = fSavedBikeLean;
            pVehicle->dwBikeUnk = dwSavedBikeUnk;

            if((float)pVehicle->dwBikeUnk < pVehicle->fBikeLean)
                pVehicle->fBikeLean = pVehicle->fBikeLean - (pVehicle->fBikeLean - (float)pVehicle->dwBikeUnk) * 0.5;
        }

        // restore the local player's internal ID.
        *pbyteCurrentPlayer = 0;

        // restore the camera modes.
        *pbyteCameraMode = byteSavedCameraMode;
        *wCameraMode2 = usSavedCameraMode2;

        // restore aim switching.
        GameSetLocalPlayerAim();
    }
    else
    {
        if (pVehicle && pVehicle->pDriver && pVehicle->pDriver->dwPedType == 0 &&
            GamePool_FindPlayerPed() == pVehicle->pDriver)
        {
            if (pVehicle->byteFlags & 0x10)
            {
                pVehicle->entity.nControlFlags &= 0xDF;
            }
            else
            {
                if(call_addr == 0x6B54C8)
                    pVehicle->entity.nControlFlags |= 0x20;
            }
        }

        ((void (*)(uintptr_t))(g_libGTASA + /*0x364B64*/0x38355C))(thiz + /*0x138*/0x198);

        if (pVehicle->pDriver)
        {
            if (pVehicle->dwFlags.bTyresDontBurst)
            {
                pVehicle->dwFlags.bTyresDontBurst = 0;
            }
            if(!pVehicle->dwFlags.bCanBeDamaged) pVehicle->dwFlags.bCanBeDamaged = true;
        }
        else
        {
            if (!pVehicle->dwFlags.bTyresDontBurst)
            {
                pVehicle->dwFlags.bTyresDontBurst = 1;
            }
            if (pVehicle->dwFlags.bCanBeDamaged) pVehicle->dwFlags.bCanBeDamaged = false;
        }

        // VEHTYPE::ProcessControl()
        ((void (*)(VEHICLE_TYPE*))(g_libGTASA + call_addr))(pVehicle);
    }
}
void HookCPad() {
    LOGI("HOOK CPAD");
    memset(&LocalPlayerKeys, 0, sizeof(PAD_KEYS));

    CHook::InlineHook("_ZN10CPlayerPed14ProcessControlEv", &CPed__ProcessControl_hook, &CPed__ProcessControl);


    CHook::InstallPLT(g_libGTASA + 0x810F20, &TaskUseGun);
    CHook::InstallPLT(g_libGTASA + 0x819A60, &CPad__TaskProcess);


    CHook::InstallPLT(g_libGTASA + /*0x5CCA1C*/0x81A9B8, (uintptr_t)AllVehicles__ProcessControl_hook); // CAutomobile::ProcessControl
    CHook::InstallPLT(g_libGTASA + /*0x5CCD74*/0x81B168, (uintptr_t)AllVehicles__ProcessControl_hook); // CBoat::ProcessControl
    CHook::InstallPLT(g_libGTASA + /*0x5CCB44*/0x81ACD8, (uintptr_t)AllVehicles__ProcessControl_hook); // CBike::ProcessControl
    CHook::InstallPLT(g_libGTASA + /*0x5CD0DC*/0x81B878, (uintptr_t)AllVehicles__ProcessControl_hook); // CPlane::ProcessControl
    CHook::InstallPLT(g_libGTASA + /*0x5CCE8C*/0x81B3A8, (uintptr_t)AllVehicles__ProcessControl_hook); // CHeli::ProcessControl
    CHook::InstallPLT(g_libGTASA + /*0x5CCC5C*/0x66D944, (uintptr_t)AllVehicles__ProcessControl_hook); // CBmx::ProcessControl
    CHook::InstallPLT(g_libGTASA + /*0x5CCFB4*/0x81AF20, (uintptr_t)AllVehicles__ProcessControl_hook); // CMonsterTruck::ProcessControl
    CHook::InstallPLT(g_libGTASA + /*0x5CD204*/0x81BAE0, (uintptr_t)AllVehicles__ProcessControl_hook); // CQuadBike::ProcessControl
    CHook::InstallPLT(g_libGTASA + /*0x5CD454*/0x81BFB0, (uintptr_t)AllVehicles__ProcessControl_hook); // CTrain::ProcessControl

    // lr/ud (onfoot)
    CHook::InlineHook("_ZN4CPad19GetPedWalkLeftRightEv", &CPad__GetPedWalkLeftRight_hook,
                      &CPad__GetPedWalkLeftRight);
    CHook::InlineHook("_ZN4CPad16GetPedWalkUpDownEv", &CPad__GetPedWalkUpDown_hook,
                      &CPad__GetPedWalkUpDown);

    // sprint/jump stuff

    CHook::InlineHook("_ZN4CPad9GetSprintEi", &CPad__GetSprint_hook, &CPad__GetSprint);
    CHook::InlineHook("_ZN4CPad12JumpJustDownEv", &CPad__JumpJustDown_hook, &CPad__JumpJustDown);
    CHook::InlineHook("_ZN4CPad7GetJumpEv", &CPad__GetJump_hook, &CPad__GetJump);
    CHook::InlineHook("_ZN4CPad15GetCarGunUpDownEbP11CAutomobilefb", &CPad__GetCarGunUpDown_hook,
                      &CPad__GetCarGunUpDown);
    CHook::InlineHook("_ZN4CPad12GetAutoClimbEv", &CPad__GetAutoClimb_hook, &CPad__GetAutoClimb);
    CHook::InlineHook("_ZN4CPad13GetAbortClimbEv", &CPad__GetAbortClimb_hook, &CPad__GetAbortClimb);

    // swimm
    CHook::InlineHook("_ZN4CPad12DiveJustDownEv", &CPad__DiveJustDown_hook, &CPad__DiveJustDown);
    CHook::InlineHook("_ZN4CPad16SwimJumpJustDownEv", &CPad__SwimJumpJustDown_hook,
                      &CPad__SwimJumpJustDown);

    CHook::InlineHook("_ZN4CPad19MeleeAttackJustDownEv", &CPad__MeleeAttackJustDown_hook,
                      &CPad__MeleeAttackJustDown);
    CHook::InlineHook("_ZN4CPad12DuckJustDownEP4CPed", &CPad__DuckJustDown_hook,
                      &CPad__DuckJustDown);
    CHook::InlineHook("_ZN4CPad8GetBlockEv", &CPad__GetBlock_hook, &CPad__GetBlock);

    // steering lr/ud (incar)
    CHook::InlineHook("_ZN4CPad20GetSteeringLeftRightEv", &CPad__GetSteeringLeftRight_hook,
                      &CPad__GetSteeringLeftRight);
    CHook::InlineHook("_ZN4CPad17GetSteeringUpDownEv", &CPad__GetSteeringUpDown_hook,
                      &CPad__GetSteeringUpDown);

    CHook::InlineHook("_ZN4CPad13GetAccelerateEv", &CPad__GetAccelerate_hook, &CPad__GetAccelerate);
    CHook::InlineHook("_ZN4CPad8GetBrakeEv", &CPad__GetBrake_hook, &CPad__GetBrake);
    CHook::InlineHook("_ZN4CPad12GetHandBrakeEv", &CPad__GetHandBrake_hook, &CPad__GetHandBrake);
    CHook::InlineHook("_ZN4CPad7GetHornEb", &CPad__GetHorn_hook, &CPad__GetHorn);


    CHook::InlineHook("_ZN4CPad19ExitVehicleJustDownEbP8CVehiclebRK7CVector",
                      &CPad__ExitVehicleJustDown_hook, &CPad__ExitVehicleJustDown);
    CHook::InlineHook("_ZN4CPad14GetExitVehicleEv", &CPad__GetExitVehicle_hook,
                      &CPad__GetExitVehicle);
    CHook::InlineHook("_ZN4CPad20GetDisplayVitalStatsEP4CPed", &CPad__GetDisplayVitalStats_hook,
                      &CPad__GetDisplayVitalStats);
    CHook::InlineHook("_ZN4CPad19GetLookBehindForPedEv", &CPad__GetLookBehindForPed_hook,
                      &CPad__GetLookBehindForPed);


    // WEAPON
    CHook::InlineHook("_ZN4CPad17GetEnterTargetingEv", &CPad__GetEnterTargeting_hook,
                      &CPad__GetEnterTargeting);
    CHook::InlineHook("_ZN4CPad9GetWeaponEP4CPedb", &CPad__GetWeapon_hook, &CPad__GetWeapon);
    CHook::InlineHook("_ZN7CCamera17IsTargetingActiveEP10CPlayerPed",
                      &CCamera_IsTargetingActive_hook, &CCamera_IsTargetingActive);
 //   CHook::InlineHook("_ZN4CPad24CycleWeaponRightJustDownEv", &CPad__CycleWeaponRightJustDown_hook,
   //                   &CPad__CycleWeaponRightJustDown);
   // CHook::InlineHook("_ZN4CPad23CycleWeaponLeftJustDownEv", &CPad__CycleWeaponLeftJustDown_hook,
   //                   &CPad__CycleWeaponLeftJustDown);

    // nitro
    CHook::InlineHook("_ZN4CPad13GetNitroFiredEv", &CPad__GetNitroFired_hook, &CPad__GetNitroFired);
    //later
    // CHook::InlineHook("_ZN4CPad16GetHydraulicJumpEv", &CPad__GetHydraulicJump_hook, &CPad__GetHydraulicJump);


    CHook::InlineHook("_ZN4CPad11GetLookLeftEb", &CPad__GetLookLeft_hook, &CPad__GetLookLeft);
    CHook::InlineHook("_ZN4CPad12GetLookRightEb", &CPad__GetLookRight_hook, &CPad__GetLookRight);

    CHook::InlineHook("_ZN4CPad18GetCarGunLeftRightEbb", (uintptr_t) CPad__GetCarGunLeftRight_hook,
                      (uintptr_t *) &CPad__GetCarGunLeftRight);
    CHook::InlineHook("_ZN4CPad15GetCarGunUpDownEbP11CAutomobilefb", &CPad__GetCarGunUpDown_hook,
                      &CPad__GetCarGunUpDown);
    CHook::InlineHook("_ZN4CPad14GetCarGunFiredEbb", &CPad__GetCarGunFired_hook,
                      &CPad__GetCarGunFired);

    CHook::InlineHook("_ZN4CPad13GetTurretLeftEv", &CPad__GetTurretLeft_hook, &CPad__GetTurretLeft);
    CHook::InlineHook("_ZN4CPad14GetTurretRightEv", &CPad__GetTurretRight_hook,
                      &CPad__GetTurretRight);

    CHook::InstallPLT(g_libGTASA + 0x829A40, &CPed__GetWeaponSkill_hook, &CPed__GetWeaponSkill);

    LOGI("END FUNC HOOK CPAD");


}
