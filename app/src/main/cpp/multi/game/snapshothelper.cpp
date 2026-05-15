#include "../main.h"
#include "game.h"
#include "RW/RenderWare.h"
#include <GLES2/gl2.h>

extern CGame* pGame;

CSnapShotHelper::CSnapShotHelper()
{
    m_camera = 0;
    m_light = 0;
    m_frame = 0;
    m_zBuffer = 0;
    m_raster = 0;

    SetUpScene();
}

void CSnapShotHelper::SetUpScene()
{
    // RpLightCreate
    m_light = ((uintptr_t(*)(int))(g_libGTASA + 0x73ADB4))(2);

    if (m_light == 0) return;
    if (m_light == 0) return;
    float rwColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    // RpLightSetColor
    ((void (*)(uintptr_t, float*))(g_libGTASA + 0x73A614))(m_light, rwColor);



    m_zBuffer = reinterpret_cast<uintptr_t>(RwRasterCreate(256, 256, 0, rwRASTERTYPEZBUFFER));
    m_camera = ((uintptr_t(*)())(g_libGTASA + 0x756184))();
    m_frame = ((uintptr_t(*)())(g_libGTASA + 0x758A7C))();
    VECTOR v = { 0.0f, 0.0f, 50.0f };
    RwFrameTranslate(m_frame, &v, rwCOMBINEPRECONCAT);
    VECTOR v1 = { 1.0f, 0.0f, 0.0f };
    RwFrameTranslate(m_frame, &v, 1);

    if (!m_camera) return;
    if (!m_frame) return;
    *(uintptr_t*)(m_camera + 0x88) = m_zBuffer;


    // RwObjectHasFrameSetFrame

    ((void(*)(uintptr_t, uintptr_t))(g_libGTASA + 0x75F178))(m_camera, m_frame);

    // RwCameraSetFarClipPlane
    ((void(*)(uintptr_t, float))(g_libGTASA + 0x755DF8))(m_camera, 300.0f);

    // RwCameraSetNearClipPlane
    ((void(*)(uintptr_t, float))(g_libGTASA + 0x755D48))(m_camera, 0.01f);

    // RwCameraSetViewWindow
    float view[2] = { 0.5f, 0.5f };
    ((void(*)(uintptr_t, float*))(g_libGTASA + 0x756088))(m_camera, view);

    // RwCameraSetProjection
    ((void(*)(uintptr_t, int))(g_libGTASA + 0x755F78))(m_camera, 1);

    uintptr_t pRwWorld = *(uintptr_t*)(g_libGTASA + 0xCB7118);
    if (pRwWorld) {
        ((void(*)(uintptr_t, uintptr_t))(g_libGTASA + 0x7450BC))(pRwWorld, m_camera);
    }
}

