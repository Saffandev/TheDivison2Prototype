// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SmartObjects/CoverObject.h"
#include "Components/BoxComponent.h"
#include "NavModifierComponent.h"

// Sets default values
ACoverObject::ACoverObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CoverBox = CreateDefaultSubobject<UBoxComponent> ( TEXT ( "CoverBox" ) );
	RootComponent = CoverBox;
	CoverNavModifier = CreateDefaultSubobject<UNavModifierComponent> ( TEXT ( "CoverNavModifier" ) );
	NullNavModifier = CreateDefaultSubobject<UNavModifierComponent> ( TEXT ( "NullNavModifier" ) );

}

void ACoverObject::SetActorOwner ( AActor* NewOwner )
{ 
	SetOwner ( NewOwner );
}

void ACoverObject::SetNavModifierExtent ( UNavModifierComponent* NavModifier,FVector Extent )
{ 
	if(NavModifier )
		NavModifier->FailsafeExtent = Extent;
}


