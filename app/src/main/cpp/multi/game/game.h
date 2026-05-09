#pragma once

#include "common.h"


#define IS_VEHICLE_MOD(x) (x >= 1000 && x <= 1193)

class CGame
{
public:
    CGame();
    ~CGame();
    void StartGame();
};