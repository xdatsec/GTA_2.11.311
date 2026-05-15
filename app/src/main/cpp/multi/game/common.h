#pragma once

#include <cstdint>
#include "Core/Vector.h"
#include <cassert>

#define PADDING(x,y) uint8_t x[y]

#define VER_2_1 true

#define IN_VEHICLE2(x) ((x->dwStateFlags & 0x100) >> 8)
#define MAX_MATERIALS_PER_MODEL 16



#define PLAYER_PED_SLOTS	120

#define IN_VEHICLE(x) ((x->dwStateFlags & 0x100) >> 8)
#define IS_CROUCHING(x) ((x->dwStateFlags >> 26) & 1)
#define IS_FIRING(x) (x & 4)
#define IS_TARGETING(x) (x & 128)


enum eStreamingFlags2 {
    GAME_REQUIRED = 0x2,
    MISSION_REQUIRED = 0x4,
    KEEP_IN_MEMORY = 0x8,
    PRIORITY_REQUEST = 0x10
};


#define ASSERT assert
#define nil nullptr

typedef  __int64_t u_native;
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


#define WEAPON_BRASSKNUCKLE				1
#define WEAPON_GOLFCLUB					2
#define WEAPON_NITESTICK				3
#define WEAPON_KNIFE					4
#define WEAPON_BAT						5
#define WEAPON_SHOVEL					6
#define WEAPON_POOLSTICK				7
#define WEAPON_KATANA					8
#define WEAPON_CHAINSAW					9
#define WEAPON_DILDO					10
#define WEAPON_DILDO2					11
#define WEAPON_VIBRATOR					12
#define WEAPON_VIBRATOR2				13
#define WEAPON_FLOWER					14
#define WEAPON_CANE						15
#define WEAPON_GRENADE					16
#define WEAPON_TEARGAS					17
#define WEAPON_MOLTOV					18
#define WEAPON_ROCKET					19
#define WEAPON_ROCKET_HS				20
#define WEAPON_FREEFALLBOMB				21
#define WEAPON_COLT45					22
#define WEAPON_SILENCED					23
#define WEAPON_DEAGLE					24
#define WEAPON_SHOTGUN					25
#define WEAPON_SAWEDOFF					26
#define WEAPON_SHOTGSPA					27
#define WEAPON_UZI						28
#define WEAPON_MP5						29
#define WEAPON_AK47						30
#define WEAPON_M4						31
#define WEAPON_TEC9						32
#define WEAPON_RIFLE					33
#define WEAPON_SNIPER					34
#define WEAPON_ROCKETLAUNCHER			35
#define WEAPON_HEATSEEKER				36
#define WEAPON_FLAMETHROWER				37
#define WEAPON_MINIGUN					38
#define WEAPON_SATCHEL					39
#define WEAPON_BOMB						40
#define WEAPON_SPRAYCAN					41
#define WEAPON_FIREEXTINGUISHER			42
#define WEAPON_CAMERA					43
#define WEAPON_NIGHTVISION				44
#define WEAPON_INFRARED					45
#define WEAPON_PARACHUTE				46
#define WEAPON_ARMOUR					47
#define WEAPON_VEHICLE					49
#define WEAPON_HELIBLADES				50
#define WEAPON_EXPLOSION				51
#define WEAPON_DROWN					53
#define WEAPON_COLLISION				54

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
    void Set(float x, float y, float z); // Declare it
} VECTOR, *PVECTOR;



static_assert(sizeof(_VECTOR) ==  0xC);

inline void VECTOR::Set(float x, float y, float z) {
    X = x;
    Y = y;
    Z = z;
}

typedef struct _RECT
{
    float fLeft;
    float fBottom;
    float fRight;
    float fTop;
} RECT, *PRECT;



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

//-----------------------------------------------------------


typedef struct _REMOVEBUILDING_DATA {
    uint32_t dwModel;
    VECTOR vecPos;
    float fRange;
} REMOVEBUILDING_DATA;



enum ePrimaryTasks //array indexes
{
    TASK_PRIMARY_PHYSICAL_RESPONSE = 0,
    TASK_PRIMARY_EVENT_RESPONSE_TEMP,
    TASK_PRIMARY_EVENT_RESPONSE_NONTEMP,
    TASK_PRIMARY_PRIMARY,
    TASK_PRIMARY_DEFAULT,
    TASK_PRIMARY_MAX
};

enum eSecondaryTasks //array indexes
{
    TASK_SECONDARY_ATTACK = 0,                // want duck to be after attack
    TASK_SECONDARY_DUCK,                    // because attack controls ducking movement
    TASK_SECONDARY_SAY,
    TASK_SECONDARY_FACIAL_COMPLEX,
    TASK_SECONDARY_PARTIAL_ANIM,
    TASK_SECONDARY_IK,
    TASK_SECONDARY_MAX
};
class CTaskManager
{
public:

