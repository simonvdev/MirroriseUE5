// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivatableActor.h"

#include "DrawDebugHelpers.h"


void AActivatableActor::TryActivate()
{
	if(bActivationState) return;
	
	if(ActivationBehavior && ActivationBehavior->CanActivate())
	{
		SetActivatableState(true);
	}
}

void AActivatableActor::TryDeactivate()
{
	if(!bActivationState) return;
	
	if(ActivationBehavior && ActivationBehavior->CanDeactivate())
	{
		SetActivatableState(false);
	}
}

void AActivatableActor::DrawDebug() const
{
	const UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is null"));
		return;
	}

	const FColor DebugColor = bActivationState ? FColor::Green : FColor::Red;

	for (const auto& LinkedActor : LinkedActors)
	{
		if (LinkedActor)
		{
			DrawDebugDirectionalArrow(World, LinkedActor->GetActorLocation(), GetActorLocation(), 1.0f, DebugColor,
			                          true);
		}
	}
}

void AActivatableActor::ActivateLinkedActors()
{
	for (const auto& LinkedActor : LinkedActors)
	{
		if (LinkedActor)
		{
			LinkedActor->TryActivate();
		}
	}
}

void AActivatableActor::DeactivateLinkedActors()
{
	for (const auto& LinkedActor : LinkedActors)
	{
		if (LinkedActor)
		{
			LinkedActor->TryDeactivate();
		}
	}
}

void AActivatableActor::SetActivatableState(const bool& bNewState)
{
	bActivationState = bNewState;

	if (bActivationState)
	{
		OnActivate();
		ActivateLinkedActors();
	}
	else
	{
		OnDeactivate();
		DeactivateLinkedActors();
	}

	if (bDebugMode)
	{
		DrawDebug();
	}
}


// Sets default values
AActivatableActor::AActivatableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AActivatableActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetActivatableState(bActivationState);

}