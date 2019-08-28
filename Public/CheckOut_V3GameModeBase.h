// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CheckOut_V3GameModeBase.generated.h"

class AObjectBase;

UCLASS()
class CHECKOUT_V3_API ACheckOut_V3GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACheckOut_V3GameModeBase();

	UFUNCTION(BlueprintNativeEvent)
		AObjectBase* CreateCustomerObject(TSubclassOf<AObjectBase>ObjectClass, FTransform ObjectTransform, int32 UID, AActor * objectOwner);
};