    union
    {
        struct
        {
            uintptr_t * pdwDamage;
            uintptr_t* pdwFallEnterExit;
            uintptr_t* pdwSwimWasted;
            uintptr_t* pdwJumpJetPack;
            uintptr_t* pdwAction;
        };
        struct
        {
            void* m_aPrimaryTasks[5];
        };

    };

    union
    {
        struct
        {
            uintptr_t* pdwFighting;
            uintptr_t* pdwCrouching;
            uintptr_t* pdwExtUnk1;
            uintptr_t* pdwExtUnk2;
            uintptr_t* pdwExtUnk3;
            uintptr_t* pdwExtUnk4;
        };
        struct
        {
            void* m_aSecondaryTasks[6];
        };

    };
    class CPed* m_pPed;

};
static_assert(sizeof(CTaskManager) ==  0x60);

class CPedIntelligence
{
public:
    class CPed* m_pPed;
    CTaskManager   Task;
};
enum eEntityType : uint8_t {
    ENTITY_TYPE_NOTHING = 0,
    ENTITY_TYPE_BUILDING,
    ENTITY_TYPE_VEHICLE,
    ENTITY_TYPE_PED,
    ENTITY_TYPE_OBJECT,
    ENTITY_TYPE_DUMMY,
    ENTITY_TYPE_NOTINPOOLS
};
enum eEntityStatus : uint8_t {
    STATUS_PLAYER = 0,
    STATUS_PLAYER_PLAYBACK_FROM_BUFFER,
    STATUS_SIMPLE,
    STATUS_PHYSICS,
    STATUS_ABANDONED,
    STATUS_WRECKED,
    STATUS_TRAIN_MOVING,
    STATUS_TRAIN_NOT_MOVING,
    STATUS_REMOTE_CONTROLLED,
    STATUS_FORCED_STOP,
    STATUS_IS_TOWED,
    STATUS_IS_SIMPLE_TOWED,
    STATUS_GHOST
};
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

    union {
        struct
        {
            uint8_t nControlFlags;
        };
        struct
        {
            eEntityType     m_nType : 3;          // Mask: & 0x7  = 7
            eEntityStatus   m_nStatus : 5;        // Mask: & 0xF8 = 248 (Remember: In the original code unless this was left shifted the value it's compared to has to be left shifted by 3!)

        };
    };

    PADDING(_pad95, 9);			// 59-68
    uint32_t flags;				// 68-72
    VECTOR vecMoveSpeed; 		// 72-84
    VECTOR vecTurnSpeed;		// 84-96
    PADDING(_pad94, 96);		// 96-184
    uintptr_t dwUnkModelRel; 	// 184-188
} ENTITY_TYPE;



typedef struct
{
    uint8_t bFlags;
    PADDING(_pad75, 3);
    VECTOR vOffset;
    uintptr_t pInterpFrame;
    uint32_t m_nNodeId;
    PADDING(_pad76, 4);
} AnimBlendFrameData;

typedef struct _WEAPON_SLOT_TYPE
{
    uint32_t dwType;
    uint32_t dwState;
    uint32_t dwAmmoInClip;
    uint32_t dwAmmo;
    PADDING(_pwep1, 16);
} WEAPON_SLOT_TYPE;  // MUST BE EXACTLY ALIGNED TO 28 bytes



typedef struct _PED_TYPE
{
    ENTITY_TYPE entity; 				// 0000-E8	;entity				- 2.0
    PADDING(_pad100, 1104);				// E8-530
    CPedIntelligence* pPedIntelligence; // 530-538
    uintptr_t dwPlayerInfoOffset;		// 540-548	;dwPlayerInfoOffset - 2.0
    PADDING(_pad106, 4);				// 548-54C
    uint32_t dwAction;					// 54C-550	;Action				- 2.0
    PADDING(_pad101, 52);				// 550-584
    uint32_t dwStateFlags; 				// 584-588	;StateFlags		- ???? ???????
    PADDING(_pad102, 16);				// 587-598
    AnimBlendFrameData* m_pPedBones[19];// 598-630
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
    PADDING(_padlast, 64);
} PED_TYPE;



struct tTransmissionGear
{
    float m_fMaxVelocity;
    float m_fChangeUpVelocity;
    float m_fChangeDownVelocity;
};


class cTransmission {
public:
    tTransmissionGear m_aGears[6];
    unsigned char m_nDriveType; // F/R/4
    unsigned char m_nEngineType; // P/D/E
    unsigned char m_nNumberOfGears; // 1 to 6
    char field_4B;
    unsigned int  m_nHandlingFlags;
    float         m_fEngineAcceleration; // 0.1 to 10.0
    float         m_fEngineInertia; // 0.0 to 50.0
    float         m_fMaxGearVelocity; // 5.0 to 150.0
    int field_5C;
    float         m_fMinGearVelocity;
    float         m_fCurrentSpeed;
};

