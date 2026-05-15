#pragma once

#include "../vendor/quaternion/quaternion.h"

#define MATERIAL_TYPE_MATERIAL	1
#define MATERIAL_TYPE_TEXT		2
struct MaterialInfo
{
    uint8_t m_bCreated;
    uint16_t wModelID;
    uint32_t dwColor;

    uint32_t oldFlags;
    RwTexture* pTex;
};
class CObject : public CEntity
{
public:
    CObject(int iModel, VECTOR vecPos, VECTOR vecRot, float fDrawDistance, uint8_t unk);
    virtual ~CObject();

    void Process(float fElapsedTime);

    virtual void TeleportTo(float fX, float fY, float fZ) override;

    void SetRotation(VECTOR *vecRotation);
    void GetRotation(float* pfX, float* pfY, float* pfZ);
    void RotateMatrix(VECTOR vecRot);
    void ApplyMoveSpeed();
    float DistanceRemaining(MATRIX4X4* matPos);

    void SetMaterial(int iModel, int iIndex, char* txdname, char* texturename, uint32_t dwColor);
    void SetMaterialText(int iIndex, char* text, int materialSize, char* fontname, int fontSize, bool bold,
                         uint32_t dwFontColor, uint32_t dwBackColor, int textAlignment);

    void MoveTo(float fX, float fY, float fZ, float fSpeed, float fRotX, float fRotY, float fRotZ);
    void StopMoving();

    void SetAttachedObject(uint16_t ObjectID, VECTOR* vecPos, VECTOR* vecRot, bool bSyncRotation);
    void SetAttachedVehicle(uint16_t VehicleID, VECTOR* vecPos, VECTOR* vecRot);

    void AttachToVehicle(CVehicle* pVehicle);
    void AttachToObject(CObject* pObject);

    void ProcessMaterialText();

    bool AttachedToMovingEntity();

    /* attach */
    uint16_t	m_AttachedVehicleID;
    uint16_t	m_AttachedObjectID;
    VECTOR		m_vecAttachedPos;
    VECTOR		m_vecAttachedRot;
    bool		m_bSyncRotation;

    int	m_MaterialTextIndex;
    MaterialInfo        m_MaterialTexture[16];
    uint32_t	m_dwMaterialColor[16];
    uintptr_t	m_MaterialTextTexture[16];
    int			m_iMaterialType[16];
    bool		m_bHasMaterial;
    bool		m_bHasMaterialText;
    /* materialText */
    char* m_szMaterialText[16];
    int			m_iMaterialSize[16];
    int			m_iMaterialFontSize[16];
    uint32_t	m_dwMaterialFontColor[16];
    uint32_t	m_dwMaterialBackColor[16];
    int			m_iMaterialTextAlign[16];

    bool		m_bAttachedToPed;
    bool		m_bForceRender;

    uint8_t		m_byteMoving;

    void InstantRotate(float x, float y, float z);

private:
    MATRIX4X4	m_Matrix;
    VECTOR		m_vecRotation;
    int			m_iModel;

    float		m_fMoveSpeed;
    bool		m_bNeedRotate;
    MATRIX4X4	m_matTarget;
    CQuaternion m_quatStart;
    CQuaternion m_quatTarget;
    VECTOR		m_vecRotationTarget;
    VECTOR		m_vecSubRotationTarget;
    float		m_fDistanceToTargetPoint;
    uint32_t	m_dwMoveTick;

};