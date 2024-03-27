// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TokenConfig.generated.h"

/**
 * 
 */
UCLASS(Config = Game, defaultconfig)
class THEDIVISON2_API UTokenConfig : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,config,Category = "Token|Combat" )
	int32 CombatToken;
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "Token|Combat" )
	float CombatTokenReleaseTimer;
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, config, Category = "Token|Cover" )
	int32 CoverChangeToken;
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, Category = "Token|Cover" )
	float CoverTokenReleaseTimer;
	
};
