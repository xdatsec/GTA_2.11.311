#pragma once

class CActor : public CEntity
{
public:
    CActor(int iSkin, float fX, float fY, float fZ, float fAngle);
    virtual ~CActor();

    void SetHealth(float fHealth);
    void SetInvulnerable(bool bInvulnerable);
    void ApplyAnimation(const char* szAnimName, const char* szAnimLib, float fDelta,
                        int bLoop, int bLockX, int bLockY, int bFreeze, int iTime);
    void ClearAnimation();
    void SetFacingAngle(float fAngle);

public:
    PED_TYPE* m_pPed;
    bool m_bInvulnerable;
};