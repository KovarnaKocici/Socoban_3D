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

private: 
	float CurrAlpha = 0.f;
	FVector StartLocation = FVector(0.f, 0.f, 0.f);
	FVector TargetLocation = FVector(0.f, 0.f, 0.f);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
		float Alpha = 0.f;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
		bool IsLocked = false;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void SetAlpha(float Value);

	UFUNCTION(BlueprintCallable)
		float GetAlpha();

	UFUNCTION(BlueprintCallable)
		void Move(FVector Movement, float Value);

	UFUNCTION(BlueprintCallable)
		void ReverseMove();
};
