#include "BuildingRemoval.h"
#include "COcclusion.h"
#include "game/Pools.h"
#include "game/Pool.h"
#include <cmath>

void CBuildingRemoval::RemoveBuildingByPtr(CEntityGTA* pEntity) {
    if (!pEntity) return;

    // Move entity underground
    VECTOR newPos = pEntity->GetPosition();
    newPos.Z -= 2000.0f;
    pEntity->vPos = newPos;

    // Set removal flags
    pEntity->nEntityFlags.m_bRemoveFromWorld = true;
    pEntity->byteAreaCode = 1;
    pEntity->nEntityFlags.m_bUsesCollision = false;

    // Handle matrix position
    if (pEntity->mat) {
        VECTOR& matrixPos = pEntity->mat->pos;
        matrixPos.Z -= 2000.0f;
    }
}

bool CBuildingRemoval::IsEntityValidForRemoval(CEntityGTA* entity) {
    if (!entity) return false;

    // Check if already removed or not visible
    if (entity->nEntityFlags.m_bRemoveFromWorld || !entity->nEntityFlags.m_bIsVisible) {
        return false;
    }

    // VTable validation for additional safety (optional but recommended)
    auto vtable = *reinterpret_cast<uintptr_t*>(entity);
    vtable -= g_libGTASA;

    if (vtable == 0x808818) return false;
    return true;
}

float CBuildingRemoval::GetDistanceBetween3DPoints(const VECTOR* point1, const VECTOR* point2) {
    if (!point1 || !point2) return NAN; // Return NAN or max float

    float dx = point1->X - point2->X;
    float dy = point1->Y - point2->Y;
    float dz = point1->Z - point2->Z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

void CBuildingRemoval::RemoveOccluders(const VECTOR& position, float radius) {
    for (int32_t i = 0; i < COcclusion::NumOccludersOnMap; i++) {
        COccluder& occluder = COcclusion::aOccluders[i];

        VECTOR occluderPos;
        occluderPos.X = (float)occluder.iCenterX * 0.25f;
        occluderPos.Y = (float)occluder.iCenterY * 0.25f;
        occluderPos.Z = (float)occluder.iCenterZ * 0.25f;

        if (GetDistanceBetween3DPoints(&position, &occluderPos) < radius) {
            occluder.iCenterX = 0;
            occluder.iCenterY = 0;
            occluder.iCenterZ = 0;

            occluder.iLength = 0;
            occluder.iWidth = 0;
            occluder.iHeight = 0;

            occluder.iRotX = 0;
            occluder.iRotY = 0;
            occluder.iRotZ = 0;
        }
    }
}

void CBuildingRemoval::ProcessRemoveBuilding(uint32_t modelId, const VECTOR& pos, float radius) {
    // Remove occluders with larger radius for safety
    RemoveOccluders(pos, 500.0f);

    // Use template function for all pools
    RemoveBuildingsInPool(GetBuildingPool(), modelId, pos, radius);
    RemoveBuildingsInPool(GetDummyPool(), modelId, pos, radius);
    RemoveBuildingsInPool(GetObjectPoolGta(), modelId, pos, radius);
}

// Template implementation for pool processing
template <typename PoolT>
void CBuildingRemoval::RemoveBuildingsInPool(PoolT* pool, uint32_t uiModel, const VECTOR& pos, float radius) {
    if (!pool) return;

    for (int i = 0; i < pool->GetSize(); i++) {
        auto* entity = pool->GetAt(i);
        if (!IsEntityValidForRemoval(entity)) continue;

        // Check model match (or -1 for all models)
        if (entity->nModelIndex == uiModel || uiModel == static_cast<uint32_t>(-1)) {
            float distance = GetDistanceBetween3DPoints(&pos, &entity->GetPosition());
            if (distance <= radius) {
                RemoveBuildingByPtr(entity);
            }
        }
    }
}

// Explicit template instantiations
template void CBuildingRemoval::RemoveBuildingsInPool<CPool<CBuilding>>(CPool<CBuilding>*, uint32_t, const VECTOR&, float);
template void CBuildingRemoval::RemoveBuildingsInPool<CPool<CDummy>>(CPool<CDummy>*, uint32_t, const VECTOR&, float);
template void CBuildingRemoval::RemoveBuildingsInPool<CPool<CObjectGTA>>(CPool<CObjectGTA>*, uint32_t, const VECTOR&, float);