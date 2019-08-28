// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CashierBase.h"
#include "Cashier_A.generated.h"

class APostProcessVolume;
UCLASS()
class CHECKOUT_V3_API ACashier_A : public ACashierBase
{
	GENERATED_BODY()

public:
	ACashier_A();

public:
	UPROPERTY(BlueprintReadWrite)	//创建Obj的开关
		bool SpawnSwitch;
	UPROPERTY(BlueprintReadWrite)
		FTimerHandle TimeHand_Export;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CashierControll")	//要给Obj进行物理掉落的模拟时间
		float ObjectPhysicalTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)	//Mask盒子 需要在关卡中指定
		APostProcessVolume * MaskPostVolume;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void LoopLoadAllMesh() override;		//实现加载所有的模型
	virtual void OnConstruction(const FTransform& Transform) override;	//每次更改当前类内容时调用

	UFUNCTION(BlueprintCallable)
		virtual void GiveAllObjectComponentDrawnLine();	//给所有Comp绘制自身向量检测方向的线段来方便观察

	UFUNCTION(BlueprintNativeEvent)	//延时调用的物品创建事件
		void DelayCreateObject();
	AObjectBase * ChooseCreateObjectComp(int32 CompNum, FStructObjectID ObjID);	//根据当前的创建计次和要创建的ObjID来自动分配给Comp来创建

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)	//输出事件
		void ExportEvent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void BeginExportEvent();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ExportImageEvent();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EndExportEvent();


		virtual void CashierReloat()override;	//重置当前收银台参数
};
