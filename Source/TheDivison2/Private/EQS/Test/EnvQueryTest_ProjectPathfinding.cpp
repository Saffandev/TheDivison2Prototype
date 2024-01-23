// Fill out your copyright notice in the Description page of Project Settings.


#include "EQS/Test/EnvQueryTest_ProjectPathfinding.h"
#include "AI/Navigation/NavAgentInterface.h"
#include "Engine/World.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(EnvQueryTest_ProjectPathfinding)

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

UEnvQueryTest_ProjectPathfinding::UEnvQueryTest_ProjectPathfinding ( const FObjectInitializer& ObjectInitializer ) : Super ( ObjectInitializer )
{
	Context = UEnvQueryContext_Querier::StaticClass ( );
	Cost = EEnvTestCost::High;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass ( );
	TestMode = EEnvTestPathfinding::PathExist;
	PathFromContext.DefaultValue = true;
	SkipUnreachable.DefaultValue = true;
	FloatValueMin.DefaultValue = 1000.0f;
	FloatValueMax.DefaultValue = 1000.0f;
	SetWorkOnFloatValues ( TestMode != EEnvTestPathfinding::PathExist );
}

void UEnvQueryTest_ProjectPathfinding::RunTest ( FEnvQueryInstance& QueryInstance ) const
{ 
	UObject* QueryOwner = QueryInstance.Owner.Get ( );
	BoolValue.BindData ( QueryOwner, QueryInstance.QueryID );
	PathFromContext.BindData ( QueryOwner, QueryInstance.QueryID );
	SkipUnreachable.BindData ( QueryOwner, QueryInstance.QueryID );
	FloatValueMin.BindData ( QueryOwner, QueryInstance.QueryID );
	FloatValueMax.BindData ( QueryOwner, QueryInstance.QueryID );

	bool bWantsPath = BoolValue.GetValue ( );
	bool bPathToItem = PathFromContext.GetValue ( );
	bool bDiscardFailed = SkipUnreachable.GetValue ( );
	float MinThresholdValue = FloatValueMin.GetValue ( );
	float MaxThresholdValue = FloatValueMax.GetValue ( );

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1> ( QueryInstance.World );
	if ( NavSys == nullptr || QueryOwner == nullptr )
	{
		return;
	}

	ANavigationData* NavData = FindNavigationData ( *NavSys, QueryOwner );
	if ( NavData == nullptr )
	{
		return;
	}

	TArray<FVector> ContextLocations;
	if ( !QueryInstance.PrepareContext ( Context, ContextLocations ) )
	{
		return;
	}

	EPathFindingMode::Type PFMode ( EPathFindingMode::Regular );
	FSharedConstNavQueryFilter NavFilter = UNavigationQueryFilter::GetQueryFilter ( *NavData, QueryOwner, GetNavFilterClass ( QueryInstance ) );

	if ( GetWorkOnFloatValues ( ) )
	{
		FFindPathSignature FindPathFunc;
		FindPathFunc.BindUObject ( this, TestMode == EEnvTestPathfinding::PathLength ?
								   ( bPathToItem ? &UEnvQueryTest_ProjectPathfinding::FindPathLengthTo : &UEnvQueryTest_ProjectPathfinding::FindPathLengthFrom ) :
								   ( bPathToItem ? &UEnvQueryTest_ProjectPathfinding::FindPathCostTo : &UEnvQueryTest_ProjectPathfinding::FindPathCostFrom ) );

		NavData->BeginBatchQuery ( );
		for ( FEnvQueryInstance::ItemIterator It ( this, QueryInstance ); It; ++It )
		{
			FVector LItemLocation = GetItemLocation ( QueryInstance, It.GetIndex ( ) );
			ProjectPointToNavMesh ( NavSys, LItemLocation );
			const FVector ItemLocation = LItemLocation;
			for ( int32 ContextIndex = 0; ContextIndex < ContextLocations.Num ( ); ContextIndex++ )
			{
				const float PathValue = FindPathFunc.Execute ( ItemLocation, ContextLocations[ContextIndex], PFMode, *NavData, *NavSys, NavFilter, QueryOwner );
				It.SetScore ( TestPurpose, FilterType, PathValue, MinThresholdValue, MaxThresholdValue );

				if ( bDiscardFailed && PathValue >= BIG_NUMBER )
				{
					It.ForceItemState ( EEnvItemStatus::Failed );
				}
			}
		}
		NavData->FinishBatchQuery ( );
	}
	else
	{
		NavData->BeginBatchQuery ( );
		if ( bPathToItem )
		{
			for ( FEnvQueryInstance::ItemIterator It ( this, QueryInstance ); It; ++It )
			{
				FVector LItemLocation = GetItemLocation ( QueryInstance, It.GetIndex ( ) );
				ProjectPointToNavMesh ( NavSys, LItemLocation );
				const FVector ItemLocation = LItemLocation;
				for ( int32 ContextIndex = 0; ContextIndex < ContextLocations.Num ( ); ContextIndex++ )
				{
					FVector Point = ContextLocations[ ContextIndex ];
					ProjectPointToNavMesh ( NavSys, Point );
					const bool bFoundPath = TestPathTo ( ItemLocation, ContextLocations[ ContextIndex ], PFMode, *NavData, *NavSys, NavFilter, QueryOwner );
					It.SetScore ( TestPurpose, FilterType, bFoundPath, bWantsPath );
				}
			}
		}
		else
		{
			for ( FEnvQueryInstance::ItemIterator It ( this, QueryInstance ); It; ++It )
			{
				FVector LItemLocation = GetItemLocation ( QueryInstance, It.GetIndex ( ) );
				ProjectPointToNavMesh ( NavSys, LItemLocation );
				const FVector ItemLocation = LItemLocation;
				for ( int32 ContextIndex = 0; ContextIndex < ContextLocations.Num ( ); ContextIndex++ )
				{
					FVector Point = ContextLocations[ ContextIndex ];
					ProjectPointToNavMesh ( NavSys, Point );

					const bool bFoundPath = TestPathFrom ( ItemLocation, ContextLocations[ ContextIndex ], PFMode, *NavData, *NavSys, NavFilter, QueryOwner );
					It.SetScore ( TestPurpose, FilterType, bFoundPath, bWantsPath );
				}
			}
		}
		NavData->FinishBatchQuery ( );
	}
}

void UEnvQueryTest_ProjectPathfinding::ProjectPointToNavMesh ( UNavigationSystemV1* Nav, FVector& Point ) const
{ 
	FNavLocation OutLocation;
	OutLocation.Location = Point;
	if ( bShouldProject )
	{
		bool b = Nav->ProjectPointToNavigation ( Point, OutLocation, ExtentX );
		//UE_LOG ( LogTemp, Warning, TEXT ( "Porjected Point : %i" ), b );
		//DrawDebugSphere ( GetWorld ( ), OutLocation.Location, 100, 10, FColor::Red, false, 5.f );
	}
	Point = OutLocation.Location;
}