//-----------------------------------------------------------

enum eVehicleLightsSize : unsigned char {
    LIGHTS_LONG,
    LIGHTS_SMALL,
    LIGHTS_BIG,
    LIGHTS_TALL
};
enum eVehicleHandlingFlags : unsigned int {
    VEHICLE_HANDLING_1G_BOOST = 0x1,
    VEHICLE_HANDLING_2G_BOOST = 0x2,
    VEHICLE_HANDLING_NPC_ANTI_ROLL = 0x4,
    VEHICLE_HANDLING_NPC_NEUTRAL_HANDL = 0x8,
    VEHICLE_HANDLING_NO_HANDBRAKE = 0x10,
    VEHICLE_HANDLING_STEER_REARWHEELS = 0x20,
    VEHICLE_HANDLING_HB_REARWHEEL_STEER = 0x40,
    VEHICLE_HANDLING_ALT_STEER_OPT = 0x80,
    VEHICLE_HANDLING_WHEEL_F_NARROW2 = 0x100,
    VEHICLE_HANDLING_WHEEL_F_NARROW = 0x200,
    VEHICLE_HANDLING_WHEEL_F_WIDE = 0x400,
    VEHICLE_HANDLING_WHEEL_F_WIDE2 = 0x800,
    VEHICLE_HANDLING_WHEEL_R_NARROW2 = 0x1000,
    VEHICLE_HANDLING_WHEEL_R_NARROW = 0x2000,
    VEHICLE_HANDLING_WHEEL_R_WIDE = 0x4000,
    VEHICLE_HANDLING_WHEEL_R_WIDE2 = 0x8000,
    VEHICLE_HANDLING_HYDRAULIC_GEOM = 0x10000,
    VEHICLE_HANDLING_HYDRAULIC_INST = 0x20000,
    VEHICLE_HANDLING_HYDRAULIC_NONE = 0x40000,
    VEHICLE_HANDLING_NOS_INST = 0x80000,
    VEHICLE_HANDLING_OFFROAD_ABILITY = 0x100000,
    VEHICLE_HANDLING_OFFROAD_ABILITY2 = 0x200000,
    VEHICLE_HANDLING_HALOGEN_LIGHTS = 0x400000,
    VEHICLE_HANDLING_PROC_REARWHEEL_1ST = 0x800000,
    VEHICLE_HANDLING_USE_MAXSP_LIMIT = 0x1000000,
    VEHICLE_HANDLING_LOW_RIDER = 0x2000000,
    VEHICLE_HANDLING_STREET_RACER = 0x4000000,
    VEHICLE_HANDLING_SWINGING_CHASSIS = 0x10000000
};

enum eVehicleHandlingModelFlags : unsigned int {
    VEHICLE_HANDLING_MODEL_IS_VAN = 0x1,
    VEHICLE_HANDLING_MODEL_IS_BUS = 0x2,
    VEHICLE_HANDLING_MODEL_IS_LOW = 0x4,
    VEHICLE_HANDLING_MODEL_IS_BIG = 0x8,
    VEHICLE_HANDLING_MODEL_REVERSE_BONNET = 0x10,
    VEHICLE_HANDLING_MODEL_HANGING_BOOT = 0x20,
    VEHICLE_HANDLING_MODEL_TAILGATE_BOOT = 0x40,
    VEHICLE_HANDLING_MODEL_NOSWING_BOOT = 0x80,
    VEHICLE_HANDLING_MODEL_NO_DOORS = 0x100,
    VEHICLE_HANDLING_MODEL_TANDEM_SEATS = 0x200,
    VEHICLE_HANDLING_MODEL_SIT_IN_BOAT = 0x400,
    VEHICLE_HANDLING_MODEL_CONVERTIBLE = 0x800,
    VEHICLE_HANDLING_MODEL_NO_EXHAUST = 0x1000,
    VEHICLE_HANDLING_MODEL_DOUBLE_EXHAUST = 0x2000,
    VEHICLE_HANDLING_MODEL_NO1FPS_LOOK_BEHIND = 0x4000,
    VEHICLE_HANDLING_MODEL_FORCE_DOOR_CHECK = 0x8000,
    VEHICLE_HANDLING_MODEL_AXLE_F_NOTILT = 0x10000,
    VEHICLE_HANDLING_MODEL_AXLE_F_SOLID = 0x20000,
    VEHICLE_HANDLING_MODEL_AXLE_F_MCPHERSON = 0x40000,
    VEHICLE_HANDLING_MODEL_AXLE_F_REVERSE = 0x80000,
    VEHICLE_HANDLING_MODEL_AXLE_R_NOTILT = 0x100000,
    VEHICLE_HANDLING_MODEL_AXLE_R_SOLID = 0x200000,
    VEHICLE_HANDLING_MODEL_AXLE_R_MCPHERSON = 0x400000,
    VEHICLE_HANDLING_MODEL_AXLE_R_REVERSE = 0x800000,
    VEHICLE_HANDLING_MODEL_IS_BIKE = 0x1000000,
    VEHICLE_HANDLING_MODEL_IS_HELI = 0x2000000,
    VEHICLE_HANDLING_MODEL_IS_PLANE = 0x4000000,
    VEHICLE_HANDLING_MODEL_IS_BOAT = 0x8000000,
    VEHICLE_HANDLING_MODEL_BOUNCE_PANELS = 0x10000000,
    VEHICLE_HANDLING_MODEL_DOUBLE_RWHEELS = 0x20000000,
    VEHICLE_HANDLING_MODEL_FORCE_GROUND_CLEARANCE = 0x40000000,
    VEHICLE_HANDLING_MODEL_IS_HATCHBACK = 0x80000000
};

