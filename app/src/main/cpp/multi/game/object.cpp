#include "../main.h"
#include "game.h"
#include "../net/netgame.h"

#include "RW/RenderWare.h"

#include <cmath>

extern CGame* pGame;
extern CNetGame* pNetGame;
extern MaterialTextGenerator* pMaterialTextGenerator;

CObject::CObject(int iModel, VECTOR vecPos, VECTOR vecRot, float fDrawDistance, uint8_t bAttached)
{
    m_AttachedVehicleID = INVALID_VEHICLE_ID;
    m_AttachedObjectID = INVALID_OBJECT_ID;
    m_bAttachedToPed = bAttached;

    m_pEntity = 0;
    m_dwGTAId = 0;

    m_vecAttachedPos.X = 0.0f;
    m_vecAttachedPos.Y = 0.0f;
    m_vecAttachedPos.Z = 0.0f;
    m_vecAttachedRot.X = 0.0f;
    m_vecAttachedRot.Y = 0.0f;
    m_vecAttachedRot.Z = 0.0f;
    m_bSyncRotation = true;

    if (!IsValidModel(iModel)) {
        iModel = 18631;
    }

    uint32_t dwRetID;
    ScriptCommand(&create_object, iModel, vecPos.X, vecPos.Y, vecPos.Z, &dwRetID);

    ENTITY_TYPE* pEntity = GamePool_Object_GetAt(dwRetID);

    if (dwRetID && pEntity)
    {
        m_pEntity = pEntity;
        m_dwGTAId = dwRetID;

        m_byteMoving = 0;
        m_fMoveSpeed = 0.0f;
        m_bNeedRotate = false;

        m_iModel = iModel;

        GetMatrix(&m_Matrix);
        m_Matrix.pos.X = vecPos.X;
        m_Matrix.pos.Y = vecPos.Y;
        m_Matrix.pos.Z = vecPos.Z;
        SetMatrix(m_Matrix);
        SetRotation(&vecRot);
    }
    memset(m_MaterialTexture, 0, sizeof(m_MaterialTexture));
    for (int i = 0; i < 16; i++)
    {
        //m_MaterialTexture[i] = 0;
        m_MaterialTextTexture[i] = 0;
        m_dwMaterialColor[i] = 0;
        m_iMaterialType[i] = 0;

        /* material text */
        m_szMaterialText[i] = nullptr;
        m_iMaterialSize[i] = 0;
        m_iMaterialFontSize[i] = 0;
        m_dwMaterialFontColor[i] = 0;
        m_dwMaterialBackColor[i] = 0;
        m_iMaterialTextAlign[i] = 0;
    }
    m_bHasMaterial = false;
    m_bHasMaterialText = false;

    m_bAttachedToPed = bAttached;

    m_bForceRender = false;
}

CObject::~CObject()
{
    ENTITY_TYPE* pEntity = GamePool_Object_GetAt(m_dwGTAId);
    m_pEntity = pEntity;

    if(pGame->GetCamera())
    {
        if(pGame->GetCamera()->GetAttachedEntity() == this)
            pGame->GetCamera()->AttachToEntity(0);
    }

    if (m_pEntity && m_pEntity->vtable != (g_libGTASA + /*0x5C7358*/0x808818)) /* CPlaceable */
    {
        ScriptCommand(&destroy_object, m_dwGTAId);
        if (GetModelRefCounts(m_iModel) == 0)
            pGame->RemoveModel(m_iModel, false);
    }

    for (int i = 0; i < 16; i++)
    {
        if (m_szMaterialText[i] != nullptr) {
            delete m_szMaterialText[i];
            m_szMaterialText[i] = nullptr;
        }
    }
}

