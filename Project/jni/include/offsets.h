#pragma once

// ZEUS KERNEL - PUBG MOBILE 4.5.0 / 64-bit Offsets
// Updated from PUBGM_(v4.5.0)_64Bit SDK dump + supplied global offsets.

// -----------------------------------------------------------------------------
// Global Bases & Core
// -----------------------------------------------------------------------------
#define OFFSET_VMATRIX 0xf1d5f70 // VMatrix_Offsets / GEngine_Offset (UEngine)
#define OFFSET_GENGINE 0xf1d5f70 // UEngine
#define OFFSET_GENGINE_ALT 0xf1d5f30 // UEngine alternative from supplied dump
#define OFFSET_GWORLD 0xf1fe880 // GWorld / Gworld_Offset
#define OFFSET_VWORLD 0xef3e0e0 // Vworld_Offsets

#define OFFSET_GNAME 0xec73720 // Gname Java - used by current read(read(libUE4 + OFFSET_GNAME) + OFFSET_GNAME_PTR) path
#define OFFSET_GNAMES 0x8939470 // GNames_Offset from SDK dumper
#define OFFSET_GNAME_PTR 0x110 // Internal padding for current GName chain

#define OFFSET_GUOBJECT 0xef3b3f0 // GUObject_Offset
#define OFFSET_GNATIVE_ANDROID_APP 0xec732a8 // GNativeAndroidApp_Offset
#define OFFSET_GET_ACTOR_ARRAY 0xa6ca440 // GetActorArrary_Offset
#define OFFSET_PROCESS_EVENT_CHILD 0x8bac0a8 // ProcessEvent child
#define OFFSET_PROCESS_EVENT_MAIN 0xa211d60 // ProcessEvent main
#define OFFSET_PS_GLOBAL_TARGETFUNC 0x6e45c70 // ps_Global_TargetFunc
#define OFFSET_MUNDO_CRASHFIX 0x57f3a0 // Mundo_CrashFix original
#define OFFSET_TERMINATION 0x5f25500 // Termination_Offsets
#define OFFSET_BULLET_SHELL_SOCK 0x97D7CA7F // BULLET SHELL SOCK _Offset

// -----------------------------------------------------------------------------
// World / NetDriver / Controller
// -----------------------------------------------------------------------------
#define OFFSET_UWORLD_NETDRIVER 0x38 // World_NetDriver
#define OFFSET_NETDRIVER_SERVERCONNECTION 0x78 // NetDriver_ServerConnection
#define OFFSET_SERVERCONNECTION_PLAYERCONTROLLER 0x30 // Player_PlayerController
#define OFFSET_PLAYERCONTROLLER_SELF 0x528 // PlayerController_AcknowledgedPawn

// -----------------------------------------------------------------------------
// GameState & Player Counts
// -----------------------------------------------------------------------------
#define OFFSET_UWORLD_GAMESTATE 0x428 // World_GameState
#define OFFSET_GAMESTATE_REALPLAYER 0x818 // PlayerNum / Real player count equivalent
#define OFFSET_GAMESTATE_ALIVEPLAYER 0xBB4 // AlivePlayerNum
#define OFFSET_GAMESTATE_ALIVETEAM 0xBB8 // AliveTeamNum

// -----------------------------------------------------------------------------
// Actor / Pawn / Character
// -----------------------------------------------------------------------------
#define OFFSET_ACTOR_MESH 0x510 // Character_Mesh
#define OFFSET_ACTOR_ROOTCOMPONENT 0x208 // Actor_RootComponent
#define OFFSET_COMPONENT_POSITION 0x1e4 // SceneComponent_RelativeLocation
#define OFFSET_ACTOR_TEAMID 0x998 // UAECharacter_TeamID
#define OFFSET_ACTOR_PLAYERNAME 0x960 // UAECharacter_PlayerName
#define OFFSET_ACTOR_HEALTH 0xe60 // STExtraCharacter_Health
#define OFFSET_ACTOR_HEALTH_MAX 0xe64 // STExtraCharacter_HealthMax
#define OFFSET_ACTOR_STATUS 0x1058 // STExtraCharacter_CurrentStates
#define OFFSET_ACTOR_ISBOT 0xa40 // UAECharacter_bIsAI
#define OFFSET_ACTOR_ENSURE 0xa59 // UAECharacter_bEnsure
#define OFFSET_ACTOR_CURRENT_VEHICLE 0xeb0 // STExtraCharacter_CurrentVehicle
#define OFFSET_ACTOR_PLAYERUID 0x988 // UAECharacter_PlayerUID
#define OFFSET_ACTOR_NATION 0x970 // UAECharacter_Nation
#define OFFSET_ACTOR_LOCALUID 0x920 // UAEPlayerController_PlayerKey
#define OFFSET_ACTOR_LOCALTEAMID 0x940 // UAEPlayerController_TeamID
#define OFFSET_ACTOR_B_IS_ENGINE_STARTED 0xbc8 // STExtraVehicleBase_bIsEngineStarted

