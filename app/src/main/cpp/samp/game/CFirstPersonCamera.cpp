//
// Created by vadim on 13.09.2025.
//

#include "../main.h"
#include "game.h"

#include "..//net/netgame.h"
#include "Scene.h"
#include "CFirstPersonCamera.hpp"

extern CNetGame* pNetGame;

bool CFirstPersonCamera::m_bEnabled = false;

CVector* RwMatrixMultiplyByVector(CVector* out, RwMatrix* mat, CVector* in)
{
    out->x = mat->right.x * in->x + mat->up.x * in->y + mat->at.x * in->z + mat->pos.x;
    out->y = mat->right.y * in->x + mat->up.y * in->y + mat->at.y * in->z + mat->pos.y;
    out->z = mat->right.z * in->x + mat->up.z * in->y + mat->at.z * in->z + mat->pos.z;
    return out;
}
void CFirstPersonCamera::ProcessCameraOnFoot(uintptr_t pCam, CPlayerPed* pPed)
{
    if (!m_bEnabled)
    {
        return;
    }

    CVector* pVec = (CVector*)(pCam + 372);

    CVector vecOffset;
    vecOffset.x = 0.35f;
    vecOffset.y = 0.1f;
    vecOffset.z = 0.1f;


    CVector vecOut;
    RwMatrixMultiplyByVector(&vecOut, &(pPed->m_HeadBoneMatrix), &vecOffset);

    if (vecOut.x != vecOut.x || vecOut.y != vecOut.y || vecOut.z != vecOut.z)
    {
        pPed->GetBonePosition(4, &vecOut);
    }
    if (vecOut.x != vecOut.x || vecOut.y != vecOut.y || vecOut.z != vecOut.z)
    {
        return;
    }

    pVec->x = vecOut.x;
    pVec->y = vecOut.y;
    pVec->z = vecOut.z;

    ((RwCamera*(*)(RwCamera*, float))(g_libGTASA + 0x7656B8))(Scene.m_pRwCamera, 0.2f);
}

CVector vecAtSaved;
CVector vecUpSaved;
bool bsaved = false;

void CFirstPersonCamera::ProcessCameraInVeh(uintptr_t pCam, CPlayerPed* pPed, CVehicle* pVeh)
{
    if (!m_bEnabled || !pPed->GetGtaVehicle())
    {
        return;
    }

    CVector* pVec = (CVector*)(pCam + 372);

    CVector vecOffset;
    vecOffset.x = 0.0f;
    vecOffset.y = 0.0f;
    vecOffset.z = 0.6f;

    uint16_t modelIndex = pPed->GetGtaVehicle()->m_nModelIndex;

    if (modelIndex == 581 || modelIndex == 509 || modelIndex == 481 || modelIndex == 462 || modelIndex == 521 || modelIndex == 463 || modelIndex == 510 ||
        modelIndex == 522 || modelIndex == 461 || modelIndex == 468 || modelIndex == 448 || modelIndex == 586)
    {
        vecOffset.x = 0.05f;
        vecOffset.y = 0.3f;
        vecOffset.z = 0.45f;
        ((RwCamera * (*)(RwCamera*, float))(g_libGTASA + 0x7656B8))(Scene.m_pRwCamera, 0.3f);
    }
    else
    {
        ((RwCamera * (*)(RwCamera*, float))(g_libGTASA + 0x7656B8))(Scene.m_pRwCamera, 0.01f);
    }

    CVector vecOut;
    RwMatrix mat;

    memcpy(&mat, pPed->m_pPed->m_matrix, sizeof(RwMatrix));
    //maybe pPed->m_pPed->GetMatrix().ToRwMatrix() ???????

    // mat is m_matrix!!!

    RwMatrixMultiplyByVector(&vecOut, &mat, &vecOffset);

    if (vecOut.x != vecOut.x || vecOut.y != vecOut.y || vecOut.z != vecOut.z)
    {
        pPed->GetBonePosition(4, &vecOut);
    }
    if (vecOut.x != vecOut.x || vecOut.y != vecOut.y || vecOut.z != vecOut.z)
    {
        return;
    }

    pVec->x = vecOut.x;
    pVec->y = vecOut.y;
    pVec->z = vecOut.z;

    //pPed->SetCurrentWeapon(0);

    if (!pVeh)
    {
        if (!pPed->IsAPassenger())
        {
            *(uint16_t*)(pCam + 14) = 16;
        }
        return;
    }
}

void CFirstPersonCamera::SetEnabled(bool bEnabled)
{
    m_bEnabled = bEnabled;
    //UnFuck(g_libGTASA + 0x0037C3B8);
    //*(float*)(g_libGTASA + 0x0037C3E0) = 1.0f;
    //*(float*)(g_libGTASA + 0x0037C3E4) = 1000.0f;
    //*(float*)(g_libGTASA + 0x0037C3E8) = 10.0f;
    //*(float*)(g_libGTASA + 0x0037C3D8) = 1000.0f;
    //*(float*)(g_libGTASA + 0x0037C3C4) = 1.0f;
    //*(float*)
}

void CFirstPersonCamera::Toggle()
{
    m_bEnabled ^= 1;
}

bool CFirstPersonCamera::IsEnabled()
{
    return m_bEnabled;
}
