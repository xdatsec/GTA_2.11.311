
#include "../vendor/armhook/patch.h"
#include "CCAM/Cameras.h"
#include "game.h"
#include "net/netgame.h"

// Delete pickup's object (CObject)
void CPickup::GetRidOfObjects() {
    CHook::CallFunction<void>(g_libGTASA + 0x3F0DE4, this);

    // Need destructor :c
//    if (m_pObject) {
//        CWorld::Remove(m_pObject);
//        delete m_pObject;
//        m_pObject = nullptr;
//    }
}

// Creates an object (CObject) for pickup. slotIndex - object to replace; use -1 (or any negative value) to create a new object
void CPickup::GiveUsAPickUpObject(CObjectGTA** obj, int32 slotIndex) {
    CHook::CallFunction<void>(g_libGTASA + 0x3F0A98, this, obj, slotIndex);
}

// Is pickup visible (checks if distance between pickup and camera is shorter than 100 units)
bool CPickup::IsVisible() {
    CCameras& TheCamera = *reinterpret_cast<CCameras*>(g_libGTASA + 0x9DF030);
    VECTOR campos = TheCamera.GetPosition();

    return DistanceBetweenPoints2D(GetPosn2D(), AsCVector(TheCamera.GetPosition())) < 100.0f;
}
inline float Distance2DSq(const VECTOR& a, const VECTOR& b)
{
    float dx = b.X - a.X;
    float dy = b.Y - a.Y;
    return dx*dx + dy*dy;
}
#include "game.h"
#include "net/netgame.h"
extern CNetGame *pNetGame;
bool CPickup::Update() {
    if(!pNetGame || !pNetGame->GetPlayerPool())
        return false;

    CPlayerPed * ped = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();

    auto oldState = m_bIsPicked;
    auto pickupPos = GetPosn();

    auto checkPickup = [&]() {

        if (m_nPickupType == PICKUP_2P) {
            if (!ped->IsInVehicle())
                return false;
            CEntityGTA * veh = reinterpret_cast<CEntityGTA *>(ped->m_pPed->pVehicle);
            auto vehPos =  veh->GetPosition();


            if (Distance2DSq(AsVECTOR(pickupPos), vehPos) < (1.0 * 1.0)) {
                Remove();
                return true;
            }

            return false;
        }
        else {
            MATRIX4X4 matpos;
            ped->GetMatrix(&matpos);
            if( Distance2DSq(AsVECTOR(pickupPos), matpos.pos) < (1.5 * 1.5))
            {
                // enable for debug LOGI("DISTANCE NEAR");
                return true;
            }
            // enable for debugLOGI("DISTANCE NOT NEAR");
            return false;
        }
    };

    m_bIsPicked = checkPickup();

    if (!oldState && m_bIsPicked)
        return true;

    return false;
}


void CPickup::Remove() {
    CHook::CallFunction<void>(g_libGTASA + 0x3F0614, this);
}
