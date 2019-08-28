// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CheckOutSave.generated.h"

/**
 * 
 */
UCLASS()
class CHECKOUT_V3_API UCheckOutSave : public USaveGame
{
	GENERATED_BODY()
public:
	UCheckOutSave();



	UPROPERTY(BlueprintReadWrite)
		int32 CreateTimes;
};
