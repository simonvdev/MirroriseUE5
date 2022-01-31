// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hittable.h"
#include "Components/SceneComponent.h"
#include "NiagaraSystem.h"
#include "LaserComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MIRRORRISE_API ULaserComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULaserComponent();

	UPROPERTY(BlueprintReadOnly)
	bool bIsOn = true;

	UPROPERTY(EditDefaultsOnly)
	bool bPlayerLaser = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraSystem* LaserEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* SB_LaserSoundEffect;

	UPROPERTY(EditDefaultsOnly, Category = "LaserSettings")
	int MaxBounceAmount = 5;

	UPROPERTY(EditDefaultsOnly, Category = "LaserSettings")
	float LaserRange = 5000;

	UPROPERTY(EditDefaultsOnly, Category = "LaserSettings")
	FName MirrorTag = "Mirror";

	UPROPERTY(EditInstanceOnly, Category= "LaserSettings")
	FLaserData LaserData;

	UPROPERTY(EditAnywhere, Category = "LaserSettings")
	bool bDebugMode = false;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect> LaserGameplayEffect;
private:
	TArray<UNiagaraComponent*> LaserEffects;

	UPROPERTY()
	UAudioComponent* LaserAudioComponent;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void UpdateLaser();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable, Category = "Laser")
	void SetLaserState(bool bState);

private:
	UFUNCTION(BlueprintInternalUseOnly)
	bool CanBounce(const FHitResult& HitResult);
};
