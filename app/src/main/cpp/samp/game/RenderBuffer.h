//
// Created on 20.09.2023.
//

#pragma once

#include "RW/rwplcore.h"
#include "common.h"

namespace RenderBuffer {

    void InjectHooks();

    /*!
    * @brief Render out the contents of the temporary buffers as a trilist.
    * Frequently inlined!
    */
    void RenderStuffInBuffer();

    /*!
    * @brief Start storing vertices. Must be followed by `StopStoring()`
    * If there isn't enough space in the buffer for the
    * specified amount of vertices/indices the buffer is
    * rendered out first.
    *
    * @param     nIndicesNeeded  No. of indices that will be stored
    * @param     nVerticesNeeded No. of vertices that will be stored
    * @param out pFirstIndex     Pointer to the first unused index in the buffer
    * @param out pFirstVertex    Pointer to the first unused vertex in the buffer
    */
    void StartStoring(
            int32 nIndicesNeeded,
            int32 nVerticesNeeded,
            RwImVertexIndex*& pFirstIndex,
            RwIm3DVertex*& pFirstVertex
    );

    /*!
    * @brief Must be enclosed by a `StartStoring()` operation
    */
    void StopStoring();

    /*!
    * @addr 0x707790
    * @brief Reset the index/vertex buffer stored counters.
    * Frequently inlined!
    */
    void ClearRenderBuffer();

    /*!
    * @notsa
    * @brief Render out the contents of the temporary buffer as specified by the arguments. Frequently inlined!
    */
    void Render(RwPrimitiveType primType, RwMatrix* ltm = nullptr, RwUInt32 /*RwIm3DTransformFlags*/ flags = 0, bool isIndexed = true);

    /*!
    * @addr notsa
    * @brief Check if the buffer can fit `nVtxNeeded` vertices
    */
    bool CanFitVertices(int32 nVtxNeeded);

    /*
    * @addr notsa
    * @brief Push a vertex to the buffer. Not to be used with `StartStoring`! Use if the VERTEXUV flag **IS NOT** used when calling `Render`
    */

    /*!
    * @addr notsa
    * @brief Push multiple indices into the buffer. Not to be used with `StartStoring`!
    */
    void PushIndices(std::initializer_list<int32> idxs, bool useCurrentVtxAsBase);

};

constexpr int32 TOTAL_TEMP_BUFFER_INDICES = 4096;
constexpr int32 TOTAL_TEMP_BUFFER_VERTICES = 2048;

constexpr int32 TOTAL_TEMP_BUFFER_3DVERTICES = 2048;
constexpr int32 TOTAL_TEMP_BUFFER_2DVERTICES = 2048;

constexpr int32 TOTAL_RADIOSITY_VERTEX_BUFFER = 1532;

struct TempVertexBuffer {
    RwIm3DVertex m_3d[TOTAL_TEMP_BUFFER_3DVERTICES]; // For Im3D rendering
    RwIm2DVertex m_2d[TOTAL_TEMP_BUFFER_2DVERTICES]; // For Im2D rendering
};
VALIDATE_SIZE(TempVertexBuffer, 0x20000);

inline int32 uiTempBufferIndicesStored;
inline int32 uiTempBufferVerticesStored;
inline RxVertexIndex aTempBufferIndices[TOTAL_TEMP_BUFFER_INDICES];
inline TempVertexBuffer TempBufferVertices;
inline RwD3D9Vertex aRadiosityVertexBuffer[TOTAL_RADIOSITY_VERTEX_BUFFER];
