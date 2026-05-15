
#include "rphanim.h"
#include "game/common.h"
#include "../vendor/armhook/patch.h"

void RpHAnimHierarchySetFreeListCreateParams(RwInt32 blockSize, RwInt32 numBlocksToPrealloc) {
    CHook::CallFunction<void>(g_libGTASA + 0x76DE30, blockSize, numBlocksToPrealloc);
}

RpHAnimHierarchy* RpHAnimHierarchyCreate(RwInt32 numNodes, RwUInt32* nodeFlags, RwInt32* nodeIDs, RpHAnimHierarchyFlag flags, RwInt32 maxInterpKeyFrameSize) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x76E6F8, numNodes, nodeFlags, nodeIDs, flags, maxInterpKeyFrameSize);
}

RpHAnimHierarchy* RpHAnimHierarchyCreateFromHierarchy(RpHAnimHierarchy* hierarchy, RpHAnimHierarchyFlag flags, RwInt32 maxInterpKeyFrameSize) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x76EA84, hierarchy, flags, maxInterpKeyFrameSize);
}

RpHAnimHierarchy* RpHAnimHierarchyDestroy(RpHAnimHierarchy* hierarchy) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x2554D4, hierarchy);
}

RpHAnimHierarchy* RpHAnimHierarchyCreateSubHierarchy(RpHAnimHierarchy* parentHierarchy, RwInt32 startNode, RpHAnimHierarchyFlag flags, RwInt32 maxInterpKeyFrameSize) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x76E8DC, parentHierarchy, startNode, flags, maxInterpKeyFrameSize);
}

RpHAnimHierarchy* RpHAnimHierarchyAttach(RpHAnimHierarchy* hierarchy) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x76EC14, hierarchy);
}

RpHAnimHierarchy* RpHAnimHierarchyDetach(RpHAnimHierarchy* hierarchy) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x76ED44, hierarchy);
}

RpHAnimHierarchy* RpHAnimHierarchyAttachFrameIndex(RpHAnimHierarchy* hierarchy, RwInt32 nodeIndex) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x76ED78, hierarchy, nodeIndex);
}

RpHAnimHierarchy* RpHAnimHierarchyDetachFrameIndex(RpHAnimHierarchy* hierarchy, RwInt32 nodeIndex) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x76EF04, hierarchy, nodeIndex);
}

RwBool RpHAnimFrameSetHierarchy(RwFrame* frame, RpHAnimHierarchy* hierarchy) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x76EF20, frame, hierarchy);
}

RpHAnimHierarchy* RpHAnimFrameGetHierarchy(RwFrame* frame) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x76EF50, frame);
}

RwMatrix* RpHAnimHierarchyGetMatrixArray(RpHAnimHierarchy* hierarchy) {
    return CHook::CallFunction<RwMatrix*>(g_libGTASA + 0x76EF18, hierarchy);
}

RwBool RpHAnimHierarchyUpdateMatrices(RpHAnimHierarchy* hierarchy) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x76EFD0, hierarchy);//
}

RwInt32 RpHAnimIDGetIndex(RpHAnimHierarchy* hierarchy, RwInt32 ID) {
    return CHook::CallFunction<RwInt32>(g_libGTASA + 0x76EF98, hierarchy, ID);
}

RwBool RpHAnimPluginAttach() {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x76DE44);
}

void RpHAnimKeyFrameApply(void* matrix, void* voidIFrame) {
    CHook::CallFunction<void>(g_libGTASA + 0x71FFA4, matrix, voidIFrame);
}

void RpHAnimKeyFrameBlend(void* voidOut, void* voidIn1, void* voidIn2, RwReal alpha) {
    CHook::CallFunction<void>(g_libGTASA + 0x72058C, voidOut, voidIn1, voidIn2, alpha);
}

void RpHAnimKeyFrameInterpolate(void* voidOut, void* voidIn1, void* voidIn2, RwReal time, void* customData) {
    CHook::CallFunction<void>(g_libGTASA + 0x72006C, voidOut, voidIn1, voidIn2, time, customData);
}

void RpHAnimKeyFrameAdd(void* voidOut, void* voidIn1, void* voidIn2) {
    CHook::CallFunction<void>(g_libGTASA + 0x720D5C, voidOut, voidIn1, voidIn2);
}

void RpHAnimKeyFrameMulRecip(void* voidFrame, void* voidStart) {
    CHook::CallFunction<void>(g_libGTASA + 0x720C64, voidFrame, voidStart);
}

RtAnimAnimation* RpHAnimKeyFrameStreamRead(RwStream* stream, RtAnimAnimation* animation) {
    return CHook::CallFunction<RtAnimAnimation*>(g_libGTASA + 0x720A9C, stream, animation);
}

RwBool RpHAnimKeyFrameStreamWrite(const RtAnimAnimation* animation, RwStream* stream) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x720B98, animation, stream);
}

RwInt32 RpHAnimKeyFrameStreamGetSize(const RtAnimAnimation* animation) {
    return CHook::CallFunction<RwInt32>(g_libGTASA + 0x720C54, animation);
}

RwBool RpHAnimFrameSetID(RwFrame* frame, RwInt32 id) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x76EF68, frame, id);
}

RwInt32 RpHAnimFrameGetID(RwFrame* frame) {
    return CHook::CallFunction<RwInt32>(g_libGTASA + 0x76EF84, frame);
}
