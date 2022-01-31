// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorBehavior.h"

bool UDoorBehavior::CanActivate()
{
	Super::CanActivate();

	currentAmountOfActivations++;

	currentAmountOfActivations = FMath::Clamp(currentAmountOfActivations,0,AmountOfActivationsNeeded);

	UE_LOG(LogTemp, Warning, TEXT("Activation %s + amount of activations: %d"),
	       currentAmountOfActivations >= AmountOfActivationsNeeded ? TEXT("true") : TEXT("false"),
	       currentAmountOfActivations)

	return currentAmountOfActivations >= AmountOfActivationsNeeded;
}

bool UDoorBehavior::CanDeactivate()
{
	Super::CanDeactivate();

	currentAmountOfActivations--;

	currentAmountOfActivations = FMath::Clamp(currentAmountOfActivations,0,AmountOfActivationsNeeded);

	return true;
}
