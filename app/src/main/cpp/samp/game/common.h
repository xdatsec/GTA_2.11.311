#pragma once

#include <cstdint>
#include "Vector.h"
#include "game/Enums/eWeaponType.h"
#include <cassert>

#define PADDING(x,y) uint8_t x[y]

#define VER_2_1 true

#define IN_VEHICLE(x) ((x->dwStateFlags & 0x100) >> 8)
#define IS_CROUCHING(x) ((x->dwStateFlags >> 26) & 1)
#define IS_FIRING(x) (x->bFiringWeapon)
#define IS_TARGETING(x) (x->bIsAimingGun)

#define ASSERT assert
#define nil nullptr

typedef int8_t    int8;
typedef int16_t   int16;
typedef int32_t   int32;
typedef int64_t   int64;
typedef uint8_t   uint8;
typedef uint16_t  uint16;
typedef uint32_t  uint32;
typedef uint64_t  uint64;
typedef intptr_t  intptr;
typedef uintptr_t uintptr;

typedef uint8     bool8;
typedef uint16    bool16;
typedef uint32    bool32;

#define VALIDATE_SIZE(struc, size) static_assert(sizeof(struc) == size, "Invalid structure size of " #struc)

VALIDATE_SIZE(bool, 1);
VALIDATE_SIZE(char, 1);
VALIDATE_SIZE(short, 2);
VALIDATE_SIZE(int, 4);
VALIDATE_SIZE(float, 4);
VALIDATE_SIZE(long long, 8);

typedef unsigned char byte;
// Macro for unused function arguments - Use it to avoid compiler warnings of unused arguments
#define UNUSED(x) (void)(x);

constexpr float E              = 2.71828f;          // e
constexpr float E_CONST        = 0.577f;            // Euler-Mascheroni constant
constexpr float FRAC_1_TAU     = 0.159154f;         // 1 / τ
constexpr float FRAC_1_PI      = 0.318309f;         // 1 / π
constexpr float FRAC_2_TAU     = 0.318309f;         // 2 / τ
constexpr float FRAC_2_PI      = 0.636619f;         // 2 / π
constexpr float FRAC_2_SQRT_PI = 1.12837f;          // 2 / √π
constexpr float FRAC_4_TAU     = 0.636619f;         // 4 / τ
constexpr float FRAC_1_SQRT_2  = 0.707106f;         // 1 / √2
constexpr float FRAC_PI_2      = 1.57079f;          // π / 2
constexpr float FRAC_PI_3      = 1.04719f;          // π / 3
constexpr float FRAC_PI_4      = 0.785398f;         // π / 4
constexpr float FRAC_PI_6      = 0.523598f;         // π / 6
constexpr float FRAC_PI_8      = 0.392699f;         // π / 8
constexpr float FRAC_TAU_2     = 3.14159f;          // τ / 2 = π
constexpr float FRAC_TAU_3     = 2.09439f;          // τ / 3
constexpr float FRAC_TAU_4     = 1.57079f;          // τ / 4
constexpr float FRAC_TAU_6     = 1.04719f;          // τ / 6
constexpr float FRAC_TAU_8     = 0.785398f;         // τ / 8
constexpr float FRAC_TAU_12    = 0.523598f;         // τ / 12
constexpr float LN_2           = 0.693147f;         // ln(2)
constexpr float LN_10          = 2.30258f;          // ln(10)
constexpr float LOG2_E         = 1.44269f;          // log2(e)
constexpr float LOG10_E        = 0.434294f;         // log10(e)
constexpr float LOG10_2        = 0.301029f;         // log10(2)
constexpr float LOG2_10        = 3.32192f;          // log2(10)
constexpr float PI             = 3.14159f;          // π
constexpr float HALF_PI        = PI / 2.0f;         // π / 2
constexpr float PI_6           = PI / 6.0f;         // π / 6
constexpr float SQRT_2         = 1.41421f;          // √2
constexpr float SQRT_3         = 1.73205f;          // √3
constexpr float TWO_PI         = 6.28318f;          // τ (TAU)

constexpr float COS_45 = SQRT_2; // cos(45deg)

constexpr float sq(float x) { return x * x; }

enum eLights
{
	// these have to correspond to their respective panels
	LEFT_HEADLIGHT = 0,
	RIGHT_HEADLIGHT,
	LEFT_TAIL_LIGHT,
	RIGHT_TAIL_LIGHT,
	/*  LEFT_BRAKE_LIGHT,
		RIGHT_BRAKE_LIGHT,
		FRONT_LEFT_INDICATOR,
		FRONT_RIGHT_INDICATOR,
		REAR_LEFT_INDICATOR,
		REAR_RIGHT_INDICATOR,*/

