// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "InputConfigData.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SpotLightComponent.h"
#include "Engine/World.h"
#include "Interactable.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head Mesh"));
	HeadMesh->SetupAttachment(BodyMesh);

	WheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel Mesh"));
	WheelMesh->SetupAttachment(RootComponent);

	Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spot Light"));
	Light->SetupAttachment(HeadMesh);

	LightProjector = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spot Light Projector"));
	LightProjector->SetupAttachment(Light);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerControllerRef = Cast<APlayerController>(GetController());

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DetectorControl(DeltaTime);

	// Uncoment to Draw Debug Sphere
	// 	DrawDebugSphere(
	// 	GetWorld(),
	// 	HitResult.ImpactPoint,
	// 	20.f,
	// 	12,
	// 	FColor::Red,
	// 	false,
	// 	-1.f);
	// }
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get the player controller
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
 
    // Get the local player subsystem
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

    // Clear out existing mapping, and add our mapping
    Subsystem->ClearAllMappings();
    Subsystem->AddMappingContext(InputMapping, 0);

	// Get the EnhancedInputComponent
	UEnhancedInputComponent* PlayerEnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Bind the actions
	PlayerEnhancedInput->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	PlayerEnhancedInput->BindAction(InputActions->InputLook, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	PlayerEnhancedInput->BindAction(InputActions->InputJump, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
	PlayerEnhancedInput->BindAction(InputActions->InputInteract, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if(Controller != nullptr)
	{
		const FVector2D MoveValue = Value.Get<FVector2D>();
		const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

		const FVector Forward = GetActorForwardVector();
		//const FVector Forward = BodyMesh->GetForwardVector();
		AddMovementInput(Forward, MoveValue.Y);

		const FVector Right = GetActorRightVector();
		//const FVector Right = BodyMesh->GetRightVector();
		AddMovementInput(Right, MoveValue.X);
	}
}

void APlayerCharacter::RotateBody(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - HeadMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

	HeadMesh->SetWorldRotation(FMath::RInterpTo(HeadMesh->GetComponentRotation(), LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this),BodyRotationSpeed));

	FVector LightToTarget = LookAtTarget - HeadMesh->GetComponentLocation();
	FRotator LightLookAtRotation = FRotator(LightToTarget.Rotation().Pitch, HeadMesh->GetComponentRotation().Yaw, 0.f);

	Light->SetWorldRotation(FMath::RInterpTo(Light->GetComponentRotation(), LightLookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this),BodyRotationSpeed));
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    if (Controller != nullptr)
    {
        const FVector2D LookValue = Value.Get<FVector2D>();

		FRotator HeadRotationTarget = FRotator(0.f, HeadMesh->GetComponentRotation().Yaw + LookValue.X * MouseSpeed, 0.f);
		HeadMesh->SetWorldRotation(FMath::RInterpTo(HeadMesh->GetComponentRotation(), HeadRotationTarget, UGameplayStatics::GetWorldDeltaSeconds(this),BodyRotationSpeed));

		FRotator LightRotationTarget = FRotator(Light->GetComponentRotation().Pitch + LookValue.Y * MouseSpeed, HeadMesh->GetComponentRotation().Yaw, 0.f);
		Light->SetWorldRotation(FMath::RInterpTo(Light->GetComponentRotation(), LightRotationTarget, UGameplayStatics::GetWorldDeltaSeconds(this),BodyRotationSpeed));
    }
}

void APlayerCharacter::Interact(const FInputActionValue& Value)
{
	if(LastHitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("INTERACTABLE INTERACT!!!"));
	}
}

void APlayerCharacter::DetectorControl(float DeltaTime)
{
	FVector LineStart = Light->GetComponentLocation();
	FVector LineEnd = LineStart + Light->GetForwardVector() * LightMaxLenght;

	FHitResult HitResult;
	bool HasHit = GetWorld()->LineTraceSingleByChannel(HitResult, LineStart, LineEnd, ECC_GameTraceChannel2);

	if(HasHit)
	{
		LightControl(HitResult, DeltaTime);

		AActor* HitActor = HitResult.GetActor();

		Interactable = HitActor->FindComponentByClass<UInteractable>();

		if(Interactable)
		{
			if(LastHitActor != HitActor)
			{
				InteractableControl(HitActor);
				LastHitActor = HitActor;
			}
		}
		else
		{
			LastHitActor = nullptr;
		}
	}
	else if (LastHitActor != nullptr)
    {
        LastHitActor = nullptr;
    }

	// Uncoment to Draw Debug Line
	//DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Red);
	
}

void APlayerCharacter::LightControl(FHitResult HitResult, float DeltaTime)
{
	float LineLenght = HitResult.Distance;

	if(LineLenght > 0)
	{
		float TargetConeAngle = FMath::RadiansToDegrees(FMath::Atan(LightConstantSize / (LineLenght / 2)));
		float ConeAngle = FMath::FInterpTo(Light->OuterConeAngle, TargetConeAngle, DeltaTime, LightAngleSpeed);

		Light->SetOuterConeAngle(ConeAngle);
		Light->SetInnerConeAngle(Light->OuterConeAngle/2);

		float TargetProjectorConeAngle = FMath::RadiansToDegrees(FMath::Atan(LightProjectorConstantSize / (LineLenght / 2)));
		float ProjectorConeAngle = FMath::FInterpTo(LightProjector->OuterConeAngle, TargetProjectorConeAngle, DeltaTime, LightAngleSpeed);

		LightProjector->SetOuterConeAngle(ProjectorConeAngle);
		LightProjector->SetInnerConeAngle(LightProjector->OuterConeAngle/2);
	}
}

void APlayerCharacter::InteractableControl(AActor* HitActor)
{
	UE_LOG(LogTemp, Warning, TEXT("INTERACTABLE"));
}


