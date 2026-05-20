#include <GLES2/gl2.h>
#include "../main.h"
#include "../vendor/armhook/patch.h"
#include "game.h"
#include "../net/netgame.h"
#include "../gui/gui.h"
#include "Textures/TextureDatabase.h"
#include "Textures/TextureDatabaseEntry.h"
#include "Textures/TextureDatabaseRuntime.h"
#include "Scene.h"
#include "sprite2d.h"
#include "Entity/PlayerPedGta.h"
#include "Pools.h"
#include "java/jniutil.h"
#include "game/Models/ModelInfo.h"
#include "MatrixLink.h"
#include "MatrixLinkList.h"
#include "game/Collision/Collision.h"
#include "TxdStore.h"
#include "util/CUtil.h"
#include "Coronas.h"
#include "Streaming.h"
#include "References.h"
#include "VisibilityPlugins.h"
#include "game/Animation/AnimManager.h"
#include "FileLoader.h"
#include "Renderer.h"
#include "CrossHair.h"
#include "World.h"
#include "java/jniutil.h"
#include "Widgets/TouchInterface.h"
#include "game/BuildingRemoval.h"

#include "graphics/ES2VertexBuffer.h"
#include "graphics/RQ_Commands.h"
#include "graphics/RQShader.h"
#include "Mobile/MobileMenu/MobileMenu.h"
#include "Mobile/MobileSettings/MobileSettings.h"
///skip for now later if above is already converted
extern UI* pUI;
extern CGame* pGame;
extern CNetGame *pNetGame;
extern MaterialTextGenerator* pMaterialTextGenerator;
extern CJavaWrapper *pJavaWrapper;

uint8_t byteInternalPlayer = 0;
CPedGTA* dwCurPlayerActor = 0;
uint8_t byteCurPlayer = 0;

extern "C" uintptr_t get_lib()
{
	return g_libGTASA;
}



// 0.3.7
PLAYERID FindPlayerIDFromGtaPtr(CEntityGTA* pEntity)
{
	if (pEntity == nullptr) return INVALID_PLAYER_ID;

	CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
	CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();

	PLAYERID PlayerID = pPlayerPool->FindRemotePlayerIDFromGtaPtr((CPedGTA*)pEntity);
	if (PlayerID != INVALID_PLAYER_ID) return PlayerID;

	VEHICLEID VehicleID = pVehiclePool->FindIDFromGtaPtr((CVehicleGTA*)pEntity);
	if (VehicleID != INVALID_VEHICLE_ID)
	{
		for (PLAYERID i = 0; i < MAX_PLAYERS; i++)
		{
			CRemotePlayer* pRemotePlayer = pPlayerPool->GetAt(i);
			if (pRemotePlayer && pRemotePlayer->CurrentVehicleID() == VehicleID) {
				return i;
			}
		}
	}

	return INVALID_PLAYER_ID;
}
// 0.3.7
PLAYERID FindActorIDFromGtaPtr(CPedGTA* pPed)
{
	if (pPed) {
		//return pNetGame->GetActorPool()->FindIDFromGtaPtr(pPed);
	}

	return INVALID_PLAYER_ID;
}

/* =============================================================================== */

void RenderEffects() {
//	RenderEffects();
    CHook::CallFunction<void>(g_libGTASA + 0x5C9E04);
    CHook::CallFunction<void>(g_libGTASA + 0x5EC7BC);
//    CRopes::Render();
//    CGlass::Render();
    CHook::CallFunction<void>(g_libGTASA + 0x5D32E4);
    CVisibilityPlugins::RenderReallyDrawLastObjects();
    CCoronas::Render();

    // FIXME
    CCamera& TheCamera = *reinterpret_cast<CCamera*>(g_libGTASA + 0x9F86F8);
    auto g_fx = *(uintptr_t *) (g_libGTASA + 0xA5BC20);
    CHook::CallFunction<void>(g_libGTASA + 0x4D5020, &g_fx, TheCamera.m_pRwCamera, false);

    CHook::CallFunction<void>(g_libGTASA + 0x5FA3B8);
    CHook::CallFunction<void>(g_libGTASA + 0x6F4AA0);
    CHook::CallFunction<void>(g_libGTASA + 0x5CD7EC);
    //   CClouds::VolumetricCloudsRender();
////    if (CHeli::NumberOfSearchLights || CTheScripts::NumberOfScriptSearchLights) {
////        CHeli::Pre_SearchLightCone();
////        CHeli::RenderAllHeliSearchLights();
////        CTheScripts::RenderAllSearchLights();
////        CHeli::Post_SearchLightCone();
////    }
    CHook::CallFunction<void>(g_libGTASA + 0x702670);
////    if (CReplay::Mode != MODE_PLAYBACK && !CPad::GetPad(0)->DisablePlayerControls) {
////        FindPlayerPed()->DrawTriangleForMouseRecruitPed();
////    }
    CHook::CallFunction<void>(g_libGTASA + 0x5EEFA8);
//    //CVehicleRecording::Render();
    CHook::CallFunction<void>(g_libGTASA + 0x5DE58C);
//    //CRenderer::RenderFirstPersonVehicle();
    CHook::CallFunction<void>(g_libGTASA + 0x5E2C9C);

    //DebugModules::Render3D();
}

/*void MainLoop();
void(*Render2dStuff)();
void Render2dStuff_hook()
{
    Render2dStuff();
    if(pNetGame)
    {
        CTextDrawPool* pTextDrawPool = pNetGame->GetTextDrawPool();
        if(pTextDrawPool) pTextDrawPool->Draw();
    }
    if (pUI) pUI->render();
    return;
}*/

void ShowHud()
{
    *(uint8_t*)(g_libGTASA + 0x9F13E8) = 1;
}
void MainLoop();
void Render2dStuff()
{
    ShowHud();

    if( CHook::CallFunction<bool>(g_libGTASA + 0x739F40) ) // emu_IsAltRenderTarget()
        CHook::CallFunction<void>(g_libGTASA + 0x73A848); // emu_FlushAltRenderTarget()

    CHook::CallFunction<void>("_Z25RwTexDictionarySetCurrentP15RwTexDictionary", (uintptr_t)NULL);


    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, RWRSTATE(FALSE));
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, RWRSTATE(FALSE));
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, RWRSTATE(TRUE));
    RwRenderStateSet(rwRENDERSTATESRCBLEND, RWRSTATE(rwBLENDSRCALPHA));
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, RWRSTATE(rwBLENDINVSRCALPHA));
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, RWRSTATE(rwRENDERSTATENARENDERSTATE));
    RwRenderStateSet(rwRENDERSTATECULLMODE, RWRSTATE(rwCULLMODECULLNONE));



    ( ( void(*)() )(g_libGTASA + 0x510D30))(); // прицелCHud::DrawCrossHairs(void)	000000000051C694

    auto radar = CTouchInterface::m_pWidgets[WIDGET_RADAR];
    if (radar) {
        radar->m_fOriginX = 49.0f;
        radar->m_fOriginY = 72.0f;

        radar->m_fScaleX = 45.0f;
        radar->m_fScaleY = 38.0f;
    }
    /*
#if VER_x32
    ((void (*)())(g_libGTASA + 0x00437B0C + 1))(); // DrawRadar
#else
    ((void (*)())(g_libGTASA + 0x51CFF0))();
#endif */

    CHook::CallFunction<void>("_ZN4CHud4DrawEv");

    ((void(*)(bool))(g_libGTASA + 0x5017B0))(false);
    CHook::CallFunction<void>("_Z12emu_GammaSeth", 1);
    ((void (*)(bool))(g_libGTASA + 0x69DDB8))(1u); // CMessages::Display
    ((void (*)(bool))(g_libGTASA + 0x5D6090))(1u); // CFont::RenderFontBuffer
    CHook::CallFunction<void>("_Z12emu_GammaSeth", 0);

    if(pNetGame)
    {
        //CTextDrawPool* pTextDrawPool = pNetGame->GetTextDrawPool();
       // if(pTextDrawPool) pTextDrawPool->Draw();
    }

    if (pUI) pUI->render();
    MainLoop();
}

/* =============================================================================== */

/* =============================================================================== */

void(*CRadar_DrawRadarGangOverlay)(uint32_t unk);
void CRadar_DrawRadarGangOverlay_hook(uint32_t unk)
{
	if (pNetGame)
	{
		//CGangZonePool *pGangZonePool = pNetGame->GetGangZonePool();
	//	if (pGangZonePool) {
		//	pGangZonePool->Draw(unk);
		//}
	}
}

#include "game/CGPS.hpp"
extern GPS* pGPS;

