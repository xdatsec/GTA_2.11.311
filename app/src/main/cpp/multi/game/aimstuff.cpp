#include "../main.h"
#include "game.h"
#include "net/netgame.h"
#include <cmath>
#include "/CCAM/Cameras.h"
CAMERA_AIM* pcaInternalAim = nullptr;
uint8_t* pbyteCameraMode = nullptr;
float* pfCameraExtZoom = nullptr;
float* pfAspectRatio = nullptr;
uint16_t* wCameraMode2 = nullptr;
float *pPlayerStats = nullptr;

uint8_t* pbyteCurrentPlayer = nullptr;

CAMERA_AIM caLocalPlayerAim;
CAMERA_AIM caRemotePlayerAim[MAX_PLAYERS];
uint8_t byteCameraMode[MAX_PLAYERS];
float fCameraExtZoom[MAX_PLAYERS];
float fCameraAspectRatio[MAX_PLAYERS];
float fLocalCameraExtZoom;
float fLocalAspectRatio;

float fRemoteSkillLevel[MAX_PLAYERS][11];
float fLocalSkillLevel[11];

// 0.3.7
CAMERA_AIM* GameGetInternalAim()
{
	// 728 (CWeapon::FireSniper())
	// TheCamera + 0x2D8 = 0x951FA8 + 0x2D8 = 0x952280
	return pcaInternalAim;
}
// 0.3.7
uint8_t GameGetLocalPlayerCameraMode()
{
	// CWeapon::FireSniper *((unsigned __int16 *)&TheCamera + 0x108 * (unsigned __int8)TheCamera_nActiveCam + 0xBF
	// TheCamera = 0x951FA8
	//return (uint8_t)*(uint16_t*)(g_libGTASA + 0x951FA8 + 0x17E);
	return *pbyteCameraMode;
}
// 0.3.7
float GameGetLocalPlayerCameraExtZoom()
{
	//return *(float*)(g_libGTASA + 0x952118 + 0x8C);
	return (*pfCameraExtZoom - 35.0) * 0.028571429;
}
// 0.3.7
float GameGetAspectRatio()
{
	//return *(float*)(g_libGTASA + 0xA26A90);
	return* pfAspectRatio;
}
// 0.3.7
void calculateAimVector(PVECTOR vec1, PVECTOR vec2)
{
	float f1, f2, f3;

	f1 = atan2(vec1->X, vec1->Y) - 1.570796370506287; // PI/2
	f2 = sin(f1);
	f3 = cos(f1);
	vec2->X = vec1->Y * 0.0 - f3 * vec1->Z;
	vec2->Y = f2 * vec1->Z - vec1->X * 0.0;
	vec2->Z = f3 * vec1->X - f2 * vec1->Y;
}
// 0.3.7
void GameSetPlayerCameraMode(uint8_t byteMode, uint8_t bytePlayerNumber)
{
	byteCameraMode[bytePlayerNumber] = byteMode;
}