void CObject::Process(float fElapsedTime)
{
    if (m_AttachedVehicleID != INVALID_VEHICLE_ID)
    {
        if (pNetGame)
        {
            CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
            if (pVehiclePool)
            {
                CVehicle* pVehicle = pVehiclePool->GetAt(m_AttachedVehicleID);
                if (pVehicle)
                {
                    if (pVehicle->IsAdded()) {
                        this->AttachToVehicle(pVehicle);
                    }
                }
            }
        }
        return;
    }

    if (m_AttachedObjectID != INVALID_OBJECT_ID)
    {
        if (pNetGame)
        {
            CObjectPool* pObjectPool = pNetGame->GetObjectPool();
            if (pObjectPool)
            {
                CObject* pObject = pObjectPool->GetAt(m_AttachedObjectID);
                if (pObject) {
                    this->AttachToObject(pObject);
                }
            }
        }

        return;
    }

    if (m_byteMoving & 1)
    {
        VECTOR vecSpeed = { 0.0f, 0.0f, 0.0f };
        MATRIX4X4 matEnt;
        GetMatrix(&matEnt);
        float distance = fElapsedTime * m_fMoveSpeed;
        float remaining = DistanceRemaining(&matEnt);
        uint32_t dwThisTick = GetTickCount();

        float posX = matEnt.pos.X;
        float posY = matEnt.pos.Y;
        float posZ = matEnt.pos.Z;

        float f1 = ((float)(dwThisTick - m_dwMoveTick)) * 0.001f * m_fMoveSpeed;
        float f2 = m_fDistanceToTargetPoint - remaining;

        if (distance >= remaining)
        {
            SetMoveSpeedVector(vecSpeed);
            SetTurnSpeedVector(vecSpeed);
            matEnt.pos.X = m_matTarget.pos.X;
            matEnt.pos.Y = m_matTarget.pos.Y;
            matEnt.pos.Z = m_matTarget.pos.Z;
            if (m_bNeedRotate) {
                m_quatTarget.GetAsMatrix(&matEnt);
            }
            UpdateMatrix(matEnt);
            StopMoving();
            return;
        }

        if (fElapsedTime <= 0.0f)
            return;

        float delta = 1.0f / (remaining / distance);
        matEnt.pos.X += ((m_matTarget.pos.X - matEnt.pos.X) * delta);
        matEnt.pos.Y += ((m_matTarget.pos.Y - matEnt.pos.Y) * delta);
        matEnt.pos.Z += ((m_matTarget.pos.Z - matEnt.pos.Z) * delta);

        distance = remaining / m_fDistanceToTargetPoint;
        float slerpDelta = 1.0f - distance;

        delta = 1.0f / fElapsedTime;
        vecSpeed.X = (matEnt.pos.X - posX) * delta * 0.02f;
        vecSpeed.Y = (matEnt.pos.Y - posY) * delta * 0.02f;
        vecSpeed.Z = (matEnt.pos.Z - posZ) * delta * 0.02f;

        if (FloatOffset(f1, f2) > 0.1f)
        {
            if (f1 > f2)
            {
                delta = (f1 - f2) * 0.1f + 1.0f;
                vecSpeed.X *= delta;
                vecSpeed.Y *= delta;
                vecSpeed.Z *= delta;
            }

            if (f2 > f1)
            {
                delta = 1.0f - (f2 - f1) * 0.1f;
                vecSpeed.X *= delta;
                vecSpeed.Y *= delta;
                vecSpeed.Z *= delta;
            }
        }

        SetMoveSpeedVector(vecSpeed);
        ApplyMoveSpeed();

        if (m_bNeedRotate)
        {
            float fx, fy, fz;
            GetRotation(&fx, &fy, &fz);
            distance = m_vecRotationTarget.Z - distance * m_vecSubRotationTarget.Z;
            vecSpeed.X = 0.0f;
            vecSpeed.Y = 0.0f;
            vecSpeed.Z = subAngle(remaining, distance) * 0.01f;
            if (vecSpeed.Z <= 0.001f)
            {
                if (vecSpeed.Z < -0.001f)
                    vecSpeed.Z = -0.001f;
            }
            else
            {
                vecSpeed.Z = 0.001f;
            }

            SetTurnSpeedVector(vecSpeed);
            GetMatrix(&matEnt);
            CQuaternion quat;
            quat.Slerp(&m_quatStart, &m_quatTarget, slerpDelta);
            quat.Normalize();
            quat.GetAsMatrix(&matEnt);
        }
        else
        {
            GetMatrix(&matEnt);
        }

        UpdateMatrix(matEnt);
    }
}

// 0.3.7
void CObject::SetRotation(VECTOR * vecRotation)
{
    if (m_pEntity && GamePool_Object_GetAt(m_dwGTAId))
    {
        ScriptCommand(&set_object_rotation, m_dwGTAId, vecRotation->X, vecRotation->Y, vecRotation->Z);
        m_vecRotation.X = vecRotation->X;
        m_vecRotation.Y = vecRotation->Y;
        m_vecRotation.Z = vecRotation->Z;
    }
}

