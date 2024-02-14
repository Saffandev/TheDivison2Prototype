// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Token/TokenObject.h"

void UTokenObject::SetTokenOwner ( AActor* Owner,bool bIsReleasing )
{ 
	if ( bIsReleasing )
	{
		bIsTokenReady = false;
		FTimerHandle ReleaseTimerHandle;
		if ( TokenOwner->GetWorld() )
		{
			TokenOwner->GetWorld ( )->GetTimerManager ( ).SetTimer ( ReleaseTimerHandle, [&] ( ) { bIsTokenReady = true; }, 2, false );
		}
		else
		{
			UE_LOG ( LogTemp, Error, TEXT ( "No World" ) );
		}
		//start timer
	}
	TokenOwner = Owner;

}

bool UTokenObject::IsTokenAvailable ( )
{
	return TokenOwner == nullptr && bIsTokenReady;
}

AActor* UTokenObject::GetOwner ( )
{
	return TokenOwner;
}
