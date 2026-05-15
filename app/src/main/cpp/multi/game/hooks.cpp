#include <GLES2/gl2.h>
#include "../main.h"
#include "../vendor/armhook/patch.h"
#include "game.h"
#include "../net/netgame.h"
#include "../gui/gui.h"
#include "Pool.h"
#include "Building/BuildingRemoval.h"
#include <queue>
#include <mutex>
//#include "../vendor/curl/easy.h"
extern UI* pUI;
extern CGame* pGame;
extern CNetGame *pNetGame;
extern MaterialTextGenerator* pMaterialTextGenerator;
inline RwMatrix* m_pWorkingMatrix1;
inline RwMatrix* m_pWorkingMatrix2;

extern "C" uintptr_t get_lib()
{
    return g_libGTASA;
}
void (*NvUtilInit)();

int(*OS_FileOpen)(uint32_t r0, uintptr_t handle, char* name, int r3);

int OS_FileOpen_hook(int r0, uintptr_t handle, char* name, int r3)
{
    if(!name)
        return 0;

    char fixedName[0xFF] = {0};
    char path[0xFF] = {0};

    // Correct Android path
    const char* pszStorage = "/storage/emulated/0/Documents/SampMobile/";

    FLog("OS_FileOpen original: %s", name);

    // copy original name
    strncpy(fixedName, name, sizeof(fixedName) - 1);

    // remove leading slash
    if(fixedName[0] == '/')
        memmove(fixedName, fixedName + 1, strlen(fixedName));

    // convert '\' -> '/'
    for(int i = 0; fixedName[i]; i++)
    {
        if(fixedName[i] == '\\')
            fixedName[i] = '/';
    }

    // build final redirected path
    snprintf(path, sizeof(path), "%s%s", pszStorage, fixedName);

    FLog("OS_FileOpen redirected: %s", path);

    // call original function
    return OS_FileOpen(r0, handle, path, r3);
}

void NvUtilInit_hook() {
    //FLog("NvUtilInit");
    g_pszStorage = "/storage/emulated/0/Android/media/com.rockstargames.gtasa/";
   // LOGI("Storage located at %s", g_pszStorage);
    NvUtilInit();
    //ReadSettingFile();


}
bool g_bPlaySAMP = false;

void MainMenu_OnStartSAMP()
{
    if(g_bPlaySAMP) return;

    //InitInMenu();
    pGame->StartGame();

    // StartGameScreen::OnNewGameCheck()
    (( void (*)())(g_libGTASA + 0x707B64))();

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
    // Ð¾Ñ‚ÐºÐ»ÑŽÑ‡Ð¸Ñ‚ÑŒ ÐºÐ½Ð¾Ð¿ÐºÑƒ Ð½Ð°Ñ‡Ð°Ñ‚ÑŒ Ð¸Ð³Ñ€Ñƒ
    if(g_bPlaySAMP)
        return;

    StartGameScreen__OnNewGameCheck();
}

struct stFile
{
    int isFileExist;
    FILE *f;
};

char lastFile[123];


stFile* NvFOpen(const char *r1)
{

    strcpy(lastFile, r1);
    g_pszStorage = "/storage/emulated/0/Android/media/com.rockstargames.gtasa/";
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


    // ----------------------------
    if(!strncmp(r1+12, "mainV1.scm", 10))
    {
        sprintf(path, "%sSAMP/main.scm", g_pszStorage);
        FLog("Loading %s", path);
    }
    // ----------------------------
    if(!strncmp(r1+12, "SCRIPTV1.IMG", 12))
    {
        sprintf(path, "%SAMP/script.img", g_pszStorage);
        FLog("Loading script.img..");
    }
    // ----------------------------
    if(!strncmp(r1, "DATA/PEDS.IDE", 13))
    {
        sprintf(path, "%sSAMP/peds.ide", g_pszStorage);
        FLog("Loading peds.ide..");
    }
    // ----------------------------
    if(!strncmp(r1, "DATA/VEHICLES.IDE", 17))
    {
        sprintf(path, "%sSAMP/vehicles.ide", g_pszStorage);
        FLog("Loading vehicles.ide..");
    }

    if (!strncmp(r1, "DATA/GTA.DAT", 12))
    {
        sprintf(path, "%sSAMP/gta.dat", g_pszStorage);
        FLog("Loading gta.dat..");
    }

    if (!strncmp(r1, "DATA/HANDLING.CFG", 17))
    {
        sprintf(path, "%sSAMP/handling.cfg", g_pszStorage);
        FLog("Loading handling.cfg..");
    }

    if (!strncmp(r1, "DATA/WEAPON.DAT", 15))
    {
        sprintf(path, "%sSAMP/weapon.dat", g_pszStorage);
        FLog("Loading weapon.dat..");
    }

    if (!strncmp(r1, "DATA/FONTS.DAT", 15))
    {
        sprintf(path, "%sdata/fonts.dat", g_pszStorage);
        FLog("Loading weapon.dat..");
    }

    if (!strncmp(r1, "DATA/PEDSTATS.DAT", 15))
    {
        sprintf(path, "%sdata/pedstats.dat", g_pszStorage);
        FLog("Loading weapon.dat..");
    }

    if (!strncmp(r1, "DATA/TIMECYC.DAT", 15))
    {
        sprintf(path, "%sdata/timecyc.dat", g_pszStorage);
        FLog("Loading weapon.dat..");
    }

    if (!strncmp(r1, "DATA/POPCYCLE.DAT", 15))
    {
        sprintf(path, "%sdata/popcycle.dat", g_pszStorage);
        FLog("Loading weapon.dat..");
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
#include "java/jniutil.h"
#include "StoredMaterials.h"
#include "game/CCAM/Cameras.h"
#include "sprite2d.h"
#include "Scene.h"

extern CJavaWrapper* pJavaWrapper;
void (*MainMenuScreen__OnExit)();
void MainMenuScreen__OnExit_hook()
{
    pGame->bIsGameExiting = true;

    pNetGame->GetRakClient()->Disconnect(0);

    pJavaWrapper->exitGame();
}
void (*CTimer_StartUserPause)();
void CTimer_StartUserPause_hook()
{
    *(uint8_t*)(g_libGTASA + 0xA00CB8) = 1;
    if (pUI) pUI->setVisible(false);

    if(pJavaWrapper)
    {
        //pJavaWrapper->SetPauseState(true);
        pJavaWrapper->hideWithoutReset();
    }
}

void (*CTimer_EndUserPause)();
void CTimer_EndUserPause_hook()
{
    *(uint8_t*)(g_libGTASA + 0xA00CB8) = 0;
    if (pUI) pUI->setVisible(true);
    if(pJavaWrapper)
    {
        pJavaWrapper->showWithoutReset();
    }
}
void(*DisplayScreen)();
void DisplayScreen_hook()
{
    /*RwCamera *camera = *(RwCamera**)(g_libGTASA + 0x9FC93C);

    if (RwCameraBeginUpdate(camera))
    {
        // DefinedState2d
        ((void(*)())(g_libGTASA + /0x5D0C64 + 1))();
        // CSprite2d::InitPerFrame()
        ((void(*)())(g_libGTASA + 0x5C89A8 + 1))();
        RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSCLAMP);
        // emu_GammaSet()
        ((void(*)(bool))(g_libGTASA + 0x1C07D0 + 1))(false);

        // CLoadingScreen::m_PercentLoaded
        float fPercentLoaded = *(float*)(g_libGTASA + 0x9920C0);

        pUI->splashscreen()->setProgressBarValue(fPercentLoaded / 100.0f);
        pUI->render();

        RwCameraEndUpdate(camera);
        RwCameraShowRaster(camera, 0, 0);
    }*/
    pJavaWrapper->ShowLoadingScreen();
}
void MainLoop();
void Render2dStuff()
{
    LOGI("RENDER 2D STUFF");
    if( CHook::CallFunction<bool>(g_libGTASA +0X72A580) ) // emu_IsAltRenderTarget()
        CHook::CallFunction<void>(g_libGTASA + 0x72AF08); // emu_FlushAltRenderTarget()

    CHook::CallFunction<void>("_Z25RwTexDictionarySetCurrentP15RwTexDictionary", (uintptr_t)NULL);

    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);

    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, RWRSTATE(FALSE));
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, RWRSTATE(FALSE));
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, RWRSTATE(TRUE));
    RwRenderStateSet(rwRENDERSTATESRCBLEND, RWRSTATE(rwBLENDSRCALPHA));
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, RWRSTATE(rwBLENDINVSRCALPHA));
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, RWRSTATE(rwRENDERSTATENARENDERSTATE));
    RwRenderStateSet(rwRENDERSTATECULLMODE, RWRSTATE(rwCULLMODECULLNONE));
    ( ( void(*)() )(g_libGTASA + 0x4F59E0) )(); // Ð¿Ñ€Ð¸Ñ†ÐµÐ»CHud::DrawCrossHairs(void)	000000000051C694
    CHook::CallFunction<void>("_ZN4CHud4DrawEv");

    ((void(*)(bool))(g_libGTASA + 0x4E5E6C))(false);
    CHook::CallFunction<void>("_Z12emu_GammaSeth", 1);
    ((void (*)(bool))(g_libGTASA + 0x68B848))(1u); // CMessages::Display
    ((void (*)(bool))(g_libGTASA + 0x5B86EC))(1u); // CFont::RenderFontBuffer
    CHook::CallFunction<void>("_Z12emu_GammaSeth", 0);


    if (pUI) pUI->render();
    MainLoop();
}