struct tHandlingData {
    int           m_nVehicleId;
    float         m_fMass; // 1.0 to 50000.0
    float field_8;
    float         m_fTurnMass;
    float         m_fDragMult;
    VECTOR       m_vecCentreOfMass; // x, y, z - 1.0 to 50000.0
    unsigned char m_nPercentSubmerged; // 10 to 120 (> 100% vehicle sinks)
    PADDING(pad_0, 3);
    float         m_fBuoyancyConstant;
    float         m_fTractionMultiplier; // 0.5 to 2.0
    cTransmission m_transmissionData;
    float         m_fBrakeDeceleration; // 0.1 to 10.0
    float         m_fBrakeBias; // 0.0 > x > 1.0
    char          m_bABS; // 0/1
    char field_9D;
    char field_9E;
    char field_9F;
    float         m_fSteeringLock; // 10.0 to 40.0
    float         m_fTractionLoss;
    float         m_fTractionBias;
    float         m_fSuspensionForceLevel; // not [L/M/H]
    float         m_fSuspensionDampingLevel; // not [L/M/H]
    float         m_fSuspensionHighSpdComDamp; // often zero - 200.0 or more for bouncy vehicles
    float         m_fSuspensionUpperLimit;
    float         m_fSuspensionLowerLimit;
    float         m_fSuspensionBiasBetweenFrontAndRear;
    float         m_fSuspensionAntiDiveMultiplier;
    float         m_fCollisionDamageMultiplier; // 0.2 to 5.0
    union {
        eVehicleHandlingModelFlags m_nModelFlags;
        struct {
            unsigned int m_bIsVan : 1;
            unsigned int m_bIsBus : 1;
            unsigned int m_bIsLow : 1;
            unsigned int m_bIsBig : 1;
            unsigned int m_bReverseBonnet : 1;
            unsigned int m_bHangingBoot : 1;
            unsigned int m_bTailgateBoot : 1;
            unsigned int m_bNoswingBoot : 1;
            unsigned int m_bNoDoors : 1;
            unsigned int m_bTandemSeats : 1;
            unsigned int m_bSitInBoat : 1;
            unsigned int m_bConvertible : 1;
            unsigned int m_bNoExhaust : 1;
            unsigned int m_bDoubleExhaust : 1;
            unsigned int m_bNo1fpsLookBehind : 1;
            unsigned int m_bForceDoorCheck : 1;
            unsigned int m_bAxleFNotlit : 1;
            unsigned int m_bAxleFSolid : 1;
            unsigned int m_bAxleFMcpherson : 1;
            unsigned int m_bAxleFReverse : 1;
            unsigned int m_bAxleRNotlit : 1;
            unsigned int m_bAxleRSolid : 1;
            unsigned int m_bAxleRMcpherson : 1;
            unsigned int m_bAxleRReverse : 1;
            unsigned int m_bIsBike : 1;
            unsigned int m_bIsHeli : 1;
            unsigned int m_bIsPlane : 1;
            unsigned int m_bIsBoat : 1;
            unsigned int m_bBouncePanels : 1;
            unsigned int m_bDoubleRwheels : 1;
            unsigned int m_bForceGroundClearance : 1;
            unsigned int m_bIsHatchback : 1;
        };
    };
    union {
        eVehicleHandlingFlags m_nHandlingFlags;
        struct {
            unsigned int m_b1gBoost : 1;
            unsigned int m_b2gBoost : 1;
            unsigned int m_bNpcAntiRoll : 1;
            unsigned int m_bNpcNeutralHandl : 1;
            unsigned int m_bNoHandbrake : 1;
            unsigned int m_bSteerRearwheels : 1;
            unsigned int m_bHbRearwheelSteer : 1;
            unsigned int m_bAltSteerOpt : 1;
            unsigned int m_bWheelFNarrow2 : 1;
            unsigned int m_bWheelFNarrow : 1;
            unsigned int m_bWheelFWide : 1;
            unsigned int m_bWheelFWide2 : 1;
            unsigned int m_bWheelRNarrow2 : 1;
            unsigned int m_bWheelRNarrow : 1;
            unsigned int m_bWheelRWide : 1;
            unsigned int m_bWheelRWide2 : 1;
            unsigned int m_bHydraulicGeom : 1;
            unsigned int m_bHydraulicInst : 1;
            unsigned int m_bHydraulicNone : 1;
            unsigned int m_bNosInst : 1;
            unsigned int m_bOffroadAbility : 1;
            unsigned int m_bOffroadAbility2 : 1;
            unsigned int m_bHalogenLights : 1;
            unsigned int m_bProcRearwheelFirst : 1;
            unsigned int m_bUseMaxspLimit : 1;
            unsigned int m_bLowRider : 1;
            unsigned int m_bStreetRacer : 1;
            unsigned int m_bSwingingChassis : 1;
        };
    };
    float              m_fSeatOffsetDistance; // // ped seat position offset towards centre of car
    unsigned int       m_nMonetaryValue; // 1 to 100000
    eVehicleLightsSize m_nFrontLights;
    eVehicleLightsSize m_nRearLights;
    unsigned char      m_nAnimGroup;
};



