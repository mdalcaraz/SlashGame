// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "Items/ItemsTypes.h"

#include "BaseCharacter.generated.h"


class AWeapon;
class UHealthBarComponent;
class UAttributeComponent;
class UAnimMontage;


UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	void DirectionalHitReact(const FVector& ImpactPoint);
	void DirectionalDie(const FVector& ImpactPoint);


protected:
	virtual void BeginPlay() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void Attack(const FInputActionValue& Value);
	virtual void Die(const FName& SectionName);
	virtual void HandleDamage(float DamageAmount);
	
	void DisableCapsule();
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticules(const FVector& ImpactPoint);
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	void PlayHitReactMontage(const FName& SectionName);
	void StopAttackMontage();
	int32 PlayAttackMontage();
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	UFUNCTION(BLueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BLueprintCallable)
	FVector GetRotationWarpTarget();

	virtual bool CanAttack();
	bool IsAlive();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UPROPERTY(EditAnywhere, Category = "Weapon")
	EOcuppedHand WeaponOcuppedHand = EOcuppedHand::EOC_None;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	EItemType	 WeaponEquippedType = EItemType::EOC_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI Navigation")
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarptargetDistance = 75.f;
	/**
	* Animation montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* FrontDeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* BackDeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* LeftDeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* RightDeathMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> NoWeaponAttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> SwordOneHandAttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> SwordTwoHandAttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> AxeOneHandAttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> AxeTwoHandAttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> HammerOneHandAttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> HammerTwoHandAttackMontageSections;
	/*
	* Components
	*/
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(VIsibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* HitParticles;
};
