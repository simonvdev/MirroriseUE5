// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hittable.generated.h"

USTRUCT(BlueprintType)
struct FLaserData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly)
	uint8 LaserID = 0;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Laser Data")
	FColor LaserColor = FColor::Red;

	FORCEINLINE bool operator==(const FLaserData& Other) const
	{
		return LaserID == Other.LaserID;
	}
};


// This class does not need to be modified.
UINTERFACE()
class UHittable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MIRRORRISE_API IHittable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnHit(const FLaserData& Data);
};
