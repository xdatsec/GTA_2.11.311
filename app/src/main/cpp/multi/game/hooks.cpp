#include <GLES2/gl2.h>
#include "../main.h"
#include "game.h"
#include "../vendor/armhook/patch.h"
#include <algorithm>
#include <string>
#include <fstream>  // <--- Add this line
#include <sys/resource.h>
extern CGame* pGame;
void (*NvUtilInit)();

int(*OS_FileOpen)(uint32_t r0, uintptr_t handle, char* name, int r3);

int OS_FileOpen_hook(int r0, uintptr_t handle, char* name, int r3)
{
    if(!name)
        return 0;

    char fixedName[0xFF] = {0};
    char path[0xFF] = {0};

    // Correct Android path
    const char* pszStorage = "/storage/emulated/0/Documents/SampMobile/";

    FLog("OS_FileOpen original: %s", name);

    // copy original name
    strncpy(fixedName, name, sizeof(fixedName) - 1);

    // remove leading slash
    if(fixedName[0] == '/')
        memmove(fixedName, fixedName + 1, strlen(fixedName));

    // convert '\' -> '/'
    for(int i = 0; fixedName[i]; i++)
    {
        if(fixedName[i] == '\\')
            fixedName[i] = '/';
    }

    // build final redirected path
    snprintf(path, sizeof(path), "%s%s", pszStorage, fixedName);

    FLog("OS_FileOpen redirected: %s", path);

    // call original function
    return OS_FileOpen(r0, handle, path, r3);
}

void NvUtilInit_hook() {
    FLog("NvUtilInit");
    g_pszStorage = (char *) (g_libGTASA + 0xCFFCB0); // StorageRootBuffer
    LOGI("Storage located at %s", g_pszStorage);
    NvUtilInit();
    //ReadSettingFile();


}
bool g_bPlaySAMP = false;

void MainMenu_OnStartSAMP()
{
    if(g_bPlaySAMP) return;

    //InitInMenu();
    pGame->StartGame();

    // StartGameScreen::OnNewGameCheck()
    (( void (*)())(g_libGTASA + 0x707B64))();

    g_bPlaySAMP = true;
}

unsigned int (*MainMenuScreen__Update)(uintptr_t thiz, float a2);
unsigned int MainMenuScreen__Update_hook(uintptr_t thiz, float a2)
{
    unsigned int ret = MainMenuScreen__Update(thiz, a2);
    MainMenu_OnStartSAMP();
    return ret;
}

void (*StartGameScreen__OnNewGameCheck)();
void StartGameScreen__OnNewGameCheck_hook()
{
    // Ð¾Ñ‚ÐºÐ»ÑŽÑ‡Ð¸Ñ‚ÑŒ ÐºÐ½Ð¾Ð¿ÐºÑƒ Ð½Ð°Ñ‡Ð°Ñ‚ÑŒ Ð¸Ð³Ñ€Ñƒ
    if(g_bPlaySAMP)
        return;

    StartGameScreen__OnNewGameCheck();
}
struct stFile
{
    int isFileExist;
    FILE *f;
};

char lastFile[123];


stFile* NvFOpen(const char *r1)
{

    strcpy(lastFile, r1);
    g_pszStorage = "/storage/emulated/0/Android/media/com.rockstargames.gtasa/";
    static char path[255]{};
    memset(path, 0, sizeof(path));

    sprintf(path, "%s%s", g_pszStorage, r1);

    // ----------------------------
    if(!strncmp(r1+12, "mainV1.scm", 10))
    {
        sprintf(path, "%DATA/main.scm", g_pszStorage);
        FLog("Loading %s", path);
    }
    // ----------------------------
    if(!strncmp(r1+12, "SCRIPTV1.IMG", 12))
    {
        sprintf(path, "%DATA/script.img", g_pszStorage);
        FLog("Loading script.img..");
    }
    // ----------------------------
    if(!strncmp(r1, "DATA/PEDS.IDE", 13))
    {
        sprintf(path, "%sSAMP/peds.ide", g_pszStorage);
        FLog("Loading peds.ide..");
    }
    // ----------------------------
    if(!strncmp(r1, "DATA/VEHICLES.IDE", 17))
    {
        sprintf(path, "%sSAMP/vehicles.ide", g_pszStorage);
        FLog("Loading vehicles.ide..");
    }

    if (!strncmp(r1, "DATA/GTA.DAT", 12))
    {
        sprintf(path, "%sSAMP/gta.dat", g_pszStorage);
        FLog("Loading gta.dat..");
    }

    if (!strncmp(r1, "DATA/HANDLING.CFG", 17))
    {
        sprintf(path, "%sSAMP/handling.cfg", g_pszStorage);
        FLog("Loading handling.cfg..");
    }

    if (!strncmp(r1, "DATA/WEAPON.DAT", 15))
    {
        sprintf(path, "%sSAMP/weapon.dat", g_pszStorage);
        FLog("Loading weapon.dat..");
    }

    if (!strncmp(r1, "DATA/FONTS.DAT", 15))
    {
        sprintf(path, "%sdata/fonts.dat", g_pszStorage);
        FLog("Loading weapon.dat..");
    }

    if (!strncmp(r1, "DATA/PEDSTATS.DAT", 15))
    {
        sprintf(path, "%sdata/pedstats.dat", g_pszStorage);
        FLog("Loading weapon.dat..");
    }

    if (!strncmp(r1, "DATA/TIMECYC.DAT", 15))
    {
        sprintf(path, "%sdata/timecyc.dat", g_pszStorage);
        FLog("Loading weapon.dat..");
    }

    if (!strncmp(r1, "DATA/POPCYCLE.DAT", 15))
    {
        sprintf(path, "%sdata/popcycle.dat", g_pszStorage);
        FLog("Loading weapon.dat..");
    }

#if VER_x32
    auto *st = (stFile*)malloc(8);
#else
    auto *st = (stFile*)malloc(0x10);
#endif
    st->isFileExist = false;

    FILE *f  = fopen(path, "rb");

    if(f)
    {
        st->isFileExist = true;
        st->f = f;
        return st;
    }
    else
    {
        FLog("NVFOpen hook | Error: file not found (%s)", path);
        free(st);
        return nullptr;
    }
}
void InjectHooks() {
    FLog("InjectHooks");

}
void InstallSpecialHooks()
{

    InjectHooks();
    CHook::InstallPLT(g_libGTASA + 0x828730, &StartGameScreen__OnNewGameCheck_hook, &StartGameScreen__OnNewGameCheck);
    CHook::InlineHook("_Z10NvUtilInitv", &NvUtilInit_hook, &NvUtilInit);
    CHook::InlineHook("_ZN14MainMenuScreen6UpdateEf", &MainMenuScreen__Update_hook, &MainMenuScreen__Update);
    CHook::Redirect(g_libGTASA+0x7C3E0C, &NvFOpen);
   // CHook::InlineHook(g_libGTASA + 0x7BF280, &OS_FileOpen_hook, &OS_FileOpen);
}
void InstallHooks()
{

}