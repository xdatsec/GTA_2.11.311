//
// Created by Dionisio on 5/9/2026.
//
#include "../main.h"
#include "../game/game.h"
#include "../vendor/armhook/patch.h"

extern CGame* pGame;

void ApplyGlobalPatches()
{
    CHook::RET("_ZN14CLoadingScreen15DisplayPCScreenEv"); // Loading screen

}