void GameAimSyncInit()
{
	memset(&caLocalPlayerAim, 0, sizeof(caLocalPlayerAim));
	memset(&caRemotePlayerAim, 0, sizeof(caRemotePlayerAim));
	memset(byteCameraMode, 4, sizeof(byteCameraMode));

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		fCameraExtZoom[i] = 1.0f;
		fCameraAspectRatio[i] = 0.333333f;

		for(int x = 0; x < ARRAY_SIZE(fLocalSkillLevel); x++)
			fRemoteSkillLevel[i][x] = 999.0f;
	}

    CCameras& TheCamera = *reinterpret_cast<CCameras*>(g_libGTASA + 0x9DF030);


	pcaInternalAim = reinterpret_cast<CAMERA_AIM *>(&TheCamera.Cams[0].Front);

	pbyteCameraMode = (uint8_t *) &TheCamera.Cams[0].Mode;

    pfAspectRatio = (float*)(g_libGTASA + 0xC54518);


    pfCameraExtZoom = &TheCamera.Cams[0].FOV;

    wCameraMode2 = &TheCamera.PlayerWeaponMode.m_nMode;

    pPlayerStats = (float*)(g_libGTASA+0x9F9F84);

    pbyteCurrentPlayer = (uint8_t*)(g_libGTASA + 0xA01210);

}
// 0.3.7
void GameSetPlayerCameraExtZoomAndAspect(uint8_t bytePlayerNumber, float fExtZoom, float fAspectRatio)
{
	fCameraExtZoom[bytePlayerNumber] = fExtZoom;
	fCameraAspectRatio[bytePlayerNumber] = fAspectRatio;
}
// 0.3.7
uint8_t GameGetPlayerCameraMode(uint8_t bytePlayerNumber)
{
	return byteCameraMode[bytePlayerNumber];
}
// 0.3.7
CAMERA_AIM* GameGetRemotePlayerAim(uint8_t bytePlayerNumber)
{
	return &caRemotePlayerAim[bytePlayerNumber];
}
// 0.3.7
void GameSetRemotePlayerAim(uint8_t bytePlayerNumber)
{
	memcpy(pcaInternalAim, &caRemotePlayerAim[bytePlayerNumber], sizeof(CAMERA_AIM));
}
// 0.3.7
void GameStoreRemotePlayerAim(uint8_t bytePlayerNumber, CAMERA_AIM* pAim)
{
	memcpy(&caRemotePlayerAim[bytePlayerNumber], pAim, sizeof(CAMERA_AIM));
}
// 0.3.7
void GameSetLocalPlayerAim()
{
	memcpy(pcaInternalAim, &caLocalPlayerAim, sizeof(CAMERA_AIM));
}
// 0.3.7
void GameStoreLocalPlayerAim()
{
	memcpy(&caLocalPlayerAim, pcaInternalAim, sizeof(CAMERA_AIM));
}
// 0.3.7
void GameSetLocalPlayerCameraExtZoomAndAspect()
{
	*pfCameraExtZoom = fLocalCameraExtZoom;
	*pfAspectRatio = fLocalAspectRatio;
}
// 0.3.7
void GameStoreLocalPlayerCameraExtZoomAndAspect()
{
	fLocalCameraExtZoom = *pfCameraExtZoom;
	fLocalAspectRatio = *pfAspectRatio;
}
// 0.3.7
void GameSetRemotePlayerCameraExtZoomAndAspect(uint8_t bytePlayerNumber)
{
	*pfCameraExtZoom = fCameraExtZoom[bytePlayerNumber] * 35.0f + 35.0f;
	*pfAspectRatio = fCameraAspectRatio[bytePlayerNumber] + 1.0f;
}

void GameResetLocalPlayerSkills()
{
	for(int i = 0; i < ARRAY_SIZE(fLocalSkillLevel); i++)
		pPlayerStats[69 + i] = 999.0f;

	GameStoreLocalPlayerSkills();
}

void GameUpdateLocalPlayerSkill(int iSkillType, uint16_t wSkillLevel)
{
	if(iSkillType < ARRAY_SIZE(fLocalSkillLevel))
		pPlayerStats[69 + iSkillType] = (float)wSkillLevel;
}

void GameSetLocalPlayerSkills()
{
	for(int i = 0; i < ARRAY_SIZE(fLocalSkillLevel); i++)
		pPlayerStats[69 + i] = fLocalSkillLevel[i];
}

void GameStoreLocalPlayerSkills()
{
	for(int i = 0; i < ARRAY_SIZE(fLocalSkillLevel); i++)
		fLocalSkillLevel[i] = pPlayerStats[69 + i];
}

void GameSetRemotePlayerSkills(uint8_t bytePlayerNumber)
{
	for(int i = 0; i < ARRAY_SIZE(fLocalSkillLevel); i++)
		pPlayerStats[69 + i] = fRemoteSkillLevel[bytePlayerNumber][i];
}

void GameStoreRemotePlayerSkills(uint8_t bytePlayerNumber, int iSkillType, uint16_t wSkillLevel)
{
	if(iSkillType < ARRAY_SIZE(fLocalSkillLevel))
		fRemoteSkillLevel[bytePlayerNumber][iSkillType] = (float)wSkillLevel;
}