//
// Created by x1y2z on 19.04.2023.
//
#include "rwplcore.h"
#include "../../main.h"
#include "../vendor/armhook/patch.h"

RwMatrix* RwMatrixUpdate(RwMatrix* matrix) {
    matrix->flags &= 0xFFFDFFFC;
    return matrix;
}

RwBool RwMatrixDestroy(RwMatrix* mpMat) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x75F948, mpMat);
}

RwV3d* RwV3dTransformPoint(RwV3d* pointOut, const RwV3d* pointIn, const RwMatrix* matrix) {
    return CHook::CallFunction<RwV3d*>(g_libGTASA + 0x762D3C, pointOut, pointIn, matrix);
}

RwV3d* RwV3dTransformPoints(RwV3d* pointsOut, const RwV3d* pointsIn, RwInt32 numPoints, const RwMatrix* matrix) {
    return CHook::CallFunction<RwV3d*>(g_libGTASA + 0x762D7C, pointsOut, pointsIn, numPoints, matrix);
}

RwMatrix* RwMatrixOrthoNormalize(RwMatrix* matrixOut, const RwMatrix* matrixIn) {
    return CHook::CallFunction<RwMatrix*>(g_libGTASA + 0x75E7D4, matrixOut, matrixIn);
}

RwUInt32 RwStreamRead(RwStream* stream, void* buffer, RwUInt32 length) {
    return CHook::CallFunction<RwUInt32>(g_libGTASA + 0x761300, stream, buffer, length);
}

RwStream* RwStreamOpen(RwStreamType type, RwStreamAccessType accessType, const void* data) {
    return CHook::CallFunction<RwStream*>(g_libGTASA + 0x7618FC, type, accessType, data);
}

RwBool RwStreamClose(RwStream* stream, void* data) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x7617D0, stream, data);
}

RwMatrix* RwMatrixTransform(RwMatrix* matrix, const RwMatrix* transform, RwOpCombineType combineOp) {
    return CHook::CallFunction<RwMatrix*>(g_libGTASA + 0x75F4BC, matrix, transform, combineOp);
}

RwMatrix* RwMatrixCreate() {
    return CHook::CallFunction<RwMatrix*>(g_libGTASA + 0x75F984);
}

RwMatrix* RwMatrixRotate(RwMatrix* pMat, CVector* axis, float angle)
{
    return CHook::CallFunction<RwMatrix*>(g_libGTASA + 0x75ED70, pMat, axis, angle, rwCOMBINEPRECONCAT);
}

RwMatrix* RwMatrixTranslate(RwMatrix *matrix, const RwV3d *translation, RwOpCombineType combineOp)
{
    return CHook::CallFunction<RwMatrix*>("_Z17RwMatrixTranslateP11RwMatrixTagPK5RwV3d15RwOpCombineType", matrix, translation, combineOp);
}