// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"
#include "Components/BoxComponent.h"

// Sets default values
ACell::ACell(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create all Components
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SampleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SampleMesh->SetupAttachment(RootComponent);

	// Use a box as a simple collision representation
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACell::OnOverlapBegin);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ACell::OnOverlapEnd);
	CollisionComponent->SetupAttachment(SampleMesh);
}

// Called when the game starts or when spawned
void ACell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACell::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("%s overlapped with: %s"), *this->GetName(), *OtherActor->GetName()));
	}
}

void ACell::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)) 
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("%s not any more overlapped with: %s"), *this->GetName(), *OtherActor->GetName()));
	}
}
