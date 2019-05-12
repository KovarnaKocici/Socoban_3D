// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPawn.h"
#include "Sokoban.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/SpringArmComponent.h"
#include "Classes/Camera/CameraComponent.h"
#include "TPPlayerMovementComponent.h"
#include "SnapToGridComponent.h"
#include "Components/InputComponent.h"


// Sets default values
ATPPawn::ATPPawn(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent->OnComponentHit.AddDynamic(this, &ATPPawn::OnCompHit);
	MeshComponent->SetCollisionObjectType(ECC_Pawn);

	//Camera
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.f;
	SpringArmComponent->SetWorldRotation(FRotator(-60.f, 0.f, 0.f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	// Create an instance of our movement component, and tell it to update the root.
	MovementComponent = CreateDefaultSubobject<UTPPlayerMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;
	MovementComponent->SetAlpha(0.05f);

	// Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	SetActorEnableCollision(true);
}

// Called when the game starts or when spawned
void ATPPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATPPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Update camera
	FRotator NewRotation = SpringArmComponent->GetComponentRotation();
	NewRotation.Yaw += MouseInput.X;
	for (int k = 0; k <= 2; k++) {
		if (FMath::IsNearlyEqual(FMath::Abs(NewRotation.Yaw), 90.f * k, 30.f))
		{
			FRotator NewYaw = GetActorRotation();
			NewYaw.Yaw = NewRotation.Yaw / FMath::Abs(NewRotation.Yaw) * 90.f * k;
			SetActorRotation(NewYaw);
			break;
		}
	}
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + MouseInput.Y, -80.f, 0.f);
	SpringArmComponent->SetWorldRotation(NewRotation);
}

// Called to bind functionality to input
void ATPPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//View
	PlayerInputComponent->BindAxis("MouseYaw", this, &ATPPawn::MouseYaw);
	PlayerInputComponent->BindAxis("MousePitch", this, &ATPPawn::MousePitch);

	//Movements
	PlayerInputComponent->BindAxis("MoveForward", this, &ATPPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPPawn::MoveRight);

}

UPawnMovementComponent* ATPPawn::GetMovementComponent() const
{
	return MovementComponent;
}

void ATPPawn::MouseYaw(float AxisValue)
{
	MouseInput.X = AxisValue;
}

void ATPPawn::MousePitch(float AxisValue)
{
	MouseInput.Y = AxisValue;
}

void ATPPawn::MoveForward(float AxisValue)
{
	if (MovementComponent && (MovementComponent->UpdatedComponent == RootComponent) && !MovementComponent->IsLocked)
	{
		if (AxisValue != 0)
		{
			UE_LOG(LogMovement, Log, TEXT("Move forward."));
			//if rotated on 90
			MovementComponent->Move(GetActorForwardVector(), AxisValue * SnapComponent->GetCurrCell()->Bounds.BoxExtent.Y * 2);
			//else ->Bounds.BoxExtent.X*2
		}
	}
}

void ATPPawn::MoveRight(float AxisValue)
{
	if (MovementComponent && (MovementComponent->UpdatedComponent == RootComponent) && !MovementComponent->IsLocked)
	{
		if (AxisValue != 0)
		{
			UE_LOG(LogMovement, Log, TEXT("Move right.."));
			//if rotated on 90
			MovementComponent->Move(GetActorRightVector(), AxisValue * SnapComponent->GetCurrCell()->Bounds.BoxExtent.X * 2);
			//else ->Bounds.BoxExtent.Y*2
		}
	}
}

void ATPPawn::OnConstruction(const FTransform & Transform) {
	Super::OnConstruction(Transform);
}


void ATPPawn::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		UE_LOG(LogCollide, Log, TEXT("%s hits %s"), *this->GetName(), *OtherActor->GetName());
		//if (GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("%s hits %s"), *this->GetName(), *OtherActor->GetName()));
	}
}