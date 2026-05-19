#include "../main.h"
#include "../game/game.h"
#include "../net/netgame.h"

#include "StreamAtObject.h"

#include "StreamInfo.h"

extern CNetGame *pNetGame;

StreamAtObject::StreamAtObject(const uint32_t color, std::string name,
                               const float distance, const WORD objectId) noexcept
    : LocalStream(StreamType::LocalStreamAtObject, color, std::move(name), distance)
    , objectId(objectId)
{}

void StreamAtObject::Tick() noexcept
{

}

void StreamAtObject::OnChannelCreate(const Channel& channel) noexcept
{

}