void CObject::InstantRotate(float x, float y, float z)
{
    if (GamePool_Object_GetAt(m_dwGTAId))
    {
        LOGI("InstantRotate: x: %f, y: %f, z: %f", x, y, z);
        ScriptCommand(&set_object_rotation, m_dwGTAId, x, y, z);
    }
}

// 0.3.7
void CObject::GetRotation(float* pfX, float* pfY, float* pfZ)
{
    if (m_pEntity) {
        MATRIX4X4* mat = m_pEntity->mat;

        if (mat) {
            // CMatrix::ConvertToEulerAngles
            ((void (*)(PMATRIX4X4, float*, float*, float*, int))(g_libGTASA + 0x510CB4))(mat, pfX, pfY, pfZ, 21);
        }

        *pfX = *pfX * 57.295776 * -1.0;
        *pfY = *pfY * 57.295776 * -1.0;
        *pfZ = *pfZ * 57.295776 * -1.0;
    }
}
// 0.3.7
void CObject::RotateMatrix(VECTOR vecRot)
{
    m_vecRotation = vecRot;

    vecRot.X *= 0.017453292f; // x * pi/180
    vecRot.Y *= 0.017453292f; // y * pi/180
    vecRot.Z *= 0.017453292f; // z * pi/180

    float cosx = cos(vecRot.X);
    float sinx = sin(vecRot.X);

    float cosy = cos(vecRot.Y);
    float siny = sin(vecRot.Y);

    float cosz = cos(vecRot.Z);
    float sinz = sin(vecRot.Z);

    float sinzx = sinz * sinx;
    float coszx = cosz * sinx;

    m_matTarget.right.X = cosz * cosy - sinzx * siny;
    m_matTarget.right.Y = coszx * siny + sinz * cosy;
    m_matTarget.right.Z = -(siny * cosx);
    m_matTarget.up.X = -(sinz * cosx);
    m_matTarget.up.Y = cosz * cosx;
    m_matTarget.up.Z = sinx;
    m_matTarget.at.X = sinzx * cosy + cosz * siny;
    m_matTarget.at.Y = sinz * siny - coszx * cosy;
    m_matTarget.at.Z = cosy * cosx;
}
// 0.3.7
void CObject::ApplyMoveSpeed()
{
    if (m_pEntity)
    {
        float fTimeStep = *(float*)(g_libGTASA + 0xA00CA4);

        MATRIX4X4 mat;
        GetMatrix(&mat);
        mat.pos.X += fTimeStep * m_pEntity->vecMoveSpeed.X;
        mat.pos.Y += fTimeStep * m_pEntity->vecMoveSpeed.Y;
        mat.pos.Z += fTimeStep * m_pEntity->vecMoveSpeed.Z;
        UpdateMatrix(mat);
    }
}
// 0.3.7
float CObject::DistanceRemaining(MATRIX4X4* matPos)
{
    float	fSX, fSY, fSZ;
    fSX = (matPos->pos.X - m_matTarget.pos.X) * (matPos->pos.X - m_matTarget.pos.X);
    fSY = (matPos->pos.Y - m_matTarget.pos.Y) * (matPos->pos.Y - m_matTarget.pos.Y);
    fSZ = (matPos->pos.Z - m_matTarget.pos.Z) * (matPos->pos.Z - m_matTarget.pos.Z);
    return (float)sqrt(fSX + fSY + fSZ);
}

void CObject::SetMaterial(int iModel, int iMaterialIndex, char* txdname, char* texturename, uint32_t dwColor)
{
    FLog("SetMaterial: model: %d, %s, %s", iModel, txdname, texturename);
    /*
    int iTryCount = 0;

    if (iMaterialIndex < 16)
    {
        if (m_MaterialTexture[iMaterialIndex]) {
            DeleteRwTexture(m_MaterialTexture[iMaterialIndex]);
            m_MaterialTexture[iMaterialIndex] = 0;
        }

        m_MaterialTexture[iMaterialIndex] = (uintptr_t)LoadTextureFromTxd(txdname, texturename);
        m_dwMaterialColor[iMaterialIndex] = dwColor;
        m_iMaterialType[iMaterialIndex] = MATERIAL_TYPE_MATERIAL;
        m_bHasMaterial = true;
        return;
    }
     */
}

