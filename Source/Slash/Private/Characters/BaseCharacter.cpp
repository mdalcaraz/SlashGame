// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapons/Weapon.h"
#include "EnhancedInputComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);


}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack(const FInputActionValue& Value)
{

}

void ABaseCharacter::Die(const FName& SectionName)
{
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

void ABaseCharacter::SpawnHitParticules(const FVector& ImpactPoint)
{
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return 0;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

int32 ABaseCharacter::PlayAttackMontage()
{
	switch (WeaponEquippedType)
	{
	case EItemType::EIT_Sword:
		if (WeaponOcuppedHand == EOcuppedHand::EOC_Right)
		{
			return PlayRandomMontageSection(AttackMontage, SwordOneHandAttackMontageSections);
		}
		else if(WeaponOcuppedHand == EOcuppedHand::EOC_Both)
		{
			return PlayRandomMontageSection(AttackMontage, SwordTwoHandAttackMontageSections);
		}
		break;
	case EItemType::EIT_Axe:
		if (WeaponOcuppedHand == EOcuppedHand::EOC_Right)
		{
			return PlayRandomMontageSection(AttackMontage, AxeOneHandAttackMontageSections);
		}
		else if (WeaponOcuppedHand == EOcuppedHand::EOC_Both)
		{
			return PlayRandomMontageSection(AttackMontage, AxeTwoHandAttackMontageSections);
		}
		break;
	case EItemType::EIT_Hammer:
		if (WeaponOcuppedHand == EOcuppedHand::EOC_Right)
		{
			return PlayRandomMontageSection(AttackMontage, HammerOneHandAttackMontageSections);
		}
		else if (WeaponOcuppedHand == EOcuppedHand::EOC_Both)
		{
			return PlayRandomMontageSection(AttackMontage, HammerTwoHandAttackMontageSections);
		}
		break;
	}
	return PlayRandomMontageSection(AttackMontage, NoWeaponAttackMontageSections);

}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}


void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)

{
	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// if CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);

	/*
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
	*/
}


void ABaseCharacter::DirectionalDie(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// if CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}
	FName Section;

	if (Theta >= -45.f && Theta < 45.f)
	{
		switch (FMath::RandRange(0, 1))
		{
		case 0:
			Section = FName("Death_Back_1");
			DeathPose = EDeathPose::EDP_Death_Back_1;
			break;
		case 1:
			Section = FName("Death_Back_2");
			DeathPose = EDeathPose::EDP_Death_Back_2;
			break;
		}
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		switch (FMath::RandRange(0, 1))
		{
		case 0:
			Section = FName("Death_Right_1");
			DeathPose = EDeathPose::EDP_Death_Right_1;
			break;
		case 1:
			Section = FName("Death_Right_2");
			DeathPose = EDeathPose::EDP_Death_Right_2;
			break;
		}
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		switch (FMath::RandRange(0, 1))
		{
		case 0:
			Section = FName("Death_Left_1");
			DeathPose = EDeathPose::EDP_Death_Left_1;
			break;
		case 1:
			Section = FName("Death_Left_2");
			DeathPose = EDeathPose::EDP_Death_Left_2;
			break;
		}
	}
	else
	{
		switch (FMath::RandRange(0, 1))
		{
		case 0:
			Section = FName("Death_Front_1");
			DeathPose = EDeathPose::EDP_Death_Right_1;
			break;
		case 1:
			Section = FName("Death_Front_2");
			DeathPose = EDeathPose::EDP_Death_Right_2;
			break;
		}

	}
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}

	Die(Section);

	/*
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
	*/
}