inline CVector AsCVector(const VECTOR& vector)
{
    CVector val;
    val.x = vector.X;
    val.y = vector.Y;
    val.z = vector.Z;
    return val;
}

inline VECTOR AsVECTOR(const CVector& vector)
{
    VECTOR val;
    val.X = vector.x;
    val.Y = vector.y;
    val.Z = vector.z;
    return val;
}

typedef struct _VEHICLE_TYPE
{
    ENTITY_TYPE entity;			// 0000-0184	;entity
    PADDING(_pad99_9, 176);				// 0188-0316
    uintptr_t pVehicleAudio;			// 0316-0320
    PADDING(_pad2121, 776);		// 904 - 1076
    tHandlingData* pHandling;	// 900-904
    PADDING(_pad212, 184);		// 904 - 1076
    union {
        uint8_t byteFlags;				// 1068-1076	;byteFlags
        struct {
            unsigned char bIsLawEnforcer : 1;
            unsigned char bIsAmbulanceOnDuty : 1;
            unsigned char bIsFireTruckOnDuty : 1;
            unsigned char bIsLocked : 1;
            unsigned char bEngineOn : 1;
            unsigned char bIsHandbrakeOn : 1;
            unsigned char bLightsOn : 1;
            unsigned char bFreebies : 1;

            unsigned char bIsVan : 1;
            unsigned char bIsBus : 1;
            unsigned char bIsBig : 1;
            unsigned char bLowVehicle : 1;
            unsigned char bComedyControls : 1;
            unsigned char bWarnedPeds : 1;
            unsigned char bCraneMessageDone : 1;
            unsigned char bTakeLessDamage : 1;

            unsigned char bIsDamaged : 1;
            unsigned char bHasBeenOwnedByPlayer : 1;
            unsigned char bFadeOut : 1;
            unsigned char bIsBeingCarJacked : 1;
            unsigned char bCreateRoadBlockPeds : 1;
            unsigned char bCanBeDamaged : 1;
            unsigned char bOccupantsHaveBeenGenerated : 1;
            unsigned char bGunSwitchedOff : 1;

            unsigned char bVehicleColProcessed : 1;
            unsigned char bIsCarParkVehicle : 1;
            unsigned char bHasAlreadyBeenRecorded : 1;
            unsigned char bPartOfConvoy : 1;
            unsigned char bHeliMinimumTilt : 1;
            unsigned char bAudioChangingGear : 1;
            unsigned char bIsDrowning : 1;
            unsigned char bTyresDontBurst : 1;

            unsigned char bCreatedAsPoliceVehicle : 1;
            unsigned char bRestingOnPhysical : 1;
            unsigned char bParking : 1;
            unsigned char bCanPark : 1;
            unsigned char bFireGun : 1;
            unsigned char bDriverLastFrame : 1;
            unsigned char bNeverUseSmallerRemovalRange : 1;
            unsigned char bIsRCVehicle : 1;

            unsigned char bAlwaysSkidMarks : 1;
            unsigned char bEngineBroken : 1;
            unsigned char bVehicleCanBeTargetted : 1;
            unsigned char bPartOfAttackWave : 1;
            unsigned char bWinchCanPickMeUp : 1;
            unsigned char bImpounded : 1;
            unsigned char bVehicleCanBeTargettedByHS : 1;
            unsigned char bSirenOrAlarm : 1;

            unsigned char bHasGangLeaningOn : 1;
            unsigned char bGangMembersForRoadBlock : 1;
            unsigned char bDoesProvideCover : 1;
            unsigned char bMadDriver : 1;
            unsigned char bUpgradedStereo : 1;
            unsigned char bConsideredByPlayer : 1;
            unsigned char bPetrolTankIsWeakPoint : 1;
            unsigned char bDisableParticles : 1;

            unsigned char bHasBeenResprayed : 1;
            unsigned char bUseCarCheats : 1;
            unsigned char bDontSetColourWhenRemapping : 1;
            unsigned char bUsedForReplay : 1;
        } dwFlags;
    };
    uint32_t dwCreationTime;			// 1076-1080
    uint8_t byteColor1;				// 1080-1081	;byteColor1			- 2.0
    uint8_t byteColor2;				// 1081-1082	;byteColor2			- 2.0
    uint8_t byteColor3;				// 1082-1083	;byteColor3			- 2.0
    uint8_t byteColor4;				// 1083-1084	;byteColor4			- 2.0
    PADDING(_pad206, 36);			// 1084-1124
    uint16_t wAlarmState;			// 1120-1122	;wAlarmState		- 2.0
    PADDING(_pad207, 2);			// 1122-1124
    PED_TYPE *pDriver;				// 1124-1128	;driver				- 2.0
    PED_TYPE *pPassengers[8];		// 1128-1156	;pPassengers		- 2.0
    PADDING(_pad201, 4);			// 1156-1164
    uint8_t byteMaxPassengers;		// 1164-1165	;byteMaxPassengers	- 2.0
    PADDING(_pad236, 11); 			// 1165-1172
    uintptr * pFireObject;			// 1172-1176
    PADDING(_pad241__, 20); 		// 1176-1196
    uint8_t byteMoreFlags;			// 1196-1197
    PADDING(_pad275_, 31); 			// 1197-1228
    float fHealth;					// 1228-1232	;fHealth			- 2.0
    _VEHICLE_TYPE* pTractor;		// 1232-1236	;pTractor			- 2.0
    _VEHICLE_TYPE* pTrailer;		// 1236-1240	;pTrailer			- 2.0
    PADDING(_pad208, 60);			// 1240-1288
    uint32_t dwDoorsLocked;			// 1288-1292	;dwDoorsLocked		- 2.0
    PADDING(_pad202, 212);			// 1292-1464
    union {
        struct {
            PADDING(_pad245, 1);		// 1464-1465
            uint8_t byteWheelStatus[4]; // 1465-1469
            uint8_t byteDoorStatus[6];	// 1469-1475
            uint8_t byteDamageUnk;		// 1475-1476
            uint32_t dwLightStatus;		// 1476-1480
            uint32_t dwPanelStatus;		// 1480-1484
        };
        struct {
            float fTrainSpeed;			// 1464-1468
            PADDING(paxs, 16);		// 1468-1484
        };
    };
    PADDING(_pad213, 40);				// 1484-1512
    _VEHICLE_TYPE* pNextCarriage;	// 1512-1516	;pNextCarriage		- 2.0
    PADDING(_pad211, 152);				// 1516-1628
    float fBikeLean;					// 1628-1632
    uint32_t dwBikeUnk;					// 1632-1636
    PADDING(pad211_, 12);				// 1636-1648
    uint8_t byteBikeWheelStatus[2];		// 1648-1650
    PADDING(_pad21331, 570);			// 1650-2176
    uint16_t wHydraThrusters;
    PADDING(_pad245123, 386);  			// 2178-2528
    float fPlaneLandingGear;			// 2528-2532
} VEHICLE_TYPE;
#pragma pack(pop)


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

