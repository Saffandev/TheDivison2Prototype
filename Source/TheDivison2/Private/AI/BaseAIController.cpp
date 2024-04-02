// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseAIController::ABaseAIController ( const FObjectInitializer& ObjectInitializer ):Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent" ) ) )
{

}

FVector ABaseAIController::GetFocalPointOnActor ( const AActor* Actor ) const
{
	if ( Actor && UGameplayStatics::GetPlayerPawn (this,0 ) == Actor )
	{
		return Actor->GetActorLocation ( ) + FocusOffset;
	}
	return FVector ( );
}

void ABaseAIController::UpdateControlRotation ( float DeltaTime, bool bUpdatePawn )
{ 
	APawn* const MyPawn = GetPawn ( );
	if ( MyPawn )
	{
		FRotator NewControlRotation = GetControlRotation ( );

		// Look toward focus
		const FVector FocalPoint = GetFocalPoint ( );
		if ( FAISystem::IsValidLocation ( FocalPoint ) )
		{
			NewControlRotation = ( FocalPoint - MyPawn->GetPawnViewLocation ( ) ).Rotation ( );
		}
		else if ( bSetControlRotationFromPawnOrientation )
		{
			NewControlRotation = MyPawn->GetActorRotation ( );
		}

		// Don't pitch view unless looking at another pawn
	/*	if ( NewControlRotation.Pitch != 0 && Cast<APawn> ( GetFocusActor ( ) ) == nullptr )
		{
			NewControlRotation.Pitch = 0.f;
		}*/

		SetControlRotation ( NewControlRotation );

		if ( bUpdatePawn )
		{
			const FRotator CurrentPawnRotation = MyPawn->GetActorRotation ( );

			if ( CurrentPawnRotation.Equals ( NewControlRotation, 1e-3f ) == false )
			{
				MyPawn->FaceRotation ( NewControlRotation, DeltaTime );
			}
		}
	}
}
