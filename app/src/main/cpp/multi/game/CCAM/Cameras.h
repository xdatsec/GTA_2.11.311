
#pragma once
#include "QueuedMode.h"
#include "Ccam.h"
#include "game/RW/rwplcore.h"
#pragma pack(push, 1)
class CCameras
{
public:
    int64 vtable; 			// 0-4		;vtable
    VECTOR vPos;				// 4-16
    float fRotZBeforeMat;		// 16-20
    MATRIX4X4 *mat;

    PADDING(CCamera1, 344);
    CCam Cams[3];  //this is fine
    PADDING(CCamera2, 28);
    CQueuedMode PlayerWeaponMode;  //0x3c size
    PADDING(CCamera3, 280);
    RwCamera *m_pRwCamera;
    PADDING(CCamera4, 56);
    MATRIX4X4 m_cameraMatrix;
    PADDING(CCamera5, 1008);

    inline const VECTOR & GetPosition() const { return mat ? mat->pos : vPos; }
    inline VECTOR& GetPosition() { return mat ? mat->pos :vPos; }
    void Init();
    void SetRwCamera(RwCamera *pCamera);
public:
    static void InjectHooks();


    void Restore();
    void RestoreCameraAfterMirror();
    void RestoreWithJumpCut();
    void RenderMotionBlur();
    void ResetDuckingSystem(CPedGTA *ped);

    void CalculateDerivedValues(bool bForMirror, bool bOriented);
    void CalculateFrustumPlanes(bool bForMirror);
   // float CalculateGroundHeight(eGroundHeightType type);
   /// void CalculateMirroredMatrix(CVector posn, float mirrorV, CMatrix* camMatrix, CMatrix* mirrorMatrix);
    void CamControl();

    //! Get the camera's front normal (Whatever that is)
   // auto GetFrontNormal2D() const { return CVector2D{ m_fCamFrontXNorm, m_fCamFrontYNorm }; }

public:
  // static CCam& GetActiveCamera(); // TODO: Replace this with `TheCamera.GetActiveCam()`
    //void SetRwCamera(RwCamera *pCamera);
   // void TakeControl(CEntityGTA *target, eCamMode modeToGoTo, eSwitchType switchType, int32 whoIsInControlOfTheCamera);
   // CMatrix& GetViewMatrix() { return m_viewMatrix; }
    bool IsSphereVisible(const CVector* origin, float radius);

public:
    // FIXME
    static void SetBehindPlayer();
    static void SetPosition(float fX, float fY, float fZ, float fRotationX, float fRotationY, float fRotationZ);
    static void LookAtPoint(float fX, float fY, float fZ, int iType);
    static void InterpolateCameraPos(CVector *posFrom, CVector *posTo, int time, uint8_t mode);
    static void InterpolateCameraLookAt(CVector *posFrom, CVector *posTo, int time, uint8_t mode);

    static CCameras& Get() {
        static CCameras* pCamera = nullptr;
        if (!pCamera) {
            pCamera = reinterpret_cast<CCameras*>(g_libGTASA + 0x9DF030);
        }
        return *pCamera;
    }

};
#pragma pack(pop)
VALIDATE_SIZE(CCameras, 0xDB0);
