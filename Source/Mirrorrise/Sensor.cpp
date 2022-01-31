// Fill out your copyright notice in the Description page of Project Settings.


#include "Sensor.h"


// Sets default values
ASensor::ASensor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASensor::BeginPlay()
{
	Super::BeginPlay();
}


void ASensor::OnHit(const FLaserData& Data)
{
	IHittable::OnHit(Data);

	// Same laser ID
	if (AcceptedLaserData == Data)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit a sensor with the same laser id!"));

		TryActivate();
	}
}