int (*CRadar__SetCoordBlip)(int r0, float X, float Y, float Z, int r4, int r5, char *name);
int CRadar__SetCoordBlip_hook(int r0, float X, float Y, float Z, int r4, int r5, char *name)
{
    if (pNetGame && !strncmp(name, "CODEWAY", 7))
    {
        float findZ = CWorld::FindGroundZForCoord(X, Y) + 1.5f;
        CVector pos = { X, Y, findZ - 1.5f };


        GPS::Set(pos, true);
        RakNet::BitStream bsSend;
        bsSend.Write(X);
        bsSend.Write(Y);
        bsSend.Write(findZ);
        pNetGame->GetRakClient()->RPC(&RPC_MapMarker, &bsSend, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
    }

    return CRadar__SetCoordBlip(r0, X, Y, Z, r4, r5, name);
}

void (*CRadar_ClearBlip)(uint32_t a2);
void CRadar_ClearBlip_hook(uint32_t a2)
{
    uintptr_t dwRetAddr = 0;
    GET_LR(dwRetAddr);

    if ((uint16_t)a2 > 249) {
        LOGI("[CRadar::ClearBlip]: Invalid blip ID (%d) called from 0x%X", (uint16_t)a2, dwRetAddr);
        return;
    }
    //im not sure about this omg
    //int32 raceBlipIndex = *(int32 *)(g_libGTASA + 0xD0DB32);
  //  if (raceBlipIndex == a2) {
    //    GPS::Set(pGame->m_vecRaceCheckpointPos, pGame->m_bRaceCheckpointsEnabled);
   // }

    CRadar_ClearBlip(a2);
}

/* =============================================================================== */

typedef struct {
    CVector     vecPosObject;
    CQuaternion m_qRotation;
    int32       wModelIndex;
    union {
        struct { // CFileObjectInstanceType
            uint32 m_nAreaCode : 8;
            uint32 m_bRedundantStream : 1;
            uint32 m_bDontStream : 1; // Merely assumed, no countercheck possible.
            uint32 m_bUnderwater : 1;
            uint32 m_bTunnel : 1;
            uint32 m_bTunnelTransition : 1;
            uint32 m_nReserved : 19;
        };
        uint32 m_nInstanceType;
    };
    int32 m_nLodInstanceIndex; // -1 - without LOD model
} stLoadObjectInstance;
VALIDATE_SIZE(stLoadObjectInstance, (VER_x32 ? 0x28 : 0x28));

CEntityGTA* (*CFileLoader__LoadObjectInstance)(CFileObjectInstance *pObject, const char *pName);
CEntityGTA* CFileLoader__LoadObjectInstance_hook(CFileObjectInstance *pObject, const char *pName)
{
    // Check if this building should be removed
    for (int i = 0; i < CBuildingRemoval::m_TotalRemovedObjects; i++)
    {
        const auto& buildingInfo = CBuildingRemoval::m_RemoveBuildings[i];
        // Check model ID match (or -1 for all models)
        if (pObject->m_nModelId == buildingInfo.modelId || buildingInfo.modelId == static_cast<uint32_t>(-1))
        {
            CVector pos;

            pos.x = pObject->m_vecPosition.x;
            pos.y = pObject->m_vecPosition.y;
            pos.z = pObject->m_vecPosition.z;

            float distance = CBuildingRemoval::GetDistanceBetween3DPoints(&pos, &buildingInfo.position);
            if (distance <= buildingInfo.radius) {
                // Replace with invisible model (19300 is commonly used as invisible model)
                pObject->m_nModelId = 19300;
                break;
            }
        }
    }
    return CFileLoader__LoadObjectInstance(pObject, pName);
}

extern int iBuildingToRemoveCount;
extern std::list<REMOVE_BUILDING_DATA> RemoveBuildingData;

int g_iLastRenderedObject;
void(*CEntity_Render)(CEntityGTA*);
void CEntity_Render_hook(CEntityGTA* thiz) {
    g_iLastRenderedObject = thiz->m_nModelIndex;
    CEntity_Render(thiz);
}

/* =============================================================================== */

/* =============================================================================== */

void (*CObject_Render)(CObjectGta* thiz);
void CObject_Render_hook(CObjectGta* thiz)
{

    //CObject_Render(thiz);
    //((void (*)(void))(g_libGTASA + (VER_x32 ? 0x005D1F98 + 1 : 0x6F6664)))();
    //((void (*)(void))(g_libGTASA + 0x5D1F5C + 1))();
}

/*((void (*)(void))(g_libGTASA + 0x5D1F48 + 1))();
				CObject_Render(thiz);
				// ActivateDirectional
				((void (*)(void))(g_libGTASA + 0x5D1F5C + 1))();*/
/* =============================================================================== */

/* =============================================================================== */

bool NotifyEnterVehicle(CVehicleGTA *_pVehicle)
{
	if(!pNetGame) {
		return false;
	}

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	if(!pVehiclePool) {
		return false;
	}

	CVehicle *pVehicle = nullptr;
	VEHICLEID VehicleID = pVehiclePool->FindIDFromGtaPtr(_pVehicle);

	if(VehicleID <= 0 || VehicleID >= MAX_VEHICLES) {
		return false;
	}

	if(!pVehiclePool->GetSlotState(VehicleID)) {
		return false;
	}

	pVehicle = pVehiclePool->GetAt(VehicleID);
	if(!pVehicle) {
		return false;
	}

	CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();

	if(pLocalPlayer) {
		FLog("Vehicle ID: %d", VehicleID);
		pLocalPlayer->SendEnterVehicleNotification(VehicleID, false);
	}

	return true;
}

int (*TaskEnterVehicle)(uintptr_t a1, uintptr_t a2);
int TaskEnterVehicleHook(uintptr_t a1, uintptr_t a2)
{
    if(!NotifyEnterVehicle((CVehicleGTA*)a1)) {
        return false;
    }

    // CTask::operator new
    uintptr_t pTask = ((uintptr_t (*)(void))(g_libGTASA + (VER_x32 ? 0x4D6A70:0x5D7414)))();

    // CTaskComplexEnterCarAsDriver::CTaskComplexEnterCarAsDriver
    ((void (__fastcall *)(uintptr_t, uintptr_t))(g_libGTASA + (VER_x32 ? 0x4F6FE0:0x6007E0)))(pTask, a1);

    // CTaskManager::SetTask
    ((int (__fastcall *)(uintptr_t, uintptr_t, int, int))(g_libGTASA + (VER_x32 ? 0x53397A:0x64E084)))(a2, pTask, 3, 0);

    return true;
}

void (*CTaskComplexLeaveCar)(uintptr_t** thiz, CVehicleGTA* pVehicle, int iTargetDoor, int iDelayTime, bool bSensibleLeaveCar, bool bForceGetOut);
void CTaskComplexLeaveCar_hook(uintptr_t** thiz, CVehicleGTA* pVehicle, int iTargetDoor, int iDelayTime, bool bSensibleLeaveCar, bool bForceGetOut)
{
	uintptr_t dwRetAddr = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (dwRetAddr));
	dwRetAddr -= g_libGTASA;

	if (dwRetAddr == 0x409A42+1 || dwRetAddr == 0x40A818+1)
	{
		if (pNetGame)
		{
			if ((CVehicleGTA*)GamePool_FindPlayerPed()->pVehicle == pVehicle)
			{
				CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
				VEHICLEID VehicleID = pVehiclePool->FindIDFromGtaPtr((CVehicleGTA*)GamePool_FindPlayerPed()->pVehicle);
				if (VehicleID != INVALID_VEHICLE_ID)
				{
					CVehicle* pVehicle = pVehiclePool->GetAt(VehicleID);
					CLocalPlayer* pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
					if (pVehicle && pLocalPlayer)
					{
						if (pVehicle->IsATrainPart())
						{
							RwMatrix mat = pVehicle->m_pVehicle->GetMatrix().ToRwMatrix();
							pLocalPlayer->GetPlayerPed()->RemoveFromVehicleAndPutAt(mat.pos.x + 2.5f, mat.pos.y + 2.5f, mat.pos.z);
						}
						else
						{
							pLocalPlayer->SendExitVehicleNotification(VehicleID);
						}
					}
				}
			}
		}
	}

	(*CTaskComplexLeaveCar)(thiz, pVehicle, iTargetDoor, iDelayTime, bSensibleLeaveCar, bForceGetOut);
}

/* =============================================================================== */

uint32_t CRadar__GetRadarTraceColor(uint32_t color, uint8_t bright, uint8_t friendly)
{
    return TranslateColorCodeToRGBA(color);
}

#if VER_x32
uint32_t CHudColours__GetIntColour(uint32 colour_id)
{
	return TranslateColorCodeToRGBA(colour_id);
}
#else
uint32_t CHudColours__GetIntColour(uintptr* thiz, uint8 colour_id)
{
    return TranslateColorCodeToRGBA(colour_id);
}
#endif

/* =============================================================================== */

void (*AND_TouchEvent)(int type, int num, int posX, int posY);
void AND_TouchEvent_hook(int type, int num, int posX, int posY)
{
	// imgui
	//bool bRet = pUI->OnTouchEvent(type, num, posX, posY);

	if (pGame->IsGamePaused())
		return AND_TouchEvent(type, num, posX, posY);

	if (pUI != nullptr)
	{
		switch (type)
		{
			case 2: // push
				pUI->touchEvent(ImVec2(posX, posY), TouchType::push);
				break;

			case 3: // move
				pUI->touchEvent(ImVec2(posX, posY), TouchType::move);
				break;

			case 1: // pop
				pUI->touchEvent(ImVec2(posX, posY), TouchType::pop);
				break;
		}

        if (pUI->keyboard()->visible() || pUI->dialog()->visible()) {
            AND_TouchEvent(1, 0, 0, 0);
            return;
        }
        else
        {
            //if (pNetGame && pNetGame->GetTextDrawPool())
           // {
            //    if (!pNetGame->GetTextDrawPool()->onTouchEvent(type, num, posX, posY)) {
             //       return AND_TouchEvent(1, 0, 0, 0);
            //    }
           // }
        }
	}

	if (pGame->IsGameInputEnabled())
		AND_TouchEvent(type, num, posX, posY);
	else
		AND_TouchEvent(1, 0, 0, 0);
}

/* =============================================================================== */

/* =============================================================================== */

/* =============================================================================== */



/* =============================================================================== */

extern CPlayerPed* g_pCurrentFiredPed;
extern BULLET_DATA* g_pCurrentBulletData;

extern int g_iLagCompensationMode;

void SendBulletSync(CVector* vecOrigin, CVector* a2, CColPoint *colPoint, CEntityGTA** ppEntity)
{
    CMatrix mat1, mat2;

    static BULLET_DATA bulletData;
    memset(&bulletData, 0, sizeof(BULLET_DATA));

    bulletData.vecOrigin.x = vecOrigin->x;
    bulletData.vecOrigin.y = vecOrigin->y;
    bulletData.vecOrigin.z = vecOrigin->z;

    bulletData.vecPos.x = colPoint->m_vecPoint.x;
    bulletData.vecPos.y = colPoint->m_vecPoint.y;
    bulletData.vecPos.z = colPoint->m_vecPoint.z;

    if (ppEntity)
    {
        CEntityGTA* pEntity = *ppEntity;
        if (pEntity)
        {
            if (g_iLagCompensationMode != 0)
            {
                bulletData.vecOffset.x = colPoint->m_vecPoint.x - pEntity->m_matrix->m_pos.x;
                bulletData.vecOffset.y = colPoint->m_vecPoint.y - pEntity->m_matrix->m_pos.y;
                bulletData.vecOffset.z = colPoint->m_vecPoint.z - pEntity->m_matrix->m_pos.z;
            }
            else
            {
                memset(&mat1, 0, sizeof(CMatrix));
                memset(&mat2, 0, sizeof(CMatrix));
                // RwMatrixOrthoNormalize
                auto entMat = pEntity->GetMatrix().ToRwMatrix();
                RwMatrixOrthoNormalize(reinterpret_cast<RwMatrix *>(&mat2), &entMat);
                // RwMatrixInvert
                Invert(mat1, mat2);

                ProjectMatrix(&bulletData.vecOffset, &mat1, &colPoint->m_vecPoint);
            }

            bulletData.pEntity = pEntity;
        }
        else bulletData.vecOffset = 0;
    }

    pGame->FindPlayerPed()->ProcessBulletData(&bulletData);
}
extern bool g_customFire;
uint32_t(*CWeapon__FireInstantHit)(CWeapon* thiz, CPedGTA* pFiringEntity, CVector* vecOrigin, CVector* muzzlePosn, CEntityGTA* targetEntity,
                                   CVector* target, CVector* originForDriveBy, bool arg6, bool muzzle);
uint32_t CWeapon__FireInstantHit_hook(CWeapon* thiz, CPedGTA* pFiringEntity, CVector* vecOrigin, CVector* muzzlePosn, CEntityGTA* targetEntity,
                                      CVector* target, CVector* originForDriveBy, bool arg6, bool muzzle)
{
    LOGI("CWeapon__FireInstantHit_hook 1");
    if (pNetGame && pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->m_pPed)		// CWeapon::Fire
    {
        if(pFiringEntity != GamePool_FindPlayerPed())
            return muzzle;

        if(pNetGame)
        {
            pNetGame->GetPlayerPool()->ApplyCollisionChecking();
        }

        if(pGame)
        {
            LOGI("CWeapon__FireInstantHit_hook 4");
            CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
            if(pPlayerPed)
                pPlayerPed->FireInstant();
        }

        if(pNetGame)
        {
            pNetGame->GetPlayerPool()->ResetCollisionChecking();
        }

        return muzzle;
    }
    LOGI("CWeapon__FireInstantHit_hook 5");
    return CWeapon__FireInstantHit(thiz, pFiringEntity, vecOrigin, muzzlePosn, targetEntity,
                                   target, originForDriveBy, arg6, muzzle);
}

bool g_bForceWorldProcessLineOfSight = false;
uint32_t (*CWeapon__ProcessLineOfSight)(CVector *vecOrigin, CVector *vecEnd, CVector *vecPos, CPedGTA **ppEntity, CWeapon *pWeaponSlot, CPedGTA **ppEntity2, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7);
uint32_t CWeapon__ProcessLineOfSight_hook(CVector *vecOrigin, CVector *vecEnd, CVector *vecPos, CPedGTA **ppEntity, CWeapon *pWeaponSlot, CPedGTA **ppEntity2, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7)
{
    uintptr_t dwRetAddr = 0;
    GET_LR(dwRetAddr);

    FLog("dwRetAddr CWeapon__ProcessLineOfSight_hook 0x%llx", dwRetAddr);

    if(dwRetAddr >= 0x6FA408 && dwRetAddr <= 0x6FBA2C)
    g_bForceWorldProcessLineOfSight = true;


    return CWeapon__ProcessLineOfSight(vecOrigin, vecEnd, vecPos, ppEntity, pWeaponSlot, ppEntity2, b1, b2, b3, b4, b5, b6, b7);
}

