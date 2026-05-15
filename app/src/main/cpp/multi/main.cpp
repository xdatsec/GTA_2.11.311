#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <dlfcn.h>
#include "log.h"
#include "StackTrace.h"
#include "patch.h"
#include "main.h"
#include "game/game.h"
struct sigaction act_old;
struct sigaction act1_old;
struct sigaction act2_old;
struct sigaction act3_old;
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
void FLog(const char* fmt, ...)
{
    char buffer[0xFF];
    static FILE* flLog = nullptr;
   // const char* pszStorage = g_pszStorage;


    //if (flLog == nullptr && pszStorage != nullptr)
    //{
    //    sprintf(buffer, "%s/samp_log.txt", pszStorage);
    //    //LOGI("buffer: %s", buffer);
    //    flLog = fopen(buffer, "a");
   // }

    memset(buffer, 0, sizeof(buffer));

    va_list arg;
    va_start(arg, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, arg);
    va_end(arg);

    LOGI("%s", buffer);
    //firebase::crashlytics::Log(buffer);

    if (flLog == nullptr) return;
    fprintf(flLog, "%s\n", buffer);
    fflush(flLog);

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

 //   CHook::CallFunction<void>(g_libGTASA + 0x7BE2E4);
    LOGI("PATCHING FPS");
    uint8_t targetFPS = 120;
    CHook::InitHookStuff();
    InstallSpecialHooks();
    //CHook::WriteMemory(g_libGTASA + 0x3545E0+1, &targetFPS, 1);
  //  CHook::WriteMemory(g_libGTASA + 0x354868+1, &targetFPS, 1);
   // CHook::WriteMemory(g_libGTASA + 0x354B84+1, &targetFPS, 1);




    pGame = new CGame();
    return JNI_VERSION_1_6;
}
