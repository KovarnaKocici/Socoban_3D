// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TPPlayerMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SOKOBAN_API UTPPlayerMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	bool isLockedForward;
	bool isLockedRight;

	FVector CurrLocation;
	FVector TargetLocation;

	FTimerHandle MovementTimerHandle;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
};