uint32_t(*CWorld__ProcessLineOfSight)(CVector*, CVector*, CColPoint *colPoint, CEntityGTA**, bool, bool, bool, bool, bool, bool, bool, bool);
uint32_t CWorld__ProcessLineOfSight_hook(CVector* vecOrigin, CVector* vecEnd, CColPoint *colPoint, CEntityGTA** ppEntity,
                                         bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7, bool b8)
{
    uintptr_t dwRetAddr = 0;
    GET_LR(dwRetAddr);

    if(dwRetAddr == 0x6FB42C || g_bForceWorldProcessLineOfSight)
    {
    g_bForceWorldProcessLineOfSight = false;
    //LOGI("CWorld_ProcessLineOfSight iLagCompensationMode: %d", g_iLagCompensationMode);
    static CVector vecPosPlusOffset;

    if (g_iLagCompensationMode != 2)
    {
    if (g_pCurrentFiredPed != pGame->FindPlayerPed())
    {
    if (g_pCurrentBulletData && g_pCurrentBulletData->pEntity)
    {
    if (*(uintptr_t*)(g_pCurrentBulletData->pEntity) != g_libGTASA+0x81E628) // CPlaceable
    {
    if (g_iLagCompensationMode)
    {
        vecPosPlusOffset.x = g_pCurrentBulletData->pEntity->GetPosition().x + g_pCurrentBulletData->vecOffset.x;
        vecPosPlusOffset.y = g_pCurrentBulletData->pEntity->GetPosition().y + g_pCurrentBulletData->vecOffset.y;
        vecPosPlusOffset.z = g_pCurrentBulletData->pEntity->GetPosition().z + g_pCurrentBulletData->vecOffset.z;
    }
    else
    {
        //FLog("vecPosPlusOffset %f %f %f", vecPosPlusOffset.x, vecPosPlusOffset.y, vecPosPlusOffset.z);
        //FLog("pEntity->GetMatrix().m_up %f %f %f", g_pCurrentBulletData->pEntity->GetMatrix().m_up.x, g_pCurrentBulletData->pEntity->GetMatrix().m_up.y, g_pCurrentBulletData->pEntity->GetMatrix().m_up.z);
        //FLog("g_pCurrentBulletData->vecOffset %f %f %f", g_pCurrentBulletData->vecOffset.x, g_pCurrentBulletData->vecOffset.y, g_pCurrentBulletData->vecOffset.z);
        ProjectMatrix((CVector*)&vecPosPlusOffset, &g_pCurrentBulletData->pEntity->GetMatrix(), &g_pCurrentBulletData->vecOffset);
        //vecPosPlusOffset.x = pEntity->GetMatrix().m_up.x * g_pCurrentBulletData->vecOffset.z + pEntity->GetMatrix().m_forward.x * g_pCurrentBulletData->vecOffset.y + pEntity->GetMatrix().m_right.x * g_pCurrentBulletData->vecOffset.x + pEntity->GetMatrix().m_pos.x;
        //vecPosPlusOffset.y = pEntity->GetMatrix().m_up.y * g_pCurrentBulletData->vecOffset.z + pEntity->GetMatrix().m_forward.y * g_pCurrentBulletData->vecOffset.y + pEntity->GetMatrix().m_right.y * g_pCurrentBulletData->vecOffset.x + pEntity->GetMatrix().m_pos.y;
        //vecPosPlusOffset.z = pEntity->GetMatrix().m_up.z * g_pCurrentBulletData->vecOffset.z + pEntity->GetMatrix().m_forward.z * g_pCurrentBulletData->vecOffset.y + pEntity->GetMatrix().m_right.z * g_pCurrentBulletData->vecOffset.x + pEntity->GetMatrix().m_pos.z;
    }

        vecEnd->x = vecPosPlusOffset.x - vecOrigin->x + vecPosPlusOffset.x;
        vecEnd->y = vecPosPlusOffset.y - vecOrigin->y + vecPosPlusOffset.y;
        vecEnd->z = vecPosPlusOffset.z - vecOrigin->z + vecPosPlusOffset.z;
    }
    }
    }
    }

    uint32_t result = CWorld__ProcessLineOfSight(vecOrigin, vecEnd, colPoint, ppEntity, b1, b2, b3, b4, b5, b6, b7, b8);

    if (g_iLagCompensationMode == 2)
    {
        if (g_pCurrentFiredPed == pGame->FindPlayerPed()) {
            SendBulletSync(vecOrigin, vecEnd, colPoint, ppEntity);
        }
        return result;
    }

    if (g_pCurrentFiredPed)
    {
    if (g_pCurrentFiredPed != pGame->FindPlayerPed())
    {
    if (g_pCurrentBulletData)
    {
    if (g_pCurrentBulletData->pEntity == nullptr)
    {
    CPedGTA* pLocalPed = GamePool_FindPlayerPed();
    if (*ppEntity == GamePool_FindPlayerPed() ||
    pLocalPed->IsInVehicle() && *ppEntity == pLocalPed->pVehicle)
    {
    result = 0;
    *ppEntity = nullptr;
    colPoint->m_vecPoint.x = 0.0f;
    colPoint->m_vecPoint.y = 0.0f;
    colPoint->m_vecPoint.z = 0.0f;
    return result;
    }
    }
    }
    }
    else {
    SendBulletSync(vecOrigin, vecEnd, colPoint, ppEntity);
    }
    }

    return result;
    }

return CWorld__ProcessLineOfSight(vecOrigin, vecEnd, colPoint, ppEntity, b1, b2, b3, b4, b5, b6, b7, b8);
}

// 0.3.7
uint32_t(*CWeapon__FireSniper)(CWeapon* thiz, CPedGTA* pFiringEntity, CEntityGTA* victim, CVector* target);
uint32_t CWeapon__FireSniper_hook(CWeapon* thiz, CPedGTA* pFiringEntity, CEntityGTA* victim, CVector* target)
{
	if (pFiringEntity == GamePool_FindPlayerPed())
	{
		if (pGame)
		{
			CPlayerPed* pPlayerPed = pGame->FindPlayerPed();
			if (pPlayerPed) {
				pPlayerPed->FireInstant();
			}
		}
	}

	return true;
}
// 0.3.7
bool(*CBulletInfo_AddBullet)(CEntityGTA* creator, int weaponType, CVector pos, CVector velocity);
bool CBulletInfo_AddBullet_hook(CEntityGTA* creator, int weaponType, CVector pos, CVector velocity)
{
	velocity.x *= 50.0f;
	velocity.y *= 50.0f;
	velocity.z *= 50.0f;

	CBulletInfo_AddBullet(creator, weaponType, pos, velocity);

	// CBulletInfo::Update
	CHook::CallFunction<void>("_ZN11CBulletInfo6UpdateEv");
	return true;
}

#pragma pack(push, 1)
struct CPedDamageResponseCalculator
{
    CPedGTA* m_pDamager;
	float m_fDamageFactor;
	int m_pedPieceType;
	int m_weaponType;
};
#pragma pack(pop)
// 0.3.7
bool ComputeDamageResponse(CPedDamageResponseCalculator* calculator, CPedGTA* pPed)
{
    CPedGTA* pGamePed = GamePool_FindPlayerPed();
	bool isLocalPed = false;

	if (!pNetGame) return false;

    CPedGTA* pDamager = calculator->m_pDamager;
	//if (pDamager != pGamePed && IsValidGamePed(pGamePed)) /* CCivilianPed */
	//	return true;

	if (pPed == pGamePed) {
		isLocalPed = true;
	}
	else if (pDamager != pGamePed) {
		return false;
	}

	CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
	CLocalPlayer* pLocalPlayer = pPlayerPool->GetLocalPlayer();
	PLAYERID PlayerID;

	if (isLocalPed)
	{
		PlayerID = FindPlayerIDFromGtaPtr(pDamager);
		pLocalPlayer->SendTakeDamageEvent(PlayerID,
										  calculator->m_fDamageFactor,
										  calculator->m_weaponType,
										  calculator->m_pedPieceType);
	}
	else
	{
		PlayerID = FindPlayerIDFromGtaPtr(pPed);
		if (PlayerID != INVALID_PLAYER_ID)
		{
			pLocalPlayer->SendGiveDamageEvent(PlayerID,
											  calculator->m_fDamageFactor,
											  calculator->m_weaponType,
											  calculator->m_pedPieceType);
			if (pPlayerPool->GetAt(PlayerID)->IsNPC())
				return true;
		}
		else
		{
			PLAYERID ActorID = FindActorIDFromGtaPtr(pPed);
			if (ActorID != INVALID_PLAYER_ID) {
				pLocalPlayer->SendGiveDamageEvent(ActorID,
												  calculator->m_fDamageFactor,
												  calculator->m_weaponType,
												  calculator->m_pedPieceType);
				return true;
			}
		}
	}


	// :check_friendly_fire
	if (!pNetGame->m_pNetSet->bFriendlyFire)
		return false;
	uint8_t byteTeam = pPlayerPool->GetLocalPlayer()->m_byteTeam;
	if (byteTeam == NO_TEAM ||
		PlayerID == INVALID_PLAYER_ID ||
		pPlayerPool->GetAt(PlayerID)->m_byteTeam != byteTeam) {
		return false;
	}

	return true;
}

#include "CFirstPersonCamera.hpp"
//#include "CHandlingDefault.h"

void (*CCam__Process)(uintptr_t thiz);

void CCam__Process_hook(uintptr_t thiz)
{
    if (!CFirstPersonCamera::IsEnabled()) {
        CCam__Process(thiz);
        return;
    }

    CVector vecSavedSpeed;
    CVehicle* pVeh = nullptr;

    float v6 = *(float*)(g_libGTASA + 0x879434);

    if (pNetGame && (*(uint16_t*)(thiz + 14) == 16 || *(uint16_t*)(thiz + 14) == 18)) {
        if (auto playerPool = pNetGame->GetPlayerPool()) {
            if (auto localPlayer = playerPool->GetLocalPlayer()) {
                CPlayerPed* pPed = localPlayer->GetPlayerPed();
                CVehicleGTA* contactVeh = (CVehicleGTA*)pPed->GetEntityUnderPlayer();
                VEHICLEID vehicleId = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(contactVeh);
                CVehicle* pVeh = pNetGame->GetVehiclePool()->GetAt(vehicleId);

                if (pVeh && pVeh->m_pVehicle) {
                    CVector vecSavedSpeed = pVeh->m_pVehicle->m_vecMoveSpeed;

                    pVeh->m_pVehicle->m_vecMoveSpeed.x *= 6.0f;
                    pVeh->m_pVehicle->m_vecMoveSpeed.y *= 6.0f;
                    pVeh->m_pVehicle->m_vecMoveSpeed.z *= 6.0f;

                    CCam__Process(thiz);

                    pVeh->m_pVehicle->m_vecMoveSpeed = vecSavedSpeed;

                    *(float*)(g_libGTASA + 0x879434) = 200.0f;
                }
            }
        }
    }

    CCam__Process(thiz);

    if (pVeh && pVeh->m_pVehicle) {
        pVeh->m_pVehicle->m_vecMoveSpeed = vecSavedSpeed;
        *(float*)(g_libGTASA + 0x879434) = v6;
    }

    if (*(uint16_t*)(thiz + 14) == 4 || *(uint16_t*)(thiz + 14) == 53) {
        if (auto playerPool = pNetGame->GetPlayerPool()) {
            if (auto localPlayer = playerPool->GetLocalPlayer()) {
                CPlayerPed* pPed = localPlayer->GetPlayerPed();
                if (pPed) {

                    *(uint32_t*)(g_libGTASA + 0x9F86F8 + 128) = 0xFFFFFFFFFFFFFFFFLL;
                    *(uint8_t*)(g_libGTASA + 0x9F86F8 + 48) = 0;

                    CFirstPersonCamera::ProcessCameraOnFoot(thiz, pPed);
                }
            }
        }
    }

    if (*(uint16_t*)(thiz + 14) == 16 || *(uint16_t*)(thiz + 14) == 18) {
        if (auto playerPool = pNetGame->GetPlayerPool()) {
            if (auto localPlayer = playerPool->GetLocalPlayer()) {
                CPlayerPed* pPed = localPlayer->GetPlayerPed();
                if (pPed) {
#if VER_x32
                    *(uint32_t*)(g_libGTASA + 0x00951FA8 + 120) = 0xFFFFFFFF;
                    *(uint32_t*)(g_libGTASA + 0x00951FA8 + 124) = 0xFFFFFFFF;
                    *(uint8_t*)(g_libGTASA + 0x00951FA8 + 40) = 0;
#else
                    *(uint32_t*)(g_libGTASA + 0x9F86F8 + 128) = 0xFFFFFFFFFFFFFFFFLL;
                    *(uint8_t*)(g_libGTASA + 0x9F86F8 + 48) = 0;
#endif
                    CFirstPersonCamera::ProcessCameraInVeh(thiz, pPed, pVeh);
                }
            }
        }
    }
}

