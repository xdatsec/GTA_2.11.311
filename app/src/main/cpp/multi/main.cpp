#include <jni.h>
#include <pthread.h>
//#include <syscall.h>

#include "main.h"
#include "game/game.h"
#include "net/netgame.h"
#include "gui/gui.h"
#include "playertags.h"
#include "audiostream.h"
#include "java/jniutil.h"
#include <dlfcn.h>
#include "StackTrace.h"

// voice
//#include "voice_new/Plugin.h"

#include "vendor/armhook/patch.h"
#include "vendor/str_obfuscator/str_obfuscator.hpp"

#include "settings.h"

struct sigaction act_old;
struct sigaction act1_old;
struct sigaction act2_old;
struct sigaction act3_old;

//#include "voice_new/PluginConfig.h"
UI* pUI = nullptr;

extern  RwMatrix* m_pWorkingMatrix1;
extern  RwMatrix* m_pWorkingMatrix2;
CNetGame *pNetGame = nullptr;
CPlayerTags* pPlayerTags = nullptr;
CSnapShotHelper* pSnapShotHelper = nullptr;
CAudioStream* pAudioStream = nullptr;
CJavaWrapper* pJavaWrapper = nullptr;
CSettings* pSettings = nullptr;
//CVoice* pVoice = nullptr;

MaterialTextGenerator* pMaterialTextGenerator = nullptr;

bool bDebug = false;
bool bGameInited = false;
bool bNetworkInited = false;


void ApplyPatches_level0();
void ApplyGlobalPatches();
void ApplyMultiTouchPatches();
//void InstallGlobalHooks();
void InitRenderWareFunctions();
void FLog(const char* fmt, ...);
//void MyLog(const char* fmt, ...);

uintptr_t g_libGTASA = 0x00;
uintptr_t g_libSAMP = 0x00;
char* g_pszStorage = nullptr;
CGame *pGame = nullptr;
void InstallSpecialHooks();
JavaVM* javaVM;
uintptr_t FindLib(const char* libname)
{
    void* handle = dlopen(libname, RTLD_LAZY);
    if (handle) {
        void* symbol = dlsym(handle, "JNI_OnLoad");
        if (symbol) {
            Dl_info info;
            if (dladdr(symbol, &info) != 0) {
                return reinterpret_cast<uintptr_t>(info.dli_fbase);
            }
        }
        dlclose(handle);
    }

    return 0;
}
void handler(int signum, siginfo_t *info, void* contextPtr)
{
    ucontext* context = (ucontext_t*)contextPtr;

    if (act_old.sa_sigaction)
    {
        act_old.sa_sigaction(signum, info, contextPtr);
    }

    if(info->si_signo == SIGSEGV)
    {
        LOGI("SIGSEGV | Fault address: 0x%x", info->si_addr);

        PRINT_CRASH_STATES(context);

        CStackTrace::printBacktrace();
    }

    return;
}

void handler1(int signum, siginfo_t *info, void* contextPtr)
{
    ucontext* context = (ucontext_t*)contextPtr;

    if (act1_old.sa_sigaction)
    {
        act1_old.sa_sigaction(signum, info, contextPtr);
    }

    if(info->si_signo == SIGABRT)
    {
        FLog("SIGABRT | Fault address: 0x%x", info->si_addr);

        PRINT_CRASH_STATES(context);

        CStackTrace::printBacktrace();
    }

    return;
}

void handler2(int signum, siginfo_t *info, void* contextPtr)
{
    ucontext* context = (ucontext_t*)contextPtr;

    if (act2_old.sa_sigaction)
    {
        act2_old.sa_sigaction(signum, info, contextPtr);
    }

    if(info->si_signo == SIGFPE)
    {
        FLog("SIGFPE | Fault address: 0x%x", info->si_addr);

        PRINT_CRASH_STATES(context);

        CStackTrace::printBacktrace();
    }

    return;
}

void handler3(int signum, siginfo_t *info, void* contextPtr)
{
    ucontext* context = (ucontext_t*)contextPtr;

    if (act3_old.sa_sigaction)
    {
        act3_old.sa_sigaction(signum, info, contextPtr);
    }

    if(info->si_signo == SIGBUS)
    {
        FLog("SIGBUS | Fault address: 0x%x", info->si_addr);

        PRINT_CRASH_STATES(context);

        CStackTrace::printBacktrace();
    }

    return;
}


