// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPlayerController.h"
#include "Sokoban.h"
#include "TPPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Classes/Camera/CameraComponent.h"
#include "TPPlayerMovementComponent.h"
#include "Components/InputComponent.h"


// Called to bind functionality to input
void ATPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//View
	InputComponent->BindAxis("MouseYaw", this, &ATPPlayerController::MouseYaw);
	InputComponent->BindAxis("MousePitch", this, &ATPPlayerController::MousePitch);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ATPPlayerController::MouseZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ATPPlayerController::MouseZoomOut);

	//Movements
	InputComponent->BindAxis("MoveForward", this, &ATPPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATPPlayerController::MoveRight);

}

void ATPPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//Update camera
	ATPPawn* pawn = Cast<ATPPawn>(GetPawn());
	if (pawn) {
		FRotator NewRotation = pawn->SpringArmComponent->GetComponentRotation();
		NewRotation.Yaw += MouseInput.X;
		for (int k = 0; k <= 2; k++) {
			if (FMath::IsNearlyEqual(FMath::Abs(NewRotation.Yaw), 90.f * k, 30.f) && !pawn->MovementComponent->IsLocked)
			{
				FRotator NewYaw = pawn->GetActorRotation();
				NewYaw.Yaw = NewRotation.Yaw / FMath::Abs(NewRotation.Yaw) * 90.f * k;
				pawn->MovementComponent->Direction = -pawn->MovementComponent->Direction;
				pawn->SetActorRotation(NewYaw);
				break;
			}
		}
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + MouseInput.Y, -80.f, 0.f);
		pawn->SpringArmComponent->SetWorldRotation(NewRotation);
	}
}

void ATPPlayerController::MouseYaw(float AxisValue)
{
	MouseInput.X = AxisValue;
}

void ATPPlayerController::MousePitch(float AxisValue)
{
	MouseInput.Y = AxisValue;
}

void ATPPlayerController::MouseZoomIn()
{
	Zoom(1);
}

void ATPPlayerController::MouseZoomOut()
{
	Zoom(-1);
}

void  ATPPlayerController::Zoom(float  AxisValue) {
	ATPPawn* pawn = Cast<ATPPawn>(GetPawn());
	if (pawn) {
		CameraZoom_v = CameraZoom_v + DiffZoom * AxisValue;

		if (CameraZoom_v <= MinZoom)
		{
			pawn->SpringArmComponent->TargetArmLength = MinZoom;
			CameraZoom_v = MinZoom;
		}
		else
			if (CameraZoom_v >= MaxZoom)
			{
				pawn->SpringArmComponent->TargetArmLength = MaxZoom;
				CameraZoom_v = MaxZoom;
			}
			else
			{
				pawn->SpringArmComponent->TargetArmLength = CameraZoom_v;
			}
	}
}

void ATPPlayerController::MoveForward(float AxisValue)
{
	ATPPawn* pawn = Cast<ATPPawn>(GetPawn());
	if (AxisValue != 0)
	{
		
		if (pawn) {
			ClientMessage(TEXT("Pawn"), TEXT("MESAAGGE"), 500);
			if (pawn->MovementComponent && pawn->MovementComponent->UpdatedComponent && !pawn->MovementComponent->IsLocked && !pawn->MovementComponent->IsMoving)
			{
				ClientMessage(TEXT("Component"), TEXT("MESAAGGE"), 500);
				if (AxisValue != 0)
				{
					UE_LOG(LogMovement, Log, TEXT("Move forward."));
					FVector Movement = pawn->GetActorForwardVector() * AxisValue;
					if (pawn->ValidGridMovement(pawn->GetActorLocation(), Movement, pawn->MoveForward))
					{
						ClientMessage(TEXT("WORK"), TEXT("MESAAGGE"), 500);
						pawn->MovementComponent->Move(Movement, AxisValue, pawn->MoveForward);
					}
				}
			}
		}
	}
}

void ATPPlayerController::MoveRight(float AxisValue)
{
	ATPPawn* pawn = Cast<ATPPawn>(GetPawn());
	if (pawn) {
		if (pawn->MovementComponent && pawn->MovementComponent->UpdatedComponent && !pawn->MovementComponent->IsLocked && !pawn->MovementComponent->IsMoving)
		{
			if (AxisValue != 0)
			{
				UE_LOG(LogMovement, Log, TEXT("Move right."));
				FVector Movement = pawn->GetActorRightVector() * AxisValue;
				if (pawn->ValidGridMovement(pawn->GetActorLocation(), Movement, pawn->MoveRight))
					pawn->MovementComponent->Move(Movement, AxisValue, pawn->MoveRight);
			}
		}
	}
}