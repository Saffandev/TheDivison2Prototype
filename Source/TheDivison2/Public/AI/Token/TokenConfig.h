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
	UPROPERTY(EditAnywhere,BlueprintReadOnly,config,Category = "Token" )
	int32 CombatToken;
	UPROPERTY ( EditAnywhere, BlueprintReadOnly, config, Category = "Token" )
	int32 CoverChangeToken;
};
