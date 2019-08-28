// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CheckOutInstance.generated.h"

class UCheckOutSave;
UCLASS()
class CHECKOUT_V3_API UCheckOutInstance : public UGameInstance
{
	GENERATED_BODY()
public:

		virtual void Init();

		UFUNCTION(BlueprintCallable)
			bool SaveCheckOut();

		UPROPERTY(BlueprintReadWrite)
			UCheckOutSave * SaveGames;
};
