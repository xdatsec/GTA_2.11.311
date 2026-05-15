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
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x7500A4, mpMat);
}

RwV3d* RwV3dTransformPoint(RwV3d* pointOut, const RwV3d* pointIn, const RwMatrix* matrix) {
    return CHook::CallFunction<RwV3d*>(g_libGTASA + 0x75332C, pointOut, pointIn, matrix);
}

RwV3d* RwV3dTransformPoints(RwV3d* pointsOut, const RwV3d* pointsIn, RwInt32 numPoints, const RwMatrix* matrix) {
    return CHook::CallFunction<RwV3d*>(g_libGTASA + 0x75336C, pointsOut, pointsIn, numPoints, matrix);
}

RwMatrix* RwMatrixOrthoNormalize(RwMatrix* matrixOut, const RwMatrix* matrixIn) {
    return CHook::CallFunction<RwMatrix*>(g_libGTASA + 0x74EE1C, matrixOut, matrixIn);
}

RwUInt32 RwStreamRead(RwStream* stream, void* buffer, RwUInt32 length) {
    return CHook::CallFunction<RwUInt32>(g_libGTASA + 0x751788, stream, buffer, length);
}

RwStream* RwStreamOpen(RwStreamType type, RwStreamAccessType accessType, const void* data) {
    return CHook::CallFunction<RwStream*>(g_libGTASA + 0x751D84, type, accessType, data);
}

RwBool RwStreamClose(RwStream* stream, void* data) {
    return CHook::CallFunction<RwBool>(g_libGTASA + 0x751C38, stream, data);
}

RwMatrix* RwMatrixTransform(RwMatrix* matrix, const RwMatrix* transform, RwOpCombineType combineOp) {
    return CHook::CallFunction<RwMatrix*>(g_libGTASA + 0x74FB7C, matrix, transform, combineOp);
}

RwMatrix* RwMatrixCreate() {
    return CHook::CallFunction<RwMatrix*>(g_libGTASA + 0x7500E0);
}

RwMatrix* RwMatrixRotate(RwMatrix* pMat, CVector* axis, float angle)
{
    return CHook::CallFunction<RwMatrix*>(g_libGTASA + 0x74F3A0, pMat, axis, angle, rwCOMBINEPRECONCAT);
}

RwMatrix* RwMatrixTranslate(RwMatrix *matrix, const RwV3d *translation, RwOpCombineType combineOp)
{
    return CHook::CallFunction<RwMatrix*>("_Z17RwMatrixTranslateP11RwMatrixTagPK5RwV3d15RwOpCombineType", matrix, translation, combineOp);
}