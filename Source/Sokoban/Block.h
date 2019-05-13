// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridPossession.h"
#include "Block.generated.h"

UCLASS()
class SOKOBAN_API ABlock : public AGridPossession
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlock(const FObjectInitializer &ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsPushable = false;

	bool IsHitted = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
		class UTPPlayerMovementComponent* MovementComponent;

protected:

	UPROPERTY(BlueprintReadOnly)
		class UBoxComponent* TopCollisionComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform & Transform) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;
	
	UFUNCTION(BlueprintCallable)
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};