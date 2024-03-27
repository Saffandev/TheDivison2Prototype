// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Token/TokenSubsystem.h"
#include "AI/Token/TokenObject.h"
#include "AI/Token/TokenConfig.h"
#include "Logging/MessageLog.h"

void UTokenSubsystem::Initialize ( FSubsystemCollectionBase& Collection )
{ 
	CreateTokens ( );
}

void UTokenSubsystem::CreateTokens ( )
{
	const UTokenConfig* TokenConfig = GetDefault<UTokenConfig> ( );
	CombatTokenCount = TokenConfig->CombatToken;
	CoverChangeTokenCount = TokenConfig->CoverChangeToken;
	if ( CombatTokenCount <= 0 )
	{
		FMessageLog MessageLog = FMessageLog ( FName ( "PIE" ) );
		MessageLog.Error ( FText::FromString ( "Combat Token count is 0, go to ProjectSettings > TokenConfig > CombatToken" ) );
	}
	if ( CoverChangeTokenCount <= 0 )
	{
		FMessageLog MessageLog = FMessageLog ( FName ( "PIE" ) );
		MessageLog.Error ( FText::FromString ( "Combat Token count is 0, go to ProjectSettings > TokenConfig > CombatToken" ) );
	}
	for ( int i = 0; i < CombatTokenCount; i++ )
	{
		UTokenObject* NewToken = NewObject<UTokenObject> ( );
		NewToken->SetTokenReleaseTimer (TokenConfig->CombatTokenReleaseTimer ); 
		CombatTokens.Add(NewToken);
		
	}
	for ( int i = 0; i < CoverChangeTokenCount; i++ )
	{
		UTokenObject* NewToken = NewObject<UTokenObject> ( );
		NewToken->SetTokenReleaseTimer ( TokenConfig->CoverTokenReleaseTimer );
		CoverChangeTokens.Add ( NewToken );
	}
}

#pragma region "Setting Token"
bool UTokenSubsystem::RequestToken ( AActor * RequestingActor, ETokenType TokenType )
{
	if ( RequestingActor == nullptr )
	{
		return false;
	}
	TArray<ETokenType> Token;
	Token.Add ( TokenType );
	ReleaseToken ( RequestingActor,Token );
	UTokenObject* FreeToken = GiveFreeTokenByType ( TokenType );
	if ( FreeToken )
	{
		FreeToken->SetTokenOwner ( RequestingActor );
		return true;
	}
	return false;
}

UTokenObject* UTokenSubsystem::GiveFreeTokenByType ( ETokenType TokenType )
{
	switch ( TokenType )
	{
	case ETokenType::ETT_CombatToken:

		return GiveFreeToken ( CombatTokens );
		break;

	case ETokenType::ETT_CoverChageToken:

		return GiveFreeToken ( CoverChangeTokens );
		break;

	default:
		break;
	}
	return nullptr;
}

UTokenObject* UTokenSubsystem::GiveFreeToken ( TArray<UTokenObject*> TokenContainer )
{
	for ( UTokenObject* Token: TokenContainer)
	{
		if ( Token->IsTokenAvailable ( ) )
		{
			return Token;
		}
	}
	return nullptr;
}

#pragma endregion

#pragma region "TokenRelease"
void UTokenSubsystem::ReleaseToken ( AActor* TokenOwner, TArray<ETokenType> TokenTypeToRelease )
{ 
	if ( TokenOwner == nullptr )
	{
		return;
	}
	for ( ETokenType TokenType : TokenTypeToRelease)
	{
		switch ( TokenType )
		{
		case ETokenType::ETT_CombatToken:

			ReleaseActorToken ( CombatTokens, TokenOwner );
			break;

		case ETokenType::ETT_CoverChageToken:

			ReleaseActorToken ( CoverChangeTokens, TokenOwner );
			break;

		default:
			break;
		}
	}
}

void UTokenSubsystem::ReleaseAllTokens ( )
{
	ReleaseActorToken ( CombatTokens, nullptr, true );
	ReleaseActorToken ( CoverChangeTokens, nullptr, true );
}

void UTokenSubsystem::ReleaseActorToken ( TArray<UTokenObject*> TokenContainer, AActor* TokenOwner, bool bReqByReleaseAll )
{ 
	for ( UTokenObject* Token: TokenContainer )
	{
		if ( bReqByReleaseAll || Token->GetOwner ( ) == TokenOwner )
		{
			UE_LOG ( LogTemp, Error, TEXT ( "Token Released" ) );
			Token->SetTokenOwner ( nullptr );
		}
	}
}

#pragma endregion

#pragma region "GettingToken"
bool UTokenSubsystem::HasToken ( AActor* RequestingActor )
{


	return false;
}
#pragma endregion
