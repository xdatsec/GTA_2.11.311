#pragma once
#include "../main.h"
#include <string.h>
#include <dlfcn.h>
#include <asm/unistd.h>
#include <unistd.h>
#include <ctype.h>
#include <link.h>
#include <sys/cachectl.h>
#include <sys/mman.h>

#ifdef __arm__
#define __32BIT
#define DETHUMB(_a) (((uintptr_t)_a) & ~0x1)
#define RETHUMB(_a) (((uintptr_t)_a) | 0x1)
#define THUMBMODE(_a) ((((uintptr_t)_a) & 0x1)||(((uintptr_t)_a) & 0x2)||(CHook::GetAddrBaseXDL((uintptr_t)_a) & 0x1))
extern "C" bool MSHookFunction(void* symbol, void* replace, void** result);
#elif defined __aarch64__
#define __64BIT
    #define DETHUMB(_a)
    #define RETHUMB(_a)
    #define THUMBMODE(_a) (false)
    #define cacheflush(c, n, zeroarg) __builtin___clear_cache((char*)(c), (char*)(n))
#else
    #error This lib is supposed to work on ARM only!
#endif

//#if VER_x32
#define GET_LR(dwRetAddr) \
    do { \
        uintptr_t lr = reinterpret_cast<uintptr_t>(__builtin_return_address(0)); \
        dwRetAddr = lr - g_libGTASA; \
    } while (0)
//#endif
#define DECL_HOOK(_ret, _name, ...)                             \
    _ret (*_name)(__VA_ARGS__);                                 \
    _ret HookOf_##_name(__VA_ARGS__)
#define DECL_HOOKi(_name, ...)                                  \
    int (*_name)(__VA_ARGS__);                                  \
    int HookOf_##_name(__VA_ARGS__)

#define DECL_HOOKv(_name, ...)                                  \
        void (*_name)(__VA_ARGS__);                                 \
        void HookOf_##_name(__VA_ARGS__)

#define SET_TO(__a1, __a2) *(void**)&(__a1) = (void*)(__a2)

#include "shadowhook.h"
#include "asm32.h"
#include "asm64.h"
#include "../GlossHook/include/Gloss.h"
#include "StackTrace.h"

class CHook {
public:
    static inline void* lib;

public:

    static uintptr_t FindLib(const char* libname)
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

    static void InitHookStuff() {
        lib = dlopen("libGame.so", RTLD_LAZY);
    }

    static void UnFuck(uintptr_t ptr, uint64_t len = PAGE_SIZE) {
#if VER_x32
        if(mprotect((void*)(ptr & 0xFFFFF000), len, PROT_READ | PROT_WRITE | PROT_EXEC) == 0)
            return;

        mprotect((void*)(ptr & 0xFFFFF000), len, PROT_READ | PROT_WRITE);
#else
        if(mprotect((void*)(ptr & 0xFFFFFFFFFFFFF000), len, PROT_READ | PROT_WRITE | PROT_EXEC) == 0)
            return;

        mprotect((void*)(ptr & 0xFFFFFFFFFFFFF000), len, PROT_READ | PROT_WRITE);
#endif
    }