#pragma pack(push, 1)

struct BULLET_SYNC
{
    uint8_t hitType;
    uint16_t hitId;
    float origin[3];
    float hitPos[3];
    float offsets[3];
    uint8_t weapId;
};
enum ePedType : int32_t
{
    PEDTYPE_PLAYER1 = 0x0,
    PEDTYPE_PLAYER2 = 0x1,
    PEDTYPE_PLAYER_NETWORK = 0x2,
    PEDTYPE_PLAYER_UNUSED = 0x3,
    PEDTYPE_CIVMALE = 0x4,
    PEDTYPE_CIVFEMALE = 0x5,
    PEDTYPE_COP = 0x6,
    PEDTYPE_GANG1 = 0x7,
    PEDTYPE_GANG2 = 0x8,
    PEDTYPE_GANG3 = 0x9,
    PEDTYPE_GANG4 = 0xA,
    PEDTYPE_GANG5 = 0xB,
    PEDTYPE_GANG6 = 0xC,
    PEDTYPE_GANG7 = 0xD,
    PEDTYPE_GANG8 = 0xE,
    PEDTYPE_GANG9 = 0xF,
    PEDTYPE_GANG10 = 0x10,
    PEDTYPE_DEALER = 0x11,
    PEDTYPE_MEDIC = 0x12,
    PEDTYPE_FIRE = 0x13,
    PEDTYPE_CRIMINAL = 0x14,
    PEDTYPE_BUM = 0x15,
    PEDTYPE_PROSTITUTE = 0x16,
    PEDTYPE_SPECIAL = 0x17,
    PEDTYPE_MISSION1 = 0x18,
    PEDTYPE_MISSION2 = 0x19,
    PEDTYPE_MISSION3 = 0x1A,
    PEDTYPE_MISSION4 = 0x1B,
    PEDTYPE_MISSION5 = 0x1C,
    PEDTYPE_MISSION6 = 0x1D,
    PEDTYPE_MISSION7 = 0x1E,
    PEDTYPE_MISSION8 = 0x1F,
    PEDTYPE_LAST_PEDTYPE = 0x20,
};

