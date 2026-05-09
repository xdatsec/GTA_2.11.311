//
// Created by Dionisio on 5/9/2026.
//

#include "../main.h"
#include "game.h"
#include "../vendor/armhook/patch.h"

CGame::CGame()
{


}

CGame::~CGame()
{

}
void ApplyGlobalPatches();
void InstallHooks();
void CGame::StartGame()
{
    FLog("Starting game..");

    // OnNewGameCheck
    //(( void (*)())(g_libGTASA + (VER_x32 ? 0x002A7270 + 1 : 0x365EA0)))();

    //*(int*)(g_libGTASA + 0xA987C8) = 8;
    //*(char*)(g_libGTASA + 0x96B514) = 0;
    //*(short*)(g_libGTASA + 0x6E00C0) = 0;
    //*(int*)(g_libGTASA + 0x6E0098) = 0;
    //*(char*)(g_libGTASA + 0x6E00D9) = 0;

    InstallHooks();
    ApplyGlobalPatches();

    //GameAimSyncInit();
    //  InitScripting();
}