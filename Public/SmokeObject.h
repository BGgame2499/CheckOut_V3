// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectBase.h"
#include "SmokeObject.generated.h"

/**
 * 
 */
UCLASS()
class CHECKOUT_V3_API ASmokeObject : public AObjectBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