	MAX_LIGHTS            // MUST BE 16 OR LESS
};

enum eDoors
{
	BONNET = 0,
	BOOT,
	FRONT_LEFT_DOOR,
	FRONT_RIGHT_DOOR,
	REAR_LEFT_DOOR,
	REAR_RIGHT_DOOR,
	MAX_DOORS
};

enum eDoorStatus
{
	DT_DOOR_INTACT = 0,
	DT_DOOR_SWINGING_FREE,
	DT_DOOR_BASHED,
	DT_DOOR_BASHED_AND_SWINGING_FREE,
	DT_DOOR_MISSING
};

enum eWheelPosition
{
	FRONT_LEFT_WHEEL = 0,
	REAR_LEFT_WHEEL,
	FRONT_RIGHT_WHEEL,
	REAR_RIGHT_WHEEL,

	MAX_WHEELS

};

#pragma pack(push, 1)
typedef struct _DAMAGE_MANAGER_INTERFACE            // 28 bytes due to the way its packed (24 containing actual data)
{
	float fWheelDamageEffect;
	uint8_t  bEngineStatus;            // old - wont be used
	uint8_t  Wheel[MAX_WHEELS];
	uint8_t  Door[MAX_DOORS];
	uint32_t Lights;            // 2 bits per light
	uint32_t Panels;            // 4 bits per panel
} DAMAGE_MANAGER_INTERFACE;
#pragma pack(pop)
#pragma pack(push, 1)
#pragma pack(push, 1)
typedef struct _VECTOR {
	union
	{
		struct
		{
			float X, Y, Z;
		};

		struct
		{
			float x, y, z;
		};
	};

	_VECTOR()
	{
		X = Y = Z = 0.0f;
	}

	_VECTOR(float f)
	{
		X = Y = Z = f;
	}

	_VECTOR(float x, float y, float z)
	{
		X = x;
		Y = y;
		Z = z;
	}
} VECTOR, *PVECTOR;
#pragma pack(pop)

typedef struct _MATRIX4X4
{
	VECTOR right;		// 0-12 	; r11 r12 r13
	uint32_t  flags;	// 12-16
	VECTOR up;			// 16-28	; r21 r22 r23
	float  pad_u;		// 28-32
	VECTOR at;			// 32-44	; r31 r32 r33
	float  pad_a;		// 44-48
	VECTOR pos;			// 48-60
	float  pad_p;		// 60-64
	uintptr *m_pRwMat;                 // XREF: _GLOBAL__sub_I_Camera.cpp+2B0/w
	uint32 m_owner;                     // XREF: _GLOBAL__sub_I_Camera.cpp+2B8/w
	PADDING(mapad, 4);
} MATRIX4X4, *PMATRIX4X4;
#pragma pack(pop)
#pragma pack(push, 1)
typedef struct _ENTITY_TYPE
{
	int64 vtable; 			// 0-4		;vtable
	VECTOR vPos;				// 4-16
	float fRotZBeforeMat;		// 16-20
	MATRIX4X4 *mat; 			// 20-24	;mat

	union {
		uintptr_t m_pRwObject;
		uintptr_t m_pRpClump;
		uintptr_t m_pRpAtomic;
	}; 							// 24-28

	union {
		uintptr_t dwProcessingFlags;
		struct {
			uintptr_t m_bUsesCollision : 1;
			uintptr_t m_bCollisionProcessed : 1;
			uintptr_t m_bIsStatic : 1;
			uintptr_t m_bHasContacted : 1;
			uintptr_t m_bIsStuck : 1;
			uintptr_t m_bIsInSafePosition : 1;
			uintptr_t m_bWasPostponed : 1;
			uintptr_t m_bIsVisible : 1;

			uintptr_t m_bIsBIGBuilding : 1;
			uintptr_t m_bRenderDamaged : 1;
			uintptr_t m_bStreamingDontDelete : 1;
			uintptr_t m_bRemoveFromWorld : 1;
			uintptr_t m_bHasHitWall : 1;
			uintptr_t m_bImBeingRendered : 1;
			uintptr_t m_bDrawLast :1;
			uintptr_t m_bDistanceFade : 1;

			uintptr_t m_bDontCastShadowsOn : 1;
			uintptr_t m_bOffscreen : 1;
			uintptr_t m_bIsStaticWaitingForCollision : 1;
			uintptr_t m_bDontStream : 1;
			uintptr_t m_bUnderwater : 1;
			uintptr_t m_bHasPreRenderEffects : 1;
			uintptr_t m_bIsTempBuilding : 1;
			uintptr_t m_bDontUpdateHierarchy : 1;

			uintptr_t m_bHasRoadsignText : 1;
			uintptr_t m_bDisplayedSuperLowLOD : 1;
			uintptr_t m_bIsProcObject : 1;
			uintptr_t m_bBackfaceCulled : 1;
			uintptr_t m_bLightObject : 1;
			uintptr_t m_bUnimportantStream : 1;
			uintptr_t m_bTunnel : 1;
			uintptr_t m_bTunnelTransition : 1;
		} nEntityFlags;
	};  						// 28-32

	uint16 RandomSeed;
	uint16_t nModelIndex; 		// 38-40
	PADDING(_pad93, 23);		// 40-72
	uint8_t byteAreaCode;		// 51-52
	PADDING(_padE94, 14);			// 52-58


	uint8_t nControlFlags;


	PADDING(_pad95, 9);			// 59-68
	uint32_t flags;				// 68-72
	VECTOR vecMoveSpeed; 		// 72-84
	VECTOR vecTurnSpeed;		// 84-96
	PADDING(_pad94, 96);		// 96-184
	uintptr_t dwUnkModelRel; 	// 184-188
} ENTITY_TYPE;
#pragma pack(pop)