    static uintptr_t GetAddrBaseXDL(uintptr_t addr)
    {
#ifdef __XDL
        xdl_info_t info;
        void* cache = NULL;
        if(!xdl_addr((void*)addr, &info, &cache)) return 0;
        xdl_addr_clean(&cache);
        return (uintptr_t)info.dli_saddr;
#endif
        return 0;
    }
    static int WriteB(uintptr_t addr, uintptr_t dest) // B instruction
    {
#ifdef __32BIT
        if(THUMBMODE(addr))
        {
            intptr_t calc = ((intptr_t)dest - (intptr_t)addr) / 2;
            if(calc >= -1 && calc < 1) return 0;
            else if(calc < -1)
            {
                goto do_b_w; // Only B.W supports negative offset!
            }
            else if(calc == 1)
            {
                return WriteNOP(addr, 1);
            }

            calc -= 2; // PC
            if(calc <= 255)
            {
                uint16_t newDest = 0xE000 | (calc & 0x7FF);
                Write(DETHUMB(addr), newDest);
                return 2;
            }
            else
            {
              do_b_w:
                // B.W goes here!
                uint32_t imm_val = (dest - addr - 4) & 0x7FFFFF;
                uint32_t newDest = ((imm_val & 0xFFF) >> 1 | 0xB800) << 16 | (imm_val >> 12 | 0xF000);

                Write(DETHUMB(addr), newDest);
                return 4;
            }
        }
        else
        {
            // Probably not working!
            uint32_t newDest = 0xEA000000 | ((dest - addr - 4) >> 2) & 0x00FFFFFF;
            Write(addr, (uintptr_t)&newDest, sizeof(uint32_t));
            return 4;
        }
#elif defined __64BIT
        // Probably, the range is [-0xFFFFFF, 0xFFFFFFF]
        uint32_t newDest = 0x14000000 | (((dest - addr) >> 2) & 0x03FFFFFF);
        Write(addr, (uintptr_t)&newDest, sizeof(uint32_t));
        return 4;
#endif
    }
    static int WriteNOP(uintptr_t addr, size_t count)
    {
#ifdef __32BIT
        if(THUMBMODE(addr))
        {
            addr = DETHUMB(addr);
            int bytesCount = 2 * count;
            uintptr_t endAddr = addr + bytesCount;
            Unprotect(addr, bytesCount);
            for (uintptr_t p = addr; p != endAddr; p += 2)
            {
                (*(char*)(p + 0)) = 0x00;
                (*(char*)(p + 1)) = 0xBF;
            }
            cacheflush(addr, endAddr, 0);
            return bytesCount;
        }
        else
        {
            int bytesCount = 4 * count;
            uintptr_t endAddr = addr + bytesCount;
            Unprotect(addr, bytesCount);
            for (uintptr_t p = addr; p != endAddr; p += 4)
            {
                (*(char*)(p + 0)) = 0x00;
                (*(char*)(p + 1)) = 0xF0;
                (*(char*)(p + 2)) = 0x20;
                (*(char*)(p + 3)) = 0xE3;
            }
            cacheflush(addr, endAddr, 0);
            return bytesCount;
        }
#elif defined __64BIT
        int bytesCount = 4 * count;
        uintptr_t endAddr = addr + bytesCount;
        Unprotect(addr, bytesCount);
        for (uintptr_t p = addr; p != endAddr; p += 4)
        {
            (*(char*)(p + 0)) = 0x1F;
            (*(char*)(p + 1)) = 0x20;
            (*(char*)(p + 2)) = 0x03;
            (*(char*)(p + 3)) = 0xD5;
        }
        cacheflush(addr, endAddr, 0);
        return bytesCount;
#endif
    }
    template<typename Addr>
    static void NOP(Addr adr, size_t count)
    {
        // fully check
        auto addr = (uintptr_t)(adr);
#if VER_x32
        int bytesCount = 2 * count;
        uintptr_t endAddr = addr + bytesCount;
        UnFuck(addr, bytesCount);
        for (uintptr_t p = addr; p != endAddr; p += 2)
        {
            (*(char*)(p + 0)) = 0x00;
            (*(char*)(p + 1)) = 0xBF;
        }
        cacheflush(addr, endAddr, 0);

#else
        if(count > 1)
            count /= 2;

        int bytesCount = 4 * count;
        uintptr_t endAddr = addr + bytesCount;
        UnFuck(addr, bytesCount);
        for (uintptr_t p = addr; p != endAddr; p += 4)
        {
            (*(char*)(p + 0)) = 0x1F;
            (*(char*)(p + 1)) = 0x20;
            (*(char*)(p + 2)) = 0x03;
            (*(char*)(p + 3)) = 0xD5;
        }
        cacheflush(addr, endAddr, 0);
#endif
    }

    static void RET(const char* sym)
    {
        // fully check
        auto addr = getSym(sym);

        RET(addr);
    }

    static void RET(uintptr_t addr)
    {
        // fully check
        #if VER_x32
        if(THUMBMODE(addr))
        {
            WriteMemory(DETHUMB(addr), (uintptr_t)"\x70\x47", 2);
        }
        else
        {
            WriteMemory(addr, (uintptr_t)"\x1E\xFF\x2F\xE1", 4);
        }
        #else
            WriteMemory(addr, (uintptr_t)"\xC0\x03\x5F\xD6", 4);
        #endif
    }

    template <typename Src>
    static void WriteMemory(uintptr_t dest, Src src, uint64_t size)
    {
        UnFuck(dest, size);
        memcpy((void*)dest, (void*)src, size);

        cacheflush(dest, dest + size, 0);
    }

    static void ReadMemory(uintptr_t addr, void* dest, size_t size)
    {
        UnFuck(addr, size);
        memcpy(dest, (void*)addr, size);
    }

    template <typename Src>
    static void Write(uintptr_t dest, Src src, uint64_t size = 0)
    {   
	    if(size <= 0)
            size = sizeof(Src);
        
        CHook::WriteMemory(dest, &src, size);
    }

    static void Write32(uintptr_t dest, uint32_t v)
    {
        uint32_t vPtr = v;

        CHook::WriteMemory(dest, (uintptr_t)&vPtr, 4);
    }

    static uintptr_t getSym(const char* sym)
    {

        auto res = (uintptr_t)dlsym(lib, sym);
        if(res == 0) {
            FLog("[ERROR]: Failed to search for libraries: %s", sym);
            exit(0);
            return 0;
        }
        return res;
    }

    template<typename T, typename A>
    static void SetVTable(T* obj, A newVTable) {
        *reinterpret_cast<uintptr_t**>(obj) = (uintptr_t*)(newVTable);
    }

