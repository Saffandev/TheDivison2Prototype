// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

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

bool ABaseAIController::LineOfSightText (AActor* Actor )
{ 
	float OtherRadius, OtherHeight;
	Actor->GetSimpleCollisionCylinder ( OtherRadius, OtherHeight );
	FVector OtherActorLocation = Actor->GetTargetLocation ( GetPawn ( ) );
	FVector ViewPoint;
	FRotator ViewRotation;
	GetActorEyesViewPoint ( ViewPoint, ViewRotation );


	FCollisionQueryParams CollisionParams ( SCENE_QUERY_STAT ( LineOfSight ), true, this->GetPawn ( ) );
	CollisionParams.AddIgnoredActor ( Actor );
	bool bhit = GetWorld ( )->LineTraceTestByChannel ( ViewPoint, OtherActorLocation, ECC_Visibility, CollisionParams );
	DrawDebugLine ( GetWorld ( ), ViewPoint, OtherActorLocation, bhit ? FColor::Green : FColor::Red, false, 0.1f );

	bhit = GetWorld ( )->LineTraceTestByChannel ( ViewPoint, OtherActorLocation + FVector ( 0.f, 0.f, OtherHeight ), ECC_Visibility, CollisionParams );
	DrawDebugLine ( GetWorld ( ), ViewPoint, OtherActorLocation + FVector ( 0.f, 0.f, OtherHeight ), bhit ? FColor::Green : FColor::Red, false, 0.1f );



	// only check sides if width of other is significant compared to distance
	if ( OtherRadius * OtherRadius / ( OtherActorLocation - ViewPoint ).SizeSquared ( ) < 0.0001f )
	{
		return false;
	}
	//try checking sides - look at dist to four side points, and cull furthest and closest
	FVector Points[ 4 ];
	Points[ 0 ] = OtherActorLocation - FVector ( OtherRadius, -1 * OtherRadius, 0 );
	Points[ 1 ] = OtherActorLocation + FVector ( OtherRadius, OtherRadius, 0 );
	Points[ 2 ] = OtherActorLocation - FVector ( OtherRadius, OtherRadius, 0 );
	Points[ 3 ] = OtherActorLocation + FVector ( OtherRadius, -1 * OtherRadius, 0 );
	DrawDebugSphere ( GetWorld ( ), Points[ 0 ], 20, 12, FColor::Red, false, .1 );
	DrawDebugSphere ( GetWorld ( ), Points[ 1 ], 20, 12, FColor::Red, false, .1 );
	DrawDebugSphere ( GetWorld ( ), Points[ 2 ], 20, 12, FColor::Red, false, .1 );
	DrawDebugSphere ( GetWorld ( ), Points[ 3 ], 20, 12, FColor::Red, false, .1 );
	int32 IndexMin = 0;
	int32 IndexMax = 0;
	FVector::FReal CurrentMax = ( Points[ 0 ] - ViewPoint ).SizeSquared ( );
	FVector::FReal CurrentMin = CurrentMax;
	for ( int32 PointIndex = 1; PointIndex < 4; PointIndex++ )
	{
		const FVector::FReal NextSize = ( Points[ PointIndex ] - ViewPoint ).SizeSquared ( );
		if ( NextSize > CurrentMin )
		{
			CurrentMin = NextSize;
			IndexMax = PointIndex;
		}
		else if ( NextSize < CurrentMax )
		{
			CurrentMax = NextSize;
			IndexMin = PointIndex;
		}
	}

	for ( int32 PointIndex = 0; PointIndex < 4; PointIndex++ )
	{
		if ( ( PointIndex != IndexMin ) && ( PointIndex != IndexMax ) )
		{
			bool bHit = GetWorld ( )->LineTraceTestByChannel ( ViewPoint, Points[ PointIndex ], ECC_Visibility, CollisionParams );
			DrawDebugLine ( GetWorld ( ), ViewPoint, Points[ PointIndex ], bHit ? FColor::Green : FColor::Red, false, 0.1f );
			if ( !bHit )
			{
				return true;
			}
		}
	}
	return false;
}
