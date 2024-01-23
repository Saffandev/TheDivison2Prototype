// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Tests/EnvQueryTest_Pathfinding.h"
#include "EnvQueryTest_ProjectPathfinding.generated.h"

/**
 * 
 */
UCLASS()
class THEDIVISON2_API UEnvQueryTest_ProjectPathfinding : public UEnvQueryTest_Pathfinding
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(EditDefaultsOnly,Category = PathFinding )
	bool bShouldProject;

	UPROPERTY ( EditDefaultsOnly, Category = PathFinding,Meta=( EditCondition = bShouldProject) )
	FVector ExtentX;
			
	virtual void RunTest ( FEnvQueryInstance& QueryInstance ) const override;

	void ProjectPointToNavMesh (class UNavigationSystemV1 *Nav, FVector& Point ) const;
};