// 0.3.7
void (*CPedDamageResponseCalculator__ComputeDamageResponse)(CPedDamageResponseCalculator* thiz, CPedGTA* pPed, uintptr_t* a3, uint32_t a4);
void CPedDamageResponseCalculator__ComputeDamageResponse_hook(CPedDamageResponseCalculator* thiz, CPedGTA* pPed, uintptr_t *a3, uint32_t a4)
{
	if (thiz == nullptr || pPed == nullptr || a3 == nullptr) return;

    if (ComputeDamageResponse(thiz, pPed))
        return;

	CPedDamageResponseCalculator__ComputeDamageResponse(thiz, pPed, a3, a4);
}

void (*CRenderer_RenderEverythingBarRoads)();
void CRenderer_RenderEverythingBarRoads_hook() {

	CRenderer_RenderEverythingBarRoads();

	//if (pNetGame) {
	//	CObjectPool* pObjectPool = pNetGame->GetObjectPool();
	//	if (pObjectPool) {
	//		for (OBJECTID i = 0; i < MAX_OBJECTS; i++) {
	//			CObject* pObject = pObjectPool->GetAt(i);
	//			if (pObject && pObject->m_bForceRender) {
    ///                // CEntity::PreRender
     //               ((void (*)(CEntityGTA*))(*(void**)(pObject->m_pEntity + (VER_x32 ? 0x48:0x48*2))))(pObject->m_pEntity);

     //               // CRenderer::RenderOneNonRoad
     //               ((void (*)(CEntityGTA*))(g_libGTASA+ (VER_x32 ? 0x41030C + 1:0x4F56E0)))(pObject->m_pEntity);
	//			}
	//		}
	//	}
}

#include "CFPSFix.h"
#include "Pickups.h"
#include "TimeCycle.h"
#include "game/Pipelines/CustomCar/CustomCarEnvMapPipeline.h"
#include "game/Pipelines/CustomBuilding/CustomBuildingDNPipeline.h"
#include "COcclusion.h"
#include "RealTimeShadowManager.h"

#include "game/Widgets/WidgetGta.h"

CFPSFix g_fps;

void (*ANDRunThread)(void* a1);
void ANDRunThread_hook(void* a1)
{
	g_fps.PushThread(gettid());

	ANDRunThread(a1);
}

static constexpr float ar43 = 4.0f/3.0f;
float *ms_fAspectRatio;
void (*DrawCrosshair)(uintptr_t* thiz);
void DrawCrosshair_hook(uintptr_t* thiz)
{
	float save1 = CCamera::m_f3rdPersonCHairMultX;
	CCamera::m_f3rdPersonCHairMultX = 0.530f - (*ms_fAspectRatio - ar43) * 0.01125f;

	float save2 = CCamera::m_f3rdPersonCHairMultY;
	CCamera::m_f3rdPersonCHairMultY = 0.400f + (*ms_fAspectRatio - ar43) * 0.03600f;

	DrawCrosshair(thiz);

	CCamera::m_f3rdPersonCHairMultX = save1;
	CCamera::m_f3rdPersonCHairMultY = save2;
}

int g_iCounterVehicleCamera = 0;
int (*CPad__CycleCameraModeDownJustDown)(void*);
int CPad__CycleCameraModeDownJustDown_hook(void* thiz)
{
    CPedGTA* pPed = GamePool_FindPlayerPed();
    if (!pPed)
        return 0;

    static uint32_t lastTick = GetTickCount();
    bool bPressed = false;

    if (bIsTouchCameraButt && GetTickCount() - lastTick >= 250)
    {
        bIsTouchCameraButt = false;
        bPressed = true;
        lastTick = GetTickCount();
    }

    if (pPed->bInVehicle)
    {
        if (bPressed)
        {
            g_iCounterVehicleCamera++;
        }

        if (g_iCounterVehicleCamera == 6)
        {
            CFirstPersonCamera::SetEnabled(true);
            return 0;
        }
        else if (g_iCounterVehicleCamera >= 7)
        {
            g_iCounterVehicleCamera = 0;
            CFirstPersonCamera::SetEnabled(false);
            return 1;
        }
        else
        {
            CFirstPersonCamera::SetEnabled(false);
        }

        return bPressed;
    }

    return 0;
}

int (*RwFrameAddChild)(int a1, int a2);
int RwFrameAddChild_hook(int a1, int a2)
{
	if(a1 == 0 || a2 == 0) return 0;
	return RwFrameAddChild(a1, a2);
}

int iLastTouchedWidgetId = -1;

int iLastReleasedWidgetId = -1;

int (*CTouchInterface__IsReleased)(int iWidgetId, int iUnk, int iEnableWidget);
int CTouchInterface__IsReleased_hook(int iWidgetId, int iUnk, int iEnableWidget)
{
	uintptr_t dwRetAddr = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (dwRetAddr));
	dwRetAddr -= g_libGTASA;

	int iReleased = CTouchInterface__IsReleased(iWidgetId, iUnk, iEnableWidget);
	if(iReleased && iEnableWidget)
	{
		iLastReleasedWidgetId = iWidgetId;
	}

	return iReleased;
}



int (*CCustomRoadsignMgr_RenderRoadsignAtomic)(int a1, int a2);
int CCustomRoadsignMgr_RenderRoadsignAtomic_hook(int a1, int a2)
{
	if ( a1 )
		return CCustomRoadsignMgr_RenderRoadsignAtomic(a1, a2);
}


RpClump* (*RpClumpForAllAtomics_orig)(RpClump* clump, RpAtomicCallBack callback, void* data);
RpClump* RpClumpForAllAtomics_hook(RpClump* clump, RpAtomicCallBack callback, void* data) {
    // --- CRASH FIX ---
    if (!clump || (uintptr_t) clump < 0x1000) {
        return nullptr;
    }

    return 0;
}

int (*CPed_UpdatePosition)(CPedGTA* a1);
int CPed_UpdatePosition_hook(CPedGTA* a1)
{
	int result; // r0

	if ( GamePool_FindPlayerPed() == a1 )
		result = CPed_UpdatePosition(a1);
	return result;
}

void (*CCamera__Process)(uintptr_t thiz);
void CCamera__Process_hook(uintptr_t thiz)
{
	//if(pGame->GetCamera())
		//pGame->GetCamera()->Update();

	CCamera__Process(thiz);
}

extern CJavaWrapper* pJavaWrapper;
void (*MainMenuScreen__OnExit)();
void MainMenuScreen__OnExit_hook()
{
	pGame->bIsGameExiting = true;

	pNetGame->GetRakClient()->Disconnect(0);

	pJavaWrapper->exitGame();
}

void (*rqVertexBufferSelect)(unsigned int **result);
void rqVertexBufferSelect_hook(unsigned int **result)
{
	uint32_t buffer = *(uint32_t *)*result;
	*result += 4;
	if ( buffer )
	{
		glBindBuffer(34962, *(uint32_t *)(buffer + 8));
		*(uint32_t*)(g_libGTASA + 0x6B8AF0) = 0;
	}
	else
	{
		glBindBuffer(34962, 0);
	}
}

uintptr_t* (*rpMaterialListDeinitialize)(RpMaterialList* matList);
uintptr_t* rpMaterialListDeinitialize_hook(RpMaterialList* matList)
{
	if(!matList || !matList->materials)
		return nullptr;

	return rpMaterialListDeinitialize(matList);
}

void (*rqVertexBufferDelete)(unsigned int **result);
void rqVertexBufferDelete_hook(unsigned int **result)
{
	uint32_t* buffer = *(uint32_t **)*result;
	*result += 4;
	glDeleteBuffers(1, reinterpret_cast<const GLuint *>(buffer + 2));
	buffer[2] = 0;
	if ( buffer )
		(*(void (**)(uint32_t *))(*buffer + 4))(buffer);
}

void rotate_ped_if_local(unsigned int *a1, unsigned int *a2)
{
	if ( GamePool_FindPlayerPed() == (CPedGTA*)a2 )
		*(uint32_t *)(a2 + 0x560) = *a1;
}

// 006778B0
int (*rxOpenGLDefaultAllInOneRenderCB)(RwResEntry* resEntry, uintptr_t object, uint8_t type, uint32_t flags);
int rxOpenGLDefaultAllInOneRenderCB_hook(RwResEntry* resEntry, uintptr_t object, uint8_t type, uint32_t flags)
{
	if(!resEntry || !flags)
		return 0;

	return rxOpenGLDefaultAllInOneRenderCB(resEntry, object, type, flags);
}

// 00677CB4
int (*CCustomBuildingDNPipeline__CustomPipeRenderCB)(RwResEntry* resEntry, uintptr_t object, uint8_t type, uint32_t flags);
int CCustomBuildingDNPipeline__CustomPipeRenderCB_hook(RwResEntry* resEntry, uintptr_t object, uint8_t type, uint32_t flags)
{
    if(!resEntry || !flags)
        return 0;

	return CCustomBuildingDNPipeline__CustomPipeRenderCB(resEntry, object, type, flags);
}




float float_4DD9E8;
float ms_fTimeStep;
float fMagic = 50.0f / 30.0f;
void (*CTaskSimpleUseGun__SetMoveAnim)(uintptr_t *thiz, uintptr_t *a2);
void CTaskSimpleUseGun__SetMoveAnim_hook(uintptr_t *thiz, uintptr_t *a2)
{
	ms_fTimeStep = *(float*)(g_libGTASA + 0x96B500);
	float_4DD9E8 = *(float*)(g_libGTASA + 0x4DD9E8);
	float_4DD9E8 = (fMagic) * (0.1f / ms_fTimeStep);
	CTaskSimpleUseGun__SetMoveAnim(thiz, a2);
}

int (*CAnimManager_UncompressAnimation)(int result);
int CAnimManager_UncompressAnimation_hook(int result)
{
	if ( result )
		return CAnimManager_UncompressAnimation(result);
	return 0;
}

void readVehiclesAudioSettings();

void (*CVehicleModelInfo__SetupCommonData)();



extern VehicleAudioPropertiesStruct VehicleAudioProperties[20000];

int (*CRadar__DrawRadarSprite)(unsigned int iconID, float x, float y, unsigned int alpha);
int CRadar__DrawRadarSprite_hook(unsigned int iconID, float x, float y, unsigned int alpha)
{
    if(iconID == 4)
    {
        return 0;
    }
    return CRadar__DrawRadarSprite(iconID, x, y, alpha);
}

