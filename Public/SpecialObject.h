
#pragma once

#include "CoreMinimal.h"
#include "ObjectBase.h"
#include "MyStruct.h"
#include "SpecialObject.generated.h"

UCLASS()
class CHECKOUT_V3_API ASpecialObject : public AObjectBase	//需要使用识别面检查的特殊Obj
{
	GENERATED_BODY()
		ASpecialObject();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FStructSpecialObjectRayRecongnition RayStruct;	//识别面结构体
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RecognitionActor")	//需要进行检测的Actor
		TSubclassOf<AActor> RecognitionActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DefaultRayLenght;	//射线长度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDebugLine;	//是否绘制Debug射线
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		virtual void InitRayStruct();	//根据当前Obj的类型来设置识别面
	UFUNCTION(BlueprintCallable)
		void ReadJSON();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)	//射线识别事件 （在Casher中被调用）
		void RayRecognition_Event();

	UFUNCTION(BlueprintCallable)
		bool CreateRaysRecognition(FStructSpecialObjectRayRecongnition &StructSpecial);

	UFUNCTION(BlueprintCallable)
		bool RecognitionUseMultiLine(FVector StartLocation, FVector EndLocation, float RayLenght, TSubclassOf<UObject> RecognitionObject);

	virtual void Tick(float DeltaTime) override;

};
