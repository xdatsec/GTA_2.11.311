//
// Created by Traw-GG on 05.10.2025.
//

#include "RealTimeShadowManager.h"
#include "../../vendor/armhook/patch.h"
#include "Mobile/MobileSettings/MobileSettings.h"
#include "Entity/CPedGTA.h"

void CRealTimeShadowManager::Init() {
    if (m_bInitialised) {
        return;
    }

    for (auto& shdw : m_apShadows) {
        shdw = new CRealTimeShadow();
        shdw->Create(true, 4, true);
    }

    m_BlurCamera.Create(6);

    m_GradientCamera.Create(6);
    m_GradientCamera.MakeGradientRaster();

    m_bInitialised = true;
}

void CRealTimeShadowManager::Exit() { // AKA `Shutdown`
    if (!m_bInitialised) {
        return;
    }

    for (auto& shdw : m_apShadows) {
        if (const auto owner = shdw->m_pOwner) {
            owner->m_pShadowData = nullptr;
        }
        delete shdw;
    }

    // Nice hack
    m_BlurCamera.Destroy();
    m_GradientCamera.Destroy();

    m_bInitialised = false;
}

void CRealTimeShadowManager::ReturnRealTimeShadow(CRealTimeShadow *shdw) {
    if (shdw->m_pOwner) {
        shdw->m_pOwner->m_pShadowData = nullptr;
    }
    shdw->m_pOwner = nullptr;
}

CRealTimeShadow* CRealTimeShadowManager::GetRealTimeShadow(CPhysical* physical)
{
    for(auto shadow : m_apShadows) {
        if(!shadow->m_pOwner) {
            physical->m_pShadowData = shadow;
            shadow->m_bKeepAlive = true;

            shadow->SetShadowedObject(physical);
            return shadow;
        }
    }
    return nullptr;
}

void CRealTimeShadowManager::DoShadowThisFrame(CPhysical* physical) {
    if (!m_bInitialised)
        return;

    const auto shdwsQuality = CMobileSettings::ms_MobileSettings[MS_Shadows].value;
    switch (shdwsQuality) {
        case 2: // Always render
            break;
        case 1: { // Only draw for main player
            if (physical->IsPed()) {
                if (physical->AsPed()->m_nPedType == PEDTYPE_PLAYER1) {
                    break;
                }
            }
            return;
        }
        default: // For any other quality: skip
            return;
    }

    if (const auto shdw = physical->m_pShadowData) {
        shdw->m_bKeepAlive = true;
    } else {
        (void)GetRealTimeShadow(physical);
    }
}

void ReturnRealTimeShadow_hook(CRealTimeShadowManager* thiz, CRealTimeShadow *shdw) {
    thiz->ReturnRealTimeShadow(shdw);
}

void DoShadowThisFrame_hook(CRealTimeShadowManager* thiz, CPhysical *physical) {
    thiz->DoShadowThisFrame(physical);
}

void CRealTimeShadowManager::InjectHooks() {
    CHook::Redirect("_ZN22CRealTimeShadowManager17DoShadowThisFrameEP9CPhysical", &DoShadowThisFrame_hook);
    CHook::Redirect("_ZN22CRealTimeShadowManager20ReturnRealTimeShadowEP15CRealTimeShadow", &ReturnRealTimeShadow_hook);
}