bool m_bUseSquareRadar = true; // todo: move to settings.ini
void(*DrawRadarMask)();
void DrawRadarMask_hook()
{
    //the bss for this has been detached
    //CHook::UnFuck(g_libGTASA + (VER_x32? 0x444510 : 0x710A20));
   // if(m_bUseSquareRadar)
  //      *(float*)(g_libGTASA + (VER_x32? 0x444510 : 0x710A20)) = 0.0001f;
  //  else
  //      *(float*)(g_libGTASA + (VER_x32? 0x444510 : 0x710A20)) = 1.5708f;

    DrawRadarMask();
//#if !VER_x32
 //   *(float*)(g_libGTASA + 0x710A20) = 1.5708f;
//#endif
}

static char szLastBufferedName[40];
int (*cHandlingDataMgr__FindExactWord)(uintptr_t *thiz, char* line, char* nameTable, int entrySize, int entryCount);
int cHandlingDataMgr__FindExactWord_hook(uintptr_t *thiz, char* line, char* nameTable, int entrySize, int entryCount)
{
    strncpy(&szLastBufferedName[0], line, entrySize);
    FLog("Handling Data: line %s, nameTable %s, entrySize %d, entryCount %d", line, nameTable, entrySize, entryCount);
    return cHandlingDataMgr__FindExactWord(thiz, line, nameTable, entrySize, entryCount);
}

float (*CRadar__LimitRadarPoint)(float* pos);
float CRadar__LimitRadarPoint_hook(float* pos)
{
    float r, angle;
    r = sqrtf(pos[0] * pos[0] + pos[1] * pos[1]);

    if (m_bUseSquareRadar)
    {
        if (r > 1.0)
        {
            if (pos[0] > -1.0f && pos[0] < 1.0f && pos[1] > -1.0f && pos[1] < 1.0f)
                r = 0.99f;
            else
            {
                angle = atan2f(pos[1], pos[0]) * 57.295779513f + 180.0f;
                if (angle <= 45.0f || angle > 315.0f)
                {
                    pos[0] = 1.0f;
                    pos[1] = sinf(angle / 57.295779513f) * 1.4142135623f;
                }
                else if (angle > 45 && angle <= 135)
                {
                    pos[0] = cosf(angle / 57.295779513f) * 1.4142135623f;
                    pos[1] = 1.0f;
                }
                else if (angle > 135 && angle <= 225)
                {
                    pos[0] = -1.0f;
                    pos[1] = sinf(angle / 57.295779513f) * 1.4142135623f;
                }
                else
                {
                    pos[0] = cosf(angle / 57.295779513f) * 1.4142135623f;
                    pos[1] = -1.0f;
                }

                pos[0] *= (-1.0f);
                pos[1] *= (-1.0f);
            }
        }
        return r;
    }

    return CRadar__LimitRadarPoint(pos);
}

void (*CVisibilityPlugins_RenderEntity_orig)(CEntityGTA* entity, float alpha);
void CVisibilityPlugins_RenderEntity_hook(CEntityGTA* entity, float alpha)
{
    if (!entity || !entity->m_pRwObject)
        return;

    CVisibilityPlugins_RenderEntity_orig(entity, alpha);
}

RpAtomic* (*AtomicDefaultRenderCallBack_orig)(RpAtomic* atomic);
RpAtomic* AtomicDefaultRenderCallBack_hook(RpAtomic* atomic)
{
    if (!atomic || (uintptr_t)atomic < 0x1000)
    {
        return atomic;
    }
    if (!atomic->geometry || (uintptr_t)atomic->geometry < 0x1000)
    {
        return atomic;
    }
    if (!atomic || !atomic->geometry)
    {
        return atomic;
    }
    return AtomicDefaultRenderCallBack_orig(atomic);
    return AtomicDefaultRenderCallBack_orig(atomic);
}

void (*CVehicle_DoHeadLightReflection)(
        CVehicleGTA* thiz,
        CMatrix* mat,
        uint32 flags,
        uint8 left,
        uint8 right
);

void (*CVehicle_DoHeadLightReflectionTwin)(
        CVehicleGTA* thiz,
        CMatrix* mat
);

void (*CVehicle_DoHeadLightReflectionSingle)(
        CVehicleGTA* thiz,
        CMatrix* mat,
        uint8 side
);


void (*CVehicle_DoReverseLightEffect)(
        CVehicleGTA* thiz,
        int mode,
        CMatrix* mat,
        uint8 r,
        uint8 g,
        uint32 intensity,
        uint8 a
);

void CVehicle_DoHeadLightReflection_hook(
        CVehicleGTA* thiz,
        CMatrix* mat,
        uint32 flags,
        uint8 left,
        uint8 right)
{
    if (!thiz || !mat)
        return;

    CVehicle_DoHeadLightReflection(thiz, mat, flags, left, right);
}

void CVehicle_DoHeadLightReflectionTwin_hook(
        CVehicleGTA* thiz,
        CMatrix* mat)
{
    if (!thiz || !mat)
        return;

    CVehicle_DoHeadLightReflectionTwin(thiz, mat);
}

void CVehicle_DoHeadLightReflectionSingle_hook(
        CVehicleGTA* thiz,
        CMatrix* mat,
        uint8 side)
{
    if (!thiz || !mat)
        return;

    CVehicle_DoHeadLightReflectionSingle(thiz, mat, side);
}

void CVehicle_DoReverseLightEffect_hook(
        CVehicleGTA* thiz,
        int mode,
        CMatrix* mat,
        uint8 r,
        uint8 g,
        uint32 intensity,
        uint8 a)
{
    r = 220;
    g = 220;
    intensity *= 2;

    CVehicle_DoReverseLightEffect(thiz, mode, mat, r, g, intensity, a);
}

void (*CAEWeatherAudioEntity__UpdateParameter)(uintptr_t thiz, uintptr_t a1, uint16_t a2);
void CAEWeatherAudioEntity__UpdateParameter_hook(uintptr_t thiz, uintptr_t a1, uint16_t a2)
{
    uint32_t* pArea = (uint32_t*)(g_libGTASA + 0xA00238);
    uint32_t old = *pArea;
    if (pGame->m_sound.bDisableInteriorAmbient) *pArea = 1;
    CAEWeatherAudioEntity__UpdateParameter(thiz, a1, a2);
    *pArea = old;
}

/* =============================================================================== */

void InstallHuaweiCrashFixHooks()
{
	CHook::InstallPLT(g_libGTASA + 0x677498, (uintptr_t)rqVertexBufferSelect_hook, (uintptr_t*)&rqVertexBufferSelect);
	CHook::InstallPLT(g_libGTASA + 0x679B14, (uintptr_t)rqVertexBufferDelete_hook, (uintptr_t*)&rqVertexBufferDelete);
	//CHook::InstallPLT(g_libGTASA + 0x677B6C, (uintptr_t)rqSetAlphaTest_hook, (uintptr_t*)&rqSetAlphaTest);
}



void InstallWeaponFireHooks()
{
	//CHook::InstallPLT(g_libGTASA + 0x6716D0, (uintptr_t)CWeapon_FireInstantHit_hook, (uintptr_t*)&CWeapon_FireInstantHit);
	//CHook::InstallPLT(g_libGTASA + 0x671F10, (uintptr_t)CWorld_ProcessLineOfSight_hook, (uintptr_t*)&CWorld_ProcessLineOfSight);
	//CHook::InstallPLT(g_libGTASA + 0x670A10, (uintptr_t)CWeapon_FireSniper_hook, (uintptr_t*)&CWeapon_FireSniper);
	//CHook::InstallPLT(g_libGTASA + 0x66EAC4, (uintptr_t)CBulletInfo_AddBullet_hook, (uintptr_t*)&CBulletInfo_AddBullet);
}

void ReadSettingFile();
void ApplyFPSPatch(uint8_t fps);
void (*NvUtilInit)();
void NvUtilInit_hook() {
    FLog("NvUtilInit");
    g_pszStorage = "/storage/emulated/0/Android/media/com.kurdish.roleplay/";

    LOGI("Storage located at %s", g_pszStorage);
    NvUtilInit();
    ApplyFPSPatch(120);
    ReadSettingFile();

}

struct stFile
{
    int isFileExist;
    FILE *f;
};

char lastFile[123];

stFile* NvFOpen(const char *r1)
{
    LOGI("NvFOpen %s", r1);
    strcpy(lastFile, r1);
    g_pszStorage = "/storage/emulated/0/Android/media/com.kurdish.roleplay/";
    static char path[255]{};
    memset(path, 0, sizeof(path));

    if (strstr(r1, "emulated") != NULL)
    {
        // text contains "emulated"
        sprintf(path, "%s", r1);
    }
    else {

        sprintf(path, "%s%s", g_pszStorage, r1);
    }

    if(!strncmp(r1, "data/script/mainv1.scm", 22))
    {
        sprintf(path, "%sSAMP/main.scm", g_pszStorage);
        FLog("Loading mainv1", path);
        FLog("Loading %s", path);
    }
    // ----------------------------
    if(!strncmp(r1, "data/script/scriptv1.img", 24))
    {
        sprintf(path, "%sSAMP/script.img", g_pszStorage);
        FLog("Loading script.img..");
        FLog("Loading %s", path);
    }
    // ----------------------------
    if(!strncmp(r1, "data/peds.ide", 13))
    {
        sprintf(path, "%sSAMP/peds.ide", g_pszStorage);
        FLog("Loading peds.ide..");
        FLog("Loading %s", path);
    }
    // ----------------------------
    if(!strncmp(r1, "data/vehicles.ide", 17))
    {
        sprintf(path, "%sSAMP/vehicles.ide", g_pszStorage);
        FLog("Loading vehicles.ide..");
        FLog("Loading %s", path);
    }

    if (!strncmp(r1, "data/gta.dat", 12))
    {
        sprintf(path, "%sSAMP/gta.dat", g_pszStorage);
        FLog("Loading gta.dat..");
        FLog("Loading %s", path);
    }

    if (!strncmp(r1, "data/handling.cfg", 17))
    {
        sprintf(path, "%sSAMP/handling.cfg", g_pszStorage);
        FLog("Loading handling.cfg..");
        FLog("Loading %s", path);
    }

    if (!strncmp(r1, "data/weapon.dat", 15))
    {
        sprintf(path, "%sSAMP/weapon.dat", g_pszStorage);
        FLog("Loading weapon.dat..");
        FLog("Loading %s", path);
    }





#if VER_x32
    auto *st = (stFile*)malloc(8);
#else
    auto *st = (stFile*)malloc(0x10);
#endif
    st->isFileExist = false;

    FILE *f  = fopen(path, "rb");

    if(f)
    {
        st->isFileExist = true;
        st->f = f;
        return st;
    }
    else
    {
        FLog("NVFOpen hook | Error: file not found (%s)", path);
        free(st);
        return nullptr;
    }
}

bool g_bPlaySAMP = false;

void MainMenu_OnStartSAMP()
{
    if(g_bPlaySAMP) return;
    //InitInMenu();
    pGame->StartGame();

    CHook::CallFunction<void>(g_libGTASA + 0x718FCC); //  Menu_NewGame();



   // CHook::CallFunction<void>(g_libGTASA + 0x5509AC);
    g_bPlaySAMP = true;
}

unsigned int (*MainMenuScreen__Update)(uintptr_t thiz, float a2);
unsigned int MainMenuScreen__Update_hook(uintptr_t thiz, float a2)
{
    unsigned int ret = MainMenuScreen__Update(thiz, a2);
    MainMenu_OnStartSAMP();
    return ret;
}

void (*StartGameScreen__OnNewGameCheck)();
void StartGameScreen__OnNewGameCheck_hook()
{
    // отключить кнопку начать игру
    if(g_bPlaySAMP)
        return;

    StartGameScreen__OnNewGameCheck();
}

void (*CTaskSimpleUseGun__RemoveStanceAnims)(uintptr* thiz, void* ped, float a3);
void CTaskSimpleUseGun__RemoveStanceAnims_hook(uintptr* thiz, void* ped, float a3)
{
    if(!thiz)
        return;

    uintptr* m_pAnim = (uintptr*)(thiz + 0x2c);
    if(m_pAnim) {
        if (!((uintptr *)(m_pAnim + 0x14)))
            return;
    }
    CTaskSimpleUseGun__RemoveStanceAnims(thiz, ped, a3);
}

