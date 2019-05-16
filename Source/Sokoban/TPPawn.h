// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GridPossession.h"
#include "Block.h"
#include "TPPawn.generated.h"

UCLASS()
class SOKOBAN_API ATPPawn : public AGridPossession
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATPPawn(const FObjectInitializer &ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsPushing = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
		class UTPPlayerMovementComponent* MovementComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//ConstructionScript
	virtual void OnConstruction(const FTransform & Transform) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	void Push(ABlock* Block);

	UFUNCTION(BlueprintCallable)
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
