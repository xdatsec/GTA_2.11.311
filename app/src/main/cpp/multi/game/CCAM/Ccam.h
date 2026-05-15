/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once
#include "game/common.h"

enum eCamMode : uint16_t {
    MODE_NONE = 0x0,
    MODE_TOPDOWN = 0x1,
    MODE_GTACLASSIC = 0x2,
    MODE_BEHINDCAR = 0x3,
    MODE_FOLLOWPED = 0x4,
    MODE_AIMING = 0x5,
    MODE_DEBUG = 0x6,
    MODE_SNIPER = 0x7,
    MODE_ROCKETLAUNCHER = 0x8,
    MODE_MODELVIEW = 0x9,
    MODE_BILL = 0xA,
    MODE_SYPHON = 0xB,
    MODE_CIRCLE = 0xC,
    MODE_CHEESYZOOM = 0xD,
    MODE_WHEELCAM = 0xE,
    MODE_FIXED = 0xF,
    MODE_1STPERSON = 0x10,
    MODE_FLYBY = 0x11,
    MODE_CAM_ON_A_STRING = 0x12,
    MODE_REACTION = 0x13,
    MODE_FOLLOW_PED_WITH_BIND = 0x14,
    MODE_CHRIS = 0x15,
    MODE_BEHINDBOAT = 0x16,
    MODE_PLAYER_FALLEN_WATER = 0x17,
    MODE_CAM_ON_TRAIN_ROOF = 0x18,
    MODE_CAM_RUNNING_SIDE_TRAIN = 0x19,
    MODE_BLOOD_ON_THE_TRACKS = 0x1A,
    MODE_IM_THE_PASSENGER_WOOWOO = 0x1B,
    MODE_SYPHON_CRIM_IN_FRONT = 0x1C,
    MODE_PED_DEAD_BABY = 0x1D,
    MODE_PILLOWS_PAPS = 0x1E,
    MODE_LOOK_AT_CARS = 0x1F,
    MODE_ARRESTCAM_ONE = 0x20,
    MODE_ARRESTCAM_TWO = 0x21,
    MODE_M16_1STPERSON = 0x22,
    MODE_SPECIAL_FIXED_FOR_SYPHON = 0x23,
    MODE_FIGHT_CAM = 0x24,
    MODE_TOP_DOWN_PED = 0x25,
    MODE_LIGHTHOUSE = 0x26,
    MODE_SNIPER_RUNABOUT = 0x27,
    MODE_ROCKETLAUNCHER_RUNABOUT = 0x28,
    MODE_1STPERSON_RUNABOUT = 0x29,
    MODE_M16_1STPERSON_RUNABOUT = 0x2A,
    MODE_FIGHT_CAM_RUNABOUT = 0x2B,
    MODE_EDITOR = 0x2C,
    MODE_HELICANNON_1STPERSON = 0x2D,
    MODE_CAMERA = 0x2E,
    MODE_ATTACHCAM = 0x2F,
    MODE_TWOPLAYER = 0x30,
    MODE_TWOPLAYER_IN_CAR_AND_SHOOTING = 0x31,
    MODE_TWOPLAYER_SEPARATE_CARS = 0x32,
    MODE_ROCKETLAUNCHER_HS = 0x33,
    MODE_ROCKETLAUNCHER_RUNABOUT_HS = 0x34,
    MODE_AIMWEAPON = 0x35,
    MODE_TWOPLAYER_SEPARATE_CARS_TOPDOWN = 0x36,
    MODE_AIMWEAPON_FROMCAR = 0x37,
    MODE_DW_HELI_CHASE = 0x38,
    MODE_DW_CAM_MAN = 0x39,
    MODE_DW_BIRDY = 0x3A,
    MODE_DW_PLANE_SPOTTER = 0x3B,
    MODE_DW_DOG_FIGHT = 0x3C,
    MODE_DW_FISH = 0x3D,
    MODE_DW_PLANECAM1 = 0x3E,
    MODE_DW_PLANECAM2 = 0x3F,
    MODE_DW_PLANECAM3 = 0x40,
    MODE_AIMWEAPON_ATTACHED = 0x41
};

