// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MirrorriseGameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedSignature, ACharacter*, Character);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckPointRegistered, AActor*, NewSpawn);

UCLASS(minimalapi)
class AMirrorriseGameMode : public AGameModeBase
{

private:
	GENERATED_BODY()
public:
	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;
	AMirrorriseGameMode();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	const FOnPlayerDiedSignature & GetOnPlayerDied() const { return  OnPlayerDied;}
	
	const FOnCheckPointRegistered & GetOnCheckPointRegistered() const { return OnCheckPointRegistered; }

	virtual void RestartPlayer(AController* NewPlayer) override;

protected:	
	UFUNCTION()
	virtual void PlayerDied(ACharacter * Character);

	UFUNCTION()
	void OnReachNewCheckpoint(AActor * NewSpawn);
	
	UPROPERTY()
	FOnPlayerDiedSignature OnPlayerDied;

	UPROPERTY(BlueprintCallable,BlueprintAssignable,BlueprintReadOnly)
	FOnCheckPointRegistered OnCheckPointRegistered;

	UPROPERTY()
	AActor * LastCheckpoint = nullptr;
};