uintptr_t CSnapShotHelper::CreateObjectSnapShot(int iModel, uint32_t dwColor, VECTOR* vecRot, float fZoom)
{
    FLog("Object snapshot: %d", iModel);

    uintptr_t raster = (uintptr_t)RwRasterCreate(256, 256, 32, rwRASTERFORMAT8888 | rwRASTERTYPECAMERATEXTURE);
    // RwTextureCreate
    uintptr_t bufferTexture = ((uintptr_t(*)(uintptr_t))(g_libGTASA + 0x75D2E4))(raster);

    if (!raster || !bufferTexture) return bufferTexture;

    if (iModel == 1373 || iModel == 3118 || iModel == 3552 || iModel == 3553)
        iModel = 18631;

    bool bNeedRemoveModel = false;
    if (!pGame->IsModelLoaded(iModel))
    {
        bNeedRemoveModel = true;
        pGame->RequestModel(iModel);
        pGame->LoadRequestedModels();
        while (!pGame->IsModelLoaded(iModel)) usleep(1000);
    }

    uintptr_t atomic = ModelInfoCreateInstance(iModel);
    if (!atomic) return bufferTexture;


    auto modelInfo = GetModelInfo(iModel);
    if (!modelInfo || !modelInfo->m_pColModel) {
        FLog("ModelInfo or ColModel is null!");
        return 0;
    }

    float fRadius = modelInfo->m_pColModel->GetBoundRadius();


    VECTOR vec;
    vec.X = 0.0f;
    vec.Y = 0.0f;
    vec.Z = 0.0f;

    fZoom = (-0.1f - fRadius * 2.25f) * fZoom;
    VECTOR vecCenter = modelInfo->m_pColModel->GetBoundCenter();


    uintptr_t parent = *(uintptr_t*)(atomic + 4);
    if (parent)
    {
        vec.X = -vec.X;
        vec.Y = fZoom;
        vec.Z = 50.0f - vec.Z;
        RwFrameTranslate(parent, &vec, 1);
        if (iModel == 18631) {
            RwFrameRotate(parent, 2, 180.0f);
        }
        else
        {
            if (vecRot->X != 0.0f) {
                RwFrameRotate(parent, 0, vecRot->X);
            }
            if (vecRot->Y != 0.0f) {
                RwFrameRotate(parent, 1, vecRot->Y);
            }
            if (vecRot->Z != 0.0f) {
                RwFrameRotate(parent, 2, vecRot->Z);
            }
        }
    }

    *(uintptr_t*)(m_camera + 0x80) = raster;

    // CVisibilityPlugins::SetRenderWareCamera
    ((void(*)(uintptr_t))(g_libGTASA + 0x5E7D50))(m_camera);
    // RwCameraClear
    ((void(*)(uintptr_t, uint32_t*, int))(g_libGTASA + 0x755F14))(m_camera, &dwColor, 3);
    RwCameraBeginUpdate((RwCamera*)m_camera);
    RpWorldAddLight(m_light);

    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)true);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)true);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEGOURAUD);
    RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)0);
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODENACULLMODE);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)false);

    // DefinedState
    ((void(*) (void))(g_libGTASA + 0x5E17E8))();

    RenderClumpOrAtomic(atomic);
    RwCameraEndUpdate((RwCamera*)m_camera);
    RpWorldRemoveLight(m_light);
    DestroyAtomicOrClump(atomic);

    if (bNeedRemoveModel)
        pGame->RemoveModel(iModel, false);

    return bufferTexture;
}
// 0.3.7
uintptr_t CSnapShotHelper::CreatePedSnapShot(int iModel, uint32_t dwColor, VECTOR* vecRot, float fZoom)
{
    FLog("Ped snapshot: %d", iModel);

    uintptr_t raster = (uintptr_t)RwRasterCreate(256, 256, 32, rwRASTERFORMAT8888 | rwRASTERTYPECAMERATEXTURE);
    // RwTextureCreate
    uintptr_t bufferTexture = ((uintptr_t(*)(uintptr_t))(g_libGTASA + 0x75D2E4))(raster);

    CPlayerPed* pPed = new CPlayerPed(208, 0, 0.0f, 0.0f, 0.0f, 0.0f);

    if (!raster || !bufferTexture || !pPed) return 0;

    float posZ = iModel == 162 ? 50.15f : 50.05f;
    float posY = fZoom * -2.25f;
    pPed->TeleportTo(0.0f, posY, posZ);
    pPed->SetModelIndex(iModel);
    pPed->SetGravityProcessing(false);
    pPed->SetCollisionChecking(false);

    MATRIX4X4 mat;
    pPed->GetMatrix(&mat);

    if (vecRot->X != 0.0f)
        RwMatrixRotate(&mat, 0, vecRot->X);
    if (vecRot->Y != 0.0f)
        RwMatrixRotate(&mat, 1, vecRot->Y);
    if (vecRot->Z != 0.0f)
        RwMatrixRotate(&mat, 2, vecRot->Z);

    pPed->UpdateMatrix(mat);

    // set camera frame buffer //
    *(uintptr_t*)(m_camera + 0x80) = raster;
    // CVisibilityPlugins::SetRenderWareCamera
    ((void(*)(uintptr_t))(g_libGTASA + 0x5E7D50))(m_camera);

    // RwCameraClear
    ((void(*)(uintptr_t, uint32_t*, int))(g_libGTASA + 0x755F14))(m_camera, &dwColor, 3);

    RwCameraBeginUpdate((RwCamera*)m_camera);

    RpWorldAddLight(m_light);

    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)true);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)true);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEGOURAUD);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)false);

    // DefinedState
    ((void(*) (void))(g_libGTASA + 0x5E17E8))();

    pPed->Add();

    pPed->ClumpUpdateAnimations(100.0f, 1);
    RenderEntity(reinterpret_cast<ENTITY_TYPE *>(pPed->m_pPed));
    RwCameraEndUpdate((RwCamera*)m_camera);

    RpWorldRemoveLight(m_light);

    pPed->Remove();

    delete pPed;

    if (!GetModelRefCounts(iModel))
        pGame->RemoveModel(iModel, false);

    return bufferTexture;
}

