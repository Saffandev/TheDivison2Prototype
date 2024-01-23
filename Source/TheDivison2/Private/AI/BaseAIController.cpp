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
		return Actor->GetActorLocation ( ) + FVector ( 0, 0, 50.f );
	}
	return FVector ( );
}
