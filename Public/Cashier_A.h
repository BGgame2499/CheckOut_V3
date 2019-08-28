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
	UPROPERTY(BlueprintReadWrite)	//����Obj�Ŀ���
		bool SpawnSwitch;
	UPROPERTY(BlueprintReadWrite)
		FTimerHandle TimeHand_Export;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CashierControll")	//Ҫ��Obj������������ģ��ʱ��
		float ObjectPhysicalTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)	//Mask���� ��Ҫ�ڹؿ���ָ��
		APostProcessVolume * MaskPostVolume;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void LoopLoadAllMesh() override;		//ʵ�ּ������е�ģ��
	virtual void OnConstruction(const FTransform& Transform) override;	//ÿ�θ��ĵ�ǰ������ʱ����

	UFUNCTION(BlueprintCallable)
		virtual void GiveAllObjectComponentDrawnLine();	//������Comp��������������ⷽ����߶�������۲�

	UFUNCTION(BlueprintNativeEvent)	//��ʱ���õ���Ʒ�����¼�
		void DelayCreateObject();
	AObjectBase * ChooseCreateObjectComp(int32 CompNum, FStructObjectID ObjID);	//���ݵ�ǰ�Ĵ����ƴκ�Ҫ������ObjID���Զ������Comp������

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)	//����¼�
		void ExportEvent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void BeginExportEvent();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ExportImageEvent();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EndExportEvent();


		virtual void CashierReloat()override;	//���õ�ǰ����̨����
};
