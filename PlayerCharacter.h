// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class INI_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
    class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputConfigData* InputActions;

	void RotateBody(FVector LookAtTarget);

private:
	APlayerController* PlayerControllerRef;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WheelMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* HeadMesh;

	UPROPERTY(VisibleAnywhere)
	class USpotLightComponent* Light;

	UPROPERTY(VisibleAnywhere)
	class USpotLightComponent* LightProjector;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float BodyRotationSpeed = 25.0;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MouseSpeed = 2.0;

	UPROPERTY(EditAnywhere, Category = "Light Control")
	float LightMaxLenght = 10000;

	UPROPERTY(EditAnywhere, Category = "Light Control")
	float LightAngleSpeed;

	UPROPERTY(EditAnywhere, Category = "Light Control")
	float LightConstantSize = 220.0;

	UPROPERTY(EditAnywhere, Category = "Light Control")
	float LightProjectorConstantSize;

	class UInteractable* Interactable;

	AActor* LastHitActor = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Handle move input
	void Move(const FInputActionValue& Value);

	// Handle look input
	void Look(const FInputActionValue& Value);

	// Handle interact input
	void Interact(const FInputActionValue& Value);

	// Handle detector control
	void DetectorControl(float DeltaTime);

	// Handle light control
	void LightControl(FHitResult HitResult, float DeltaTime);

	// Handle interactables control
	void InteractableControl(AActor* HitActor);
};