#pragma pack(push, 1)
struct  CCam // sizeof=0x228
{                                       // XREF: CCamera/r CCam_0/r ...
    bool bBelowMinDist;
    bool bBehindPlayerDesired;
    bool m_bCamLookingAtVector;
    bool m_bCollisionChecksOn;
    bool m_bFixingBeta;
    bool m_bTheHeightFixerVehicleIsATrain;
    bool LookBehindCamWasInFront;
    bool LookingBehind;
    bool LookingLeft;
    bool LookingRight;
    bool ResetStatics;
    bool ResetKeyboardStatics;
    bool Rotating;
    char pad;
    uint16 Mode;                         // XREF: CTaskSimpleUseGun::ProcessPed(CPed *)+A7C/r
    // CTaskSimpleUseGun::AimGun(CPed *)+1A4/r ...
    uint32 m_uiFinishTime;
    int m_iDoCollisionChecksOnFrameNum;
    int m_iDoCollisionCheckEveryNumOfFrames;
    int m_iFrameNumWereAt;
    int DirectionWasLooking;
    float m_fSyphonModeTargetZOffSet;
    float m_fAlphaSpeedOverOneFrame;
    float m_fBetaSpeedOverOneFrame;
    float m_fCamBufferedHeight;
    float m_fCamBufferedHeightSpeed;
    float m_fCloseInPedHeightOffset;
    float m_fCloseInPedHeightOffsetSpeed;
    float m_fCloseInCarHeightOffset;
    float m_fCloseInCarHeightOffsetSpeed;
    float m_fDimensionOfHighestNearCar;
    float m_fDistanceBeforeChanges;
    float m_fFovSpeedOverOneFrame;
    float m_fMinDistAwayFromCamWhenInterPolating;
    float m_fPedBetweenCameraHeightOffset;
    float m_fPlayerInFrontSyphonAngleOffSet;
    float m_fRadiusForDead;
    float m_fRealGroundDist;
    float m_fTimeElapsedFloat;
    float m_fTilt;
    float m_fTiltSpeed;
    float m_fTransitionBeta;
    float m_fTrueBeta;
    float m_fTrueAlpha;
    float m_fInitialPlayerOrientation;
    float Alpha;
    float AlphaSpeed;
    float FOV;
    float FOVSpeed;
    float Beta;
    float BetaSpeed;
    float Distance;
    float DistanceSpeed;
    float CA_MIN_DISTANCE;
    float CA_MAX_DISTANCE;
    float SpeedVar;
    float m_fCameraHeightMultiplier;
    float m_fTargetZoomGroundOne;
    float m_fTargetZoomGroundTwo;
    float m_fTargetZoomGroundThree;
    float m_fTargetZoomOneZExtra;
    float m_fTargetZoomTwoZExtra;
    float m_fTargetZoomTwoInteriorZExtra;
    float m_fTargetZoomThreeZExtra;
    float m_fTargetZoomZCloseIn;
    float m_fMinRealGroundDist;
    float m_fTargetCloseInDist;
    float Beta_Targeting;
    float X_Targetting;
    float Y_Targetting;
    uint32 CarWeAreFocussingOn;
    float CarWeAreFocussingOnI;
    float m_fCamBumpedHorz;
    float m_fCamBumpedVert;
    uint32 m_nCamBumpedTime;
    VECTOR m_cvecSourceSpeedOverOneFrame;
    VECTOR m_cvecTargetSpeedOverOneFrame;
    VECTOR m_cvecUpOverOneFrame;
    VECTOR m_cvecTargetCoorsForFudgeInter;
    VECTOR m_cvecCamFixedModeVector;
    VECTOR m_cvecCamFixedModeSource;
    VECTOR m_cvecCamFixedModeUpOffSet;
    VECTOR m_vecLastAboveWaterCamPosition;
    VECTOR m_vecBufferedPlayerBodyOffset;
    VECTOR Front;                      // XREF: CRunningScript::ProcessCommands1100To1199(int)+A2C/r
    // CRunningScript::ProcessCommands1100To1199(int)+A34/r ...
    VECTOR Source;                     // XREF: CRunningScript::ProcessCommands1100To1199(int)+694/r
    // CRunningScript::ProcessCommands1100To1199(int)+698/r ...
    VECTOR SourceBeforeLookBehind;
    VECTOR Up;                         // XREF: CWeapon::FireInstantHit(CEntity *,VECTOR *,VECTOR *,CEntity *,VECTOR *,VECTOR *,bool,bool)+F04/r
    // CWeapon::FireInstantHit(CEntity *,VECTOR *,VECTOR *,CEntity *,VECTOR *,VECTOR *,bool,bool)+F08/r ...
    VECTOR m_arrPreviousVectors[2];
    VECTOR m_aTargetHistoryPos[4];
    uint32 m_nTargetHistoryTime[4];
    int32 m_nCurrentHistoryPoints;
    PADDING(campad, 4);
    uintptr_t *CamTargetEntity;           // XREF: CPools::LoadVehiclePool(void)+51C/w
    // CPools::LoadVehiclePool(void)+528/r ...
    float m_fCameraDistance;
    float m_fIdealAlpha;
    float m_fPlayerVelocity;
    PADDING(campad1, 4);
    uintptr_t *m_pLastCarEntered;
    uintptr_t *m_pLastPedLookedAt;
    bool m_bFirstPersonRunAboutActive;
    PADDING(campad2, 7);
};
#pragma pack(pop)
VALIDATE_SIZE(CCam, 0x228);