struct CBaseModelInfo;
struct CPedModelInfo;
class CColModel;
struct  CBaseModelInfo // sizeof=0x48
{                                       // XREF: CAtomicModelInfo/r
    uintptr vtable;
    uint32 m_hashKey;
    char m_modelName[21];
    PADDING(basemodel3,1);
    int16 m_numRefs;
    int16 m_txdIndex;
    uint8 m_alpha;
    uint8 m_num2dEffects;
    int16 m_n2dEffects;
    int16 m_dynamicIndex;
    uint16 m_flags;
    PADDING(basemodel1,2);
    CColModel *m_pColModel;
    float m_lodDistance;
    PADDING(basemodel2,4);
    uintptr *m_pRwObject;
    CPedModelInfo*     AsPedModelInfoPtr()     { return reinterpret_cast<CPedModelInfo*>(this); }

};
struct CModelInfo
{

};
static inline CBaseModelInfo* ms_modelInfoPtrs[20000];
static CBaseModelInfo* GetModelInfo(int index) { return ms_modelInfoPtrs[index]; }

struct  CClumpModelInfo : CBaseModelInfo // sizeof=0x50
{                                       // XREF: CWeaponModelInfo/r
    u_native m_animFileIndex;
};
struct CPedModelInfo : public CClumpModelInfo{
public:
    int         m_nAnimType;
    ePedType    m_nPedType;
    uint32_t    m_nStatType;
    uint16_t    m_nCarsCanDriveMask;
    uint16_t    m_nPedFlags;
    uintptr_t   *m_pHitColModel; // CColModel
    int8_t      m_nRadio1;
    int8_t      m_nRadio2;
    uint8_t     m_nRace;
    uint8_t     skip_1;
    int16_t     m_AudioPedType;
    int16_t     m_FirstVoice;
    int16_t     m_LastVoice;
    int16_t     m_NextVoice;
    PADDING(cpedinfo, 4);
};
static_assert(sizeof(CPedModelInfo) ==  0x78);

struct CSphere // sizeof=0x10
{
    VECTOR m_vecCentre;
    float m_fRadius;
};
VALIDATE_SIZE(CSphere, 0x10);
struct CBox // sizeof=0x18
{
    VECTOR m_vecMin;
    VECTOR m_vecMax;
};
VALIDATE_SIZE(CBox, 0x18);
struct  CBoundingBox // sizeof=0x28
{
     CBox m_boxBound;
     CSphere m_sphereBound;
};
VALIDATE_SIZE(CBoundingBox, 0x28);
class CColModel {
public:
    CBox m_boxBound;
    CSphere m_boundSphere;
    uint8 m_nColSlot;
    PADDING(padcol, 7);
    uintptr *m_pColData;
    [[nodiscard]] float GetBoundRadius() const noexcept { return m_boundSphere.m_fRadius; }
    auto& GetBoundCenter() { return m_boundSphere.m_vecCentre; }
    auto& GetBoundingBox() { return m_boxBound; }
    auto& GetBoundingSphere() { return m_boundSphere; }
    uintptr* GetData() const { return m_pColData; }
};
VALIDATE_SIZE(CColModel, 0x38);
struct  CPlayerInfoGta // sizeof=0x1D8
{
    PED_TYPE * m_pPed;
    PADDING(padummy1, 464);
};
VALIDATE_SIZE(CPlayerInfoGta, 0x1D8);
static inline CPlayerInfoGta Players[1004];

struct CPhysical;
struct CPtrNodeSingleLink
{
    PADDING(fakepad1, 16);
};

struct CPtrNodeDoubleLink
{
    PADDING(fakepad2, 24);
};





struct CEntryInfoNode
{
    PADDING(fakepad8, 40);
};

class CEntityGTA
{
public:
    int64 vtable; 			// 0-4		;vtable
    VECTOR vPos;				// 4-16
    float fRotZBeforeMat;		// 16-20
    MATRIX4X4 *mat; 			// 20-24	;mat
    uintptr *m_pRwObject;
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
    uint8_t byteAreaCode;		// 51-52e
    PADDING(fakepad11, 20);

public:
    inline const VECTOR& GetPosition() const { return mat ? mat->pos : vPos; }
    inline VECTOR& GetPosition() { return mat ? mat->pos : vPos; }

    void SetPosn(float x, float y, float z) {
        auto& pos = GetPosition();
        pos.Set(x, y, z);
    }

};
static_assert(sizeof(CEntityGTA) ==  0x60);

