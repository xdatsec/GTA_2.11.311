
#include "rphanim.h"
#include "game/common.h"
#include "../vendor/armhook/patch.h"

void RpHAnimHierarchySetFreeListCreateParams(RwInt32 blockSize, RwInt32 numBlocksToPrealloc) {
    CHook::CallFunction<void>(g_libGTASA + 0x7DA60, blockSize, numBlocksToPrealloc);
}

RpHAnimHierarchy* RpHAnimHierarchyCreate(RwInt32 numNodes, RwUInt32* nodeFlags, RwInt32* nodeIDs, RpHAnimHierarchyFlag flags, RwInt32 maxInterpKeyFrameSize) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x77E2FC, numNodes, nodeFlags, nodeIDs, flags, maxInterpKeyFrameSize);
}

RpHAnimHierarchy* RpHAnimHierarchyCreateFromHierarchy(RpHAnimHierarchy* hierarchy, RpHAnimHierarchyFlag flags, RwInt32 maxInterpKeyFrameSize) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x77E69C, hierarchy, flags, maxInterpKeyFrameSize);
}

RpHAnimHierarchy* RpHAnimHierarchyDestroy(RpHAnimHierarchy* hierarchy) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x77E458, hierarchy);
}

RpHAnimHierarchy* RpHAnimHierarchyCreateSubHierarchy(RpHAnimHierarchy* parentHierarchy, RwInt32 startNode, RpHAnimHierarchyFlag flags, RwInt32 maxInterpKeyFrameSize) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x77E4F4, parentHierarchy, startNode, flags, maxInterpKeyFrameSize);
}

RpHAnimHierarchy* RpHAnimHierarchyAttach(RpHAnimHierarchy* hierarchy) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x77E818, hierarchy);
}

RpHAnimHierarchy* RpHAnimHierarchyDetach(RpHAnimHierarchy* hierarchy) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x77E950, hierarchy);
}

RpHAnimHierarchy* RpHAnimHierarchyAttachFrameIndex(RpHAnimHierarchy* hierarchy, RwInt32 nodeIndex) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x77E984, hierarchy, nodeIndex);
}

RpHAnimHierarchy* RpHAnimHierarchyDetachFrameIndex(RpHAnimHierarchy* hierarchy, RwInt32 nodeIndex) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x77EB14, hierarchy, nodeIndex);
}

RwBool RpHAnimFrameSetHierarchy(RwFrame* frame, RpHAnimHierarchy* hierarchy) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x77EB34, frame, hierarchy);
}

RpHAnimHierarchy* RpHAnimFrameGetHierarchy(RwFrame* frame) {
    return CHook::CallFunction<RpHAnimHierarchy*>(g_libGTASA + 0x77EB64, frame);
}

RwMatrix* RpHAnimHierarchyGetMatrixArray(RpHAnimHierarchy* hierarchy) {
    return CHook::CallFunction<RwMatrix*>(g_libGTASA + 0x77EB2C, hierarchy);
}

RwBool RpHAnimHierarchyUpdateMatrices(RpHAnimHierarchy* hierarchy) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x77EBE0, hierarchy);//
}

RwInt32 RpHAnimIDGetIndex(RpHAnimHierarchy* hierarchy, RwInt32 ID) {
    return CHook::CallFunction<RwInt32>(g_libGTASA + 0x77EBAC, hierarchy, ID);
}

RwBool RpHAnimPluginAttach() {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x77DA74);
}

void RpHAnimKeyFrameApply(void* matrix, void* voidIFrame) {
    CHook::CallFunction<void>(g_libGTASA + 0x72FDFC, matrix, voidIFrame);
}

void RpHAnimKeyFrameBlend(void* voidOut, void* voidIn1, void* voidIn2, RwReal alpha) {
    CHook::CallFunction<void>(g_libGTASA + 0x7303B0, voidOut, voidIn1, voidIn2, alpha);
}

void RpHAnimKeyFrameInterpolate(void* voidOut, void* voidIn1, void* voidIn2, RwReal time, void* customData) {
    CHook::CallFunction<void>(g_libGTASA + 0x72FEC0, voidOut, voidIn1, voidIn2, time, customData);
}

void RpHAnimKeyFrameAdd(void* voidOut, void* voidIn1, void* voidIn2) {
    CHook::CallFunction<void>(g_libGTASA + 0x730B0C, voidOut, voidIn1, voidIn2);
}

void RpHAnimKeyFrameMulRecip(void* voidFrame, void* voidStart) {
    CHook::CallFunction<void>(g_libGTASA + 0x730A50, voidFrame, voidStart);
}

RtAnimAnimation* RpHAnimKeyFrameStreamRead(RwStream* stream, RtAnimAnimation* animation) {
    return CHook::CallFunction<RtAnimAnimation*>(g_libGTASA + 0x73088C, stream, animation);
}

RwBool RpHAnimKeyFrameStreamWrite(const RtAnimAnimation* animation, RwStream* stream) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x730984, animation, stream);
}

RwInt32 RpHAnimKeyFrameStreamGetSize(const RtAnimAnimation* animation) {
    return CHook::CallFunction<RwInt32>(g_libGTASA + 0x730A40, animation);
}

RwBool RpHAnimFrameSetID(RwFrame* frame, RwInt32 id) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x77EB7C, frame, id);
}

RwInt32 RpHAnimFrameGetID(RwFrame* frame) {
    return CHook::CallFunction<RwInt32>(g_libGTASA + 0x77EB98, frame);
}