void RenderEffects() {
//	RenderEffects();
    CHook::CallFunction<void>(g_libGTASA + 0x5AC7D8);
    CHook::CallFunction<void>(g_libGTASA + 0x5CEDB0);
//    CRopes::Render();
//    CGlass::Render();
    CHook::CallFunction<void>(g_libGTASA + 0x5B5C48);
    //CVisibilityPlugins::RenderReallyDrawLastObjects();
    CHook::CallFunction<void>(g_libGTASA + 0x5E8C04);

    //CCoronas::Render();
    CHook::CallFunction<void>(g_libGTASA + 0x5B0B88);

    // FIXME
    CCameras& TheCamera = *reinterpret_cast<CCameras*>(g_libGTASA + 0x9DF030);
    auto g_fx = *(uintptr_t *) (g_libGTASA + 0xA42560);
    CHook::CallFunction<void>(g_libGTASA + 0x4B7BD0, &g_fx, TheCamera.m_pRwCamera, false);

    CHook::CallFunction<void>(g_libGTASA + 0x5DC790);
    CHook::CallFunction<void>(g_libGTASA + 0x6E3560);
    CHook::CallFunction<void>(g_libGTASA + 0x5B033C);
    //   CClouds::VolumetricCloudsRender();
////    if (CHeli::NumberOfSearchLights || CTheScripts::NumberOfScriptSearchLights) {
////        CHeli::Pre_SearchLightCone();
////        CHeli::RenderAllHeliSearchLights();
////        CTheScripts::RenderAllSearchLights();
////        CHeli::Post_SearchLightCone();
////    }
    CHook::CallFunction<void>(g_libGTASA + 0x6F1324);
////    if (CReplay::Mode != MODE_PLAYBACK && !CPad::GetPad(0)->DisablePlayerControls) {
////        FindPlayerPed()->DrawTriangleForMouseRecruitPed();
////    }
    CHook::CallFunction<void>(g_libGTASA + 0x5D13F0);
//    //CVehicleRecording::Render();
    CHook::CallFunction<void>(g_libGTASA + 0x5C10A0);
//    //CRenderer::RenderFirstPersonVehicle();
    CHook::CallFunction<void>(g_libGTASA + 0x5C56A8);

    //DebugModules::Render3D();
}

