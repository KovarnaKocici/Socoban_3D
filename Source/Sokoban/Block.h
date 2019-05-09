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

	UPROPERTY(BlueprintReadOnly)
		class UBoxComponent* TopCollisionComponent;

	UPROPERTY(BlueprintReadOnly)
		class UBoxComponent* InnerCollisionComponent;

	UPROPERTY(BlueprintReadOnly)
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = View)
		UStaticMesh* DefaultMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = View)
		UMaterialInterface* DefaultMaterial;

	//Snap Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
		class USnapToGridComponent* SnapComponent;

	FVector DeltaMove;
	FVector DeltaSize;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform & Transform) override;

	virtual void EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;

	virtual void EditorApplyScale(const FVector & DeltaScale, const FVector * PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
	
	virtual void PostEditMove(bool bFinished) override;

	UFUNCTION(BlueprintCallable)
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void ApplyTranslation(const FVector & DeltaTranslation);
	
};