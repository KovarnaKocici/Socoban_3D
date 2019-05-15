// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPawn.h"
#include "Sokoban.h"
#include "Block.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/SpringArmComponent.h"
#include "Classes/Camera/CameraComponent.h"
#include "SnapToGridComponent.h"
#include "Components/InputComponent.h"
#include "TPPlayerMovementComponent.h"


// Sets default values
ATPPawn::ATPPawn(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent->SetCollisionObjectType(ECC_Pawn);
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ATPPawn::OnOverlapBegin);

	//Camera
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.f;
	CameraZoom_v = SpringArmComponent->TargetArmLength;
	SpringArmComponent->SetWorldRotation(FRotator(-60.f, 0.f, 0.f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	// Create an instance of our movement component, and tell it to update the root.
	MovementComponent = CreateDefaultSubobject<UTPPlayerMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;
	MovementComponent->SetAlpha(0.025f);

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
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &ATPPawn::MouseZoomIn);
	PlayerInputComponent->BindAction("ZoomOut",  IE_Pressed, this, &ATPPawn::MouseZoomOut);

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

void ATPPawn::MouseZoomIn()
{
	Zoom(1);
}

void ATPPawn::MouseZoomOut()
{
	Zoom(-1);
}

void  ATPPawn::Zoom(float  AxisValue) {
	CameraZoom_v = CameraZoom_v + DiffZoom * AxisValue;

	if (CameraZoom_v <= MinZoom)
	{
		SpringArmComponent->TargetArmLength = MinZoom;
		CameraZoom_v = MinZoom;
	}
	else
		if (CameraZoom_v >= MaxZoom)
		{
			SpringArmComponent->TargetArmLength = MaxZoom;
			CameraZoom_v = MaxZoom;
		}
		else
		{
			SpringArmComponent->TargetArmLength = CameraZoom_v;
		}
}

void ATPPawn::MoveForward(float AxisValue)
{
	if (MovementComponent && (MovementComponent->UpdatedComponent == RootComponent) && !MovementComponent->IsLocked)
	{
		if (AxisValue != 0)
		{
			UE_LOG(LogMovement, Log, TEXT("Move forward."));
			FVector Movement = GetActorForwardVector() * AxisValue;
			if(ValidGridMovement(GetActorLocation(), Movement, EMoveDirection::MoveForward))
				MovementComponent->Move(Movement, AxisValue, EMoveDirection::MoveForward);
		}
	}
}

void ATPPawn::MoveRight(float AxisValue)
{
	if (MovementComponent && (MovementComponent->UpdatedComponent == RootComponent) && !MovementComponent->IsLocked)
	{
		if (AxisValue != 0)
		{
			UE_LOG(LogMovement, Log, TEXT("Move right."));
			FVector Movement = GetActorRightVector() * AxisValue;
			if (ValidGridMovement(GetActorLocation(), Movement, EMoveDirection::MoveRight))
				MovementComponent->Move(Movement, AxisValue, EMoveDirection::MoveRight);
		}
	}
}

void ATPPawn::Push(ABlock* Block)
{
	IsPushing = true;
	Block->IsHitted = false;
	FVector Movement = FVector(0.f, 0.f, 0.f);
	if (MovementComponent->Direction == EMoveDirection::MoveForward)
	Movement = GetActorForwardVector() * MovementComponent->GetAxisValue();
	else
		if (MovementComponent->Direction == EMoveDirection::MoveRight)
			Movement = GetActorRightVector() * MovementComponent->GetAxisValue();

	if (ValidGridMovement(Block->GetActorLocation(), Movement, MovementComponent->Direction))
	{
		//Push
		UE_LOG(LogMovement, Log, TEXT("Push."));
		Block->MovementComponent->Move(Movement, MovementComponent->GetAxisValue(), MovementComponent->Direction);
	}
	else
	{
		MovementComponent->ReverseMove();
		UE_LOG(LogMovement, Log, TEXT("Reverse move in Pawn push. IsPushing %d"), IsPushing);
	}

}

void ATPPawn::OnConstruction(const FTransform & Transform) 
{
	Super::OnConstruction(Transform);
}

void  ATPPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		UE_LOG(LogCollision, Log, TEXT("%s overlapped with %s."), *OverlappedComp->GetName(), *OtherActor->GetName());
		ABlock* block = Cast<ABlock>(OtherActor);
		if (block)
		{
			if (block->IsPushable && !IsPushing)
			{
				Push(block);
				//Continue move
			}
		else
			if ( !block->IsPushable || (block->IsPushable && block->IsHitted))
			{
				MovementComponent->ReverseMove();
				UE_LOG(LogMovement, Log, TEXT("Reverse move in Pawn overlap. "));
				block->IsHitted = false;
			}
		}
	}
}