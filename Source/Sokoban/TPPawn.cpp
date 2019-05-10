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
	MovementComponent->isLockedForward = false;
	MovementComponent->isLockedRight = false;

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

	FRotator NewYaw = GetActorRotation();
	NewYaw.Yaw += MouseInput.X;
	SetActorRotation(NewYaw);
	
	FRotator NewPitch = SpringArmComponent->GetComponentRotation();
	NewPitch.Pitch = FMath::Clamp(NewPitch.Pitch + MouseInput.Y, -80.f, 0.f);
	SpringArmComponent->SetWorldRotation(NewPitch);
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

void ATPPawn::MouseYaw(float axis)
{
	MouseInput.X = axis;
}

void ATPPawn::MousePitch(float axis)
{
	MouseInput.Y = axis;
}

void ATPPawn::MoveForward(float AxisValue)
{
	if (MovementComponent && (MovementComponent->UpdatedComponent == RootComponent) && !MovementComponent->isLockedForward)
	{
		if (AxisValue != 0)
		{
			MovementComponent->isLockedRight = true;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("isLockedRight = true")));
				MovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
		}
	}
}

void ATPPawn::MoveRight(float AxisValue)
{
	if (MovementComponent && (MovementComponent->UpdatedComponent == RootComponent) && !MovementComponent->isLockedRight)
	{
		if (AxisValue != 0)
		{
			MovementComponent->isLockedForward = true;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("isLockedForward = true")));
			MovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
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