void(*CRadar_DrawRadarGangOverlay)(uint32_t unk);
void CRadar_DrawRadarGangOverlay_hook(uint32_t unk)
{
    if (pNetGame)
    {
        CGangZonePool *pGangZonePool = pNetGame->GetGangZonePool();
        if (pGangZonePool) {
            pGangZonePool->Draw(unk);
        }
    }
}
inline float FindGroundZForCoord(float x, float y) {
    return CHook::CallFunction<float>(g_libGTASA + 0x4B336C, x, y);
}
int (*CRadar__SetCoordBlip)(int r0, float X, float Y, float Z, int r4, int r5, char *name);
int CRadar__SetCoordBlip_hook(int r0, float X, float Y, float Z, int r4, int r5, char *name)
{
    if (pNetGame && !strncmp(name, "CODEWAY", 7))
    {
        float findZ = FindGroundZForCoord(X, Y) + 1.5f;
        VECTOR pos = { X, Y, findZ - 1.5f };

        /*
         * //later
        GPS::Set(pos, true);
         */
        RakNet::BitStream bsSend;
        bsSend.Write(X);
        bsSend.Write(Y);
        bsSend.Write(findZ);
        pNetGame->GetRakClient()->RPC(&RPC_MapMarker, &bsSend, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
    }

    return CRadar__SetCoordBlip(r0, X, Y, Z, r4, r5, name);
}
typedef struct {
    VECTOR     vecPosObject;
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
VALIDATE_SIZE(stLoadObjectInstance,  0x28);

ENTITY_TYPE* (*CFileLoader__LoadObjectInstance)(CFileObjectInstance *pObject, const char *pName);
ENTITY_TYPE* CFileLoader__LoadObjectInstance_hook(CFileObjectInstance *pObject, const char *pName)
{
    // Check if this building should be removed
    for (int i = 0; i < CBuildingRemoval::m_TotalRemovedObjects; i++)
    {
        const auto& buildingInfo = CBuildingRemoval::m_RemoveBuildings[i];
        // Check model ID match (or -1 for all models)
        if (pObject->m_nModelId == buildingInfo.modelId || buildingInfo.modelId == static_cast<uint32_t>(-1))
        {
            VECTOR pos;

            pos.X = pObject->m_vecPosition.X;
            pos.Y = pObject->m_vecPosition.Y;
            pos.Z = pObject->m_vecPosition.Z;

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
constexpr uint32 MAX_COLLECTED_PICKUPS = 20;
constexpr uint32 MAX_PICKUP_MESSAGES = 16;
constexpr uint32 MAX_NUM_PICKUPS = 620;
void* (*CPickup_Update)();
void* CPickup_Update_hook()
{
    static std::array<CPickup, MAX_NUM_PICKUPS>& aPickUps = *(std::array<CPickup, MAX_NUM_PICKUPS>*)(g_libGTASA + 0x96A470);

    auto start = 620 * (CTimer::GetFrameCounter() % 32) / 32;
    auto end   = 620 * (CTimer::GetFrameCounter() % 32 + 1) / 32;
    for (auto i = start; i < end; i++) {
        auto& pickup = aPickUps[i];
        if (pickup.m_nPickupType == PICKUP_NONE)
            continue;

        if (pickup.m_nFlags.bVisible = pickup.IsVisible()) {
            if (!pickup.m_nFlags.bDisabled && !pickup.m_pObject) {
                pickup.GiveUsAPickUpObject(&pickup.m_pObject, -1);


                if (auto& obj = pickup.m_pObject; obj) {
                    ((void(*)(CEntityGTA*))(g_libGTASA + 0x4AAD34))(obj);
                }
            }
            CPickupPool *pPickups = pNetGame->GetPickupPool();
            if(pPickups->GetIDFromGTAId(i) == -1)
            {
               // CHook::CallFunction<void>(g_libGTASA + 0x3F06A0);
            }else{
                if (pickup.Update()) {
                    if (pickup.m_nPickupType == PICKUP_ONCE)
                    {
                        ((void (*)(CEntityGTA*))(g_libGTASA + 0x4AAB00)) (pickup.m_pObject);
                    }
                    pPickups->PickedUp( i );

                }

            }


        } else {
            pickup.GetRidOfObjects();
        }
    }

}

CStoredMaterials gStoredEntityMaterials;

RwObject* RwFrameForAllObjectsCallback(RwObject* object, void* data) {
    if (object->type != rpATOMIC)
        return object;

    const auto* atomic = reinterpret_cast<RpAtomic*>(object);
    if (!atomic->geometry)
        return object;

    const auto* geometry = atomic->geometry;
    const auto* pObject = reinterpret_cast<CObject*>(data);
    if (!pObject)
        return object;

    const int numMaterials = std::min(geometry->matList.numMaterials, 16);
    const auto materials = std::span(geometry->matList.materials, numMaterials);
    const auto objMaterials = std::span(pObject->m_MaterialTexture, numMaterials);

    std::transform(
            materials.begin(), materials.end(),
            objMaterials.begin(),
            materials.begin(),
            [](RpMaterial* material, const MaterialInfo& objMaterial) {
                if (objMaterial.m_bCreated && objMaterial.pTex &&
                    material && material->texture)
                {
                    gStoredEntityMaterials.Add(&material->texture);
                    material->texture = objMaterial.pTex;
                }
                return material;
            }
    );

    return object;
}
void (*CObject_Render)(CObjectGTA* thiz);
void CObject_Render_hook(CObjectGTA* thiz)
{
    CObjectGTA *object = thiz;

    if (!thiz)
        return;

    const RwObject* atomic = reinterpret_cast<const RwObject *>(thiz->m_pRwObject);
    if (!atomic || !atomic->parent)
        return;

    const RpClump* clump = reinterpret_cast<const RpClump *>(thiz->m_pRwObject);
    if (!clump)
        return;

    if(pNetGame && object != 0)
    {
        CObject *pObject = pNetGame->GetObjectPool()->FindObjectFromGtaPtr(
                reinterpret_cast<ENTITY_TYPE *>(object));
        if(pObject && pObject->m_pEntity)
        {
            RwObject* rwObject = (RwObject*)pObject->m_pEntity->m_pRwObject;
            if(rwObject)
            {
                // SetObjectMaterial
                if(pObject->m_bHasMaterial || pObject->m_bHasMaterialText)
                {
                    RwFrameForAllObjects((RwFrame*)rwObject->parent, (RwObject *(*)(RwObject *, void *))RwFrameForAllObjectsCallback, pObject);
                    //RpAtomic* atomic = (RpAtomic*)object->m_pRwAtomic;
                    //RpGeometryForAllMaterials(atomic->geometry, ObjectMaterialCallBack, (void*)pObject);
                }
                // SetObjectMaterialText
                if(pObject->m_bHasMaterialText)
                {
                   RwFrameForAllObjects((RwFrame*)rwObject->parent, (RwObject *(*)(RwObject *, void *))ObjectMaterialTextCallBack, pObject);
                    //RpAtomic* atomic = (RpAtomic*)object->m_pRwAtomic;
                    //RpGeometryForAllMaterials(atomic->geometry, ObjectMaterialTextCallBack, (void*)pObject);
                }
            }


        }

        CObject_Render(object);
        gStoredEntityMaterials.Reset();
    }

    //((void (*)(void))(g_libGTASA + (VER_x32 ? 0x005D1F98 + 1 : 0x6F6664)))();
    //((void (*)(void))(g_libGTASA + 0x5D1F5C + 1))();
}
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
            if (pNetGame && pNetGame->GetTextDrawPool())
            {
                if (!pNetGame->GetTextDrawPool()->onTouchEvent(type, num, posX, posY)) {
                    return AND_TouchEvent(1, 0, 0, 0);
                }
            }
        }
    }

    if (pGame->IsGameInputEnabled())
        AND_TouchEvent(type, num, posX, posY);
    else
        AND_TouchEvent(1, 0, 0, 0);
}
inline std::queue<std::function<void()>> tasks;
inline std::mutex mtx;
void ProcessMainThreadTasks()
{
    if (tasks.empty())
        return;

    std::function<void()> task;
    {
        std::lock_guard<std::mutex> lock(mtx);

        task = std::move(tasks.front());
        tasks.pop();
    }
    task();
}
void CGame_InitialiseOnceBeforeRW() {
    CHook::CallFunction<void>("_ZN10CMemoryMgr4InitEv");
    CHook::CallFunction<void>("_ZN14MobileSettings10InitializeEv"); // Ð²Ð¿Ð°Ð´Ð»Ñƒ Ñ€ÐµÐ²ÐµÑ€Ñ�Ð¸Ñ‚ÑŒ Ñ‚.Ðº. Ð¼ÐµÐ½ÑŽ Ð½Ð°Ð´Ð¾ Ð²Ð¾Ð¾Ð±Ñ‰Ðµ ÑƒÐ´Ð°Ð»Ð¸Ñ‚ÑŒ
    CHook::CallFunction<void>("_ZN13CLocalisation10InitialiseEv");
    CHook::CallFunction<void>("_ZN8CFileMgr10InitialiseEv"); // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½
    CHook::CallFunction<void>("_Z12CdStreamIniti", 8);

    CHook::CallFunction<void>("_ZN4CPad10InitialiseEv");
}
void CGame_Process_hook()
{
    if(pGame->bIsGameExiting)return;

    static uint32_t dwLastMemoryClearTick = 0;
    uint32_t dwCurrentTick = GetTickCount();

    if (dwCurrentTick - dwLastMemoryClearTick > 10000)
    {
        dwLastMemoryClearTick = dwCurrentTick;

        if (pGame)
        {
            CPlayerPed* pPlayerPed = pGame->FindPlayerPed();
            if (pPlayerPed && pPlayerPed->m_pPed)
            {
                CVector player_pos;
                player_pos.x = pPlayerPed->m_pEntity->mat->pos.x;
                player_pos.y = pPlayerPed->m_pEntity->mat->pos.y;
                pGame->RefreshStreamingAt(player_pos.x, player_pos.y);
                pGame->LoadRequestedModels();
            }
        }
    }
    if (pNetGame)
    {
        CObjectPool* pObjectPool = pNetGame->GetObjectPool();
        if (pObjectPool) {
            pObjectPool->Process();
            pObjectPool->ProcessMaterialText();
        }

        CTextDrawPool* pTextDrawPool = pNetGame->GetTextDrawPool();
        if (pTextDrawPool) {
            pTextDrawPool->SnapshotProcess();
        }
    }
    ProcessMainThreadTasks();


    uint32_t CurrentTimeInCycles;
    uint32_t v1; // r4
    uint32_t v2; // r5
    uint32_t v3; // r5

    //FIXME
    ((void(*)())(g_libGTASA + 0x47D250))(); // CPad::UpdatePads()
    ((void(*)())(g_libGTASA + 0x4E56C0))(); // CTouchInterface::Clear()
    ((void(*)())(g_libGTASA + 0x71D590))(); // CHID::Update()


//	CLoadMonitor::BeginFrame(&g_LoadMonitor);
    CurrentTimeInCycles = CTimer::GetCurrentTimeInCycles();
    v1 = CurrentTimeInCycles / CTimer::GetCyclesPerMillisecond();

    ((void(*)())(g_libGTASA + 0x39A7DC))();  //CStreaming::Update();
    // CHook::CallFunction<void>(g_libGTASA+0x3941C4);// CTrain::UpdateTrains();


    v2 = CTimer::GetCurrentTimeInCycles();
    v3 = v2 / CTimer::GetCyclesPerMillisecond();

    //	CCutsceneMgr::Update();

    if ( !(CTimer::m_CodePause << 0x18) )
    {
        auto gMobileMenu = (uintptr_t *) (g_libGTASA + 0xCF43D8);
        ((void(*)(uintptr_t*))(g_libGTASA + 0x6FC0B0))(gMobileMenu); // MobileMenu::Update
    }

    // CTheZones::Update()

    // CCover::Update()

    CCameras& TheCamera = *reinterpret_cast<CCameras*>(g_libGTASA + 0x9DF030);

//	auto p_tx = (CSimpleTransform *)&TheCamera + 0x14 + 0x30;
//	if ( !TheCamera.m_pMat )
//		p_tx = *TheCamera + 0x4;

    //CAudioZones::Update(0, p_tx->m_translate);

    *(int32_t*)(g_libGTASA + 0xCADE28) = 0; // CWindModifiers::Number

    if ( !CTimer::m_CodePause && !CTimer::m_UserPause )
    {
        CSprite2d::SetRecipNearClip();
        ((void (*)()) (g_libGTASA + 0x5D90AC))(); // CSprite2d::InitPerFrame();
        ((void (*)()) (g_libGTASA + 0x5B8074))(); // CFont::InitPerFrame()
        // CCheat::DoCheats();
        // CClock::Update()

        ((void (*)()) (g_libGTASA + 0x5DCF00))(); // CWeather::Update()
        ((void(*)())(g_libGTASA + 0x3FFDE8))(); // CTheScripts::Process()
        // CCollision::Update()
        //CCollision::Update();

        // CPathFind::UpdateStreaming

        CHook::CallFunction<void>(g_libGTASA+0x6C2368);// CTrain::UpdateTrains();
        //CHook::CallFunction<void>(g_libGTASA+(VER_x32?0x572EBC+1:0x695608));// CHeli::UpdatHelis
        // CDarkel::Update()
        ((void(*)())(g_libGTASA + 0x5CECF4))(); // CSkidmarks::Update();
        ((void(*)())(g_libGTASA + 0x5BAC70))(); // CGlass::Update()
        // CWanted::UpdateEachFrame();
        // CCreepingFire::Update();
        // CSetPieces::Update();

        auto gFireManager = (uintptr_t *) (g_libGTASA + 0x9E5A28);
        ((void (*)(uintptr_t *)) (g_libGTASA + 0x475034))(gFireManager); // CFireManager::Update

        // FIXME: add if
        ((void(*)(bool))(g_libGTASA + 0x5A1FDC))(false); // CPopulation::Update Ð½ÑƒÐ¶Ð½Ð¾ (

        ((void (*)()) (g_libGTASA + 0x6E8854))(); // CWeapon::UpdateWeapons()
//		if ( !CCutsceneMgr::ms_running )
//			CTheCarGenerators::Process();
//		CCranes::UpdateCranes();
//		CClouds::Update();
        ((void (*)()) (g_libGTASA + 0x5B55D8))(); // CMovingThings::Update();
        ((void(*)())(g_libGTASA + 0x5DC710))(); // CWaterCannons::Update()
//		CUserDisplay::Process();
        ((void (*)()) (g_libGTASA + 0x4AF97C))(); // CWorld::Process()

//		CLoadMonitor::EndFrame(&g_LoadMonitor);

        if ( !CTimer::bSkipProcessThisFrame )
        {
            CHook::CallFunction<void>(g_libGTASA + 0x3F06A0);
            //CPickup_Update_hook();
//			CCarCtrl::PruneVehiclesOfInterest();
            CHook::CallFunction<void>(g_libGTASA+0x3DF894); //CGarages::Update();
// 			CEntryExitManager::Update();
            CHook::CallFunction<void>(g_libGTASA+0x43C5F0); //	CStuntJumpManager::Update();
            ((void (*)()) (g_libGTASA + 0x5ABDA8))(); // CBirds::Update()
            ((void (*)()) (g_libGTASA + 0x5D0D0C))(); // CSpecialFX::Update()
            // CRopes::Update();
        }
        ((void (*)()) (g_libGTASA + 0x5C2118))(); // CPostEffects::Update()
        ((void (*)()) (g_libGTASA + 0x4A55C0))(); // CTimeCycle::Update() crash without
        // CPopCycle::Update()

        // CInterestingEvents::ScanForNearbyEntities

        ((void (*)(CCameras*)) (g_libGTASA + 0x45D46C))(&TheCamera); // CCamera::Process()

        // CCullZones::Update() Ð¼ÐµÐ½Ñ‚Ñ‹ Ð½Ðµ Ð¼Ð¾Ð³ÑƒÑ‚ Ð½Ð°Ð¹Ñ‚Ð¸?
        CHook::CallFunction<void>(g_libGTASA+0x3D8384);// CGameLogic::Update()
        // CGangWars::Update();
        // CConversations::Update()
        // CPedToPlayerConversations::Update()
        // CBridge::Update()

        ((void (*)()) (g_libGTASA + 0x5B2540))(); // CCoronas::DoSunAndMoon()
        ((void (*)()) (g_libGTASA + 0x5B0A90))(); // CCoronas::Update()
        ((void (*)()) (g_libGTASA + 0x5CDF20))(); // CShadows::UpdatePermanentShadows()

        // CPlantMgr::Update

        ((void (*)()) (g_libGTASA + 0x349E94))(); // CCustomBuildingRenderer::Update()
//		if ( v6 <= 3 )
//			CCarCtrl::GenerateRandomCars();
//		CRoadBlocks::GenerateRoadBlocks();
//		CCarCtrl::RemoveDistantCars();
//		CCarCtrl::RemoveCarsIfThePoolGetsFull();
        auto temp = TheCamera.m_pRwCamera;

        auto g_fx = *(uintptr_t *) (g_libGTASA + 0xA42560);
        ((void (*)(uintptr_t*, RwCamera*, float )) (g_libGTASA + 0x4B7BC4))(&g_fx, temp, CTimer::ms_fTimeStep / 50.0f); // Fx_c::Update

        auto g_breakMan = (uintptr_t *) (g_libGTASA + 0xBE5B58);
        ((void (*)(uintptr_t*, float )) (g_libGTASA + 0x51BB4C))(g_breakMan, CTimer::ms_fTimeStep); // BreakManager_c::Update

        // InteriorManager_c::Update(&g_interiorMan);
        // ProcObjectMan_c::Update

        // WaterCreatureManager_c::Update

        ((void (*)()) (g_libGTASA + 0x6DE278))(); // CWaterLevel::PreRenderWater()
    }

    //CHook::CallFunction<void>(g_libGTASA+(VER_x32?0x572EBC+1:0x695608));
//	CCheat::ProcessAllCheats();
    static bool once = false;
    if (!once)
    {
        once = true;
        return;
    }

}
uint32_t CHudColours__GetIntColour(uintptr* thiz, uint8 colour_id)
{
    return TranslateColorCodeToRGBA(colour_id);
}
void (*CWorld_ProcessPedsAfterPreRender)();
void CWorld_ProcessPedsAfterPreRender_Hook()
{
    CWorld_ProcessPedsAfterPreRender();

    if (pNetGame)
    {
        CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
        if (pPlayerPool)
            pPlayerPool->ProcessAttachedObjects();
    }
}
void (*CTaskComplexLeaveCar)(uintptr_t** thiz, VEHICLE_TYPE* pVehicle, int iTargetDoor, int iDelayTime, bool bSensibleLeaveCar, bool bForceGetOut);
void CTaskComplexLeaveCar_hook(uintptr_t** thiz, VEHICLE_TYPE* pVehicle, int iTargetDoor, int iDelayTime, bool bSensibleLeaveCar, bool bForceGetOut)
{
    uintptr_t dwRetAddr = 0;
    GET_LR(dwRetAddr);

    if (dwRetAddr == 0x49035C || dwRetAddr == 0x490CF4)
    {
        if (pNetGame)
        {
            if ((VEHICLE_TYPE*)GamePool_FindPlayerPed()->pVehicle == pVehicle)
            {
                CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
                VEHICLEID VehicleID = pVehiclePool->FindIDFromGtaPtr((VEHICLE_TYPE*)GamePool_FindPlayerPed()->pVehicle);
                if (VehicleID != INVALID_VEHICLE_ID)
                {
                    CVehicle* pVehicle = pVehiclePool->GetAt(VehicleID);
                    CLocalPlayer* pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
                    if (pVehicle && pLocalPlayer)
                    {
                        CPlayerPed *pPlayerPed = pGame->FindPlayerPed();

                        if(pPlayerPed->IsInJetpackMode())
                        {
                            pPlayerPed->StopJetpack();
                            pLocalPlayer->SendExitVehicleNotification(VehicleID);

                        }else if (pVehicle->IsATrainPart())
                        {
                            pLocalPlayer->SendExitVehicleNotification(VehicleID);
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

#pragma pack(push, 1)
struct CPedDamageResponseCalculator
{
    PED_TYPE* m_pDamager;
    float m_fDamageFactor;
    int m_pedPieceType;
    int m_weaponType;
};
#pragma pack(pop)
PLAYERID FindActorIDFromGtaPtr(PED_TYPE* pPed)
{
    if (pPed) {
        return pNetGame->GetActorPool()->FindIDFromGtaPtr(pPed);
    }

    return INVALID_PLAYER_ID;
}

bool ComputeDamageResponse(CPedDamageResponseCalculator* calculator, PED_TYPE* pPed)
{
    PED_TYPE* pGamePed = GamePool_FindPlayerPed();
    bool isLocalPed = false;

    if (!pNetGame) return false;

    PED_TYPE* pDamager = calculator->m_pDamager;


    //pUI->chat()->addDebugMessage("DAMAGE weapon id %i",calculator->m_weaponType);

    if (pPed == pGamePed) {
        isLocalPed = true;
    }
    else if (pDamager != pGamePed) {


    }

    CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
    CLocalPlayer* pLocalPlayer = pPlayerPool->GetLocalPlayer();
    PLAYERID PlayerID;


    if (pDamager != pGamePed) /* CCivilianPed */
    {
        PlayerID = FindPlayerNumFromPedPtr(pGamePed);

        //pUI->chat()->addDebugMessage("TRUE pDamager != pGamePed && IsValidGamePed(pGamePed");
        if(calculator->m_weaponType == 54 || calculator->m_weaponType == 53 || calculator->m_weaponType == 51) //splat //drown //expostion
        {
            pLocalPlayer->SendTakeDamageEvent(INVALID_PLAYER_ID,
                                              calculator->m_fDamageFactor,
                                              calculator->m_weaponType,calculator->m_pedPieceType);
        }

        return true;
    }

    if (isLocalPed)
    {
        //pUI->chat()->addDebugMessage("TRUE ISLOCALPED");
        PlayerID = FindPlayerNumFromPedPtr(pDamager);

        pLocalPlayer->SendTakeDamageEvent(PlayerID,
                                          calculator->m_fDamageFactor,
                                          calculator->m_weaponType,
                                          calculator->m_pedPieceType);
    }
    else
    {
        PlayerID = FindPlayerNumFromPedPtr(pPed);
        if (PlayerID != INVALID_PLAYER_ID)
        {
            //pUI->chat()->addDebugMessage("PlayerID != INVALID_PLAYER_ID");

            pLocalPlayer->SendGiveDamageEvent(PlayerID,
                                              calculator->m_fDamageFactor,
                                              calculator->m_weaponType,
                                              calculator->m_pedPieceType);

            if (pPlayerPool->GetAt(PlayerID)->IsNPC())
                return true;
        }
        else
        {
            //pUI->chat()->addDebugMessage("TRIE ACTOR ID");

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

void (*CPedDamageResponseCalculator__ComputeDamageResponse)(CPedDamageResponseCalculator* thiz, PED_TYPE* pPed, uintptr_t* a3, uint32_t a4);
void CPedDamageResponseCalculator__ComputeDamageResponse_hook(CPedDamageResponseCalculator* thiz, PED_TYPE* pPed, uintptr_t *a3, uint32_t a4)
{

    if (thiz == nullptr || pPed == nullptr || a3 == nullptr) return;
    if (ComputeDamageResponse(thiz, pPed))
        return;

    CPedDamageResponseCalculator__ComputeDamageResponse(thiz, pPed, a3, a4);
}
static constexpr float ar43 = 4.0f/3.0f;
float *ms_fAspectRatio;
void (*DrawCrosshair)(uintptr_t* thiz);
void DrawCrosshair_hook(uintptr_t* thiz)
{
    float save1 = *CCamera::m_f3rdPersonCHairMultX;
    *CCamera::m_f3rdPersonCHairMultX = 0.530f - (*ms_fAspectRatio - ar43) * 0.01125f;

    float save2 = *CCamera::m_f3rdPersonCHairMultY;
    *CCamera::m_f3rdPersonCHairMultY = 0.400f + (*ms_fAspectRatio - ar43) * 0.03600f;

    DrawCrosshair(thiz);

    *CCamera::m_f3rdPersonCHairMultX = save1;
    *CCamera::m_f3rdPersonCHairMultY = save2;
}

VECTOR& (*FindPlayerSpeed)(int a1);
VECTOR& FindPlayerSpeed_hook(int a1)
{
    uintptr_t dwRetAddr = 0;
    GET_LR(dwRetAddr);
    dwRetAddr -= g_libGTASA;

    if(dwRetAddr == 0x4FD2CC)
    {
        if(pNetGame)
        {
            CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
            if(pPlayerPed &&
               pPlayerPed->IsInVehicle() &&
               pPlayerPed->IsAPassenger())
            {
                VECTOR vec = _VECTOR(-1.0f);
                return vec;
            }
        }
    }

    return FindPlayerSpeed(a1);
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

   // int32 raceBlipIndex = *(int32 *)(g_libGTASA + (VER_x32 ? 0x6E00BC : 0x8BE7E8));
   // if (raceBlipIndex == a2) {
      //  GPS::Set(pGame->m_vecRaceCheckpointPos, pGame->m_bRaceCheckpointsEnabled);
  //  }

    CRadar_ClearBlip(a2);
}

enum TextureDatabaseFormat
{
    DF_UNC = 0x0,
    DF_DXT = 0x1,
    DF_360 = 0x2,
    DF_PS3 = 0x3,
    DF_PVR = 0x4,
    DF_ETC = 0x5,
    DF_Default = 0x6,
    DF_ALL = 0x7,
};
void InitGui();
bool CGame__InitialiseRenderWare_hook()
{

    CCameras& TheCamera = *reinterpret_cast<CCameras*>(g_libGTASA + 0x9DF030);
    CScene& Scenes = *reinterpret_cast<CScene*>(g_libGTASA + 0xCB7118);

    CHook::CallFunction<void>(g_libGTASA + 0x5E4A98);

    CHook::CallFunction<void>(g_libGTASA + 0x5E57E4);

    LOGI("Loading SAMP texture database..");
    // uint32_t result = CGame__InitialiseRenderWare();
    // TextureDatabaseRuntime::Load()
    ((void(*)(const char*, int, int))(g_libGTASA + /*0x1BF244*/0x7880A0))("samp", 0, DF_Default);
    ((void(*)(const char*, int, int))(g_libGTASA + /*0x1BF244*/0x7880A0))("mobile", 0, DF_Default);
    ((void(*)(const char*, int, int))(g_libGTASA + /*0x1BF244*/0x7880A0))("txd", 0, DF_Default);
    ((void(*)(const char*, int, int))(g_libGTASA + /*0x1BF244*/0x7880A0))("gta3", 0, DF_Default);
    ((void(*)(const char*, int, int))(g_libGTASA + /*0x1BF244*/0x7880A0))("gta_int", 0, DF_Default);
    ((void(*)(const char*, int, int))(g_libGTASA + /*0x1BF244*/0x7880A0))("player", 0, DF_PVR);
    ((void(*)(const char*, int, int))(g_libGTASA + /*0x1BF244*/0x7880A0))("menu", 0, DF_PVR);
    LOGI("Loading SAMP texture database.. 1");

    const auto camera = RwCameraCreate();
    if (!camera) {
        CHook::CallFunction<void>(g_libGTASA + 0x5E4118, camera);
        return false;
    }

    const auto frame = RwFrameCreate();
    rwObjectHasFrameSetFrame(&camera->object.object, frame);
    camera->frameBuffer = RwRasterCreate(RsGlobal->maximumWidth, RsGlobal->maximumHeight, 0, rwRASTERTYPECAMERA);
    camera->zBuffer = RwRasterCreate(RsGlobal->maximumWidth, RsGlobal->maximumHeight, 0, rwRASTERTYPEZBUFFER);
    if (!camera->object.object.parent) {
        CHook::CallFunction<void>(g_libGTASA + 0x5E4118, camera);

        return false;
    }
    Scenes.m_pRwCamera = camera;
    TheCamera.Init();
    TheCamera.SetRwCamera(Scenes.m_pRwCamera);
    RwCameraSetFarClipPlane(Scenes.m_pRwCamera, 2000.0f);
    RwCameraSetNearClipPlane(Scenes.m_pRwCamera, 0.9f);
    CHook::CallFunction<void>(g_libGTASA + 0x5E3FD0, Scenes.m_pRwCamera, nullptr, 0.7f, 4.0f / 3.0f);

    RwBBox bb;
    bb.sup = { 10'000.0f,  10'000.0f,  10'000.0f};
    bb.inf = {-10'000.0f, -10'000.0f, -10'000.0f};

    if (Scenes.m_pRpWorld = RpWorldCreate(&bb); !Scenes.m_pRpWorld) {
        CHook::CallFunction<void>(g_libGTASA + 0x5E4118, Scenes.m_pRwCamera);

        Scenes.m_pRwCamera = nullptr;

        return false;
    }
    RpWorldAddCamera(Scenes.m_pRpWorld, Scenes.m_pRwCamera);
    CHook::CallFunction<void>(g_libGTASA + 0x53F010, Scenes.m_pRpWorld);
    CFont::Initialise();
    CHook::CallFunction<void>(g_libGTASA + 0x53F448); // CHud::Initialise();
    CHook::CallFunction<void>(g_libGTASA + 0x5C089C); // CPlayerSkin::Initialise();
    CHook::CallFunction<void>(g_libGTASA + 0x5C2114);
    m_pWorkingMatrix1 = RwMatrixCreate();
    m_pWorkingMatrix2 = RwMatrixCreate();

    InitGui();
    return true;


}

void InjectHooks() {


    FLog("InjectHooks");


    CHook::Write(g_libGTASA + 0XBBD6A8, &ms_modelInfoPtrs);
    CHook::Write(g_libGTASA+0x826B58, &COcclusion::aOccluders); // 0xA41140 0xCE3EE8 old
    CHook::Write(g_libGTASA+0x826B20, &COcclusion::NumOccludersOnMap); // 0xA45790 0xCE8538 old

    CHook::Redirect("_ZN5CGame22InitialiseOnceBeforeRWEv", &CGame_InitialiseOnceBeforeRW);

    CHook::Redirect("_ZN5CGame7ProcessEv", &CGame_Process_hook);

    //GOT MISSING
    //CHook::InlineHook(g_libGTASA + 0x6FD668, (uintptr_t)MainMenuScreen__OnExit_hook, (uintptr_t*)&MainMenuScreen__OnExit);
    // samp main loop
    CTimer::InjectHooks();
    CPools::InjectHooks();

    HookCPad();
    
}




constexpr uint32_t TOTAL_IMG_ARCHIVES = 8;

struct tStreamingFileDesc {
    char m_name[40];                    // XREF: CStreaming::InitImageList(void)+2C/w
    // CStreaming::InitImageList(void)+3C/w ...
    bool m_bRegister;                   // XREF: CStreaming::InitImageList(void)+84/w

    int32 m_lsn;
};
VALIDATE_SIZE(tStreamingFileDesc, 0x30);

static inline tStreamingFileDesc ms_files[TOTAL_IMG_ARCHIVES];
void(*CStream_InitImageList)();
void CStream_InitImageList_hook()
{
    FLog("Initializing ImageList..");

    for (auto & ms_file : ms_files) {
        ms_file.m_name[0] = 0;
        ms_file.m_lsn = 0;
    }
    // CStreaming::AddImageToList
    ((uintptr_t(*)(const char*, int))(g_libGTASA + 0x3998E0))("TEXDB\\SAMPCOL.IMG", 1);
    ((uint32_t(*)(char*, uint32_t))(g_libGTASA + 0x3998E0))("TEXDB\\GTA3.IMG", 1);
    ((uint32_t(*)(char*, uint32_t))(g_libGTASA + 0x3998E0))("TEXDB\\GTA_INT.IMG", 1);
    ((uintptr_t(*)(const char*, int))(g_libGTASA + 0x3998E0))("TEXDB\\SAMP.IMG", 1);
    return;
}
#include "CFPSFix.h"
CFPSFix g_fps;

void (*ANDRunThread)(void* a1);
void ANDRunThread_hook(void* a1)
{
    g_fps.PushThread(gettid());

    ANDRunThread(a1);
}




void (*CPlaceable_InitMatrixArray)(void);
void CPlaceable_InitMatrixArray_hook(void)
{
    // CMatrixLinkList::Init
    ((void (*)(uintptr_t, size_t))(g_libGTASA + 0x48DFA4))(g_libGTASA + 0x9E7F98, 10000);
}
void (*CRenderer_RenderEverythingBarRoads)();
void CRenderer_RenderEverythingBarRoads_hook() {

    CRenderer_RenderEverythingBarRoads();

    if (pNetGame) {
        CObjectPool* pObjectPool = pNetGame->GetObjectPool();
        if (pObjectPool) {
            for (OBJECTID i = 0; i < MAX_OBJECTS; i++) {
                CObject* pObject = pObjectPool->GetAt(i);
                if (pObject && pObject->m_bForceRender) {
                    RenderEntity(reinterpret_cast<ENTITY_TYPE *>(pObject->m_pEntity));
                }
            }
        }
    }
}

void InstallSpecialHooks()
{
    InjectHooks();

    CHook::Redirect("_ZN5CGame20InitialiseRenderWareEv", &CGame__InitialiseRenderWare_hook);
    CHook::InstallPLT(g_libGTASA + 0x828730, &StartGameScreen__OnNewGameCheck_hook, &StartGameScreen__OnNewGameCheck);
    CHook::InlineHook("_Z10NvUtilInitv", &NvUtilInit_hook, &NvUtilInit);
    CHook::RET("_ZN12CCutsceneMgr16LoadCutsceneDataEPKc"); // LoadCutsceneData
    CHook::RET("_ZN12CCutsceneMgr10InitialiseEv");			// CCutsceneMgr::Initialise

    CHook::Redirect(g_libGTASA+0x7C3E0C, &NvFOpen);
    CHook::InlineHook("_ZN14MainMenuScreen6UpdateEf", &MainMenuScreen__Update_hook, &MainMenuScreen__Update);


    CHook::InlineHook(g_libGTASA + 0x399784, (uintptr_t)CStream_InitImageList_hook, (uintptr_t*)&CStream_InitImageList);

   // CHook::InlineHook("_Z12ANDRunThreadPv", (uintptr_t)ANDRunThread_hook, (uintptr_t*)& ANDRunThread);



    // placeable matrix alloc
    CHook::InstallPLT(g_libGTASA + 0x8328E0, (uintptr_t)CPlaceable_InitMatrixArray_hook, (uintptr_t*)& CPlaceable_InitMatrixArray);
    // render objects 3000+- pos
    CHook::InstallPLT(g_libGTASA + 0x82FCE0, (uintptr_t)CRenderer_RenderEverythingBarRoads_hook, (uintptr_t*)&CRenderer_RenderEverythingBarRoads);

    LOGI("END SPECIAL HOOKS");

}

void InstallHooks()
{
    LOGI("INSTALL HOOKS");
    CHook::Redirect("_Z13Render2dStuffv", &Render2dStuff);
    CHook::Redirect("_Z13RenderEffectsv", &RenderEffects);
    CHook::InlineHook("_Z14AND_TouchEventiiii", &AND_TouchEvent_hook, &AND_TouchEvent);
    CHook::InlineHook("_ZN6CRadar20DrawRadarGangOverlayEb", &CRadar_DrawRadarGangOverlay_hook, &CRadar_DrawRadarGangOverlay);
    // gangzones
    CHook::InlineHook("_ZN6CRadar20DrawRadarGangOverlayEb", &CRadar_DrawRadarGangOverlay_hook, &CRadar_DrawRadarGangOverlay);
    // radar
    CHook::InlineHook("_ZN6CRadar12SetCoordBlipE9eBlipType7CVectorj12eBlipDisplayPc",
                      &CRadar__SetCoordBlip_hook, &CRadar__SetCoordBlip);

    // removebuilding
    CHook::InlineHook("_ZN11CFileLoader18LoadObjectInstanceEP19CFileObjectInstancePKc", &CFileLoader__LoadObjectInstance_hook, &CFileLoader__LoadObjectInstance);

    // obj material

    CHook::InlineHook("_ZN7CObject6RenderEv", &CObject_Render_hook, & CObject_Render);



    CHook::Redirect("_ZN11CHudColours12GetIntColourEh", &CHudColours__GetIntColour); // dangerous


    //CHook::installPLTHook(g_libGTASA+0x6733F0, (uintptr_t)TaskEnterVehicle_hook, (uintptr_t*)&TaskEnterVehicle);

    //CHook::InstallPLT(g_libGTASA + 0x77D628, (uintptr_t)CTaskComplexLeaveCar_hook, (uintptr_t*)& CTaskComplexLeaveCar);
    CHook::InlineHook("_ZN20CTaskComplexLeaveCarC1EP8CVehicleiibb", (uintptr_t)CTaskComplexLeaveCar_hook, (uintptr_t*)& CTaskComplexLeaveCar);


    // attach obj to ped
    CHook::InlineHook("_ZN6CWorld25ProcessPedsAfterPreRenderEv", &CWorld_ProcessPedsAfterPreRender_Hook, &CWorld_ProcessPedsAfterPreRender);
    // game pause
    //CHook::InlineHook("_ZN6CTimer14StartUserPauseEv", (uintptr_t)CTimer_StartUserPause_hook, (uintptr_t*)&CTimer_StartUserPause);
    //CHook::InlineHook("_ZN6CTimer12EndUserPauseEv", (uintptr_t)CTimer_EndUserPause_hook, (uintptr_t*)&CTimer_EndUserPause);
    // aim


    // ComputeDamageResponse
    CHook::InlineHook("_ZN28CPedDamageResponseCalculator21ComputeDamageResponseEP4CPedR18CPedDamageResponseb", &CPedDamageResponseCalculator__ComputeDamageResponse_hook, &CPedDamageResponseCalculator__ComputeDamageResponse);

    // Crosshair Fix
    ms_fAspectRatio = (float*)(g_libGTASA+0xC54518);
    CHook::InstallPLT(g_libGTASA + 0x831708, (uintptr_t)DrawCrosshair_hook, (uintptr_t*)&DrawCrosshair);

    // fix radar in passenger
    // CHook::InstallPLT(g_libGTASA+0x82BFC0, (uintptr_t)FindPlayerSpeed_hook, (uintptr_t*)&FindPlayerSpeed);


    //later
    //CHook::installPLTHook(g_libGTASA + 0x674280, (uintptr_t) CVehicleModelInfo__SetupCommonData_hook, (uintptr_t*)&CVehicleModelInfo__SetupCommonData);
    // CHook::installPLTHook(g_libGTASA + 0x06D008, (uintptr_t) CAEVehicleAudioEntity__GetVehicleAudioSettings_hook, (uintptr_t*)&CAEVehicleAudioEntity__GetVehicleAudioSettings);

    CHook::InlineHook("_ZN6CRadar9ClearBlipEi", &CRadar_ClearBlip_hook, &CRadar_ClearBlip);


}