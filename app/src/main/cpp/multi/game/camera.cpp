#include "../main.h"
#include "game.h"
#include "camera.h"
#include "game/CCAM/Cameras.h"

float* CCamera::m_f3rdPersonCHairMultX;
float* CCamera::m_f3rdPersonCHairMultY;

CCamera::CCamera()
{
	m_bLocked = 0;
	m_pEntity = nullptr;

	// TheCamera->m_CameraMatrix

    CCameras & TheCamera = *reinterpret_cast<CCameras*>(g_libGTASA + 0x9DF030);
    

	m_matPos = TheCamera.m_cameraMatrix;

	m_f3rdPersonCHairMultX = (float*)(g_libGTASA + 0x9DFDE8);
	m_f3rdPersonCHairMultY = (float*)(g_libGTASA + 0x9DFDEC);
}
// 0.3.7
void CCamera::SetBehindPlayer()
{
	ScriptCommand(&lock_camera_position, 0);
	ScriptCommand(&restore_camera_to_user);
	m_bLocked = 0;
	ScriptCommand(&set_camera_behind_player);
	ScriptCommand(&restore_camera_jumpcut);
}
// 0.3.7
void CCamera::SetPosition(float fX, float fY, float fZ, float fRotationX, float fRotationY, float fRotationZ)
{
	ScriptCommand(&restore_camera_to_user);
	m_bLocked = 0;
	m_pEntity = nullptr;
	ScriptCommand(&set_camera_position, fX, fY, fZ, fRotationX, fRotationY, fRotationZ);
}
// 0.3.7
void CCamera::LookAtPoint(float fX, float fY, float fZ, int iType)
{
	ScriptCommand(&restore_camera_to_user);
	m_bLocked = 0;
	m_pEntity = nullptr;
	ScriptCommand(&point_camera, fX, fY, fZ, iType);
}
// 0.3.7
void CCamera::Restore()
{
	m_bLocked = 0;
	m_pEntity = nullptr;
	ScriptCommand(&restore_camera_jumpcut);
}

// 0.3.7
void CCamera::InterpolateCameraPos(VECTOR *vecFrom, VECTOR *vecTo, int iTime, uint8_t byteMode)
{
	ScriptCommand(&restore_camera_to_user);
	ScriptCommand(&lock_camera_position, 1);
	ScriptCommand(&set_camera_pos_time_smooth,
		vecFrom->X, vecFrom->Y, vecFrom->Z,
		vecTo->X, vecTo->Y, vecTo->Z,
		iTime, byteMode);
}

// 0.3.7
void CCamera::InterpolateCameraLookAt(VECTOR *vecFrom, VECTOR *vecTo, int iTime, uint8_t byteMode)
{
	ScriptCommand(&lock_camera_target_point, 1);
	ScriptCommand(&point_camera_transverse,
		vecFrom->X, vecFrom->Y, vecFrom->Z,
		vecTo->X, vecTo->Y, vecTo->Z,
		iTime, byteMode);
}

// 0.3.7
void CCamera::GetMatrix(PMATRIX4X4 Matrix)
{
	Matrix->right.X = m_matPos.right.X;
	Matrix->right.Y = m_matPos.right.Y;
	Matrix->right.Z = m_matPos.right.Z;

	Matrix->up.X = m_matPos.up.X;
	Matrix->up.Y = m_matPos.up.Y;
	Matrix->up.Z = m_matPos.up.Z;

	Matrix->at.X = m_matPos.at.X;
	Matrix->at.Y = m_matPos.at.Y;
	Matrix->at.Z = m_matPos.at.Z;

	Matrix->pos.X = m_matPos.pos.X;
	Matrix->pos.Y = m_matPos.pos.Y;
	Matrix->pos.Z = m_matPos.pos.Z;
}

void CCamera::Update()
{
	if(m_pEntity)
	{
		MATRIX4X4 matEntity;
		m_pEntity->GetMatrix(&matEntity);
		if(matEntity.pos.X < 20000.0f && matEntity.pos.X > -20000.0f &&
			matEntity.pos.Y < 20000.0f && matEntity.pos.Y > -20000.0f &&
			matEntity.pos.Z < 100000.0f && matEntity.pos.Z > -10000.0f)
		{
			InterpolateCameraPos(&matEntity.pos, &matEntity.pos, 100.0f, 1);
		}
	}
}

void CCamera::AttachToEntity(CEntity *pEntity)
{
	m_pEntity = pEntity;

	Update();
}