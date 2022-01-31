// Copyright Epic Games, Inc. All Rights Reserved.

#include "MirrorriseGameMode.h"
#include "MirrorriseCharacter.h"

AActor* AMirrorriseGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	AActor * RandomStart =  Super::ChoosePlayerStart_Implementation(Player);

	
	return LastCheckpoint != nullptr ? LastCheckpoint : RandomStart;
}

AActor* AMirrorriseGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	UWorld* World = GetWorld();
	
	AActor* BestStart = ChoosePlayerStart(Player);
	if (BestStart == nullptr)
	{
		// No player start found
		UE_LOG(LogGameMode, Log, TEXT("FindPlayerStart: PATHS NOT DEFINED or NO PLAYERSTART with positive rating"));

		// This is a bit odd, but there was a complex chunk of code that in the end always resulted in this, so we may as well just 
		// short cut it down to this.  Basically we are saying spawn at 0,0,0 if we didn't find a proper player start
		BestStart = World->GetWorldSettings();
	}

	return BestStart;
}

AMirrorriseGameMode::AMirrorriseGameMode()
	: Super()
{
	if(!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this,&AMirrorriseGameMode::PlayerDied);
	}

	if(!OnCheckPointRegistered.IsBound())
	{
		OnCheckPointRegistered.AddDynamic(this,&AMirrorriseGameMode::OnReachNewCheckpoint);
	}
}

void AMirrorriseGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	
}

void AMirrorriseGameMode::OnReachNewCheckpoint(AActor* NewSpawn)
{
	LastCheckpoint = NewSpawn;
}

void AMirrorriseGameMode::PlayerDied(ACharacter* Character)
{
	AController * Controller = Character->GetController();

	
	RestartPlayer(Controller);
}
