// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridPossession.generated.h"

UCLASS(abstract)
class SOKOBAN_API AGridPossession : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridPossession(const FObjectInitializer &ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = View)
		UStaticMesh* DefaultMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = View)
		UMaterialInterface* DefaultMaterial;

	UPROPERTY(BlueprintReadOnly)
		UStaticMeshComponent* MeshComponent;

	//Snap Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
		class USnapToGridComponent* SnapComponent;

	FVector DeltaMove;
	FVector DeltaSize;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
		class UBoxComponent* InnerCollisionComponent;

	FVector Apply(const FVector & CurrValue, FVector & Accumulator, const FVector & Delta);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform & Transform) override;

	virtual void EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;

	virtual void EditorApplyScale(const FVector & DeltaScale, const FVector * PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;

	virtual void PostEditMove(bool bFinished) override;


	UFUNCTION()
		void ApplyTranslation(const FVector & DeltaTranslation);

	UFUNCTION()
		void ApplyScale(const FVector & DeltaScale);
};
