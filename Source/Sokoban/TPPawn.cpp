// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPawn.h"
#include "Sokoban.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
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

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->OnComponentHit.AddDynamic(this, &ATPPawn::OnCompHit);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionObjectType(ECC_Pawn);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	MeshComponent->SetGenerateOverlapEvents(true);

	//Mesh as Root
	RootComponent = MeshComponent;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraMain"));
	CameraComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 400.0f), FRotator(-90.0f, 0.0f, 0.0f));
	CameraComponent->SetupAttachment(RootComponent);

	// Create an instance of our movement component, and tell it to update the root.
	MovementComponent = CreateDefaultSubobject<UTPPlayerMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;

	// Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//Create Snap Component
	SnapComponent = CreateDefaultSubobject<USnapToGridComponent>(TEXT("SnapComponent"));

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

}

// Called to bind functionality to input
void ATPPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPPawn::MoveRight);

}

UPawnMovementComponent* ATPPawn::GetMovementComponent() const
{
	return MovementComponent;
}

void ATPPawn::MoveForward(float AxisValue)
{
	if (MovementComponent && (MovementComponent->UpdatedComponent == RootComponent))
	{
		MovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
	}
}

void ATPPawn::MoveRight(float AxisValue)
{
	if (MovementComponent && (MovementComponent->UpdatedComponent == RootComponent))
	{
		MovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
	}
}

void ATPPawn::OnConstruction(const FTransform & Transform) {
	if (DefaultMesh) {
		MeshComponent->SetStaticMesh(DefaultMesh);
		MeshComponent->SetMaterial(0, DefaultMaterial);
	}

	//Snap to grid cells
	//SnapComponent->Snap();
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