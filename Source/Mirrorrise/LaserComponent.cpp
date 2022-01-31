// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserComponent.h"
#include "DrawDebugHelpers.h"
#include "GASSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
ULaserComponent::ULaserComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void ULaserComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < MaxBounceAmount; i++)
	{
		UNiagaraComponent* EffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			LaserEffect,
			this,
			FName(""),
			GetOwner()->GetActorLocation(),
			GetOwner()->GetActorRotation(),
			EAttachLocation::SnapToTarget,
			false);

		LaserEffects.Add(EffectComponent);

		// Set Color parameter
		EffectComponent->SetColorParameter(FName("Color"), LaserData.LaserColor);
	}

	LaserAudioComponent = UGameplayStatics::SpawnSoundAttached(SB_LaserSoundEffect, this);
}


// Called every frame
void ULaserComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsOn)
	{
		UpdateLaser();
	}
}

/**
 * @brief Updates the laser every tick
 */
void ULaserComponent::UpdateLaser()
{
	if (!LaserEffect->IsValid())
	{
		return;
	}

	for (const auto& Effect : LaserEffects)
	{
		Effect->SetActive(false);
	}

	FHitResult HitResult;
	FVector Start = GetComponentLocation();
	FVector LaserDir = FRotationMatrix(GetComponentRotation()).GetScaledAxis(EAxis::X);
	FVector End = Start + LaserDir * LaserRange;

	ECollisionChannel CollisionChannel = bPlayerLaser ? ECC_Visibility : ECC_Camera;

	for (int i = 0; i < MaxBounceAmount; i++)
	{
		FCollisionQueryParams CollisionParams;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, CollisionChannel, CollisionParams);


		if (!LaserEffects.IsValidIndex(i))
		{
			continue;
		}

		// Amount of laser effect == MaxBounceAmount see BeginPlay
		UNiagaraComponent* CurrentEffectComponent = LaserEffects[i];

		if (bHit)
		{
			CurrentEffectComponent->SetActive(true);
			CurrentEffectComponent->SetVariableVec3(FName("BeamStartPos"), Start);
			CurrentEffectComponent->SetVariableVec3(FName("BeamEndPos"), HitResult.ImpactPoint);

			if (bDebugMode)
			{
				DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Emerald, false, 0.1f, 0, 10);
			}

			if (CanBounce(HitResult))
			{
				CollisionChannel = ECC_Camera;
				
				AActor* HitActor = HitResult.GetActor();


				//If valid hit calculate the reflection for the new laser dir
				LaserDir = FMath::GetReflectionVector(LaserDir, HitResult.Normal);

				// Add the laser dir to the hit location because we don't want to have our next raycast intersect with the hit actor
				Start = HitResult.Location + LaserDir;
				End = HitResult.Location + LaserDir * LaserRange;
			}
		}
		else
		{
			CurrentEffectComponent->SetActive(true);
			CurrentEffectComponent->SetVariableVec3(FName("BeamStartPos"), Start);
			CurrentEffectComponent->SetVariableVec3(FName("BeamEndPos"), End);

			if (bDebugMode)
			{
				DrawDebugLine(GetWorld(), Start, End, FColor::Emerald, false, 0.1f, 0, 10);
			}
			// If we didn't hit a new surface then don't continue with for loop
			break;
		}
	}
}

void ULaserComponent::SetLaserState(bool bState)
{
	bIsOn = bState;

	// If effect is being turned of make sure all laser effects are turned off
	if (!bIsOn)
	{
		for (const auto& Effect : LaserEffects)
		{
			Effect->SetActive(false);
		}

		if (LaserAudioComponent)
		{
			LaserAudioComponent->Stop();
		}
	}
	else
	{
		if (LaserAudioComponent)
		{
			LaserAudioComponent->Play();
		}
	}
}

/**
 * @brief checks if the raycast can bounce of the surface
 * @param HitResult Hit result for laser raycast
 * @return returns true when the hit actor has the mirror tag
 */
bool ULaserComponent::CanBounce(const FHitResult& HitResult)
{
	AActor* HitActor = HitResult.GetActor();

	if(HitActor)
	{
		// For objects that implement UHittable
		const bool bImplements = HitActor->Implements<UHittable>();
		
		if (bImplements)
		{
			IHittable* HitInterface = Cast<IHittable>(HitActor);
			HitInterface->OnHit(LaserData);
		}

		UActorComponent * Component = HitActor->GetComponentByClass(UGASSystemComponent::StaticClass());

		// Adding gameplay effect to anything that use GAS
		if(IsValid(Component))
		{
			UGASSystemComponent * GasSystemComponent = Cast<UGASSystemComponent>(Component);

			FGameplayEffectContextHandle EffectContextHandle = GasSystemComponent->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);

			const FGameplayEffectSpecHandle EffectSpecHandle = GasSystemComponent->MakeOutgoingSpec(
				LaserGameplayEffect, 1, EffectContextHandle);

			if (EffectSpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle GEHandle = GasSystemComponent->ApplyGameplayEffectSpecToSelf(
					*EffectSpecHandle.Data.Get());
			}
		}
	}
	
	return IsValid(HitActor) && HitActor->ActorHasTag(MirrorTag);
}