// -----------------------------------------------------------------------------
// Level & Actors Array
// -----------------------------------------------------------------------------
#define OFFSET_UWORLD_PERSISTENT_LEVEL 0x30 // World_PersistentLevel
#define OFFSET_LEVEL_ACTORS_ARRAY 0xA0 // Actors_Offset from supplied dump
#define OFFSET_LEVEL_ACTORS_COUNT 0xA8 // Actors count, paired with Actors_Offset

// -----------------------------------------------------------------------------
// Mesh & Bones
// -----------------------------------------------------------------------------
#define OFFSET_MESH_BONE_ARRAY 0xc40 // SkeletalMeshComponent_CachedBoneSpaceTransforms
#define OFFSET_MESH_COMPONENT_SPACE 0xc50 // SkeletalMeshComponent_CachedComponentSpaceTransforms
#define OFFSET_MESH_BONE_PTR 0x30 // BonePtr compatibility
#define OFFSET_MESH_STATIC_MESH 0x9a8 // StaticMeshComponent_StaticMesh - kept for current source compatibility

// -----------------------------------------------------------------------------
// Vehicle
// -----------------------------------------------------------------------------
#define OFFSET_ACTOR_VEHICLE_COMMON 0xc08 // STExtraVehicleBase_VehicleCommon
#define OFFSET_VEHICLE_HEALTH 0x354 // UVehicleCommonComponent_HP
#define OFFSET_VEHICLE_HEALTH_MAX 0x350 // UVehicleCommonComponent_HPMax
#define OFFSET_VEHICLE_FUEL 0x43c // UVehicleCommonComponent_Fuel
#define OFFSET_VEHICLE_FUEL_MAX 0x438 // UVehicleCommonComponent_FuelMax

// -----------------------------------------------------------------------------
// Weapon & Ammo
// -----------------------------------------------------------------------------
#define OFFSET_ACTOR_WEAPON_MANAGER 0x2608 // STExtraBaseCharacter_WeaponManagerComponent
#define OFFSET_WEAPON_CURRENT_REPLICATED 0x5d8 // WeaponManagerComponent_CurrentWeaponReplicated
#define OFFSET_WEAPON_SHOOT_ENTITY 0x1370 // STExtraShootWeapon_ShootWeaponEntityComp
#define OFFSET_WEAPON_ID 0x1e0 // WeaponEntity_WeaponId
#define OFFSET_WEAPON_AMMO_CURRENT 0x1018 // STExtraShootWeapon_CurBulletInClip
#define OFFSET_WEAPON_AMMO_MAX 0x1030 // STExtraShootWeapon_CurMaxBulletNumInOneClip
#define OFFSET_WEAPON_B_IS_FIRING 0x1830 // STExtraBaseCharacter_bIsWeaponFiring
#define OFFSET_WEAPON_B_IS_GUN_ADS 0x1134 // STExtraCharacter_bIsGunADS
#define OFFSET_WEAPON_ENTITY_COMP 0x1370 // STExtraShootWeapon_ShootWeaponEntityComp

// -----------------------------------------------------------------------------
// Class / Loot / Inventory
// -----------------------------------------------------------------------------
#define OFFSET_ACTOR_CLASSID 0x18 // ClassID / FName index compatibility
#define OFFSET_ACTOR_DEADBOX_INV 0x940 // Current source compatibility; SDK PickUpListWrapperActor_PickUpDataList is 0x968
#define OFFSET_PICKUP_DATA_LIST 0x968 // PickUpListWrapperActor_PickUpDataList
#define OFFSET_PICKUP_DEFINE_ID 0x5f8 // PickUpWrapperActor_DefineID
#define OFFSET_PICKUP_COUNT 0x610 // PickUpWrapperActor_Count
#define OFFSET_INV_COUNT 0x8 // Inventory/TArray Count compatibility
#define OFFSET_INV_ARRAY 0x4 // Current source compatibility
#define OFFSET_INV_ITEM_ID 0x38 // Item ID stride compatibility

// -----------------------------------------------------------------------------
// Camera & View
// -----------------------------------------------------------------------------
#define OFFSET_SELF_PLAYER_CONTROLLER 0x4b18 // STExtraPlayerCharacter_STPlayerController
#define OFFSET_PLAYER_CAMERA_MANAGER 0x548 // PlayerController_PlayerCameraManager
#define OFFSET_CAMERA_CACHE 0x520 // PlayerCameraManager_CameraCache
#define OFFSET_ACTOR_ROTATION 0xd8 // Local Rotation compatibility
#define OFFSET_ACTOR_ATTACH_INFO 0x200 // FixAttachInfoList / current bone transform compatibility

// Old reference values kept only for comparison/debugging.
#define OFFSET_WEAPON_AMMO_CURRENT_OLD 0xfb8
#define OFFSET_WEAPON_AMMO_MAX_OLD 0xfd0
