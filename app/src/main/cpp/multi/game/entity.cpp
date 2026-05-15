#include "../main.h"
#include "game.h"
#include "../net/netgame.h"
#include <cmath>

extern CGame* pGame;
extern CNetGame *pNetGame;

// 0.3.7
void CEntity::GetMatrix(PMATRIX4X4 Matrix)
{
    if (!m_pEntity || !m_pEntity->mat) return;

    Matrix->right.X = m_pEntity->mat->right.X;
    Matrix->right.Y = m_pEntity->mat->right.Y;
    Matrix->right.Z = m_pEntity->mat->right.Z;

    Matrix->up.X = m_pEntity->mat->up.X;
    Matrix->up.Y = m_pEntity->mat->up.Y;
    Matrix->up.Z = m_pEntity->mat->up.Z;

    Matrix->at.X = m_pEntity->mat->at.X;
    Matrix->at.Y = m_pEntity->mat->at.Y;
    Matrix->at.Z = m_pEntity->mat->at.Z;

    Matrix->pos.X = m_pEntity->mat->pos.X;
    Matrix->pos.Y = m_pEntity->mat->pos.Y;
    Matrix->pos.Z = m_pEntity->mat->pos.Z;
}
// 0.3.7
void CEntity::SetMatrix(MATRIX4X4 Matrix)
{
    if (!m_pEntity || !m_pEntity->mat) return;

    m_pEntity->mat->right.X = Matrix.right.X;
    m_pEntity->mat->right.Y = Matrix.right.Y;
    m_pEntity->mat->right.Z = Matrix.right.Z;

    m_pEntity->mat->up.X = Matrix.up.X;
    m_pEntity->mat->up.Y = Matrix.up.Y;
    m_pEntity->mat->up.Z = Matrix.up.Z;

    m_pEntity->mat->at.X = Matrix.at.X;
    m_pEntity->mat->at.Y = Matrix.at.Y;
    m_pEntity->mat->at.Z = Matrix.at.Z;

    m_pEntity->mat->pos.X = Matrix.pos.X;
    m_pEntity->mat->pos.Y = Matrix.pos.Y;
    m_pEntity->mat->pos.Z = Matrix.pos.Z;
}
// 0.3.7
void CEntity::GetMoveSpeedVector(PVECTOR Vector)
{
    Vector->X = m_pEntity->vecMoveSpeed.X;
    Vector->Y = m_pEntity->vecMoveSpeed.Y;
    Vector->Z = m_pEntity->vecMoveSpeed.Z;
}
// 0.3.7
void CEntity::SetMoveSpeedVector(VECTOR Vector)
{
    m_pEntity->vecMoveSpeed.X = Vector.X;
    m_pEntity->vecMoveSpeed.Y = Vector.Y;
    m_pEntity->vecMoveSpeed.Z = Vector.Z;
}
// 0.3.7
void CEntity::GetTurnSpeedVector(PVECTOR Vector)
{
    Vector->X = m_pEntity->vecTurnSpeed.X;
    Vector->Y = m_pEntity->vecTurnSpeed.Y;
    Vector->Z = m_pEntity->vecTurnSpeed.Z;
}
// 0.3.7
void CEntity::SetTurnSpeedVector(VECTOR Vector)
{
    m_pEntity->vecTurnSpeed.X = Vector.X;
    m_pEntity->vecTurnSpeed.Y = Vector.Y;
    m_pEntity->vecTurnSpeed.Z = Vector.Z;
}
// 0.3.7
uint CEntity::GetModelIndex()
{
    return m_pEntity->nModelIndex;
}
// 0.3.7
void CEntity::SetModelIndex(uint uiModel)
{
    if (!m_pEntity) return;

    int iTryCount = 0;
    if (!pGame->IsModelLoaded(uiModel) && !GetModelRWObject(uiModel))
    {
        pGame->RequestModel(uiModel);
        pGame->LoadRequestedModels();
        while (!pGame->IsModelLoaded(uiModel))
        {
            sleep(1);
            if (iTryCount > 200)
            {
                //if (gui) gui->chat()->addDebugMessage("Warning: Model %u wouldn't load in time!", uiModel);
                return;
            }

            iTryCount++;
        }
    }


    // CEntity::DeleteRWObject
    ((void (*)(ENTITY_TYPE*))(*(void**)(m_pEntity->vtable + 72)))(m_pEntity);
    m_pEntity->nModelIndex = uiModel;
    // CEntity::SetModelIndex
    ((void (*)(ENTITY_TYPE*, unsigned int))(*(void**)(m_pEntity->vtable + 48)))(m_pEntity, uiModel);
}
// 0.3.7
void CEntity::TeleportTo(float fX, float fY, float fZ)
{
    MATRIX4X4 mat;

    if (m_pEntity && m_pEntity->vtable != (g_libGTASA + /*0x5C7358*/0x808818)) /* CPlaceable */
    {
        uint16_t modelIndex = m_pEntity->nModelIndex;
        if (modelIndex != TRAIN_PASSENGER_LOCO &&
            modelIndex != TRAIN_FREIGHT_LOCO &&
            modelIndex != TRAIN_TRAM)
            //((void(*)(ENTITY_TYPE*, float, float, float, bool))(*(void**)(m_pEntity->vtable + 0x3C)))(m_pEntity, fX, fY, fZ, 0);
        {
            ((void(*)(ENTITY_TYPE*, float, float, float))(*(void**)(m_pEntity->vtable + 0x3C*2)))(m_pEntity, fX, fY, fZ);
        }
        else
            ScriptCommand(&put_train_at, m_dwGTAId, fX, fY, fZ);
    }
}
// 0.3.7
bool CEntity::IsAdded()
{
    if (m_pEntity)
    {
        if (m_pEntity->vtable == (g_libGTASA + /*0x5C7358*/0x808818)) // CPlaceable
            return false;

        if (m_pEntity->dwUnkModelRel)
            return true;
    }

    return false;
}
// 0.3.7
void CEntity::Add()
{
    if (!m_pEntity || m_pEntity->vtable == (g_libGTASA + /*0x5C7358*/0x808818)) { // CPlaceable
        return;
    }

    if (!m_pEntity->dwUnkModelRel) {

        VECTOR vec = { 0.0f, 0.0f, 0.0f };

        SetMoveSpeedVector(vec);
        SetTurnSpeedVector(vec);

        // CWorld::Add
        ((void(*)(ENTITY_TYPE*))(g_libGTASA + /*0x3C14B0*/0x4AAD34))(m_pEntity);

        MATRIX4X4 mat;
        GetMatrix(&mat);
        TeleportTo(mat.pos.X, mat.pos.Y, mat.pos.Z);
    }
}
// 0.3.7
void CEntity::Remove()
{
    if (!m_pEntity || m_pEntity->vtable == (g_libGTASA + /*0x5C7358*/0x808818)) { // CPlaceable
        return;
    }

    if (m_pEntity->dwUnkModelRel) {
        // CWorld::Remove
        ((void(*)(ENTITY_TYPE*))(g_libGTASA + /*0x3C1500*/0x4AAB00))(m_pEntity);
    }
}