//-----------------------------------------------------------



#pragma pack(push, 1)
typedef struct
{
	uint8_t bFlags;
	PADDING(_pad75, 3);
	VECTOR vOffset;
	uintptr_t pInterpFrame;
	uint32_t m_nNodeId;
	PADDING(_pad76, 4);
} AnimBlendFrameData1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _WEAPON_SLOT_TYPE
{
	uint32_t dwType;
	uint32_t dwState;
	uint32_t dwAmmoInClip;
	uint32_t dwAmmo;
	PADDING(_pwep1, 16);
} WEAPON_SLOT_TYPE;  // MUST BE EXACTLY ALIGNED TO 28 bytes
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _PED_TASKS_TYPE
{
	uintptr_t *pdwPed; 					// 0-4
	// Basic Tasks
	uintptr_t *pdwDamage; 				// 4-8
	uintptr_t *pdwFallEnterExit; 		// 8-12
	uintptr_t *pdwSwimWasted;		 	// 12-16
	uintptr_t *pdwJumpJetPack; 			// 16-20
	uintptr_t *pdwAction; 				// 20-24
	// Extended Tasks
	uintptr_t *pdwFighting; 				// 24-28
	uintptr_t *pdwCrouching; 			// 28-32
	uintptr_t *pdwExtUnk1; 				// 32-36
	uintptr_t *pdwExtUnk2; 				// 36-40
	uintptr_t *pdwExtUnk3; 				// 40-44
	uintptr_t *pdwExtUnk4; 				// 44-48
} PED_TASKS_TYPE;
#pragma pack(pop)


class CTaskManager2
{
public:
	void* m_aPrimaryTasks[5];
	void* m_aSecondaryTasks[6];
	class CPed* m_pPed;
};

class CPedIntelligence2
{
public:
	class CPed* m_pPed;
	CTaskManager2   m_TaskMgr;
};
#pragma pack(push, 1)
typedef struct _PED_TYPE
{
	ENTITY_TYPE entity; 				// 0000-E8	;entity				- 2.0
	PADDING(_pad100, 1096);				// E8-530
	CPedIntelligence2* pPedIntelligence; // 530-538
	PED_TASKS_TYPE *Tasks; 				// 538-540
	uintptr_t dwPlayerInfoOffset;		// 540-548	;dwPlayerInfoOffset - 2.0
	PADDING(_pad106, 4);				// 548-54C
	uint32_t dwAction;					// 54C-550	;Action				- 2.0
	PADDING(_pad101, 52);				// 550-584
	uint32_t dwStateFlags; 				// 584-588	;StateFlags		- ???? ???????
	PADDING(_pad102, 16);				// 587-598
	AnimBlendFrameData1* m_pPedBones[19];// 598-630
	PADDING(_pad174, 124);				// 630-6AC
	float fHealth;		 				// 6AC-6B0	;Health				- 2.0
	float fMaxHealth;					// 6B0-6B4	;MaxHealth			- 2.0
	float fArmour;						// 6B4-6B8	;Armour				- 2.0
	PADDING(_pad103, 12);				// 6B8-6C4
	float fRotation1;					// 6C4-6C8	;Rotation1			- 2.0
	float fRotation2;					// 6C8-6CC	;Rotation2			- 2.0
	PADDING(_pad104, 60);				// 6CC-708
	uintptr_t pVehicle;					// 708-710	;pVehicle			- 2.0
	PADDING(_pad105, 16);				// 710-720
	uint32_t dwPedType;					// 720-724	;dwPedType			- 2.0
	PADDING(_pad107, 12);				// 724-730
	WEAPON_SLOT_TYPE WeaponSlots[13];	// 730-8D0	;WeaponSlots		- 2.0
	PADDING(_pad108, 12);				// 8D0-8DC
	uint8_t byteCurWeaponSlot;			// 8DC-8DD	;byteCurWeaponSlot	- 2.0
	PADDING(_pad109, 95);				// 8DD-93C
	uint32_t dwWeaponUsed;				// 93C-940	;dwWeaponUsed		- 2.0
	ENTITY_TYPE* pdwDamageEntity;		// 940-948	;pdwDamageEntity
	PADDING(_pad282, 40);		 		// 1896-1932
	uintptr *dwEntryExit;				// 1932-1936
} PED_TYPE;
#pragma pack(pop)

