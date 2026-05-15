#pragma once

#include "common.h"

class CStoredMaterials {
    struct StoredMaterial {
        uintptr_t*  address;
        uintptr_t   value;
    };

    std::array<StoredMaterial, 10'000> list{};
    size_t count = 0;

public:
    template <typename Addr>
    void Add(Addr addr)
    {
        if (count < list.size()) {
            list[count++] = StoredMaterial{
                    reinterpret_cast<uintptr_t*>(addr),
                    *reinterpret_cast<uintptr_t*>(addr)
            };
        }
    }

    void Reset() {
        for (size_t i = 0; i < count; ++i)
        {
            *list[i].address = list[i].value;
        }
        count = 0;
    }

    auto begin() const { return list.begin(); }
    auto end() const { return list.begin() + count; }
};
