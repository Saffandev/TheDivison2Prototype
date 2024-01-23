// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CombatAISubSystem.h"

void UCombatAISubSystem::ReserveLocation ( AActor* Actor, FVector Location )
{ 
	if ( IsValid ( Actor ) )
	{
		ReservedLocations.Add ( Actor, Location );
	}
}

void UCombatAISubSystem::FreeLocation ( AActor * Actor )
{ 
	if ( IsValid ( Actor ) )
	{
		ReservedLocations.Remove ( Actor );
	}
}

void UCombatAISubSystem::ClearAllLocations ( )
{ 
	ReservedLocations.Empty ( );
}

TArray<FVector> UCombatAISubSystem::GetReservedLocations ( )
{
	TArray<FVector> ReservedLocationsArray;
	ReservedLocations.GenerateValueArray ( ReservedLocationsArray );
	return ReservedLocationsArray;
}
