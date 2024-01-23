// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CombatAISubSystem.generated.h"

/**
 * 
 */
UCLASS()
class THEDIVISON2_API UCombatAISubSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	/*
	* It wil Reserve the feeded location i.e. It will store the location inside of an array that we can use later in the eqs or anywhere to 
	* create a small zone for each combat AI
	* @param Actor		The Actor who wants to reserve its location.
	* @param Location	That location which actor watns to reserve.
	*/
	UFUNCTION(BlueprintCallable,Category = "AISubSystem||LocationReservation" )
	void ReserveLocation ( AActor* Actor, FVector Location );

	/*
	* It will remove the reserved location assocaited with the Actor.
	* @param Actor		The Actor who wants to free its reserved location.
	*/
	UFUNCTION(BlueprintCallable,Category = "AISubSystem||LocationReservation" )
	void FreeLocation ( AActor* Actor );

	/*
	* It will clear all the reserved locations.
	*/
	UFUNCTION(BlueprintCallable,Category = "AISubSystem||LocationReservation" )
	void ClearAllLocations ( );

	/*
	* This will return all the reserved location 
	*/
	UFUNCTION ( BlueprintCallable,BlueprintPure, Category = "AISubSystem||LocationReservation" )
	TArray<FVector> GetReservedLocations ( );


private:
	TMap<AActor*, FVector> ReservedLocations;
};
