// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ActivationBehavior.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew)
class MIRRORRISE_API UActivationBehavior : public UObject
{
	GENERATED_BODY()
public:
	virtual bool CanActivate();

	virtual bool CanDeactivate();
};
