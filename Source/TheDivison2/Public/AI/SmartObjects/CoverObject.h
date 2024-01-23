// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverObject.generated.h"

class UBoxComponent;
class UNavModifierComponent;

UCLASS()
class THEDIVISON2_API ACoverObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoverObject();

	UFUNCTION(BlueprintCallable,Category = "Cover" )
	void SetActorOwner ( AActor* NewOwner );

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent,CallInEditor,Category = "Cover" )
	void Generate ( );
	UFUNCTION(Blueprintcallable,Category = "Cover" )
	void SetNavModifierExtent ( FVector Extent );
	UFUNCTION(BlueprintPure,Category = "Cover"  )
	AActor* GetActorOwner ( ) const
	{
		return GetOwner();
	}


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Cover" )
	UBoxComponent* CoverBox;
	UPROPERTY(VisibleAnywhere,Category = "Cover" )
	UNavModifierComponent* NavModifier;


};