#include <jni.h>
#include <pthread.h>
//#include <syscall.h>

#include "main.h"
#include "game/game.h"
#include "net/netgame.h"
#include "gui/gui.h"
#include "playertags.h"
#include "audiostream.h"
#include "java/jniutil.h"
#include <dlfcn.h>
#include "StackTrace.h"

// voice
//#include "voice_new/Plugin.h"

#include "vendor/armhook/patch.h"
#include "vendor/str_obfuscator/str_obfuscator.hpp"

#include "settings.h"


/*
Peerapol Unarak
*/


int work = 0;

bool Mchat = false;

void ReadSettingFile()
{
    /*char path[255] = { 0 };
    //sprintf(path, "%ssamp.set", pGame->GetDataDirectory());
    sprintf(path, "%sNickName.ini", pGame->GetDataDirectory());

    FILE* fp = fopen(path, "r");
    if (fp == NULL) return;

    char buf[1024];

    // nickname
    if (fgets(buf, 1024, fp) != NULL) {
        buf[strcspn(buf, "\n\r")] = 0;
        strcpy(g_nick, buf);
    }

    fclose(fp);*/

    pSettings = new CSettings();

    //firebase::crashlytics::SetUserId(pSettings->Get().szNickName);
}

int hashing(const char* str) {
    int hashing = 5381;
    int c;
    while (c = *str++) {
        hashing = ((hashing << 5) + hashing) + c; /* hash * 33 + c */
        if (hashing < 0) hashing = 100;
    }
    if (hashing < 0) hashing = 100;
    return hashing;
}


int _curlWriteFunc(char* data, size_t size, size_t nmemb, std::string* buffer)
{
    int result = 0;
    if (buffer != nullptr)
    {
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }
    return result;
}

void* Init(void*)
{

    while (true)
    {
        if (*(int*)(g_libGTASA + 0x89E618) == 7) {
            pGame->StartGame();
            break;
        }
        else {
            usleep(500);
        }
    }

    pthread_exit(0);
}

void DoDebugLoop()
{
    // ...
}

void DoDebugStuff()
{
    // ...

    MATRIX4X4 mat;
    pGame->FindPlayerPed()->GetMatrix(&mat);

    for (int i = 0; i < 100; i++)
    {
        CPlayerPed* ped = pGame->NewPlayer(i, mat.pos.X + i, mat.pos.Y, mat.pos.Z, 0.0f, false, false);
        //ped->SetCollisionChecking(false);
        //ped->SetGravityProcessing(false);
    }
}

