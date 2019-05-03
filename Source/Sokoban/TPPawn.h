// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TPPawn.generated.h"

UCLASS()
class SOKOBAN_API ATPPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATPPawn(const FObjectInitializer &ObjectInitializer);

	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = View)
		UStaticMesh* DefaultMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = View)
		UMaterialInterface* DefaultMaterial;

	//CameraComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class UCameraComponent* CameraComponent;

	//Movement Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class UTPPlayerMovementComponent* MovementComponent;

	//Snap Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class USnapToGridComponent* SnapComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	//ConstructionScript
	virtual void OnConstruction(const FTransform & Transform) override;

	UFUNCTION(BlueprintCallable)
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
