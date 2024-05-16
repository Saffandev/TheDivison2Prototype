// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SmartObjects/CoverObject.h"
#include "Components/BoxComponent.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "NavModifierComponent.h"

// Sets default values
ACoverObject::ACoverObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//CoverBox
	CoverBox = CreateDefaultSubobject<UBoxComponent> ( TEXT ( "CoverBox" ) );
	RootComponent = CoverBox;

	//Cover Visual Box
	CoverVisualBox = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "CoverVisualBox" ) );
	CoverVisualBox->SetupAttachment ( CoverBox );
	CoverVisualBox->bIsEditorOnly = true;
	CoverVisualBox->SetHiddenInGame ( true );
	CoverVisualBox->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	CoverVisualBox->SetCanEverAffectNavigation ( false );
	//NavModifiers
	CoverNavModifier = CreateDefaultSubobject<UNavModifierComponent> ( TEXT ( "CoverNavModifier" ) );
	NullNavModifier = CreateDefaultSubobject<UNavModifierComponent> ( TEXT ( "NullNavModifier" ) );

}

void ACoverObject::BeginPlay ( )
{ 
	Super::BeginPlay ( );
	for ( int32 i = 0; i < CrouchPoints.Num(); i++ )
	{
		FVector Point = CrouchPoints[ i ];
		Point = UKismetMathLibrary::TransformLocation ( GetActorTransform ( ), Point );
		VectorFloor ( Point );
		CrouchPoints[ i ] = Point;
	}
	for ( int32 i = 0; i < PeakPoints.Num ( ); i++ )
	{
		FVector Point = PeakPoints[ i ];
		Point = UKismetMathLibrary::TransformLocation ( GetActorTransform ( ), Point );
		VectorFloor ( Point );
		PeakPoints[ i ] = Point;
	}
}

void ACoverObject::GenerateCoverShape ( )
{ 
	CoverBox->SetBoxExtent ( CoverBox->GetScaledBoxExtent ( ) );
	SetActorScale3D ( FVector ( 1, 1, 1 ) );
	FVector CoverBoxExtent = CoverBox->GetUnscaledBoxExtent ( );
	CoverNavModifier->FailsafeExtent = CoverBoxExtent + 50 ;
	NullNavModifier->FailsafeExtent = FVector(CoverBoxExtent.X - 50,CoverBoxExtent.Y - 50, CoverBoxExtent.Z);
	SetCoverVisMaterial ( );
#if WITH_EDITOR
	CoverVisualBox->SetWorldScale3D ( CoverBoxExtent / 50 );
	
#endif
}

TArray<FVector> ACoverObject::GetCrouchPoints ( )
{
	return CrouchPoints;
}

TArray<FVector> ACoverObject::GetPeakPoitns ( )
{
	return PeakPoints;
}

void ACoverObject::GeneratePoints ( )
{ 
	PeakPoints.Empty ( );
	CrouchPoints.Empty ( );
	for ( int32 index_X = -1; index_X <= 1; index_X++ )
	{
		for ( int32 index_Y = -1; index_Y <= 1; index_Y++ )
		{
			//Creating Peak Points (corner points)
			if ( UKismetMathLibrary::Abs ( index_X ) + UKismetMathLibrary::Abs ( index_Y ) == 2 )
			{
				FVector Point = CreatePoint ( index_X, index_Y );
				if ( CheckPointValidity ( Point ) )
				{
					PeakPoints.Add ( Point );
					CreateCrouchPointForPeakPoint ( Point );
				}
			}	
			//Creating crouch points, side points
			else if ( (index_X + index_Y != 0) && 
					  UKismetMathLibrary::Abs ( index_X ) * UKismetMathLibrary::Abs ( index_Y ) == 0 )
			{
				FVector Point = CreatePoint ( index_X, index_Y );
				if ( CheckPointValidity ( Point ) )
				{
					CrouchPoints.Add ( Point );
				}
			}
		}
	}
}

bool ACoverObject::CheckPointValidity ( FVector& Point )
{ 
	FVector TempPoint = Point;
	TempPoint = UKismetMathLibrary::TransformLocation ( GetActorTransform( ), TempPoint );
	FCollisionQueryParams Queryparams;
	Queryparams.AddIgnoredActor ( this );
	bool bHit = GetWorld ( )->SweepTestByChannel ( TempPoint, 
												   TempPoint, 
												   FQuat::Identity, 
												   ECollisionChannel::ECC_Visibility, 
												   FCollisionShape::MakeSphere ( 10.f ), 
												   Queryparams );
	//DrawDebugSphere ( GetWorld ( ), TempPoint, 20, 10, bHit ? FColor::Red : FColor::Blue, false, 4 );
	if ( bHit == false )
	{
		FNavLocation Loc;
		
		bool bProjected = UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation ( TempPoint, Loc,FVector(50,50,200) );
		if ( bProjected )
		{
			Point = UKismetMathLibrary::InverseTransformLocation ( GetActorTransform ( ), Loc.Location );
		}

		return bProjected;
	}
	return false;
	
}

FVector ACoverObject::CreatePoint ( int32 index_X, int32 index_Y )
{

	FVector BoxExtent = CoverBox->GetUnscaledBoxExtent ( );
	float PointX = index_X * CoverScale * BoxExtent.X;
	float PointY = index_Y * CoverScale * BoxExtent.Y;

	return FVector (PointX,PointY,0.f);
}

void ACoverObject::CreateCrouchPointForPeakPoint ( FVector PeakPoint )
{
	/*here we are creating 2 crouch points for each peak points, one in X direction and one in Y*/
	FVector BoxExtent = CoverBox->GetUnscaledBoxExtent ( );
	//Creating point in X direction
	if ( BoxExtent.X > 170 )
	{
		FVector Point;
		Point.X = PeakPoint.X + (150 * (PeakPoint.X > 0 ? -1 : 1));
		Point.Y = PeakPoint.Y;
		/*
		*here we are adding a small offset to Z because the peak point is already projected, 
		*so if we don't elevate out point about the ground that it will result in false when checking the point validity,
		*as we only want to check the X and Y for the point
		*/
		Point.Z = PeakPoint.Z + 30;
		//DrawDebugSphere ( GetWorld ( ), Point, 20, 10, FColor::Blue, false, 4 );
		if ( CheckPointValidity ( Point ) )
		{
			CrouchPoints.Add ( Point );
		}
	}
	if ( BoxExtent.Y > 170 )
	{
		FVector Point;
		Point.Y = PeakPoint.Y + ( 150 * (PeakPoint.Y > 0 ? -1 : 1) );
		Point.X = PeakPoint.X;
		Point.Z = PeakPoint.Z + 30;
		if ( CheckPointValidity ( Point ) )
		{
			CrouchPoints.Add ( Point );
		}
	}
}

void ACoverObject::VectorFloor ( FVector& Point )
{
	Point.X = FMath::Floor ( Point.X );
	Point.Y = FMath::Floor ( Point.Y );
	Point.Z = FMath::Floor ( Point.Z );
}

void ACoverObject::SetNavModifierExtent ( UNavModifierComponent* NavModifier,FVector Extent )
{ 
	if(NavModifier )
		NavModifier->FailsafeExtent = Extent;
}

