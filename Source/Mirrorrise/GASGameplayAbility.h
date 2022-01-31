// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mirrorrise.h"
#include "Abilities/GameplayAbility.h"
#include "GASGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class MIRRORRISE_API UGASGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	TEnumAsByte<EGASAbilityInputID> AbilityInputID = None;
};
