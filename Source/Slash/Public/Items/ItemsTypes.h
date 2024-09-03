#pragma once

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EOC_None UMETA(DisplayName = "None"),
	EIT_GenericItem UMETA(DisplayName = "Generic Item"),
	EIT_Sword UMETA(DisplayName = "Sword"),
	EIT_Axe UMETA(DisplayName = "Axe"),
	EIT_Hammer UMETA(DisplayName = "Hammer")
};

UENUM(BlueprintType)
enum class EOcuppedHand : uint8
{
	EOC_None UMETA(DisplayName = "None Hand"),
	EOC_Left UMETA(DisplayName = "Left Hand"),
	EOC_Right UMETA(DisplayName = "Right Hand"),
	EOC_Both UMETA(DisplayName = "Two Hand")
};
