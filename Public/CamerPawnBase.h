// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CamerPawnBase.generated.h"

class UCameraComponent;

UCLASS()
class CHECKOUT_V3_API ACamerPawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACamerPawnBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent * CameraComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void Visibility__GetRenderedActors(TArray<AActor*>& CurrentlyRenderedActors, float MinRecentTime);

	UFUNCTION(BlueprintCallable)
		void Visibility__GetNotRenderedActors(TArray<AActor*>& CurrentlyNotRenderedActors, float MinRecentTime);
};
