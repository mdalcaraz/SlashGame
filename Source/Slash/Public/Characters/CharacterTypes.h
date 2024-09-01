#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon"),
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive			UMETA(DisplayName = "Alive"),
	EDP_Death_Forward_1 UMETA(DisplayName = "Death_Forward_1"),
	EDP_Death_Forward_2 UMETA(DisplayName = "Death_Forward_2"),
	EDP_Death_Back_1	UMETA(DisplayName = "Death_Back_1"),
	EDP_Death_Back_2	UMETA(DisplayName = "Death_Back_2"),
	EDP_Death_Right_1	UMETA(DisplayName = "Death_Right_1"),
	EDP_Death_Right_2	UMETA(DisplayName = "Death_Right_2"),
	EDP_Death_Left_1	UMETA(DisplayName = "Death_Left_1"),
	EDP_Death_Left_2	UMETA(DisplayName = "Death_Left_2")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Atttacking UMETA(DisplayName = "Attacking"),
};