// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivationBehavior.h"
#include "GameFramework/Actor.h"
#include "ActivatableActor.generated.h"


UCLASS()
class MIRRORRISE_API AActivatableActor : public AActor
{
	GENERATED_BODY()

public:
	AActivatableActor();

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ActivatableActor")
	bool bActivationState = true;

	UPROPERTY(EditInstanceOnly, Category = "ActivatableActor")
	bool bDebugMode;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "ActivatableActor")
	TArray<TObjectPtr<AActivatableActor>> LinkedActors;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "ActivatableActor",Instanced)
	TObjectPtr<UActivationBehavior> ActivationBehavior;
	

private:
	void DrawDebug() const;

	void ActivateLinkedActors();
	void DeactivateLinkedActors();


	
protected:
	virtual void BeginPlay() override;

public:
	
	virtual void TryActivate();
	virtual void TryDeactivate();

	UFUNCTION(BlueprintImplementableEvent)
	void OnActivate();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeactivate();

	UFUNCTION(BlueprintCallable)
	void SetActivatableState(const bool& bNewState);

private:

};
