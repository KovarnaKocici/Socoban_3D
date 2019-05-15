// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridPossession.h"
#include "Marker.generated.h"

/**
 * 
 */
UCLASS()
class SOKOBAN_API AMarker : public AGridPossession
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
		class UBoxComponent* TopCenterCollisionComponent;

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	AMarker(const FObjectInitializer &ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform & Transform) override;

	UFUNCTION(BlueprintCallable)
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