enum ePanels
{
	FRONT_LEFT_PANEL = 0,
	FRONT_RIGHT_PANEL,
	REAR_LEFT_PANEL,
	REAR_RIGHT_PANEL,
	WINDSCREEN_PANEL,            // needs to be in same order as in component.h
	FRONT_BUMPER,
	REAR_BUMPER,

	MAX_PANELS            // MUST BE 8 OR LESS
};

enum eComponentStatus
{
	DT_PANEL_INTACT = 0,
	//  DT_PANEL_SHIFTED,
	DT_PANEL_BASHED,
	DT_PANEL_BASHED2,
	DT_PANEL_MISSING
};

#pragma pack(push, 1)
typedef struct _ANIMATION_DATA {
	union {
		int iValue;
		struct {
			unsigned short sId : 16;
			unsigned char cFrameDelta : 8;
			unsigned char cLoopA : 1;
			unsigned char cLoopX : 1;
			unsigned char cLoopY : 1;
			unsigned char cLoopF : 1;
			unsigned char cTime : 2;
		};
	};
} ANIMATION_DATA;
#pragma pack(pop)

//-----------------------------------------------------------

enum eWeaponState : uint32 {
    WEAPONSTATE_READY = 0,
    WEAPONSTATE_FIRING,
    WEAPONSTATE_RELOADING,
    WEAPONSTATE_OUT_OF_AMMO,
    WEAPONSTATE_MELEE_MADECONTACT,
};

#pragma pack(push, 1)
struct CWeapon
{
	uint32 dwType;
	uint32 dwState;
	uint32_t dwAmmoInClip;
	uint32_t dwAmmo;
	uint32_t m_nTimer;
	bool m_bFirstPersonWeaponModeSelected;
	bool m_bDontPlaceInHand;
	uint8_t pad[2];
	uintptr_t *m_pWeaponFxSys;
};
#pragma pack(pop)
static_assert(sizeof(CWeapon) == 0x20, "Invalid size CPlaceable");

//-----------------------------------------------------------

//-----------------------------------------------------------


struct VehicleAudioPropertiesStruct
{
	int16_t VehicleType;		//	1: +  0
	int16_t EngineOnSound;	//  2: +  2
	int16_t EngineOffSound;	//  3: +  4
	int16_t field_4;			//  4: +  6
	float field_5;			//  5: +  8
	float field_6;			//  6: + 12
	char HornTon;				//  7: + 16
	char field_8[3];			//	8: + 17, zeros
	float HornHigh;			//  9: + 20
	char DoorSound;			// 10: + 24
	char field_11[1];			// 11: + 25, zeros
	char RadioNum;			// 12: + 26
	char RadioType;			// 13: + 27
	char field_14;			// 14: + 28
	char field_15[3];			// 15: + 29, zeros
	float field_16;			// 16: + 32
};

#pragma pack(push, 1)
typedef struct _REMOVE_BUILDING_DATA
{
    unsigned short usModelIndex;
    CVector vecPos;
    float fRange;
} REMOVE_BUILDING_DATA;
#pragma pack(pop)

struct BULLET_SYNC
{
    uint8_t hitType;
    uint16_t hitId;
    float origin[3];
    float hitPos[3];
    float offsets[3];
    uint8_t weapId;
};

