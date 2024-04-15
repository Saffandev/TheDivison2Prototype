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

	/*UFUNCTION(BlueprintCallable,Category = "Cover" )
	void SetActorOwner ( AActor* NewOwner );*/

	virtual void BeginPlay ( ) override;

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent,CallInEditor,Category = "Cover" )
	void Generate ( );

	/*Function to generate cover points*/
	UFUNCTION ( BlueprintCallable, CallInEditor, Category = "Cover" )
	void GeneratePoints ( );

	UFUNCTION(BlueprintCallable,CallInEditor,Category = "Cover" )
	void GenerateCoverShape ( );

	/*Function to return Generated Crouch Cover Points */
	UFUNCTION( BlueprintPure,Category = "Cover" )
	TArray<FVector> GetCrouchPoints ( );

	/*Function to return Generated Peak Cover Poitns*/
	UFUNCTION(BlueprintPure,Category = "Cover" )
	TArray<FVector> GetPeakPoitns ( );

	/*Functin to set the failsafe extents of the nav modifier*/
	UFUNCTION(Blueprintcallable,Category = "Cover" )
	void SetNavModifierExtent ( UNavModifierComponent* NavModifier, FVector Extent );

private:
	/*Function to check if the generated points is valid or not (here valid means that if the points is valid for the AI to take cover)*/
	bool CheckPointValidity ( FVector& Point );

	/*Function to generate the point based of provided index */
	FVector CreatePoint (int32 index_X, int32 index_Y);

	/*Function To generate CrouchCoverPoint*/
	void CreateCrouchPointForPeakPoint ( FVector PeakPoint );

	void VectorFloor ( FVector& Point );
protected:
	/*Cover box depending on which cover points are generated*/
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Cover" )
	UBoxComponent* CoverBox;

	UPROPERTY ( VisibleAnywhere, BlueprintReadOnly, Category = "Cover" )
	UStaticMeshComponent* CoverVisualBox;
	/*NavModifier defining cover area,area around the cover*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Cover" )
	UNavModifierComponent* CoverNavModifier;

	/*NavModifier difing null area, area under the cover*/
	UPROPERTY ( VisibleAnywhere, BlueprintReadOnly, Category = "Cover" )
	UNavModifierComponent* NullNavModifier;
	
	/**
	* Value to expand or contract the cover point,
	* Value < 1 will contract the cover point
	* Value > 1 wil expand the cover points 
	*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Cover" )
	float CoverScale = 1.f;

	UPROPERTY(EditAnywhere,meta = (MakeEditWidget = true ),Category = "Cover" )
	TArray<FVector> CrouchPoints;
	UPROPERTY ( EditAnywhere, meta = ( MakeEditWidget = true ),Category = "Cover" )
	TArray<FVector> PeakPoints;

};
