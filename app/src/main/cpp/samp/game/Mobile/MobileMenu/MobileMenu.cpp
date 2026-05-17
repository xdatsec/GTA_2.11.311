//
// Created by kuzia15 on 05.08.2025.
//

#include "MobileMenu.h"
#include "patch.h"
#include "game/game.h"
#include "../../net/netgame.h"
extern CNetGame* pNetGame;

CMobileMenu* gMobileMenu;
bool MobileMenuBool = false;

void (*InitForPause)(uintptr* thiz);
void (*CMobileMenu__Unload)(uintptr* thiz);
void (*CMobileMenu__Initiliaze)(uintptr* thiz);
void (*CMobileMenu__0CMobileMenu)(uintptr* thiz);

bool bAFKInitialized = false;

void InitForPause_hook(uintptr* thiz)
{
    if(!bAFKInitialized) {
        MobileMenuBool = true;
        bAFKInitialized = true;
    }
    InitForPause(thiz);
}

void CMobileMenu__Unload_hook(uintptr* thiz) {
    MobileMenuBool = false;
    bAFKInitialized = false;
    CMobileMenu__Unload(thiz);
}

void CMobileMenu__Initiliaze_hook(uintptr* thiz) {
    MobileMenuBool = false;
    bAFKInitialized = false;
    CMobileMenu__Initiliaze(thiz);
}

void CMobileMenu__0CMobileMenu_hook(uintptr* thiz) {
    MobileMenuBool = false;
    bAFKInitialized = false;
    CMobileMenu__0CMobileMenu(thiz);
}

void CMobileMenu::InjectHooks() {
    gMobileMenu = (CMobileMenu*)(g_libGTASA + 0xD0DB20);

    CHook::InlineHook("_ZN10MobileMenu12InitForPauseEv", &InitForPause_hook, &InitForPause);
    CHook::InlineHook("_ZN10MobileMenu6UnloadEv", &CMobileMenu__Unload_hook, &CMobileMenu__Unload);
    //CHook::InlineHook("_ZN10MobileMenuD2Ev", &CMobileMenu__0CMobileMenu_hook, &CMobileMenu__0CMobileMenu);
    //CHook::InlineHook("j__ZN10MobileMenu10InitializeEv", &CMobileMenu__Initiliaze_hook, &CMobileMenu__Initiliaze);
}