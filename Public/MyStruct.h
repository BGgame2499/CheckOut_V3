// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyEnum.h"
#include "MyStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct  FStructObjectID
{
	GENERATED_BODY()
public:
	FStructObjectID();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjID")
		int32 UID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjID")
		int32 CID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjID")
		int32 MID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjID")
		int32 MaskID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjID")
		ObjectType Type;
};
USTRUCT(BlueprintType)
struct  FStructSpecialObjectRayRecongnition
{
	GENERATED_BODY()
public:
	FStructSpecialObjectRayRecongnition();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool BX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool BY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool BZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CID")
		int32 ChangeCID;
};