uintptr_t CSnapShotHelper::CreateVehicleSnapShot(int iModel, uint32_t dwColor, VECTOR* vecRot, float fZoom, uint32_t dwColor1, uint32_t dwColor2)
{
    FLog("Vehicle snapshot: %d", iModel);

    uintptr_t raster = (uintptr_t)RwRasterCreate(256, 256, 32, rwRASTERFORMAT8888 | rwRASTERTYPECAMERATEXTURE);
    // RwTextureCreate
    uintptr_t bufferTexture = ((uintptr_t(*)(uintptr_t))(g_libGTASA + 0x75D2E4))(raster);

    if (iModel == 570) {
        iModel = 538;
    }
    else if (iModel == 569) {
        iModel = 537;
    }

    CVehicle* pVehicle = new CVehicle(iModel, 0.0f, 0.0f, 50.0f, 0.0f, false, false);

    if (!raster || !bufferTexture || !pVehicle) return 0;

    pVehicle->SetGravityProcessing(false);
    pVehicle->SetCollisionChecking(false);
    float radius = GetModelColSphereRadius(iModel);
    float posY = (-1.0f - (radius + radius)) * fZoom;
    if (pVehicle->GetVehicleSubtype() == VEHICLE_SUBTYPE_BOAT) {
        posY = -5.5f - radius * 2.5f;
    }

    pVehicle->TeleportTo(0.0f, posY, 50.0f);
    if (dwColor1 != 0xFFFFFFFF && dwColor2 != 0xFFFFFFFF) {
        pVehicle->SetColor(dwColor1, dwColor2);
    }

    MATRIX4X4 mat;
    pVehicle->GetMatrix(&mat);

    if (vecRot->X != 0.0f) {
        RwMatrixRotate(&mat, 0, vecRot->X);
    }
    if (vecRot->Y != 0.0f) {
        RwMatrixRotate(&mat, 1, vecRot->Y);
    }
    if (vecRot->Z != 0.0f) {
        RwMatrixRotate(&mat, 2, vecRot->Z);
    }

    pVehicle->UpdateMatrix(mat);

    *(uintptr_t*)(m_camera + 0x80) = raster;
    // CVisibilityPlugins::SetRenderWareCamera
    ((void(*)(uintptr_t))(g_libGTASA + 0x5E7D50))(m_camera);

    // RwCameraClear
    ((void(*)(uintptr_t, uint32_t*, int))(g_libGTASA + 0x1D5D70 + 1))(m_camera, &dwColor, 3);

    RwCameraBeginUpdate((RwCamera*)m_camera);
    RpWorldAddLight(m_light);

    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)true);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)true);
    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEGOURAUD);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)false);

    // DefinedState
    ((void(*) (void))(g_libGTASA + 0x5E17E8))();

    pVehicle->Add();
    RenderEntity(reinterpret_cast<ENTITY_TYPE *>(pVehicle->m_pVehicle));
    RwCameraEndUpdate((RwCamera*)m_camera);
    RpWorldRemoveLight(m_light);

    pVehicle->Remove();
    delete pVehicle;

    return bufferTexture;
}