void DoInitStuff()
{
    if (bGameInited == false)
    {
        pPlayerTags = new CPlayerTags();
        pSnapShotHelper = new CSnapShotHelper();
        pMaterialTextGenerator = new MaterialTextGenerator();
        pAudioStream = new CAudioStream();
        pAudioStream->Initialize();

        //pUI->splashscreen()->setVisible(false);
        //pJavaWrapper->HideLoadingScreen();
        pUI->chat()->setVisible(true);
        Mchat = true;

        pGame->Initialize();
        pGame->SetMaxStats();
        pGame->ToggleThePassingOfTime(false);

        // voice
        LogVoice("[dbg:samp:load] : module loading...");

       // for (const auto& loadCallback : Samp::loadCallbacks) {
       //     if (loadCallback != nullptr) {
       //         loadCallback();
      //      }
      //  }

      //  Samp::loadStatus = true;

        LogVoice("[dbg:samp:load] : module loaded");

        if (bDebug)
        {
            pGame->GetCamera()->Restore();
            pGame->GetCamera()->SetBehindPlayer();
            pGame->DisplayHUD(true);
            pGame->EnableClock(false);

            DoDebugStuff();
        }

        bGameInited = true;
    }

    if (!bNetworkInited && !bDebug)
    {
        //ReadSettingFile();

        pNetGame = new CNetGame(" play.uifserver.net", 7776, "John_Doe", "");
        bNetworkInited = true;
    }
}
/*
extern "C" {
JNIEXPORT void JNICALL Java_com_meta_samp_game_SAMP_initializeSAMP(JNIEnv *pEnv, jobject thiz)
{
    ReadSettingFile();
    pJavaWrapper = new CJavaWrapper(pEnv, thiz);
}
JNIEXPORT void JNICALL Java_com_meta_samp_game_SAMP_onInputEnd(JNIEnv *pEnv, jobject thiz, jbyteArray str)
{
    if(pUI)
    {
        pUI->keyboard()->sendForGB(pEnv, thiz, str);
    }
}

JNIEXPORT void JNICALL Java_com_meta_samp_game_SAMP_onEventBackPressed(JNIEnv *pEnv, jobject thiz)
{
    if(pSettings->Get().iAndroidKeyboard) pJavaWrapper->HideKeyboard();
}
JNIEXPORT void JNICALL Java_com_meta_samp_game_ui_dialog_DialogManager_sendDialogResponse(JNIEnv* pEnv, jobject thiz, jint i3, jint i, jint i2, jbyteArray str)
{
    jboolean isCopy = true;

    jbyte* pMsg = pEnv->GetByteArrayElements(str, &isCopy);
    jsize length = pEnv->GetArrayLength(str);

    std::string szStr((char*)pMsg, length);

    if(pNetGame) {
        pNetGame->SendDialogResponse(i, i3, i2, (char*)szStr.c_str());
        //pGame->FindPlayerPed()->TogglePlayerControllableWithoutLock(true);
    }

    pEnv->ReleaseByteArrayElements(str, pMsg, JNI_ABORT);
}

}
*/
void MainLoop()
{
    if (pGame->bIsGameExiting) return;

    DoInitStuff();

    if (bDebug) {
        DoDebugLoop();
    }

    if (pNetGame) {
        pNetGame->Process();

        CTextDrawPool* pTextDrawPool = pNetGame->GetTextDrawPool();
        if(pTextDrawPool) pTextDrawPool->Draw();
    }

    if (pNetGame)
    {
        if (pNetGame->GetPlayerPool())
        {
            if (pNetGame->GetPlayerPool()->GetLocalPlayer())
            {
                CPlayerPed* pLocalPlayerPed = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
                if (pLocalPlayerPed && pNetGame->GetGameState() == GAMESTATE_CONNECTED)
                {
                    pGame->DisplayHUD(false);

                }
                else {
                   // *(uint8_t*)(g_libGTASA + /*0x8EF36B*/0x9A4C794) = 1;
                    //pJavaWrapper->HideHud();
                }

            }
        }
    }

    if (pAudioStream) {
        pAudioStream->Process();
    }

}

void InitGui()
{
    // new voice
    //Plugin::OnPluginLoad();
    //Plugin::OnSampLoad();

    std::string font_path = string_format("%sfonts/%s", pGame->GetDataDirectory(), FONT_NAME);
    pUI = new UI(ImVec2(RsGlobal->maximumWidth, RsGlobal->maximumHeight), font_path.c_str());
    pUI->initialize();
    pUI->performLayout();
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{

    javaVM = vm;
    LOGI("SA-MP library loaded! Build time: " __DATE__ " " __TIME__);

    g_libGTASA = FindLib("libGame.so");
    if (g_libGTASA == 0x00) {
        LOGE("libGTASA.so address was not found! ");
        return JNI_VERSION_1_6;
    }

    g_libSAMP = FindLib("libmulti.so");
    if (g_libSAMP == 0x00) {
        LOGE("libsamp.so address was not found! ");
        return JNI_VERSION_1_6;
    }

    uintptr_t libGame = FindLib("libGame.so");
    uintptr_t libmulti = FindLib("libmulti.so");
    uintptr_t libc = FindLib("libc.so");

    LOGI("libGame.so: 0x%x", libGame);
    LOGI("libmulti.so: 0x%x", libmulti);
    LOGI("libc.so: 0x%x", libc);

    char str[100];



    CHook::Write(g_libGTASA +0x8245F0, &m_pWorkingMatrix1);
    CHook::Write(g_libGTASA + 0x8245F8, &m_pWorkingMatrix2);

    struct sigaction act;
    act.sa_sigaction = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &act, &act_old);

    struct sigaction act1;
    act1.sa_sigaction = handler1;
    sigemptyset(&act1.sa_mask);
    act1.sa_flags = SA_SIGINFO;
    sigaction(SIGABRT, &act1, &act1_old);

    struct sigaction act2;
    act2.sa_sigaction = handler2;
    sigemptyset(&act2.sa_mask);
    act2.sa_flags = SA_SIGINFO;
    sigaction(SIGFPE, &act2, &act2_old);

    struct sigaction act3;
    act3.sa_sigaction = handler3;
    sigemptyset(&act3.sa_mask);
    act3.sa_flags = SA_SIGINFO;
    sigaction(SIGBUS, &act3, &act3_old);

    CHook::CallFunction<void>(g_libGTASA + 0x7BE2E4);
    LOGI("PATCHING FPS");
    uint8_t targetFPS = 120;
    CHook::InitHookStuff();
    InstallSpecialHooks();
    CHook::WriteMemory(g_libGTASA + 0x3545E0+1, &targetFPS, 1);
    CHook::WriteMemory(g_libGTASA + 0x354868+1, &targetFPS, 1);
    CHook::WriteMemory(g_libGTASA + 0x354B84+1, &targetFPS, 1);



    ApplyPatches_level0();

    InitRenderWareFunctions();
    pGame = new CGame();
    return JNI_VERSION_1_6;
}



