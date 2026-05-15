#pragma once
#include "../game/common.h"
struct RemoveBuildingInfo {
    uint32_t modelId;
    VECTOR position;
    float radius;
};

class CBuildingRemoval {
public:
    static constexpr int MAX_REMOVALS = 1200;
    static inline RemoveBuildingInfo m_RemoveBuildings[MAX_REMOVALS];
    static inline int m_TotalRemovedObjects = 0;

public:
    static void ProcessRemoveBuilding(uint32_t modelId, const VECTOR& pos, float radius);
    static void RemoveOccluders(const VECTOR& position, float radius);

    static void RemoveBuildingByPtr(CEntityGTA* pEntity);
    static bool IsEntityValidForRemoval(CEntityGTA* entity);
    static float GetDistanceBetween3DPoints(const VECTOR* point1, const VECTOR* point2);

    // Helper template for pool processing
    template <typename PoolT>
    static void RemoveBuildingsInPool(PoolT* pool, uint32_t uiModel, const VECTOR& pos, float radius);
};