void CObject::SetMaterialText(int index, char* text, int materialSize, char* fontname, int fontSize, bool bold,
                              uint32_t dwFontColor, uint32_t dwBackColor, int textAlignment)
{
    if (index > 16) return;

    if (m_MaterialTextTexture[index]) {
        DeleteRwTexture(m_MaterialTextTexture[index]);
        m_MaterialTextTexture[index] = 0;
    }

    m_MaterialTextIndex = index;

    m_dwMaterialColor[index] = 0;
    m_iMaterialType[index] = MATERIAL_TYPE_TEXT;

    if (m_szMaterialText[index] != nullptr) {
        delete[] m_szMaterialText[index];
        m_szMaterialText[index] = nullptr;
    }

    m_szMaterialText[index] = new char[2048];
    strcpy(m_szMaterialText[index], text);

    m_iMaterialSize[index] = materialSize;
    m_iMaterialFontSize[index] = fontSize;
    m_dwMaterialFontColor[index] = dwFontColor;
    m_dwMaterialBackColor[index] = dwBackColor;
    m_iMaterialTextAlign[index] = textAlignment;
}

void CObject::ProcessMaterialText()
{
    for (int i = 0; i < 16; i++)
    {
        if (m_iMaterialType[i] == MATERIAL_TYPE_TEXT && m_MaterialTextTexture[i] == 0)
        {
            m_iMaterialFontSize[i]*=0.75f;
            m_MaterialTextTexture[i] = pMaterialTextGenerator->Generate(m_szMaterialText[i], m_iMaterialSize[i], m_iMaterialFontSize[i],
                                                                        false, m_dwMaterialFontColor[i], m_dwMaterialBackColor[i], m_iMaterialTextAlign[i]);
            m_bHasMaterialText = true;
        }
    }
}

// 0.3.7
void CObject::MoveTo(float fX, float fY, float fZ, float fSpeed, float fRotX, float fRotY, float fRotZ)
{
    MATRIX4X4 mat;
    this->GetMatrix(&mat);

    if (m_byteMoving & 1) {
        this->StopMoving();
        mat.pos.X = m_matTarget.pos.X;
        mat.pos.Y = m_matTarget.pos.Y;
        mat.pos.Z = m_matTarget.pos.Z;

        if (m_bNeedRotate) {
            m_quatTarget.GetAsMatrix(&mat);
        }

        this->UpdateMatrix(mat);
    }

    m_dwMoveTick = GetTickCount();
    m_fMoveSpeed = fSpeed;
    m_matTarget.pos.X = fX;
    m_matTarget.pos.Y = fY;
    m_matTarget.pos.Z = fZ;
    m_byteMoving |= 1;

    if (fRotX <= -999.0f || fRotY <= -999.0f || fRotZ <= -999.0f) {
        m_bNeedRotate = false;
    }
    else
    {
        m_bNeedRotate = true;

        VECTOR vecRot;
        MATRIX4X4 matrix;
        this->GetRotation(&vecRot.X, &vecRot.Y, &vecRot.Z);
        m_vecRotationTarget.X = fixAngle(fRotX);
        m_vecRotationTarget.Y = fixAngle(fRotY);
        m_vecRotationTarget.Z = fixAngle(fRotZ);

        m_vecSubRotationTarget.X = subAngle(vecRot.X, fRotX);
        m_vecSubRotationTarget.Y = subAngle(vecRot.Y, fRotY);
        m_vecSubRotationTarget.Z = subAngle(vecRot.Z, fRotZ);

        this->RotateMatrix(VECTOR{ fRotX, fRotY, fRotZ });
        this->GetMatrix(&matrix);
        m_quatStart.SetFromMatrix(matrix);
        m_quatTarget.SetFromMatrix(m_matTarget);
        m_quatStart.Normalize();
        m_quatTarget.Normalize();
    }

    m_fDistanceToTargetPoint = this->GetDistanceFromPoint(m_matTarget.pos);

    if (pNetGame) {
        CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
        if (pPlayerPool) {
            //pPlayerPool->GetLocalPlayer()->UpdateSurfing();
        }
    }

    // sub_1009F070
    m_pEntity->flags &= 0xFFFFFFF7;
}
// 0.3.7
void CObject::StopMoving()
{
    VECTOR vec = { 0.0f, 0.0f, 0.0f };
    this->SetMoveSpeedVector(vec);
    this->SetTurnSpeedVector(vec);

    m_byteMoving &= ~1;
}