int (*CCollision__ProcessVerticalLine)(float *a1, float *a2, int a3, int a4, int *a5, int a6, int a7, int a8);
int CCollision__ProcessVerticalLine_hook(float *a1, float *a2, int a3, int a4, int *a5, int a6, int a7, int a8)
{
    int result; // r0

    if (a3)
        result = CCollision__ProcessVerticalLine(a1, a2, a3, a4, a5, a6, a7, a8);
    else
        result = 0;
    return result;
}



int (*CTaskSimpleGetUp__ProcessPed)(uintptr_t* thiz, CPedGTA* ped);
int CTaskSimpleGetUp__ProcessPed_hook(uintptr_t* thiz, CPedGTA* ped)
{
    //return false;
    if(!ped)return 0;
    int res = 0;
    try {
        res = CTaskSimpleGetUp__ProcessPed(thiz, ped);
    }
    catch(...) {
        return 0;
    }

    return res;
}

int64 getmip()
{
    return 1;
}

uint64_t* RQCommand_rqSetAlphaTest(uint64_t *result)
{
    *result += 8;
    return result;
}

int64 GetInputType(void)
{
    return 0LL;
}

int(*CAnimBlendNode__FindKeyFrame)(int, float, int, int);
int CAnimBlendNode__FindKeyFrame_hook(int a1, float a2, int a3, int a4)
{
    if (*(uintptr_t*)(a1 + 16))
    {
        return CAnimBlendNode__FindKeyFrame(a1, a2, a3, a4);
    }
    else return 0;
}

RwFrame* CClumpModelInfo_GetFrameFromId_Post(RwFrame* pFrameResult, RpClump* pClump, int id)
{
    if (pFrameResult)
        return pFrameResult;

    uintptr_t calledFrom = 0;
    __asm__ volatile ("mov %0, lr" : "=r" (calledFrom));
    calledFrom -= g_libGTASA;

    if (calledFrom == 0x00515708                // CVehicle::SetWindowOpenFlag
        || calledFrom == 0x00515730             // CVehicle::ClearWindowOpenFlag
        || calledFrom == 0x00338698             // CVehicleModelInfo::GetOriginalCompPosition
        || calledFrom == 0x00338B2C)            // CVehicleModelInfo::CreateInstance
        return nullptr;

    for (uint i = 2; i < 40; i++)
    {
        RwFrame* pNewFrameResult = nullptr;
        uint     uiNewId = id + (i / 2) * ((i & 1) ? -1 : 1);

        pNewFrameResult = ((RwFrame * (*)(RpClump * pClump, int id))(g_libGTASA + (VER_2_1 ? 0x003856D0 : 0x00335CC0) + 1))(pClump, i);

        if (pNewFrameResult)
        {
            return pNewFrameResult;
        }
    }

    return nullptr;
}
RwFrame* (*CClumpModelInfo_GetFrameFromId)(RpClump*, int);
RwFrame* CClumpModelInfo_GetFrameFromId_hook(RpClump* a1, int a2)
{
    return CClumpModelInfo_GetFrameFromId_Post(CClumpModelInfo_GetFrameFromId(a1, a2), a1, a2);
}

void (*FxEmitterBP_c__Render)(uintptr_t* a1, int a2, int a3, float a4, char a5);
void FxEmitterBP_c__Render_hook(uintptr_t* a1, int a2, int a3, float a4, char a5)
{
    if(!a1 || !a2) return;
    uintptr_t* temp = *((uintptr_t**)a1 + 3);
    if (!temp)
    {
        return;
    }
    FxEmitterBP_c__Render(a1, a2, a3, a4, a5);
}

uint64_t(*CWorld_ProcessPedsAfterPreRender)();
uint64_t CWorld_ProcessPedsAfterPreRender_hook()
{
    uint64_t res = CWorld_ProcessPedsAfterPreRender();
    if (pNetGame && pNetGame->GetPlayerPool())
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            CPlayerPed* pPed = nullptr;
            if (pNetGame->GetPlayerPool()->GetLocalPlayerID() == i)
            {
                pPed = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
            }
            else
            {
                if (pNetGame->GetPlayerPool()->GetSlotState(i))
                {
                    pPed = pNetGame->GetPlayerPool()->GetAt(i)->GetPlayerPed();
                }
            }
            if (!pPed) continue;
            else
            {
                pPed->ProcessAttach();
            }
        }
    }

    return res;
}

bool (*RwResourcesFreeResEntry)(void* entry);
bool RwResourcesFreeResEntry_hook(void* entry)
{
    bool result;
    if (entry) result = RwResourcesFreeResEntry(entry);
    else result = false;
    return result;
}

static uint32_t dwRLEDecompressSourceSize = 0;

size_t (*OS_FileRead)(OSFile a1, void *buffer, size_t numBytes);
size_t OS_FileRead_hook(OSFile a1, void *buffer, size_t numBytes)
{
    //dwRLEDecompressSourceSize = numBytes;

    return OS_FileRead(a1, buffer, numBytes);
}

void (*RLEDecompress)(uint8_t* pDest, size_t uiDestSize, uint8_t const* pSrc, size_t uiSegSize, uint32_t uiEscape);
void RLEDecompress_hook(uint8_t* pDest, size_t uiDestSize, const uint8_t* pSrc, size_t uiSegSize, uint32_t uiEscape) {

    if (!pDest || !pSrc || uiDestSize == 0 || uiSegSize == 0) {
        // Обработка некорректных входных данных или размеров
        // Здесь можно сгенерировать исключение или вернуть код ошибки
        return;
    }

    const uint8_t* pTempSrc = pSrc;
    const uint8_t* const pEndOfDest = pDest + uiDestSize;
    const uint8_t* const pEndOfSrc = pSrc + dwRLEDecompressSourceSize; // Предполагается, что dwRLEDecompressSourceSize определено правильно

    try {
        while (pDest < pEndOfDest && pTempSrc < pEndOfSrc) {
            if (*pTempSrc == uiEscape) {
                if (pTempSrc + 1 >= pEndOfSrc || pTempSrc[1] == 0 || pTempSrc + 2 + uiSegSize > pEndOfSrc) {
                    // Обработка ошибки, неверное значение ucCurSeg или недостаточно данных в исходном буфере
                    throw std::runtime_error("rled error 1");
                }

                uint8_t ucCurSeg = pTempSrc[1];
                while (ucCurSeg--) {
                    if (pDest + uiSegSize > pEndOfDest) {
                        // Обработка ошибки, недостаточно места в целевом буфере
                        throw std::runtime_error("rled error 2");
                    }
                    memcpy(pDest, pTempSrc + 2, uiSegSize);
                    pDest += uiSegSize;
                }
                pTempSrc += 2 + uiSegSize;
            } else {
                if (pDest + uiSegSize > pEndOfDest || pTempSrc + uiSegSize > pEndOfSrc) {
                    // Обработка ошибки, недостаточно данных в исходном буфере или недостаточно места в целевом буфере
                    throw std::runtime_error("rled error 3");
                }
                memcpy(pDest, pTempSrc, uiSegSize);
                pDest += uiSegSize;
                pTempSrc += uiSegSize;
            }
        }

        dwRLEDecompressSourceSize = 0;
    } catch (const std::exception& e) {
        FLog("%s", e.what());
    }
}

void* g_pSprintButton = nullptr;
void (*CWidgetButtonSprint_Constructor)(void* thiz, char const* name, void* pos);
void (*CWidgetButtonSprint_Destructor)(void* thiz);
void (*CWidgetButton_Draw)(void* thiz);
void (*CWidgetButtonSprint_Update)(void* thiz);
void CWidgetButtonSprint_Constructor_hook(void* thiz, char const* name, void* pos)
{
    g_pSprintButton = thiz;
    CWidgetButtonSprint_Constructor(thiz, name, pos);
}
void CWidgetButtonSprint_Destructor_hook(void* thiz)
{
    if (thiz == g_pSprintButton)
        g_pSprintButton = nullptr;
    CWidgetButtonSprint_Destructor(thiz);
}
void CWidgetButton_Draw_hook(void* thiz)
{
    if (thiz == g_pSprintButton)
    {
        if (pGame)
        {
            CPlayerPed* pPed = pGame->FindPlayerPed();
            if (pPed && pPed->IsInVehicle())
            {
                return;
            }
        }
    }
    CWidgetButton_Draw(thiz);
}
void CWidgetButtonSprint_Update_hook(void* thiz)
{
    if (pGame)
    {
        CPlayerPed* pPed = pGame->FindPlayerPed();
        if (pPed && pPed->IsInVehicle())
        {
            return;
        }
    }
    CWidgetButtonSprint_Update(thiz);
}

void (*CGame_Process)();
void CGame_Process_hook()
{
    if(pGame->bIsGameExiting)return;

    CGame_Process();

    if (pNetGame)
    {
        if(pGame && pGame->FindPlayerPed() && pUI && pUI->buttonpanel() && pUI->buttonpanel()->m_bH)
        {
            if(pGame->FindPlayerPed()->IsInVehicle())
            {
                pUI->buttonpanel()->m_bH->setCaption("D/B");
            }
            else
                pUI->buttonpanel()->m_bH->setCaption("H");
        }

       // CObjectPool* pObjectPool = pNetGame->GetObjectPool();
        //if (pObjectPool) {
        //    pObjectPool->Process();
         //   pObjectPool->ProcessMaterialText();
       // }

        //CTextDrawPool* pTextDrawPool = pNetGame->GetTextDrawPool();
        //if (pTextDrawPool) {
        //    pTextDrawPool->SnapshotProcess();
        //}
    }
}

float (*CDraw__SetFOV)(float thiz, float a2);
float CDraw__SetFOV_hook(float thiz, float a2)
{
    float tmp = (float)((float)((float)(*(float *)&*(float *)(g_libGTASA + 0xC6DC70) - 1.3333) * 11.0) / 0.44444) + thiz;
    if(tmp > 100) tmp = 100.0;
    *(float *)(g_libGTASA + 0x88309C) = tmp;
    return thiz;
}

void(*CStreaming__Init2)();
void CStreaming__Init2_hook()
{
    CStreaming__Init2();
    auto &msAvailable = CStreaming::ms_memoryAvailable;
    msAvailable = 0x20000000;
}

int(*mpg123_param)(void* mh, int key, long val, int ZERO, double fval);
int mpg123_param_hook(void* mh, int key, long val, int ZERO, double fval)
{
    // 0x2000 = MPG123_SKIP_ID3V2
    // 0x200  = MPG123_FUZZY
    // 0x100  = MPG123_SEEKBUFFER
    // 0x40   = MPG123_GAPLESS
    return mpg123_param(mh, key, val | (0x2000 | 0x200 | 0x100 | 0x40), ZERO, fval);
}

