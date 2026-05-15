#pragma once
#include "entity.h"

class CCamera
{
public:
	CCamera();
	~CCamera() {};

	void SetBehindPlayer();
	void SetPosition(float fX, float fY, float fZ, float fRotationX, float fRotationY, float fRotationZ);
	void LookAtPoint(float fX, float fY, float fZ, int iType);
	void Restore();
	void InterpolateCameraPos(VECTOR *vecFrom, VECTOR *vecTo, int iTime, uint8_t byteMode);
	void InterpolateCameraLookAt(VECTOR *vecFrom, VECTOR *vecTo, int iTime, uint8_t byteMode);

	void GetMatrix(PMATRIX4X4 Matrix);

	void AttachToEntity(CEntity* pEntity);
	void Update();

	CEntity *GetAttachedEntity() {return m_pEntity;};

public:
	int m_bLocked;
	MATRIX4X4 m_matPos;
	CEntity *m_pEntity;

	static float* m_f3rdPersonCHairMultX;
	static float* m_f3rdPersonCHairMultY;
};