struct CPhysical: CEntityGTA
{
    PADDING(physical, 312);
};
static_assert(sizeof(CPhysical) ==  0x198);

struct CPedGTA :CPhysical
{
    PADDING(fakepad12, 2032);
};
static_assert(sizeof(CPedGTA) ==  0x988);

struct CVehicleGTA:CPhysical
{
    PADDING(fakepad13, 1472);
};
static_assert(sizeof(CVehicleGTA) ==  0x758);

struct CDummy :CEntityGTA
{
};
static_assert(sizeof(CDummy) ==  0x60);


struct  CObjectGTA : CPhysical  // sizeof=0x1F8
{
    uintptr *m_pControlListNode;
    uint8 ObjectCreatedBy;
    uint8 ObjectMessage;
    uint16 ObjectMessageAmount;
    union {
        struct {
            uint32 bIsPickup : 1;               // 0x1
            uint32 b0x02 : 1;                   // 0x2 - collision related
            uint32 bPickupPropertyForSale : 1;  // 0x4
            uint32 bPickupInShopOutOfStock : 1; // 0x8
            uint32 bHasBrokenGlass : 1;         // 0x10
            uint32 bGlassBrokenAltogether : 1;  // 0x20
            uint32 bIsExploded : 1;             // 0x40
            uint32 bChangesVehColor : 1;        // 0x80

            uint32 bIsLampPost : 1;
            uint32 bIsTargetable : 1;
            uint32 bIsBroken : 1;
            uint32 bTrainCrossEnabled : 1;
            uint32 bIsPhotographed : 1;
            uint32 bIsLiftable : 1;
            uint32 bIsDoorMoving : 1;
            uint32 bIsDoorOpen : 1;

            uint32 bHasNoModel : 1;
            uint32 bIsScaled : 1;
            uint32 bCanBeAttachedToMagnet : 1;
            uint32 bDamaged : 1;
            uint32 b0x100000 : 1;
            uint32 b0x200000 : 1;
            uint32 bFadingIn : 1; // works only for objects with type 2 (OBJECT_MISSION)
            uint32 bAffectedByColBrightness : 1;

            uint32 bEnableDisabledAttractors : 1;
            uint32 bDoNotRender : 1;
            uint32 bFadingIn2 : 1;
            uint32 b0x08000000 : 1;
            uint32 b0x10000000 : 1;
            uint32 b0x20000000 : 1;
            uint32 b0x40000000 : 1;
            uint32 b0x80000000 : 1;
        } objectFlags;
        uint32 m_nObjectFlags;
    };
    uint8 m_nCollisionDamageEffect;
    uint8 m_nStoredCollisionDamageEffect;
    uint8 KeepieUppyCounter;
    uint8 m_Garage;
    int8 LastDamagedWeaponType;
    uint8 m_storedCollisionLighting;
    int16 m_nParentModelIndex;
    uint8 m_colour1;
    uint8 m_colour2;
    uint8 m_colour3;
    uint8 m_colour4;
    uint32 m_nEndOfLifeTime;
    float m_fHealth;
    float m_fOriginalAngle;
    float m_fScale;
    PADDING(objpad, 2);
    uintptr *m_pObjectInfo;
    uintptr *m_pFire;
    int16 StreamedScriptBrainToLoad;

    const char *m_remapTxdName;
    uintptr *m_pRemapTexture;
    uintptr *m_pRelatedDummy;
    int32 m_burnTimer;
    float m_burnMult;
};
static_assert(sizeof(CObjectGTA) ==  0x1F8);

struct CBuilding : CPhysical
{

};
static_assert(sizeof(CDummy) ==  0x60);

struct CCutsceneObject : CObjectGTA
{
    PADDING(fakepad5, 40);
};
static_assert(sizeof(CCutsceneObject) ==  0x220);

struct CTask
{
    PADDING(fakepad7, 16);
};
struct CTaskSimple : CTask
{

};
struct CTaskSimpleAnim : CTaskSimple
{
    PADDING(fakepad7, 9);
};
struct CTaskSimpleRunNamedAnim : CTaskSimpleAnim
{
    PADDING(fakepad7, 91);
};
struct CTaskSimpleSlideToCoord : CTaskSimpleRunNamedAnim
{
    PADDING(fakepad7, 28);
};
static_assert(sizeof(CTaskSimpleSlideToCoord) ==  0x90);
struct AutoMobileGTA : CVehicleGTA
{

    PADDING(fakepad4, 1136);
};
struct CHeli :AutoMobileGTA
{
    PADDING(fakepad4, 152);
};
static_assert(sizeof(CHeli) ==  0xC60);

struct CCopPed : CPedGTA
{
    PADDING(fakepad3, 64);
};
static_assert(sizeof(CCopPed) ==  0x9C8);
#pragma pack(pop)