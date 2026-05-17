/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/

#include "AnimBlendAssocGroup.h"
#include "../vendor/armhook/patch.h"

// 0x4CDE70
CAnimBlendAssocGroup::CAnimBlendAssocGroup() {
//    m_pAnimBlock = nullptr;
//    m_nNumAnimations = 0;
//    m_nGroupID = 0xFFFFFFFF;
    CHook::CallFunction<void>(g_libGTASA + 0x36C50C, this);
    //plugin::CallMethod<0x4CDE70, CAnimBlendAssocGroup*>(this);
}

// 0x4CE0B0
CAnimBlendAssociation* CAnimBlendAssocGroup::CopyAnimation(const char* AnimName) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x36D02C, this, AnimName);
}

// 0x4CE130
CAnimBlendAssociation* CAnimBlendAssocGroup::CopyAnimation(uint32 ID) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x36D0C4, this, ID);
}

// 0x4CE220
void CAnimBlendAssocGroup::CreateAssociations(const char* szBlockName) {
    return CHook::CallFunction<void>(g_libGTASA + 0x36C6EC, szBlockName);
}

// 0x4CE6E0
void CAnimBlendAssocGroup::CreateAssociations(const char* AnimName, RpClump* clump, char** arg3, int32 NumAnimations) {
    CHook::CallFunction<void>(g_libGTASA + 0x36CD74, this, AnimName, clump, arg3, NumAnimations);
}

// 0x4CE3B0
void CAnimBlendAssocGroup::CreateAssociations(const char* AnimName, const char* arg2, const char* arg3, uint32 strStorageSz) {
    CHook::CallFunction<void>(g_libGTASA + 0x36C904, this, AnimName, arg2, arg3, strStorageSz);
}

// 0x4CDFF0
void CAnimBlendAssocGroup::DestroyAssociations() {
    CHook::CallFunction<void>(g_libGTASA + 0x36C59C, this);
}

CAnimBlendStaticAssociation* CAnimBlendAssocGroup::GetAnimation(const char* AnimName) {
    return CHook::CallFunction<CAnimBlendStaticAssociation*>(g_libGTASA + 0x36CFC0, this, AnimName);
}

CAnimBlendStaticAssociation* CAnimBlendAssocGroup::GetAnimation(uint32 id) {
    return &this->m_pAssociations[id - this->m_nIdOffset];
}

// 0x4CE1B0
uint32 CAnimBlendAssocGroup::GetAnimationId(const char* AnimName) {
    return CHook::CallFunction<uint32>(g_libGTASA + 0x36D130, this, AnimName);
}

// 0x4CDFB0
void CAnimBlendAssocGroup::InitEmptyAssociations(RpClump* clump) {
    CHook::CallFunction<void>(g_libGTASA + 0x36CF4C, this, clump);
}

// 0x4CE1D0
CAnimBlendAssocGroup::~CAnimBlendAssocGroup() {
    CHook::CallFunction<void>(g_libGTASA + 0x36C520, this);
}

