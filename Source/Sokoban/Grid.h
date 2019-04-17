// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

UCLASS()
class SOKOBAN_API AGrid : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrid(const FObjectInitializer &ObjectInitializer);

	//SceneComponent as Root
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Initialize)
		USceneComponent* SceneComponent;

	//MeshComponent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialize)
		UStaticMesh* DefaultMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialize)
		int Rows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialize)
		int Columns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialize)
		UMaterialInterface* Material1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialize)
		UMaterialInterface* Material2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//ConstructionScript
	virtual void OnConstruction(const FTransform & Transform) override;

	UFUNCTION()
	UMaterialInterface* ChooseCellMaterial(int row, int column);

	UFUNCTION(BlueprintCallable)
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};