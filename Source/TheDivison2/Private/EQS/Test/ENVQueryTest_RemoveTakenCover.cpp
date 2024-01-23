// Fill out your copyright notice in the Description page of Project Settings.


#include "EQS/Test/ENVQueryTest_RemoveTakenCover.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "AI/SmartObjects/CoverObject.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ENVQueryTest_RemoveTakenCover)
#define LOCTEXT_NAMESPACE "EnvQueryGenerator"


UENVQueryTest_RemoveTakenCover::UENVQueryTest_RemoveTakenCover (const FObjectInitializer& ObjectInitializer ):Super( ObjectInitializer )
{
	Cost = EEnvTestCost::Low;
	Context = UEnvQueryContext_Querier::StaticClass ( );
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass ( );
	SetWorkOnFloatValues ( false );

}


void UENVQueryTest_RemoveTakenCover::RunTest ( FEnvQueryInstance& QueryInstance ) const
{
	UObject* DataOwner = QueryInstance.Owner.Get ( );
	BoolValue.BindData ( DataOwner, QueryInstance.QueryID );
	TArray<AActor*> CoverObj;
	//UE_LOG ( LogTemp, Error, TEXT ( "RunTest" ) );

	for ( FEnvQueryInstance :: ItemIterator It ( this, QueryInstance ); It; ++It )
	{
		AActor* CoverItem = GetItemActor ( QueryInstance, It.GetIndex ( ) );
		//UE_LOG ( LogTemp, Error, TEXT ( "Cover Item name : %s" ), *CoverItem->GetName ( ));
		bool bHasCoverOwner = false;
		if ( CoverItem )
		{
			 bHasCoverOwner = CoverItem->GetOwner ( ) == nullptr;
		}
		It.SetScore ( TestPurpose, FilterType, bHasCoverOwner, BoolValue.GetValue() );
	}
}

FText UENVQueryTest_RemoveTakenCover::GetDescriptionTitle ( ) const
{
	return FText::FromString ( "Remove used covers" );
}

FText UENVQueryTest_RemoveTakenCover::GetDescriptionDetails ( ) const
{
	return FText::FromString ( "Remove used covers" );

}

void UENVQueryTest_RemoveTakenCover::PostLoad ( )
{ 
	Super::PostLoad ( );
}
