// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPlayerCameraComponent::UPlayerCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(PlayerController)
	{
		PlayerController->SetViewTarget(GetOwner());
	}
}


// Called every frame
void UPlayerCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PlayerLocation = PlayerController->GetPawn()->GetActorLocation();
	CameraLocation = GetOwner()->GetActorLocation();

	if(XPositionOffsetControlDistX || XPositionOffsetControlDistY)
	{
		XPositionOffsetControl(DeltaTime);
	}

	if(PlayerController)
	{
		FollowPlayer(DeltaTime);
	}

	CameraHeightControl(DeltaTime);

	GetOwner()->SetActorLocation(CameraLocation);
}

void UPlayerCameraComponent::FollowPlayer(float DeltaTime)
{
	if(FollowPlayerX)
	{
		float TargetXValue = FMath::FInterpTo(CameraLocation.X, PlayerLocation.X + XPositionOffset, DeltaTime, CameraXInterpolationSpeed);
		CameraLocation.X = TargetXValue;
	}

	if(FollowPlayerY)
	{
		float TargetYValue = FMath::FInterpTo(CameraLocation.Y, PlayerLocation.Y, DeltaTime, CameraYInterpolationSpeed);
		CameraLocation.Y = TargetYValue;
		CameraLocation.X = PlayerLocation.X + XPositionOffset;
	}
	else if (CameraLocation.Y < 0 || CameraLocation.Y > 0)
	{
		float TargetYValue = FMath::FInterpTo(CameraLocation.Y, 0, DeltaTime, CameraYInterpolationSpeed);
		CameraLocation.Y = TargetYValue;
		CameraLocation.X = PlayerLocation.X + XPositionOffset;
	}
}

void UPlayerCameraComponent::CameraHeightControl(float DeltaTime)
{
	if(HeightControlDistX)
	{
		InterpolationAlphaZ = FMath::Clamp((PlayerLocation.X - IniHeightLocation.X) / (EndHeightLocation.X - IniHeightLocation.X), 0.f, 1.f);
	}
	else if(HeightControlDistY)
	{
		InterpolationAlphaZ = FMath::Clamp((PlayerLocation.Y - IniHeightLocation.Y) / (EndHeightLocation.Y - IniHeightLocation.Y), 0.f, 1.f);
	}

	float TargetZValue = FMath::Lerp(IniHeightLocation.Z, EndHeightLocation.Z, InterpolationAlphaZ);
	float ZPosition = FMath::FInterpTo(CameraLocation.Z, TargetZValue, DeltaTime, CameraZInterpolationSpeed);

	CameraLocation.Z = ZPosition;
}

void UPlayerCameraComponent::XPositionOffsetControl(float DeltaTime)
{
	if(XPositionOffsetControlDistX)
	{
		InterpolationAlphaOffsetX = FMath::Clamp((PlayerLocation.X - IniXPositionOffsetLocation.X) / (EndXPositionOffsetLocation.X - IniXPositionOffsetLocation.X), 0.f, 1.f);
	}
	else if(XPositionOffsetControlDistY)
	{
		InterpolationAlphaOffsetX = FMath::Clamp((PlayerLocation.Y - IniXPositionOffsetLocation.Y) / (EndXPositionOffsetLocation.Y - IniXPositionOffsetLocation.Y), 0.f, 1.f);
	}

	XPositionOffset = FMath::Lerp(IniPositionOffsetDistX, EndPositionOffsetDistX, InterpolationAlphaOffsetX);

	UE_LOG(LogTemp, Warning, TEXT("%f"), XPositionOffset);
}

void UPlayerCameraComponent::SetCameraHeightLocation(FVector IniLocation, FVector EndLocation)
{
	IniHeightLocation = IniLocation;
	EndHeightLocation = EndLocation;
}

void UPlayerCameraComponent::SetFollowPlayerX(bool FollowX)
{
	FollowPlayerX = FollowX;
}

void UPlayerCameraComponent::SetFollowPlayerY(bool FollowY)
{
	FollowPlayerY = FollowY;
}

void UPlayerCameraComponent::SetHeightControlDistX(bool ControlDistX)
{
	HeightControlDistX = ControlDistX;
}

void UPlayerCameraComponent::SetHeightControlDistY(bool ControlDistY)
{
	HeightControlDistY = ControlDistY;
}

void UPlayerCameraComponent::SetInterpolationSpeedX(float SpeedX)
{
	CameraXInterpolationSpeed = SpeedX;
}

void UPlayerCameraComponent::SetInterpolationSpeedY(float SpeedY)
{
	CameraYInterpolationSpeed = SpeedY;
}

void UPlayerCameraComponent::SetInterpolationSpeedZ(float SpeedZ)
{
	CameraZInterpolationSpeed = SpeedZ;
}

void UPlayerCameraComponent::SetIniPositionOffsetDistX(float IniDistX)
{
	IniPositionOffsetDistX = IniDistX;
}

void UPlayerCameraComponent::SetEndPositionOffsetDistX(float EndDistX)
{
	EndPositionOffsetDistX = EndDistX;
}

void UPlayerCameraComponent::SetXPositionOffsetControlDistX(bool ControlDistX)
{
	XPositionOffsetControlDistX = ControlDistX;
}

void UPlayerCameraComponent::SetXPositionOffsetControlDistY(bool ControlDistY)
{
	XPositionOffsetControlDistY = ControlDistY;
}

void UPlayerCameraComponent::SetXPositionOffsetLocation(FVector IniXOffsetControlLocation, FVector EndXOffsetControlLocation)
{
	IniXPositionOffsetLocation = IniXOffsetControlLocation;
	EndXPositionOffsetLocation = EndXOffsetControlLocation;
}
