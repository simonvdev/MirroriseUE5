// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivatableActor.h"
#include "Hittable.h"
#include "Sensor.generated.h"


UCLASS()
class MIRRORRISE_API ASensor : public AActivatableActor, public IHittable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASensor();

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FLaserData AcceptedLaserData;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnHit(const FLaserData& Data) override;
};