#pragma pack(push, 1)
typedef struct _REMOVEBUILDING_DATA {
	uint32_t dwModel;
	CVector vecPos;
	float fRange;
} REMOVEBUILDING_DATA;
#pragma pack(pop)

//-----------------------------------------------------------

#define	VEHICLE_SUBTYPE_CAR				1
#define	VEHICLE_SUBTYPE_BIKE			2
#define	VEHICLE_SUBTYPE_HELI			3
#define	VEHICLE_SUBTYPE_BOAT			4
#define	VEHICLE_SUBTYPE_PLANE			5
#define	VEHICLE_SUBTYPE_PUSHBIKE		6
#define	VEHICLE_SUBTYPE_TRAIN			7

//-----------------------------------------------------------

#define TRAIN_PASSENGER_LOCO			538
#define TRAIN_FREIGHT_LOCO				537
#define TRAIN_PASSENGER					570
#define TRAIN_FREIGHT					569
#define TRAIN_TRAM						449
#define HYDRA							520

//-----------------------------------------------------------

#define ACTION_WASTED					55
#define ACTION_DEATH					54
#define ACTION_INCAR					50
#define ACTION_NORMAL					1
#define ACTION_SCOPE					12
#define ACTION_NONE						0 

//-----------------------------------------------------------

#define WEAPON_MODEL_BRASSKNUCKLE		331 // was 332
#define WEAPON_MODEL_GOLFCLUB			333
#define WEAPON_MODEL_NITESTICK			334
#define WEAPON_MODEL_KNIFE				335
#define WEAPON_MODEL_BAT				336
#define WEAPON_MODEL_SHOVEL				337
#define WEAPON_MODEL_POOLSTICK			338
#define WEAPON_MODEL_KATANA				339
#define WEAPON_MODEL_CHAINSAW			341
#define WEAPON_MODEL_DILDO				321
#define WEAPON_MODEL_DILDO2				322
#define WEAPON_MODEL_VIBRATOR			323
#define WEAPON_MODEL_VIBRATOR2			324
#define WEAPON_MODEL_FLOWER				325
#define WEAPON_MODEL_CANE				326
#define WEAPON_MODEL_GRENADE			342 // was 327
#define WEAPON_MODEL_TEARGAS			343 // was 328
#define WEAPON_MODEL_MOLOTOV			344 // was 329
#define WEAPON_MODEL_COLT45				346
#define WEAPON_MODEL_SILENCED			347
#define WEAPON_MODEL_DEAGLE				348
#define WEAPON_MODEL_SHOTGUN			349
#define WEAPON_MODEL_SAWEDOFF			350
#define WEAPON_MODEL_SHOTGSPA			351
#define WEAPON_MODEL_UZI				352
#define WEAPON_MODEL_MP5				353
#define WEAPON_MODEL_AK47				355
#define WEAPON_MODEL_M4					356
#define WEAPON_MODEL_TEC9				372
#define WEAPON_MODEL_RIFLE				357
#define WEAPON_MODEL_SNIPER				358
#define WEAPON_MODEL_ROCKETLAUNCHER		359
#define WEAPON_MODEL_HEATSEEKER			360
#define WEAPON_MODEL_FLAMETHROWER		361
#define WEAPON_MODEL_MINIGUN			362
#define WEAPON_MODEL_SATCHEL			363
#define WEAPON_MODEL_BOMB				364
#define WEAPON_MODEL_SPRAYCAN			365
#define WEAPON_MODEL_FIREEXTINGUISHER	366
#define WEAPON_MODEL_CAMERA				367
#define WEAPON_MODEL_NIGHTVISION		368	// newly added
#define WEAPON_MODEL_INFRARED			369	// newly added
#define WEAPON_MODEL_JETPACK			370	// newly added
#define WEAPON_MODEL_PARACHUTE			371
#define WEAPON_MODEL_PARACHUTE			371

#define OBJECT_PARACHUTE				3131
#define OBJECT_CJ_CIGGY					1485
#define OBJECT_DYN_BEER_1				1486
#define OBJECT_CJ_BEER_B_2				1543
#define OBJECT_CJ_PINT_GLASS			1546

template<typename T>
auto lerp(const T& from, const T& to, float t) {
    return to * t + from * (1.f - t);
}

inline const float invLerp(float fMin, float fMax, float fVal) {
    return (fVal - fMin) / (fMax - fMin);
}

namespace notsa {
    namespace detail {
        static void VerifyMacroImpl(bool result) {
            assert(result); // In release mode this won't do anything
        }
    };
};
//! Macro for passing a string var to *scanf_s function.
#define VERIFY notsa::detail::VerifyMacroImpl