float CEntity::GetDistanceFromCamera()
{
    MATRIX4X4 matEnt;
    if (!m_pEntity || m_pEntity->vtable == 0x808818) // CPlaceable
        return 100000.0f;

    GetMatrix(&matEnt);
    ENTITY_TYPE & TheCamera = *reinterpret_cast<ENTITY_TYPE*>(g_libGTASA + 0x9DF030);

    float tmpX1 = TheCamera.mat->pos.X;
    float tmpY1 = TheCamera.mat->pos.Y;
    float tmpZ1 = TheCamera.mat->pos.Z;

    float tmpX = (matEnt.pos.X - tmpX1);
    float tmpY = (matEnt.pos.Y - tmpY1);
    float tmpZ = (matEnt.pos.Z - tmpZ1);

    return sqrt(tmpX * tmpX + tmpY * tmpY + tmpZ * tmpZ);
}

// 0.3.7
float CEntity::GetDistanceFromPoint(VECTOR Vector)
{
    MATRIX4X4 mat;
    GetMatrix(&mat);

    float tmpX = (mat.pos.X - Vector.X) * (mat.pos.X - Vector.X);
    float tmpY = (mat.pos.Y - Vector.Y) * (mat.pos.Y - Vector.Y);
    float tmpZ = (mat.pos.Z - Vector.Z) * (mat.pos.Z - Vector.Z);

    return (float)sqrt(tmpX + tmpY + tmpZ);
}

