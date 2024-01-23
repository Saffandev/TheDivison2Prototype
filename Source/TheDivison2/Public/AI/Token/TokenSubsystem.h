// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TokenSubsystem.generated.h"

class UTokenObject;
/**
 * 
 */

UENUM(BlueprintType )
enum class ETokenType : uint8
{
	ETT_CombatToken UMETA(ToolTip = "Token for shooting" ),
	ETT_CoverChageToken UMETA(ToolTip = "Token for changing cover" )
};

UCLASS()
class THEDIVISON2_API UTokenSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Initialize ( FSubsystemCollectionBase& Collection ) override;
	/*
	* Function used for generating the Token, to set the token no.go to : ProjectSetting > Token
	*/
	UFUNCTION(BlueprintCallable,Category = "Token" )
	void CreateTokens ( );

	/**
	* Will assing a token for the requesting Actor if available,return true if token as assigned 
	* @param RequestingActor	The Actor who is requesting for the token.
	* @param Token				Type of token which actor wants
	* @return					Return true if actor is assigned with a token
	*/
	UFUNCTION(BlueprintCallable,Category = "Token" )
	bool RequestToken ( AActor* RequestingActor,ETokenType TokenType);

	/*
	* This funciton will release all the token owned by the actor
	* @param TokenOwner			Actor who is asking for the token to be released
	*/
	UFUNCTION(BlueprintCallable,Category = "Token" )
	void ReleaseToken ( AActor* TokenOwner ,TArray<ETokenType> TokenTypeToRelease);

	/*
	* It will release all the tokens of all type of tokens.
	*/
	UFUNCTION(BlueprintCallable,Category = "Token" )
	void ReleaseAllTokens ( );

	/**
	* Will return bool depending if the Requesting Actor has a token asssigned or not
	* @param RequestingActor	The Actor who is requesing for the token.
	* @return					True if actor has a token
	*/
	UFUNCTION ( BlueprintCallable, Category = "Token")
	bool HasToken ( AActor* RequestingActor );


private:
	// will return the a token of specified type if there is any free/unused token available
	UTokenObject* GiveFreeTokenByType ( ETokenType TokenType);

	// will return the a token from, the tokenContainer if there is any free/unused token available
	UTokenObject* GiveFreeToken ( TArray<UTokenObject*> TokenContainer );


	void ReleaseActorToken ( TArray<UTokenObject*> TokenContainer, AActor* TokenOwner,bool bReqByReleaseAll = false );

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly )
	TArray<UTokenObject*> CombatTokens;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly )
	TArray<UTokenObject*> CoverChangeTokens;
	int32 CombatTokenCount;
	int32 CoverChangeTokenCount;
	TMap<AActor*, ETokenType> ActorList;

};
