// Copyright Epic Games, Inc. All Rights Reserved.

#include "MirrorriseCharacter.h"

#include "GASAttributeSet.h"
#include "GASGameplayAbility.h"
#include "GASSystemComponent.h"
#include "MirrorriseGameMode.h"
#include "MirrorriseProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AMirrorriseCharacter

UAbilitySystemComponent* AMirrorriseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

AMirrorriseCharacter::AMirrorriseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));


	// AbilitySystem
	AbilitySystemComponent = CreateDefaultSubobject<UGASSystemComponent>("AbilitySystem");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this,&AMirrorriseCharacter::OnHealthChanged);

	// AttributSet
	AttributeSet = CreateDefaultSubobject<UGASAttributeSet>("Attributes");
}

void AMirrorriseCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AMirrorriseCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
			DefaultAttributeEffect, 1, EffectContextHandle);

		if (EffectSpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
				*EffectSpecHandle.Data.Get());
		}
	}
}

void AMirrorriseCharacter::GiveDefaultAbilities()
{
	//Only server can give Abilities
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (const auto& DefaultAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(DefaultAbility, 1, DefaultAbility.GetDefaultObject()->AbilityInputID, this));
		}
	}
}

// Called On Server
void AMirrorriseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();

	GiveDefaultAbilities();
}

//On Client
void AMirrorriseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();

	//AbilitySystemComponent->BindAbilityActivationToInputComponent()
}

void AMirrorriseCharacter::OnHealthChanged(const FOnAttributeChangeData & Data)
{
	const float Health = Data.NewValue;
	
	if(!(Health > 0))
	{
		RestartPlayer();
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMirrorriseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMirrorriseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMirrorriseCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMirrorriseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMirrorriseCharacter::LookUpAtRate);

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds
		{
			"Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(Confirm),
			static_cast<int32>(Cancel)
		};

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void AMirrorriseCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMirrorriseCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMirrorriseCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMirrorriseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMirrorriseCharacter::Destroyed()
{
	Super::Destroyed();

	if(const UWorld * World = GetWorld())
	{
		if(const AMirrorriseGameMode * GameMode = Cast<AMirrorriseGameMode>(World->GetAuthGameMode()))
		{
			GameMode->GetOnPlayerDied().Broadcast(this);
		}
	}
}

void AMirrorriseCharacter::RestartPlayer()
{
	AController * CachedController = GetController();

	Destroy();

	if(const UWorld * World = GetWorld())
	{
		if(AMirrorriseGameMode * GameMode = Cast<AMirrorriseGameMode>(World->GetAuthGameMode()))
		{
			GameMode->RestartPlayer(CachedController);
		}
	}
}