void InjectHooks()
{
    FLog("InjectHooks");
    CHook::Write(g_libGTASA + 0xCD0868, &Scene);

#if !VER_x32 // mb all.. wtf crash x64?
    CHook::RET("_ZN11CPlayerInfo14LoadPlayerSkinEv");
    CHook::RET("_ZN11CPopulation10InitialiseEv");
#endif
    CCustomCarEnvMapPipeline::InjectHooks();
    CCamera::InjectHooks(); //
    CReferences::InjectHooks(); //
    CModelInfo::injectHooks(); //
    CTimer::InjectHooks(); //
    //cTransmission::InjectHooks(); //
    CAnimBlendAssociation::InjectHooks(); //
    //cHandlingDataMgr::InjectHooks(); //
    RQShader::InjectHooks();
    CPools::InjectHooks(); //
    CVehicleGTA::InjectHooks(); //
    CMatrixLink::InjectHooks(); //
    CMatrixLinkList::InjectHooks(); //
    CStreaming::InjectHooks();
    CPlaceable::InjectHooks(); //
    CMatrix::InjectHooks(); //
    CCollision::InjectHooks(); //
    //CIdleCam::InjectHooks(); //
    CTouchInterface::InjectHooks(); //
    CWidgetGta::InjectHooks();
    CEntityGTA::InjectHooks(); //
    CPhysical::InjectHooks(); //
   // CAnimManager::InjectHooks(); //
    //CCarEnterExit::InjectHooks();
    CPlayerPedGta::InjectHooks(); //
    CTaskManager::InjectHooks(); //
    //CPedIntelligence::InjectHooks(); //
    CWorld::InjectHooks(); //
    CGame::InjectHooks();
    ES2VertexBuffer::InjectHooks();
    //CRQ_Commands::InjectHooks();
    CTxdStore::InjectHooks();
    CVisibilityPlugins::InjectHooks();
    //CAdjustableHUD::InjectHooks();

    // new
    //CClouds::InjectHooks();
    //CWeather::InjectHooks();
    //RenderBuffer::InjectHooks();
    CTimeCycle::InjectHooks();
    //CCoronas::InjectHooks();
    //CDraw::InjectHooks();
    //CClock::InjectHooks();
    //CBirds::Init();
    CVehicleModelInfo::InjectHooks();
    //CPathFind::InjectHooks();
    CSprite2d::InjectHooks();
    //CFileLoader::InjectHooks();
    //CPickups::InjectHooks();
    //CRenderer::InjectHooks();
   // CStreamingInfo::InjectHooks();
    TextureDatabase::InjectHooks();
    TextureDatabaseEntry::InjectHooks();
    TextureDatabaseRuntime::InjectHooks();
    //CCustomBuildingDNPipeline::InjectHooks();
    //CWidgetRadar::InjectHooks();

    //CRealTimeShadowManager::InjectHooks();

    //COcclusion::InjectHooks();

   // CMobileMenu::InjectHooks();
   // CMobileSettings::InjectHooks();

    //CRealTimeShadowManager::InjectHooks();
    //CHook::Write(g_libGTASA+(VER_x32 ? 0xA41140 : 0xCE3EE8), &COcclusion::aOccluders);
    //CHook::Write(g_libGTASA+(VER_x32 ? 0xA45790:0xCE8538), &COcclusion::NumOccludersOnMap);
}

void InstallUrezHooks()
{
    CHook::UnFuck(g_libGTASA + 0x24E4C4);
    *(char*)(g_libGTASA + 0x24E4C4 + 12) = 'd';
    *(char*)(g_libGTASA + 0x24E4C4 + 13) = 'x';
    *(char*)(g_libGTASA + 0x24E4C4 + 14) = 't';

    CHook::UnFuck(g_libGTASA + 0x246D17);
    *(char*)(g_libGTASA + 0x246D17 + 12) = 'd';
    *(char*)(g_libGTASA + 0x246D17+ 13) = 'x';
    *(char*)(g_libGTASA + 0x246D17 + 14) = 't';
}

void (*CPlaceable_InitMatrixArray)(CMatrixLinkList *thiz, int32 size);
void CPlaceable_InitMatrixArray_hook(CMatrixLinkList *thiz, int32 size)
{
    // CMatrixLinkList::Init
    CHook::CallFunction<void>(g_libGTASA + 0x4AC048, thiz, 10000);
}
int (*_RwTextureDestroy)(int a1);
int _RwTextureDestroy_hook(int a1)
{
    int result; // r0

    if ( (unsigned int)(a1 + 1) >= 2 )
        result = _RwTextureDestroy(a1);
    else
        result = 0;
    return result;
}



void (*EmuShader_Select)(
        void* thiz,
        bool isNewSelection
);
void EmuShader_Select_hook(
        void* thiz,
        bool isNewSelection
){
    if (!thiz)
        return;

    EmuShader_Select(thiz, isNewSelection);
}

void (*TidyUpModelInfo2_orig)(CEntityGTA* pEntity, bool bForce);

void TidyUpModelInfo2_hook(CEntityGTA* pEntity, bool bForce)
{
    if (!pEntity || (uintptr_t)pEntity < 0x1000)
    {
        return; // If the entity is invalid, do nothing.
    }
    TidyUpModelInfo2_orig(pEntity, bForce);
}

int (*CTextureDatabaseRuntime__GetEntry)(TextureDatabaseRuntime *a1,const char *name, bool *hasSibling);
int CTextureDatabaseRuntime__GetEntry_hook(TextureDatabaseRuntime *a1, const char *name, bool *hasSibling)
{
    int result; // r0

    if (a1)
        result = CTextureDatabaseRuntime__GetEntry(a1, name, hasSibling);
    else
        result = -1;
    return result;
}
void InstallCrashFixHooks()
{
    // some crashfixes
    CHook::InstallPLT(g_libGTASA + 0x845C20, &CCustomRoadsignMgr_RenderRoadsignAtomic_hook, &CCustomRoadsignMgr_RenderRoadsignAtomic);
    CHook::InstallPLT(g_libGTASA + 0x83F7F0, &_RwTextureDestroy_hook, &_RwTextureDestroy);
    //CHook::InlineHook("ZN4CPed14UpdatePositionEv", (uintptr_t)CPed_UpdatePosition_hook, (uintptr_t*)&CPed_UpdatePosition);
    CHook::InstallPLT(g_libGTASA + 0x845D08, &RwFrameAddChild_hook, &RwFrameAddChild);
    //GlossPltHook(g_libGTASA,"_Z20RpClumpForAllAtomicsP7RpClumpPFP8RpAtomicS2_PvES3__ptr", &CTextureDatabaseRuntime__GetEntry_hook, &CTextureDatabaseRuntime__GetEntry,NULL);


    //CHook::InlineHook(g_libGTASA + 0x38BCF8, (uintptr_t)CustomPipeRenderCB_hook, (uintptr_t*)&CustomPipeRenderCB);

    CHook::InstallPLT(g_libGTASA + 0x848CD0, &rpMaterialListDeinitialize_hook, &rpMaterialListDeinitialize);
    //CHook::InstallPLT(g_libGTASA + (VER_x32 ? 0x6778A8:0x84D188), (uintptr_t)rxOpenGLDefaultAllInOneRenderCB_hook, (uintptr_t*)&rxOpenGLDefaultAllInOneRenderCB);
    //CHook::InstallPLT(g_libGTASA + (VER_x32 ? 0x677CAC:0x84D988), (uintptr_t)CustomPipeRenderCB_hook, (uintptr_t*)&CustomPipeRenderCB);
    CHook::InlineHook("_ZN9EmuShader6SelectEb", &EmuShader_Select_hook, &EmuShader_Select);

    CHook::InlineHook("_ZN12CAnimManager19UncompressAnimationEP19CAnimBlendHierarchy", &CAnimManager_UncompressAnimation_hook,&CAnimManager_UncompressAnimation);

    //more crashfix
    CHook::InlineHook(g_libGTASA + 0x496798, &TidyUpModelInfo2_hook, &TidyUpModelInfo2_orig);


}
void InstallSpecialHooks()
{
    InjectHooks();

    InstallUrezHooks();

    CHook::Redirect("_ZN5CGame20InitialiseRenderWareEv", &CGame::InitialiseRenderWare);
    CHook::InstallPLT(g_libGTASA + 0x83DE90, &StartGameScreen__OnNewGameCheck_hook, &StartGameScreen__OnNewGameCheck);
    CHook::InlineHook("_Z10NvUtilInitv", &NvUtilInit_hook, &NvUtilInit);

    CHook::RET("_ZN12CCutsceneMgr16LoadCutsceneDataEPKc"); // LoadCutsceneData
    CHook::RET("_ZN12CCutsceneMgr10InitialiseEv");			// CCutsceneMgr::Initialise

    CHook::Redirect(g_libGTASA+0x7D624C, &NvFOpen);

    CHook::InlineHook("_ZN14MainMenuScreen6UpdateEf", &MainMenuScreen__Update_hook, &MainMenuScreen__Update);

    CHook::RET("_ZN4CPed31RemoveWeaponWhenEnteringVehicleEi"); // CPed::RemoveWeaponWhenEnteringVehicle

    //CHook::InstallPLT(g_libGTASA + (VER_x32 ? 0x6701D4 : 0x840708), &RLEDecompress_hook, &RLEDecompress);

   // CHook::InlineHook("_Z11OS_FileReadPvS_i", &OS_FileRead_hook, &OS_FileRead);

	//CHook::InlineHook("_Z32_rxOpenGLDefaultAllInOneRenderCBP10RwResEntryPvhj", &rxOpenGLDefaultAllInOneRenderCB_hook, &rxOpenGLDefaultAllInOneRenderCB);
	//CHook::InlineHook("_ZN25CCustomBuildingDNPipeline18CustomPipeRenderCBEP10RwResEntryPvhj", &CCustomBuildingDNPipeline__CustomPipeRenderCB_hook, &CCustomBuildingDNPipeline__CustomPipeRenderCB);

    CHook::InlineHook("_Z27AtomicDefaultRenderCallBackP8RpAtomic", &AtomicDefaultRenderCallBack_hook, &AtomicDefaultRenderCallBack_orig);
    CHook::InlineHook("_ZN18CVisibilityPlugins12RenderEntityEPvf", &CVisibilityPlugins_RenderEntity_hook, &CVisibilityPlugins_RenderEntity_orig);
    CHook::InlineHook("_ZN15CMatrixLinkList4InitEi", &CPlaceable_InitMatrixArray_hook, & CPlaceable_InitMatrixArray);

}
RwFrame * (*CClumpModelInfo__GetFrameFromId)(RpClump * a1, int32 a2);
RwFrame * CClumpModelInfo__GetFrameFromId_hook(RpClump * a1, int32 a2)
{

    uintptr_t dwRetAddr = 0;
    GET_LR(dwRetAddr);
    dwRetAddr -= g_libGTASA;

    if(!CClumpModelInfo__GetFrameFromId(a1, a2))
    {
        if(dwRetAddr > 0x6E4248)
        {
            if(dwRetAddr == 0x6E427C || dwRetAddr ==0x6E42B0)
                return CClumpModelInfo__GetFrameFromId(a1, a2);
        }
        else if(dwRetAddr == 0x53513C|| dwRetAddr == 0x5342FC)
            return CClumpModelInfo__GetFrameFromId(a1, a2);

        RwFrame * address = CClumpModelInfo__GetFrameFromId(a1, 2);
        if(!address)
        {
            for(int i = 3; i < 39; ++i)
            {

                RwFrame *  address = CClumpModelInfo__GetFrameFromId(a1, i);
                if(address) return address;
            }
        }

        return 0;
    }

    return CClumpModelInfo__GetFrameFromId(a1, a2);
}

uintptr (*CAudioEngine__Service)(uintptr_t a1);
uintptr CAudioEngine__Service_hook(uintptr_t a1)
{

    if(!a1) return 0;
    return CAudioEngine__Service(a1);
}
uintptr (*CAEVehicleAudioEntity__GetAccelAndBrake)(uintptr * a1, uintptr  *  a2);
uintptr CAEVehicleAudioEntity__GetAccelAndBrake_hook(uintptr  *  a1, uintptr *  a2)
{
    if(!a1 || !a2) return 0;
    return CAEVehicleAudioEntity__GetAccelAndBrake(a1, a2);
}

