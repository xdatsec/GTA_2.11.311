//
// Created by Dionisio on 5/15/2026.
//
#include "Cameras.h"
#include "../vendor/armhook/patch.h"
#include "../game/scripting.h"
void CCameras::Init() {
    CHook::CallFunction<void>(g_libGTASA + 0x53ECB4, this);
}
void CCameras::SetRwCamera(RwCamera *pCamera) {
    CHook::CallFunction<void>(g_libGTASA + 0x461E88, this, pCamera);
}


void CCameras::InjectHooks() {

}


bool CCameras::IsSphereVisible(const CVector* origin, float radius) {
    return CHook::CallFunction<bool>("_ZN7CCamera15IsSphereVisibleERK7CVectorf", this, origin, radius);
}



/*
void CCameras::TakeControl(CEntityGTA *target, eCamMode modeToGoTo, eSwitchType switchType, int32 whoIsInControlOfTheCamera) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x003E1714 + 1 : 0x4BF474), this, target, modeToGoTo, switchType, whoIsInControlOfTheCamera);
}

float CCameras::CalculateGroundHeight(eGroundHeightType type) {
    return CHook::CallFunction<float>(g_libGTASA + (VER_x32 ? 0x3DC5C8 + 1 : 0x4BA958), this, type);
}
 */

void CCameras::RestoreWithJumpCut() {
    CHook::CallFunction<void>(g_libGTASA + 0x45BC88, this);
}

void CCameras::SetBehindPlayer()
{
    ScriptCommand(&lock_camera_position, 0);
    ScriptCommand(&restore_camera_to_user);
    ScriptCommand(&set_camera_behind_player);
    ScriptCommand(&restore_camera_jumpcut);
}


void CCameras::SetPosition(float fX, float fY, float fZ, float fRotationX, float fRotationY, float fRotationZ)
{
    ScriptCommand(&restore_camera_to_user);

    ScriptCommand(&set_camera_position, fX, fY, fZ, fRotationX, fRotationY, fRotationZ);
}



void CCameras::LookAtPoint(float fX, float fY, float fZ, int iType)
{
    ScriptCommand(&restore_camera_to_user);
    ScriptCommand(&point_camera, fX, fY, fZ, iType);
}


void CCameras::InterpolateCameraPos(CVector *posFrom, CVector *posTo, int time, uint8_t mode)
{
    CCameras& TheCamera = *reinterpret_cast<CCameras*>(g_libGTASA + 0x9DF030);

    ScriptCommand(&restore_camera_to_user);
    ScriptCommand(&lock_camera_position1, 1);
    ScriptCommand(&set_camera_pos_time_smooth, posFrom->x, posFrom->y, posFrom->z, posTo->x, posTo->y, posTo->z, time, mode);
}


void CCameras::InterpolateCameraLookAt(CVector *posFrom, CVector *posTo, int time, uint8_t mode)
{
    ScriptCommand(&lock_camera_position, 1);
    ScriptCommand(&point_camera_transverse, posFrom->x, posFrom->y, posFrom->z, posTo->x, posTo->y, posTo->z, time, mode);
}