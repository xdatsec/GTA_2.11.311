//
// Created by x1y2z on 01.07.2023.
//

#include "TxdStore.h"
#include "../vendor/armhook/patch.h"

int32 CTxdStore::GetNumRefs(int32 index){
    return CHook::CallFunction<int32>(g_libGTASA + 0x602AC8, index);
}

void CTxdStore::RemoveTxd(int32 index) {
    CHook::CallFunction<void>(g_libGTASA + 0x602E7C, index);
}

void CTxdStore::InjectHooks() {
  //  CHook::Redirect(g_libGTASA, 0x0055BF14, &CTxdStore::GetNumRefs);
}

int32 CTxdStore::FindTxdSlot(const char *name) {
    return CHook::CallFunction<int32>(g_libGTASA + 0x602BA0, name);
}

int32 CTxdStore::FindTxdSlot(uint32 hash) {
    assert("NO x64 call");
}

int32 CTxdStore::AddTxdSlot(const char *name, const char *dbName, bool keepCPU) {
    return CHook::CallFunction<int32>(g_libGTASA + 0x6026FC, name, dbName, keepCPU);
}

void CTxdStore::Initialise() {
    CHook::CallFunction<void>(g_libGTASA + 0x6025C4);
}

void CTxdStore::PushCurrentTxd() {

    CHook::CallFunction<void>(g_libGTASA + 0x603020);
}

void CTxdStore::PopCurrentTxd() {
    CHook::CallFunction<void>(g_libGTASA + 0x603050);
}

void CTxdStore::SetCurrentTxd(int32 index) {
    CHook::CallFunction<void>(g_libGTASA + 0x602F60, index, nullptr);
}