int (*GetMeshPriority)(RpMesh const* rpMesh);
int GetMeshPriority_hook(RpMesh const* rpMesh)
{
    if(rpMesh)
    {
        RpMaterial* rpMeshMat = rpMesh->material;
        if(rpMeshMat)
        {
            if(rpMeshMat->texture)
            {
                if(!rpMeshMat->texture->raster)
                    return 0;
            }
        }
    }

    return GetMeshPriority(rpMesh);
}
int (*CUpsideDownCarCheck__IsCarUpsideDown)(int thiz, uintptr_t a2);
int CUpsideDownCarCheck__IsCarUpsideDown_hook(int thiz, uintptr_t a2)
{
    FLog("[MISC] CUpsideDownCarCheck__IsCarUpsideDown_hook");
    if(*(uintptr_t*)(a2 + 18))
        return CUpsideDownCarCheck__IsCarUpsideDown(thiz, a2);

    return 0;
}
void InstallMiscHooks()
{

    CHook::InlineHook("_ZN15CClumpModelInfo14GetFrameFromIdEP7RpClumpi", &CClumpModelInfo__GetFrameFromId_hook, &CClumpModelInfo__GetFrameFromId);

    CHook::Redirect("_Z10GetTexturePKc", &CUtil::GetTexture);

    CHook::InlineHook("_ZN22TextureDatabaseRuntime8GetEntryEPKcRb", &CTextureDatabaseRuntime__GetEntry_hook, &CTextureDatabaseRuntime__GetEntry);
    CHook::InlineHook("_ZN19CUpsideDownCarCheck15IsCarUpsideDownEPK8CVehicle", &CUpsideDownCarCheck__IsCarUpsideDown_hook, &CUpsideDownCarCheck__IsCarUpsideDown);

    CHook::InlineHook("_ZN12CAudioEngine7ServiceEv", &CAudioEngine__Service_hook, &CAudioEngine__Service);

    CHook::InlineHook("_ZN21CAEVehicleAudioEntity16GetAccelAndBrakeER14cVehicleParams", &CAEVehicleAudioEntity__GetAccelAndBrake_hook, &CAEVehicleAudioEntity__GetAccelAndBrake);

    CHook::InlineHook("_ZN13FxEmitterBP_c6RenderEP8RwCamerajfh", &FxEmitterBP_c__Render_hook,
                      &FxEmitterBP_c__Render);

    CHook::InlineHook("_Z15GetMeshPriorityPK6RpMesh", &GetMeshPriority_hook, &GetMeshPriority);

}
#include <EGL/egl.h>
#include <GLES2/gl2.h>   // If using OpenGL ES 2.0 or 3.0

void InstallHooks()
{
    CHook::Redirect("_Z13Render2dStuffv", &Render2dStuff);
    CHook::Redirect("_Z13RenderEffectsv", &RenderEffects);
    CHook::InlineHook("_Z14AND_TouchEventiiii", &AND_TouchEvent_hook, &AND_TouchEvent);

    CHook::Redirect("_ZN11CHudColours12GetIntColourEh", &CHudColours__GetIntColour); // dangerous
    CHook::Redirect("_ZN6CRadar19GetRadarTraceColourEjhh", &CRadar__GetRadarTraceColor); // dangerous
    CHook::InlineHook("_ZN6CRadar12SetCoordBlipE9eBlipType7CVectorj12eBlipDisplayPc",
                      &CRadar__SetCoordBlip_hook, &CRadar__SetCoordBlip);
    CHook::InlineHook("_ZN6CRadar20DrawRadarGangOverlayEb", &CRadar_DrawRadarGangOverlay_hook, &CRadar_DrawRadarGangOverlay);



    //?they removef the exit button
    //CHook::InlineHook("_ZN14MainMenuScreen6OnExitEv", &MainMenuScreen__OnExit_hook, &MainMenuScreen__OnExit);

    CHook::InlineHook("_ZN17CTaskSimpleUseGun17RemoveStanceAnimsEP4CPedf", &CTaskSimpleUseGun__RemoveStanceAnims_hook, &CTaskSimpleUseGun__RemoveStanceAnims);

    // Bullet sync
    //CHook::InlineHook("_ZN7CWeapon14FireInstantHitEP7CEntityP7CVectorS3_S1_S3_S3_bb", &CWeapon__FireInstantHit_hook, &CWeapon__FireInstantHit);
    CHook::InlineHook("_ZN7CWeapon14FireInstantHitEP7CEntityP7CVectorS3_S1_S3_S3_bb", &CWeapon__FireInstantHit_hook, &CWeapon__FireInstantHit);
    CHook::InlineHook("_ZN7CWeapon10FireSniperEP4CPedP7CEntityP7CVector", &CWeapon__FireSniper_hook, &CWeapon__FireSniper);
    CHook::InlineHook("_ZN6CWorld18ProcessLineOfSightERK7CVectorS2_R9CColPointRP7CEntitybbbbbbbb", &CWorld__ProcessLineOfSight_hook, &CWorld__ProcessLineOfSight);
    CHook::InlineHook("_ZN28CPedDamageResponseCalculator21ComputeDamageResponseEP4CPedR18CPedDamageResponseb", &CPedDamageResponseCalculator__ComputeDamageResponse_hook, &CPedDamageResponseCalculator__ComputeDamageResponse);
    CHook::InlineHook("_ZN7CWeapon18ProcessLineOfSightERK7CVectorS2_R9CColPointRP7CEntity11eWeaponTypeS6_bbbbbbb", &CWeapon__ProcessLineOfSight_hook, &CWeapon__ProcessLineOfSight);
    CHook::InlineHook("_ZN11CBulletInfo9AddBulletEP7CEntity11eWeaponType7CVectorS3_", &CBulletInfo_AddBullet_hook, &CBulletInfo_AddBullet);

    CHook::InlineHook("_ZN11CFileLoader18LoadObjectInstanceEP19CFileObjectInstancePKc", &CFileLoader__LoadObjectInstance_hook, &CFileLoader__LoadObjectInstance);

    CHook::InlineHook("_ZN6CRadar9ClearBlipEi", &CRadar_ClearBlip_hook, &CRadar_ClearBlip);

    CHook::InlineHook("_ZN10CCollision19ProcessVerticalLineERK8CColLineRK7CMatrixR9CColModelR9CColPointRfbbP15CStoredCollPoly", &CCollision__ProcessVerticalLine_hook, &CCollision__ProcessVerticalLine);

    CHook::InlineHook("_ZN16CTaskSimpleGetUp10ProcessPedEP4CPed", &CTaskSimpleGetUp__ProcessPed_hook, &CTaskSimpleGetUp__ProcessPed); // CTaskSimpleGetUp::ProcessPed
    CHook::InlineHook("_ZN7CObject6RenderEv", &CObject_Render_hook, & CObject_Render);

    CHook::InlineHook("_ZN4CCam7ProcessEv", CCam__Process_hook, &CCam__Process);

    CHook::Redirect("_Z19PlayerIsEnteringCarv", &PlayerIsEnteringCar);
    if(*(uint8_t *)(g_libGTASA + 0xD13C60))
    {
        CHook::Redirect("_ZNK14TextureListing11GetMipCountEv", &getmip);
    }
    /*
    if (!eglGetProcAddress("glAlphaFuncQCOM")) {
        // If "glAlphaFuncQCOM" is not available, try "glAlphaFunc"

        if (eglGetProcAddress("glAlphaFunc")) {
            // If "glAlphaFunc" is found, store the address in the global library
            *((void**)(g_libGTASA + 0xD13CD8)) = (void*)eglGetProcAddress("glAlphaFunc");
        } else {
            // If neither function is available, hook the fallback symbol
            CHook::Redirect("_Z25RQ_Command_rqSetAlphaTestRPc", &RQCommand_rqSetAlphaTest);
        }
    }
     */

    CHook::Redirect("_ZN4CHID12GetInputTypeEv", &GetInputType);

    //CHook::InlineHook("_ZN9CRenderer24RenderEverythingBarRoadsEv", &CRenderer_RenderEverythingBarRoads_hook, &CRenderer_RenderEverythingBarRoads);

#if VER_x32
    CHook::InlineHook("_ZN14CAnimBlendNode12FindKeyFrameEf", &CAnimBlendNode__FindKeyFrame_hook, &CAnimBlendNode__FindKeyFrame);
    CHook::InlineHook("_ZN15CClumpModelInfo14GetFrameFromIdEP7RpClumpi", &CClumpModelInfo_GetFrameFromId_hook, &CClumpModelInfo_GetFrameFromId);
#endif


    CHook::InlineHook("_Z23RwResourcesFreeResEntryP10RwResEntry", &RwResourcesFreeResEntry_hook,
                      &RwResourcesFreeResEntry);

    ms_fAspectRatio = (float *) (g_libGTASA + 0xC6DC70);
    CHook::InlineHook("_ZN4CHud14DrawCrossHairsEv", &DrawCrosshair_hook, &DrawCrosshair);

    // retexture
    //CHook::InlineHook("_ZN7CEntity6RenderEv", &CEntity_Render_hook, &CEntity_Render);
    CHook::InstallPLT(g_libGTASA + 0x841A88, &CEntity_Render_hook, &CEntity_Render);

    //CHook::InlineHook("_ZN26CAEGlobalWeaponAudioEntity21ServiceAmbientGunFireEv", &TaskEnterVehicleHook, &TaskEnterVehicle);
    //idk what uses of this but skip for now
   // CHook::Write(g_libGTASA + 0x5DF790, 0x90000AA9);
    //CHook::Write(g_libGTASA + 0x5DF794, 0xBD48D521);


    CHook::InlineHook("_ZN5CDraw6SetFOVEfb", &CDraw__SetFOV_hook, &CDraw__SetFOV);
    CHook::InlineHook("_ZN10CStreaming5Init2Ev", &CStreaming__Init2_hook, &CStreaming__Init2);

    //CHook::InlineHook("mpg123_param", &mpg123_param_hook, &mpg123_param);

    CHook::InlineHook("_ZN4CPad27CycleCameraModeDownJustDownEv",
                      CPad__CycleCameraModeDownJustDown_hook, &CPad__CycleCameraModeDownJustDown);

    //CHook::InlineHook("_ZN4CCam7ProcessEv", CCam__Process_hook, &CCam__Process);


    CHook::NOP(g_libGTASA + 0x716AD0, 1); // Game - SocialClub

    //CHook::InlineHook("_ZN6CRadar9ClearBlipEi", CRadar__ClearBlip_hook, &CRadar__ClearBlip);
    CHook::InlineHook("_ZN6CRadar13DrawRadarMaskEv", &DrawRadarMask_hook, &DrawRadarMask);
    CHook::InlineHook("_ZN6CRadar15LimitRadarPointER9CVector2D", &CRadar__LimitRadarPoint_hook, &CRadar__LimitRadarPoint);
    CHook::InlineHook("_ZN6CRadar15DrawRadarSpriteEtffh", (uintptr_t) CRadar__DrawRadarSprite_hook,
                      (uintptr_t *) &CRadar__DrawRadarSprite);


    //CHook::InlineHook("_Z16_rxPacketDestroyP8RxPacket", rxPacketDestroy_hook, rxPacketDestroy);

    //CHook::InlineHook("_ZN18CVisibilityPlugins19InitAlphaEntityListEv", CVisibilityPlugins__InitAlphaEntityList_hook, CVisibilityPlugins__InitAlphaEntityList);

    //CHook::InlineHook("_ZN9CRenderer9ScanWorldEv", CRenderer__ScanWorld_hook, CRenderer__ScanWorld);

    //CHook::InlineHook("_ZN16cHandlingDataMgr13FindExactWordEPcS0_ii", &cHandlingDataMgr__FindExactWord_hook, &cHandlingDataMgr__FindExactWord);
    //CHook::Redirect("_ZN16cHandlingDataMgr22ConvertDataToGameUnitsEP13tHandlingData", &cHandlingDataMgr__ConvertDataToGameUnits);
    CHook::InlineHook("_ZN21CAEWeatherAudioEntity16UpdateParametersEP8CAESounds", &CAEWeatherAudioEntity__UpdateParameter_hook, &CAEWeatherAudioEntity__UpdateParameter);
    HookCPad();
    InstallCrashFixHooks();
    InstallMiscHooks();
}