// 0.3.7
void CObject::SetAttachedObject(uint16_t ObjectID, VECTOR* vecPos, VECTOR* vecRot, bool bSyncRotation)
{
    if (ObjectID == INVALID_OBJECT_ID)
    {
        m_AttachedObjectID = INVALID_OBJECT_ID;
        m_vecAttachedPos.X = 0.0f;
        m_vecAttachedPos.Y = 0.0f;
        m_vecAttachedPos.Z = 0.0f;
        m_vecAttachedRot.X = 0.0f;
        m_vecAttachedRot.Y = 0.0f;
        m_vecAttachedRot.Z = 0.0f;
        m_bSyncRotation = false;
    }
    else
    {
        m_AttachedObjectID = ObjectID;
        m_vecAttachedPos.X = vecPos->X;
        m_vecAttachedPos.Y = vecPos->Y;
        m_vecAttachedPos.Z = vecPos->Z;
        m_vecAttachedRot.X = vecRot->X;
        m_vecAttachedRot.Y = vecRot->Y;
        m_vecAttachedRot.Z = vecRot->Z;
        m_bSyncRotation = bSyncRotation;
    }
}
// 0.3.7
void CObject::SetAttachedVehicle(uint16_t VehicleID, VECTOR* vecPos, VECTOR* vecRot)
{
    if (VehicleID == INVALID_VEHICLE_ID)
    {
        m_AttachedVehicleID = INVALID_VEHICLE_ID;
        m_vecAttachedPos.X = 0.0f;
        m_vecAttachedPos.Y = 0.0f;
        m_vecAttachedPos.Z = 0.0f;
        m_vecAttachedRot.X = 0.0f;
        m_vecAttachedRot.Y = 0.0f;
        m_vecAttachedRot.Z = 0.0f;
    }
    else
    {
        m_AttachedVehicleID = VehicleID;
        m_vecAttachedPos.X = vecPos->X;
        m_vecAttachedPos.Y = vecPos->Y;
        m_vecAttachedPos.Z = vecPos->Z;
        m_vecAttachedRot.X = vecRot->X;
        m_vecAttachedRot.Y = vecRot->Y;
        m_vecAttachedRot.Z = vecRot->Z;
    }
}
// 0.3.7
void CObject::AttachToVehicle(CVehicle* pVehicle)
{
    if (!ScriptCommand(&is_object_attached, m_dwGTAId)) {
        ScriptCommand(&attach_object_to_car,
                      m_dwGTAId,
                      pVehicle->m_dwGTAId,
                      m_vecAttachedPos.X,
                      m_vecAttachedPos.Y,
                      m_vecAttachedPos.Z,
                      m_vecAttachedRot.X,
                      m_vecAttachedRot.Y,
                      m_vecAttachedRot.Z);
    }
}
// 0.3.7
void CObject::AttachToObject(CObject* pObject)
{
    if (!ScriptCommand(&is_object_attached, m_dwGTAId)) {
        ScriptCommand(&attach_object_to_object,
                      m_dwGTAId,
                      pObject->m_dwGTAId,
                      m_vecAttachedPos.X,
                      m_vecAttachedPos.Y,
                      m_vecAttachedPos.Z,
                      m_vecAttachedRot.X,
                      m_vecAttachedRot.Y,
                      m_vecAttachedRot.Z);
    }
}

bool CObject::AttachedToMovingEntity()
{
    if(m_AttachedObjectID == INVALID_OBJECT_ID)
    {
        if(m_AttachedVehicleID != INVALID_VEHICLE_ID)
            return true;

        return (m_byteMoving & 1);
    }
    else
    {
        if(m_AttachedObjectID >= 0 && m_AttachedObjectID < MAX_OBJECTS)
        {
            if(pNetGame)
            {
                CObjectPool *pObjectPool = pNetGame->GetObjectPool();
                if(pObjectPool)
                {
                    CObject *pObject = pObjectPool->GetAt(m_AttachedObjectID);
                    if(pObject) return (pObject->m_byteMoving & 1);
                }
            }
        }
    }

    return false;
}

void CObject::TeleportTo(float fX, float fY, float fZ) {

    /*if (fX > 3000.0f || fX < -3000.0f ||
        fY > 3000.0f || fY < -3000.0f) {
        m_bForceRender = true;
    }
    else {
        m_bForceRender = false;
    }*/

    int v4 = 0;
    int v5 = 0;
    if ( fX < -3000.0 )
        v4 = 1;
    if ( fX > 3000.0 )
        v5 = 1;

    m_bForceRender = (fY > 3000.0) | v4 | v5 | (fY < -3000.0);

    CEntity::TeleportTo(fX, fY, fZ);
}