// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TokenObject.generated.h"

/**
 * 
 */
UCLASS()
class THEDIVISON2_API UTokenObject : public UObject
{
	GENERATED_BODY()
	

public:
	void SetTokenOwner (AActor* Owner,bool bIsReleasing );
	bool IsTokenAvailable ( );
	AActor* GetOwner ( );

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly )
	AActor* TokenOwner;
	bool bIsTokenReady = true;
};
