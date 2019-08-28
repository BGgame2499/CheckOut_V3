// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyEnum.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class ObjectType : uint8
{
	All,
	Other,
	Bucket,
	Food,
	Smoke,
	Milk,
	Water,
	ShortPot,
	LongSmoke

};