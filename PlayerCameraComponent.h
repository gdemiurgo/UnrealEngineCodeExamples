// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCameraComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INI_API UPlayerCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerCameraComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	class UCameraComponent* CameraComp;

	class APlayerController* PlayerController;

	FVector CameraLocation;
	FVector PlayerLocation;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	bool FollowPlayerX = true;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	bool FollowPlayerY;


	UPROPERTY(EditAnywhere, Category = "Camera Position")
	float XPositionOffset;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	bool XPositionOffsetControlDistX;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	bool XPositionOffsetControlDistY;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	float IniPositionOffsetDistX;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	float EndPositionOffsetDistX;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	FVector IniXPositionOffsetLocation;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	FVector EndXPositionOffsetLocation;


	UPROPERTY(EditAnywhere, Category = "Camera Position")
	bool HeightControlDistX = true;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	bool HeightControlDistY;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	FVector IniHeightLocation;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	FVector EndHeightLocation;


	UPROPERTY(EditAnywhere, Category = "Camera Position")
	float CameraXInterpolationSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	float CameraYInterpolationSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	float CameraZInterpolationSpeed = 5.f;

	float InterpolationAlphaZ;

	float InterpolationAlphaOffsetX;

	void FollowPlayer(float DeltaTime);

	void CameraHeightControl(float DeltaTime);

	void XPositionOffsetControl(float DeltaTime);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetCameraHeightLocation(FVector IniLocation, FVector EndLocation);

	//Follow Player Control
	UFUNCTION(BlueprintCallable)
	void SetFollowPlayerX(bool FollowX);

	UFUNCTION(BlueprintCallable)
	void SetFollowPlayerY(bool FollowY);

	//Camera Height Control
	UFUNCTION(BlueprintCallable)
	void SetHeightControlDistX(bool ControlDistX);

	UFUNCTION(BlueprintCallable)
	void SetHeightControlDistY(bool ControlDistY);

	//Interpolation Speed
	UFUNCTION(BlueprintCallable)
	void SetInterpolationSpeedX(float SpeedX);

	UFUNCTION(BlueprintCallable)
	void SetInterpolationSpeedY(float SpeedY);

	UFUNCTION(BlueprintCallable)
	void SetInterpolationSpeedZ(float SpeedZ);

	UFUNCTION(BlueprintCallable)
	void SetIniPositionOffsetDistX(float IniDistX);

	UFUNCTION(BlueprintCallable)
	void SetEndPositionOffsetDistX(float EndDistX);

	UFUNCTION(BlueprintCallable)
	void SetXPositionOffsetControlDistX(bool ControlDistX);

	UFUNCTION(BlueprintCallable)
	void SetXPositionOffsetControlDistY(bool ControlDistY);

	UFUNCTION(BlueprintCallable)
	void SetXPositionOffsetLocation(FVector IniXOffsetControlLocation, FVector EndXOffsetControlLocation);
		
};