    template<typename Ret, typename T, typename... Args>
    static Ret CallVTableFunctionByNum(T* obj, int num, Args... args) {
        auto vtable = *reinterpret_cast<uintptr_t**>(obj);

        auto func = reinterpret_cast<Ret(*)(T*, Args...)>(vtable[num]);

        return func(obj, std::forward<Args>(args)...);
    }

    template <typename Ret, typename... Args>
    static inline Ret CallFunction(uintptr_t address, Args... args)
    {
        return (( Ret(*)(Args...))(address) )(args...);
    }

    template <typename Ret, typename... Args>
    static Ret CallFunction(const char* sym, Args... args) {
        static std::unordered_map<std::string, uintptr_t> addr_map;

        auto it = addr_map.find(sym);
        uintptr_t addr;

        if (it == addr_map.end()) {
            addr = (uintptr_t)dlsym(lib, sym);
            if (addr == 0) {
                FLog("[ERROR]: Function not found: %s", sym);
                exit(0);
            }
            addr_map[sym] = addr;
        } else {
            addr = it->second;
        }

        return ((Ret(*)(Args...))(addr))(args...);
    }
    bool hookBranchLinkInternal(void* addr, void* func, void** original)
    {
        if (addr == NULL || func == NULL || addr == func) return false;
            #ifdef __32BIT
                i_set mode = I_ARM;
                if (THUMBMODE(addr)) mode = I_THUMB;
            #else
                i_set mode = I_ARM64;
            #endif
            return GlossHookBranchBL(addr, func, original, mode) != NULL;
        return false;
    }


    template<class A, class B>
    bool HookBL(A addr, B func) { return hookBranchLinkInternal((void*)addr, (void*)func, (void**)NULL); }
    template<class A, class B, class C>
    bool HookBL(A addr, B func, C original) { return hookBranchLinkInternal((void*)addr, (void*)func, (void**)original); }


    template <typename Addr, typename Func, typename Orig>
    static void InstallPLT(Addr addr, Func hook_func, Orig* orig_func)
    {
        UnFuck(addr);

        *orig_func = reinterpret_cast<Orig>(*(uintptr_t*)addr);

        *(uintptr_t*)addr = reinterpret_cast<uintptr_t>(hook_func);
    }

    template <typename Addr, typename Func>
    static void InstallPLT(Addr addr, Func hook_func)
    {
        UnFuck(addr);
        *(uintptr_t*)addr = reinterpret_cast<uintptr_t>(hook_func);
    }

    template <typename Func, typename Orig>
    static void InlineHook(const char* sym, Func func, Orig orig)
    {
        shadowhook_hook_sym_name(
                "libGame.so",
                sym,
                (void *)func,
                (void **)orig);
    }



    template <typename Ptr, typename Func, typename Orig>
    static void InlineHook(Ptr ptr, Func func, Orig orig)
    {
        shadowhook_hook_func_addr(
                (void*)ptr,
                (void *)func,
                (void **)orig);
    }
    static void codeInject64(uintptr_t addr, uintptr_t func, int reg)
    {
        // ARM64 instructions are 4 bytes each.
        // We need 2 instructions (8 bytes) + 1 pointer (8 bytes) = 16 bytes total.
        uint32_t instructions[4];

        // 1. LDR reg, #8 (Load literal: PC + 8 bytes)
        // Hex: 0x58000040 | reg
        instructions[0] = 0x58000040 | (reg & 0x1F);

        // 2. BR reg (Branch to Register)
        // Hex: 0xD61F0000 | (reg << 5)
        instructions[1] = 0xD61F0000 | ((reg & 0x1F) << 5);

        // 3. The 64-bit destination address (8 bytes)
        *(uintptr_t*)&instructions[2] = func;

        // Write 16 bytes total
        WriteMemory(addr, (uintptr_t)instructions, 16);
    }
    template <typename Func>
    static void Redirect(const char* sym, Func func)
    {
        auto addr = getSym(sym);

        Redirect(addr, func);
    }
    template <typename Ptr, typename Func>
    static void Redirect(Ptr ptr, Func func)
    {
#ifdef __32BIT
        uint32_t hook[2] = {0xE51FF004, reinterpret_cast<uintptr_t>(func)};
        if (THUMBMODE(ptr)) {
            ptr &= ~0x1;
            if (ptr & 0x2) {
                NOP(RETHUMB(ptr), 1);
                ptr += 2;
            }
            hook[0] = 0xF000F8DF;
        }
        WriteMemory(DETHUMB(ptr), reinterpret_cast<uintptr_t>(hook), sizeof(hook));
#elif defined __64BIT
        UnFuck(ptr, 16);
        uint64_t hook[2] = {0xD61F022058000051, reinterpret_cast<uintptr_t>(func)};
        WriteMemory(ptr, reinterpret_cast<uintptr_t>(hook), sizeof(hook));
#endif
    }
};