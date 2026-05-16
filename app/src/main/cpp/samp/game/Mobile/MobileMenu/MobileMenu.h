//
// Created by kuzia15 on 29.04.2024.
//

#pragma once

#include "../../common.h"
#include "../../RW/rwcore.h"
#include "../../Core/Vector2D.h"
//#include "Radar.h"
using tBlipHandle = uint32; // TODO: Use struct above
enum OSPointerState : int32_t
{
    OSPS_ButtonReleased = 0x0,
    OSPS_ButtonUp = 0x1,
    OSPS_ButtonPressed = 0x2,
    OSPS_ButtonDown = 0x3,
    OSPS_ButtonInvalid = 0xFF
};

struct MenuScreen
{
    int (**_vptr$MenuScreen)(void);
    RwTexture *arrowTex;
    float opacity;
    bool hasBack;
};

template <typename T> struct OSArray
{
    uint32_t numAlloced;
    uint32_t numEntries;
    T *dataPtr;
};

class CMobileMenu {
public:
    CVector2D               bgUVSize;
    CVector2D               bgTargetCoords;
    CVector2D               bgCurCoords;
    CVector2D               bgStartCoords;
    OSArray<MenuScreen*>    screenStack;
    MenuScreen*             pendingScreen;
    RwTexture*              bgTex;
    RwTexture*              sliderEmpty;
    RwTexture*              sliderFull;
    RwTexture*              sliderNub;
    RwTexture*              controlsBack;
    RwTexture*              controlsBack2;
    tBlipHandle             waypoint_blip;
    bool                    m_WantsToRestartGame;
    bool                    WantsToLoad;
    int                     SelectedSlot;
    bool                    CurrentGameNotResumable;
    bool                    InitializedForSignOut;
    float                   NEW_MAP_SCALE;
    float                   MAP_OFFSET_X;
    float                   MAP_OFFSET_Y;
    float                   MAP_AREA_X;
    float                   MAP_AREA_Y;
    bool                    DisplayingMap;
    bool                    isMapMode;
    bool                    pointerMode;
    bool                    isMouse;
    CVector2D               pointerCoords[4];
    OSPointerState          pointerState[4];
    uint32_t                pointerPress[4];

public:
    static void InjectHooks();

};
VALIDATE_SIZE(OSArray<MenuScreen*>, (VER_x32 ? 0xC : 0x10));
VALIDATE_SIZE(CMobileMenu, (VER_x32 ? 0xB0 : 0xD0));

extern CMobileMenu* gMobileMenu;
extern bool g_bMenuActive;