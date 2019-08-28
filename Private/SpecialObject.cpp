// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialObject.h"
#include "..\Public\SpecialObject.h"
#include "CashierBase.h"
#include "Json.h"
#include "Engine.h"

ASpecialObject::ASpecialObject()
{
	
	DefaultRayLenght = 1000.0f;
	IsDebugLine = false;
	
}

void ASpecialObject::BeginPlay()
{
	Super::BeginPlay();

	RecognitionActor = GetWorld()->GetFirstPlayerController()->GetPawn()->GetClass();	//默认得到当前操作的Pawn为识别检查对象

	if (!RecognitionActor)
	{
		UE_LOG(LogTemp, Error, TEXT("Find RecognitionActor failed!!!"));
	}
	else
	{
		InitRayStruct();
	}
}

void ASpecialObject::RayRecognition_Event_Implementation()
{
	if (!CreateRaysRecognition(RayStruct))	//如果射线没有碰到指定的Object 则更改CID
	{
		UE_LOG(LogTemp, Warning, TEXT("DontRecognition!"));

		ObjectIDStruct.CID = RayStruct.ChangeCID;
	}
}
bool ASpecialObject::CreateRaysRecognition(FStructSpecialObjectRayRecongnition & StructSpecial)	//根据结构体发射不同方向的射线检测
{
	if (!ObjectMesh->GetStaticMesh())
	{
		return false;
	}


	FVector StarLocation = ObjectMesh->GetRelativeTransform().GetLocation();

	if (StructSpecial.FX)
	{
		if (RecognitionUseMultiLine(StarLocation, ObjectMesh->GetForwardVector(), DefaultRayLenght, RecognitionActor))
		{
			return true;
		}
	}
	if (StructSpecial.BX)
	{
		if (RecognitionUseMultiLine(StarLocation, ObjectMesh->GetForwardVector(), -DefaultRayLenght, RecognitionActor))
		{
			return true;
		}
	}
	if (StructSpecial.FY)
	{
		if (RecognitionUseMultiLine(StarLocation, ObjectMesh->GetRightVector(), DefaultRayLenght, RecognitionActor))
		{
			return true;
		}
	}
	if (StructSpecial.BY)
	{
		if (RecognitionUseMultiLine(StarLocation, ObjectMesh->GetRightVector(), -DefaultRayLenght, RecognitionActor))
		{
			return true;
		}
	}
	if (StructSpecial.FZ)
	{
		if (RecognitionUseMultiLine(StarLocation, ObjectMesh->GetUpVector(), DefaultRayLenght, RecognitionActor))
		{
			return true;
		}
	}
	if (StructSpecial.BZ)
	{
		if (RecognitionUseMultiLine(StarLocation, ObjectMesh->GetUpVector(), -DefaultRayLenght, RecognitionActor))
		{
			return true;
		}
	}

	return false;
}
bool ASpecialObject::RecognitionUseMultiLine(FVector StartLocation, FVector EndLocation, float RayLenght, TSubclassOf<UObject> RecognitionObject)
{

	FVector NewEndLocation = EndLocation * RayLenght + StartLocation;	//计算射线终点

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableComponent")),false);
	TraceParams.bReturnPhysicalMaterial = false;  
	TraceParams.AddIgnoredActor(this);
	TArray<FHitResult> Hit;
	if (GetWorld()->LineTraceMultiByObjectType(Hit, StartLocation, NewEndLocation, ObjectParams, TraceParams))	//返回射线碰到的所有object
	{
		if (IsDebugLine)
		{
			DrawDebugLine(GetWorld(), StartLocation, NewEndLocation, FColor(255, 0, 0), false, 5.0f, 0.0f, 10.0f);
		}

		for (int i = 0; i < Hit.Num(); i++)	//查找是否有碰到指定的Obj类型
		{
			if (Hit.IsValidIndex(i))
			{
				if (Hit[i].GetActor()->GetClass() == RecognitionObject)
				{
					return true;
				}
			}

		}
	}


	return false;
}

void ASpecialObject::InitRayStruct()
{
	switch (ObjectIDStruct.Type)
	{
	case ObjectType::Bucket:
		RayStruct.BZ = false;
		RayStruct.BX = false;
		RayStruct.FX = false;
		RayStruct.BY = false;
		RayStruct.FY = false;
		RayStruct.ChangeCID = 2050;
		break;
	case ObjectType::Food:
		RayStruct.BZ = false;
		RayStruct.BX = false;
		RayStruct.FX = false;
		RayStruct.BY = false;
		RayStruct.FY = false;
		RayStruct.ChangeCID = 2050;
		break;
	case ObjectType::Water:
		RayStruct.BZ = false;
		RayStruct.FZ = false;
		RayStruct.ChangeCID = 2071;
		break;
	case ObjectType::Smoke:
		RayStruct.BX = false;
		RayStruct.FX = false;
		RayStruct.BZ = false;
		RayStruct.FZ = false;

		RayStruct.ChangeCID = 2063;
		break;
	case ObjectType::Milk:
		RayStruct.BX = false;
		RayStruct.FX = false;
		RayStruct.BZ = false;
		RayStruct.FZ = false;

		RayStruct.ChangeCID = 2063;
		break;
	case ObjectType::ShortPot :
		RayStruct.BZ = false;
		RayStruct.FZ = false;
		RayStruct.ChangeCID = 2071;
		break;
	default:
		RayStruct.ChangeCID = ObjectIDStruct.CID;
		break;
	}
}

void ASpecialObject::ReadJSON()	//未使用
{
	int32 _score = 0;	

	FString JsonStr;	
	TSharedRef< TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR> > > JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR> >::Create(&JsonStr);      
	JsonWriter->WriteObjectStart();	
	JsonWriter->WriteValue(TEXT("ChangeCID"), _score);
	//<span style = "white-space:pre">	< / span>JsonWriter->WriteObjectEnd();	// Close the writer and finalize the output such that JsonStr has what we want	
	JsonWriter->Close();	
	//return JsonStr;

}


void ASpecialObject::Tick(float DeltaTime)
{
}
