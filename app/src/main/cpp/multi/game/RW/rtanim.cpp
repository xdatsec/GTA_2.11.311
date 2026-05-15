#include "RenderWare.h"
#include "game/common.h"
#include "../vendor/armhook/patch.h"

void RtAnimAnimationFreeListCreateParams(RwInt32 blockSize, RwInt32 numBlocksToPrealloc) {
    CHook::CallFunction<void>(g_libGTASA + 0x7776E0, blockSize, numBlocksToPrealloc);
}

RwBool RtAnimInitialize() {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x7776F4);
}

RwBool RtAnimRegisterInterpolationScheme(RtAnimInterpolatorInfo* interpolatorInfo) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x7777C4, interpolatorInfo);
}

RtAnimInterpolatorInfo* RtAnimGetInterpolatorInfo(RwInt32 typeID) {
    return CHook::CallFunction<RtAnimInterpolatorInfo*>(g_libGTASA + 0x777854, typeID);
}

RtAnimAnimation* RtAnimAnimationCreate(RwInt32 typeID, RwInt32 numFrames, RwInt32 flags, RwReal duration) {
    return CHook::CallFunction<RtAnimAnimation*>(g_libGTASA + 0x777894, typeID, numFrames, flags, duration);
}

RwBool RtAnimAnimationDestroy(RtAnimAnimation* animation) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x77796C, animation);
}

RtAnimAnimation* RtAnimAnimationRead(const RwChar* filename) {
    return CHook::CallFunction<RtAnimAnimation*>(g_libGTASA + 0x777994, filename);
}

RwBool RtAnimAnimationWrite(const RtAnimAnimation* animation, const RwChar* filename) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x777BD8, animation, filename);
}

RtAnimAnimation* RtAnimAnimationStreamRead(RwStream* stream) {
    return CHook::CallFunction<RtAnimAnimation*>(g_libGTASA + 0x7779FC, stream);
}

RwBool RtAnimAnimationStreamWrite(const RtAnimAnimation* animation, RwStream* stream) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x777C28, animation, stream);
}

RwInt32 RtAnimAnimationStreamGetSize(const RtAnimAnimation* animation) {
    return CHook::CallFunction<RwInt32>(g_libGTASA + 0x777D24, animation);
}

RwUInt32 RtAnimAnimationGetNumNodes(const RtAnimAnimation* animation) {
    return CHook::CallFunction<RwUInt32>(g_libGTASA + 0x777D44, animation);
}

RtAnimInterpolator* RtAnimInterpolatorCreate(RwInt32 numNodes, RwInt32 maxInterpKeyFrameSize) {
    return CHook::CallFunction<RtAnimInterpolator*>(g_libGTASA + 0x777D88, numNodes, maxInterpKeyFrameSize);
}

void RtAnimInterpolatorDestroy(RtAnimInterpolator* anim) {
    CHook::CallFunction<void>(g_libGTASA + 0x777DFC, anim);
}

RwBool RtAnimInterpolatorSetCurrentAnim(RtAnimInterpolator* animI, RtAnimAnimation* anim) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x777E10, animI, anim);
}

RwBool RtAnimInterpolatorSetKeyFrameCallBacks(RtAnimInterpolator* anim, RwInt32 keyFrameTypeID) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x777F40, anim, keyFrameTypeID);
}

void RtAnimInterpolatorSetAnimLoopCallBack(RtAnimInterpolator* anim, RtAnimCallBack callBack, void* data) {
    CHook::CallFunction<void>(g_libGTASA + 0x777FC0, anim, callBack, data);
}

void RtAnimInterpolatorSetAnimCallBack(RtAnimInterpolator* anim, RtAnimCallBack callBack, RwReal time, void* data) {
    CHook::CallFunction<void>(g_libGTASA + (VER_x32 ? 0x1EB3DE + 1 : 0x288610), anim, callBack, time, data);
}

RwBool RtAnimInterpolatorCopy(RtAnimInterpolator* outAnim, RtAnimInterpolator* inAnim) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x777FC8, outAnim, inAnim);
}

RwBool RtAnimInterpolatorSubAnimTime(RtAnimInterpolator* anim, RwReal time) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x778004, anim, time);
}

RwBool RtAnimInterpolatorAddAnimTime(RtAnimInterpolator* anim, RwReal time) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x778174, anim, time);
}

RwBool RtAnimInterpolatorSetCurrentTime(RtAnimInterpolator* anim, RwReal time) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x778340, anim, time);
}

RwBool RtAnimAnimationMakeDelta(RtAnimAnimation* animation, RwInt32 numNodes, RwReal time) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x778374, animation, numNodes, time);
}

RwBool RtAnimInterpolatorBlend(RtAnimInterpolator* outAnim, RtAnimInterpolator* inAnim1, RtAnimInterpolator* inAnim2, RwReal alpha) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x778514, outAnim, inAnim1, inAnim2, alpha);
}

RwBool RtAnimInterpolatorAddTogether(RtAnimInterpolator* outAnim, RtAnimInterpolator* inAnim1, RtAnimInterpolator* inAnim2) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x7785BC, outAnim, inAnim1, inAnim2);
}

RtAnimInterpolator* RtAnimInterpolatorCreateSubInterpolator(RtAnimInterpolator* parentAnim, RwInt32 startNode, RwInt32 numNodes, RwInt32 maxInterpKeyFrameSize) {
    return CHook::CallFunction<RtAnimInterpolator*>(g_libGTASA + 0x778654, parentAnim, startNode, numNodes, maxInterpKeyFrameSize);
}

RwBool RtAnimInterpolatorBlendSubInterpolator(RtAnimInterpolator* outAnim, RtAnimInterpolator* inAnim1, RtAnimInterpolator* inAnim2, RwReal alpha) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x7786EC, outAnim, inAnim1, inAnim2, alpha);
}

RwBool RtAnimInterpolatorAddSubInterpolator(RtAnimInterpolator* outAnim, RtAnimInterpolator* mainAnim, RtAnimInterpolator* subAnim) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x778924, outAnim, mainAnim, subAnim);
}
