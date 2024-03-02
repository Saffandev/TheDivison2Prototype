// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "AITraitsDataAsset.generated.h"

/**
 * 
 */

UENUM(BlueprintType )
enum class EAIType : uint8
{
	None,
	Rushers,
	Rangers,
	Shotgunner,
	HeavyMachineGunner
};
UENUM ( BlueprintType )
enum class EWeaponType : uint8
{
	SMG,
	Assault,
	Shotgun,
	LMG
};

USTRUCT(BlueprintType )
struct FAITraits : public FTableRowBase
{
	GENERATED_BODY ( )

	/*Type of the Enemy*/
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits" )
	EAIType AIType;

	/*Whether the AI can take cover or not*/
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits" )
	bool bCanTakeCover;

	/*Whether AI can throw the Grenade*/
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits" )
	bool bCanThrowGrenade;

	/*Walking Speed of the AI*/
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits||Speed" )
	float WalkingSpeed;

	/*Sprint speed of the AI*/
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits||Speed" )
	float SprintSpeed;

	/*Speed of the AI while shooting */
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits||Speed" )
	float ADSSpeed;

	/*Health of the AI(Player Health = 100)*/
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits" )
	float Health;

	/*How fast can AI Advance, i.e. how fast will it get close to the player*/
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits" )
	float AdvancingTime;
	
	/*Threshold value at which AI should be stunned*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AITraits||Stun" )
	float StunThreshold;

	/*For how long the AI should stay stunned*/
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits||Stun" )
	float StunDuration;

	/*For how long AI should Fire*/
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits" )
	float FireDuration;

	/*Minimum Gap before requesting CombatToken*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "AITraits" )
	float FireGap;

	/*Maximum range at which AI can fire*/
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits" )
	float MaxCombatRange;

	/*Minimum distance between AI and Player*/
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits" )
	float MinCombatRange;

	/*Class of the weapon that will be used by this AI*/
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits" )
	TSubclassOf<AActor> GunClass;

	/*Behavior tree that this AI will be using*/
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits" )
	class UBehaviorTree* BehaviorTree;

	/*Firing animation used by this AI*/
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits" )
	UAnimMontage* FireMontage;
};
UCLASS(BlueprintType)
class THEDIVISON2_API UAITraitsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "AITraits" )
	FAITraits AITraits;
};
