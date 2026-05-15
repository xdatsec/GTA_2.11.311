#pragma once

#include <cstdint>

class CEntity
{
public:
    CEntity() {};
    virtual ~CEntity() {};
    virtual void Add();
    virtual void Remove();

    bool IsAdded();

    void GetMatrix(PMATRIX4X4 Matrix);
    void SetMatrix(MATRIX4X4 Matrix);

    void GetMoveSpeedVector(PVECTOR Vector);
    void SetMoveSpeedVector(VECTOR Vector);

    void GetTurnSpeedVector(PVECTOR Vector);
    void SetTurnSpeedVector(VECTOR Vector);

    unsigned int GetModelIndex();
    void SetModelIndex(unsigned int uiModel);

    virtual void TeleportTo(float x, float y, float z);
    float GetDistanceFromLocalPlayerPed();
    float GetDistanceFromCamera();
    float GetDistanceFromPoint(VECTOR Vector);

    uintptr_t GetRWObject();

    void SetCollisionChecking(bool bCheck);
    bool GetCollisionChecking();

    void SetGravityProcessing(bool state);

    void UpdateMatrix(MATRIX4X4 mat);
    void UpdateRwMatrixAndFrame();

    void Render();

   // RpHAnimHierarchy* GetAnimHierarchyFromSkinClump();

    ENTITY_TYPE*	m_pEntity;
    uint32_t		m_dwGTAId;

    bool IsStationary();

    bool EnforceWorldBoundries(float fPX, float fZX, float fPY, float fNY);
    bool HasExceededWorldBoundries(float fPX, float fZX, float fPY, float fNY);

    void UpdateRpHAnim();
};