uint32_t GetTickCount()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void FLog(const char* fmt, ...)
{
    char buffer[0xFF];
    static FILE* flLog = nullptr;
    const char* pszStorage = CGame::GetDataDirectory();


    if (flLog == nullptr && pszStorage != nullptr)
    {
        //sprintf(buffer, "%s/samp_log.txt", pszStorage);
       // LOGI("buffer: %s", buffer);
      //  flLog = fopen(buffer, "a");
    }

    memset(buffer, 0, sizeof(buffer));

    va_list arg;
    va_start(arg, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, arg);
    va_end(arg);

    LOGI("%s", buffer);


    if (flLog == nullptr) return;
    fprintf(flLog, "%s\n", buffer);
    fflush(flLog);

    return;
}

void ChatLog(const char* fmt, ...)
{
    char buffer[0xFF];
    static FILE* flLog = nullptr;
    const char* pszStorage = CGame::GetDataDirectory();


    if (flLog == nullptr && pszStorage != nullptr)
    {
        sprintf(buffer, "%s/chat_log.txt", pszStorage);
        flLog = fopen(buffer, "a");
    }

    memset(buffer, 0, sizeof(buffer));

    va_list arg;
    va_start(arg, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, arg);
    va_end(arg);

    if (flLog == nullptr) return;
    fprintf(flLog, "%s\n", buffer);
    fflush(flLog);

    return;
}

void MyLog(const char* fmt, ...)
{
    char buffer[0xFF];
    static FILE* flLog = nullptr;
    const char* pszStorage = CGame::GetDataDirectory();


    if (flLog == nullptr && pszStorage != nullptr)
    {
        sprintf(buffer, "%s/samp_log.txt", pszStorage);
        LOGI("buffer: %s", buffer);
        flLog = fopen(buffer, "a");
    }

    memset(buffer, 0, sizeof(buffer));

    va_list arg;
    va_start(arg, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, arg);
    va_end(arg);

    if (flLog == nullptr) return;
    fprintf(flLog, "%s\n", buffer);
    fflush(flLog);

    return;
}

void MyLog2(const char* fmt, ...)
{
    char buffer[0xFF];
    static FILE* flLog = nullptr;
    const char* pszStorage = CGame::GetDataDirectory();


    if (flLog == nullptr && pszStorage != nullptr)
    {
        sprintf(buffer, "%s/samp_log.txt", pszStorage);
        LOGI("buffer: %s", buffer);
        flLog = fopen(buffer, "a");
    }

    memset(buffer, 0, sizeof(buffer));

    va_list arg;
    va_start(arg, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, arg);
    va_end(arg);

    if (pUI) pUI->chat()->addDebugMessage(buffer);

    if (flLog == nullptr) return;
    fprintf(flLog, "%s\n", buffer);
    fflush(flLog);
    return;
}

void LogVoice(const char* fmt, ...)
{
    char buffer[0xFF];
    static FILE* flLog = nullptr;
    const char* pszStorage = CGame::GetDataDirectory();

    if (flLog == nullptr && pszStorage != nullptr)
    {
       // sprintf(buffer, "%sSAMP/%s", pszStorage, SV::kLogFileName);
        flLog = fopen(buffer, "w");
    }

    memset(buffer, 0, sizeof(buffer));

    va_list arg;
    va_start(arg, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, arg);
    va_end(arg);

    __android_log_write(ANDROID_LOG_INFO, "AXL", buffer);

    if (flLog == nullptr) return;
    fprintf(flLog, "%s\n", buffer);
    fflush(flLog);

    return;
}
