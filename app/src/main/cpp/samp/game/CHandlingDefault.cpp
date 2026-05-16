#include "../main.h"
#include "game.h"
#include "../net/netgame.h"
#include "CHandlingDefault.h"

tHandlingData CHandlingDefault::m_aDefaultModelHandlings[MAX_VEHICLE_MODELS];
bool CHandlingDefault::bIsSlotUsed[MAX_VEHICLE_MODELS]{false};

void CHandlingDefault::GetDefaultHandling(uint16_t usHandlingID, tHandlingData* tDest)
{
    FLog("copy handling %d", usHandlingID);

    if(bIsSlotUsed[usHandlingID] == true)
        memcpy((void*)tDest, &m_aDefaultModelHandlings[usHandlingID], sizeof(tHandlingData));
    else
        FLog("Warning. No handling slon n = %d", usHandlingID);
}

void CHandlingDefault::FillDefaultHandling(uint16_t usHandlingID, tHandlingData* pSrc)
{
    bIsSlotUsed[usHandlingID] = true;

    memset(&m_aDefaultModelHandlings[usHandlingID], 0, sizeof(tHandlingData));

    if (pSrc)
    {
        memcpy(&m_aDefaultModelHandlings[usHandlingID], pSrc, sizeof(tHandlingData));
    }
}
