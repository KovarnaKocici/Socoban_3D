// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SnapToGridComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	//TEnumAsByte<ETraceDirection> direction;

	UFUNCTION(BlueprintCallable)
		void Snap();

	UFUNCTION(BlueprintCallable)
		bool SnapToFloor(float traceLength, int direction);

	UFUNCTION(BlueprintCallable)
		bool SnapToCellCenter(float traceLength, int direction);

	UFUNCTION(BlueprintCallable)
		FVector NewLocationRelativeToObject(ECollisionChannel Type, FVector Start, FVector  End, int index);
};


