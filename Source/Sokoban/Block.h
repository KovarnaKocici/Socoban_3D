// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

UCLASS()
class SOKOBAN_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlock(const FObjectInitializer &ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	UStaticMeshComponent* MeshComponent;

	//Snap Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class USnapToGridComponent* SnapComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//ConstructionScript
	virtual void OnConstruction(const FTransform & Transform) override;

};