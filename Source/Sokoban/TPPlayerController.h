// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SOKOBAN_API ATPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public: 

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	void PlayerTick(float DeltaTime);

	//Camera Rotation
	void MouseYaw(float AxisValue);

	void MousePitch(float AxisValue);

	//Camera Zoom
	void MouseZoomIn();

	void MouseZoomOut();

	void Zoom(float AxisValue);

	float CameraZoom_v = 400.f;

	const float MinZoom = 150.f;

	const float MaxZoom = 800.f;

	const float DiffZoom = 25.f;

	FVector2D MouseInput;

	//Movements
	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

};
