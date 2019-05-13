// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SnapToGridComponent.generated.h"

UCLASS()
class SOKOBAN_API USnapToGridComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USnapToGridComponent();

	enum ETraceDirection 
	{
		TopDown = -1 UMETA(DisplayName = "TopDown"),
		DownTop = 1 UMETA(DisplayName = "DownTop ")
	};


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TraceLength = 0;

	UFUNCTION(BlueprintCallable)
		bool NewLocationXYRelToGrid(UPrimitiveComponent* cell, FVector& location);

	UFUNCTION(BlueprintCallable)
		bool NewLocationZRelToActors(FVector Start, FVector  End, int direction, FVector& location);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void SetTraceLength(float Length);

	UFUNCTION(BlueprintCallable)
		float GetTraceLength();

	UFUNCTION(BlueprintCallable)
		UPrimitiveComponent* GetCellByLocation(FVector Location);

	UFUNCTION(BlueprintCallable)
		UPrimitiveComponent* GetCurrCell();

	UFUNCTION(BlueprintCallable)
		bool SnapToCell(UPrimitiveComponent* cell);

	UFUNCTION(BlueprintCallable)
		bool SnapToFloor(int direction);
};


