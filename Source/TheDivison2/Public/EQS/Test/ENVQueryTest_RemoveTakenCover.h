// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "UObject/ObjectMacros.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "ENVQueryTest_RemoveTakenCover.generated.h"

/**
 * 
 */
UCLASS()
class THEDIVISON2_API UENVQueryTest_RemoveTakenCover : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()
	
	//UENVQueryTest_RemoveTakenCover (const FObjectInitializer& ObjectInitializer );

	virtual void RunTest ( FEnvQueryInstance& QueryInstance ) const override;
	
	UPROPERTY ( EditDefaultsOnly)
	TSubclassOf<UEnvQueryContext> Context;

	virtual FText GetDescriptionTitle ( ) const override;
	virtual FText GetDescriptionDetails ( ) const override;

	virtual void PostLoad ( ) override;
};