// 0.3.7
uintptr_t CEntity::GetRWObject()
{
    if (m_pEntity)
        return m_pEntity->m_pRwObject;

    return 0;
}
// 0.3.7
float CEntity::GetDistanceFromLocalPlayerPed()
{
    MATRIX4X4	matFromPlayer;
    MATRIX4X4	matThis;
    float		fSX, fSY, fSZ;

    CPlayerPed* pLocalPlayerPed = pGame->FindPlayerPed();
    CLocalPlayer* pLocalPlayer = nullptr;

    if (!pLocalPlayerPed) return 10000.0f;
    if (!m_pEntity) return 10000.0f;

    GetMatrix(&matThis);

    if (pNetGame) {
        pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
        if (pLocalPlayer && (pLocalPlayer->IsSpectating() || pLocalPlayer->IsInRCMode())) {
            pGame->GetCamera()->GetMatrix(&matFromPlayer);
        }
        else {
            pLocalPlayerPed->GetMatrix(&matFromPlayer);
        }
    }
    else {
        pLocalPlayerPed->GetMatrix(&matFromPlayer);
    }

    fSX = (matThis.pos.X - matFromPlayer.pos.X) * (matThis.pos.X - matFromPlayer.pos.X);
    fSY = (matThis.pos.Y - matFromPlayer.pos.Y) * (matThis.pos.Y - matFromPlayer.pos.Y);
    fSZ = (matThis.pos.Z - matFromPlayer.pos.Z) * (matThis.pos.Z - matFromPlayer.pos.Z);

    return (float)sqrt(fSX + fSY + fSZ);
}

void CEntity::SetCollisionChecking(bool bCheck)
{
    if (m_pEntity && m_pEntity->vtable != (g_libGTASA + /*0x5C7358*/0x808818))
    {
        if (bCheck)
            m_pEntity->dwProcessingFlags |= 1;
        else
            m_pEntity->dwProcessingFlags &= 0xFFFFFFFE;
    }
}

bool CEntity::GetCollisionChecking()
{
    if (m_pEntity && m_pEntity->vtable != (g_libGTASA + /*0x5C7358*/0x808818))
        return m_pEntity->dwProcessingFlags & 1;

    return true;
}

void CEntity::SetGravityProcessing(bool state)
{
    if (m_pEntity && m_pEntity->vtable != (g_libGTASA + /*0x5C7358*/0x808818))
    {
        if (state)
            m_pEntity->dwProcessingFlags &= 0x7FFFFFFD;
        else
            m_pEntity->dwProcessingFlags |= 0x80000002;
    }
}
// 0.3.7
void CEntity::UpdateMatrix(MATRIX4X4 matrix)
{
    if (m_pEntity && m_pEntity->mat)
    {
        // CPhysical::Remove
        ((void (*)(ENTITY_TYPE*))(*(uintptr_t*)(m_pEntity->vtable + 0x20)))(m_pEntity);

        this->SetMatrix(matrix);
        this->UpdateRwMatrixAndFrame();

        // CPhysical::Add
        ((void (*)(ENTITY_TYPE*))(*(uintptr_t*)(m_pEntity->vtable + 0x10)))(m_pEntity);
    }
}

