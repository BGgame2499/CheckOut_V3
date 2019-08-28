// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorBase.generated.h"

class UStaticMeshComponent;

UCLASS()
class CHECKOUT_V3_API AFloorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorBase();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent * PlaneMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ChangeSpeed;

protected:

	UPROPERTY(BlueprintReadWrite)
		FTimerHandle Hand;
	UPROPERTY(BlueprintReadWrite)
		int32 MinRandomIndex;
	UPROPERTY(BlueprintReadWrite)
		int32 MaxRandomIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
		static UTexture2D* LoadTexture2D(const FString path, bool& IsValid, int32& OutWidth, int32& OutHeight);

	UFUNCTION(BlueprintCallable)
		void RandomeChangeTexture();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
