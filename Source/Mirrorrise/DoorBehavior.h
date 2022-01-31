// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivationBehavior.h"
#include "UObject/Object.h"
#include "DoorBehavior.generated.h"

/**
 * 
 */
UCLASS()
class MIRRORRISE_API UDoorBehavior : public UActivationBehavior
{
	GENERATED_BODY()
public:
	virtual bool CanActivate() override;
	virtual bool CanDeactivate() override;
	
	UPROPERTY(EditInstanceOnly)
	int AmountOfActivationsNeeded = 1;
private:
	int currentAmountOfActivations;
};