// 0.3.7
void CEntity::UpdateRwMatrixAndFrame()
{
    if (m_pEntity && m_pEntity->vtable != (g_libGTASA + 0x808818)) // CPlaceable
    {
        if (m_pEntity->m_pRwObject)
        {
            if (m_pEntity->mat)
            {
                uintptr_t pRwMatrix = *(uintptr_t*)(m_pEntity->m_pRwObject + 8) + 0x20;

                // CMatrix::UpdateRwMatrix
                ((void (*)(MATRIX4X4*, uintptr_t))(g_libGTASA + 0x537688))(m_pEntity->mat, pRwMatrix);

                // CEntity::UpdateRwFrame
                ((void (*)(ENTITY_TYPE*))(g_libGTASA + 0x4CBAEC))(m_pEntity);
            }
        }
    }
}
// 0.3.7
/*
void CEntity::Render()
{
	if(!m_pEntity && IsGameEntityArePlaceable(m_pEntity))
		return;

	uintptr_t pRwObject = GetRWObject();

	int iModel = GetModelIndex();
	if (iModel >= 400 && iModel <= 611 && pRwObject) {
		// CVisibilityPlugins::SetupVehicleVariables
		((void (*)(uintptr_t))(g_libGTASA + 0x5D4B40 + 1))(pRwObject);
	}

	// CEntity::PreRender
	((void (*)(ENTITY_TYPE*))(*(void**)(m_pEntity->vtable + 0x48)))(m_pEntity);

	if (pRwObject) {
		// CRenderer::RenderOneNonRoad
		((void (*)(ENTITY_TYPE*))(g_libGTASA + 0x4102BC + 1))(m_pEntity);
	}
}
*/
/*
RpHAnimHierarchy* CEntity::GetAnimHierarchyFromSkinClump()
{
	return ((RpHAnimHierarchy* (*)(uint32_t))(g_libGTASA+0x5D1021))(*(uint32_t *)(*((uint32_t *)this+1) + 24));
}
*/

void CEntity::UpdateRpHAnim()
{
    if(!m_pEntity) return;

    ((void (*)(uint32_t))(g_libGTASA + 0x4CBB00))(*((uint32_t *)this));
}

bool CEntity::IsStationary()
{
    if(!IsAdded()) return false; // movespeed vectors are invalid if its not added

    if(m_pEntity->vecMoveSpeed.X == 0.0f &&
       m_pEntity->vecMoveSpeed.Y == 0.0f &&
       m_pEntity->vecMoveSpeed.Z == 0.0f)
    {
        return true;
    }
    return false;
}

bool CEntity::EnforceWorldBoundries(float fPX, float fZX, float fPY, float fNY)
{
    if(!m_pEntity) return false;

    MATRIX4X4 matWorld;
    VECTOR vecMoveSpeed;
    GetMatrix(&matWorld);
    GetMoveSpeedVector(&vecMoveSpeed);

    if(matWorld.pos.X > fPX)
    {
        if(vecMoveSpeed.X != 0.0f)
        {
            vecMoveSpeed.X = -0.2f;
            vecMoveSpeed.Z = 0.1f;
        }

        SetMoveSpeedVector(vecMoveSpeed);
        matWorld.pos.Z += 0.04f;
        SetMatrix(matWorld);
        return true;
    }
    else if(matWorld.pos.X < fZX)
    {
        if(vecMoveSpeed.X != 0.0f)
        {
            vecMoveSpeed.X = 0.2f;
            vecMoveSpeed.Z = 0.1f;
        }

        SetMoveSpeedVector(vecMoveSpeed);
        matWorld.pos.Z += 0.04f;
        SetMatrix(matWorld);
        return true;
    }
    else if(matWorld.pos.Y > fPY)
    {
        if(vecMoveSpeed.Y != 0.0f)
        {
            vecMoveSpeed.Y = -0.2f;
            vecMoveSpeed.Z = 0.1f;
        }

        SetMoveSpeedVector(vecMoveSpeed);
        matWorld.pos.Z += 0.04f;
        SetMatrix(matWorld);
        return true;
    }
    else if(matWorld.pos.Y < fNY)
    {
        if(vecMoveSpeed.Y != 0.0f)
        {
            vecMoveSpeed.Y = 0.2f;
            vecMoveSpeed.Z = 0.1f;
        }

        SetMoveSpeedVector(vecMoveSpeed);
        matWorld.pos.Z += 0.04f;
        SetMatrix(matWorld);
        return true;
    }

    return false;
}

bool CEntity::HasExceededWorldBoundries(float fPX, float fZX, float fPY, float fNY)
{
    if(m_pEntity)
    {
        MATRIX4X4 matWorld;
        GetMatrix(&matWorld);

        if(matWorld.pos.X > fPX || matWorld.pos.X < fZX || matWorld.pos.Y > fPY || matWorld.pos.Y < fNY)
            return true;
    }

    return false;
}