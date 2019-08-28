
#pragma once

#include "CoreMinimal.h"
#include "ObjectBase.h"
#include "MyStruct.h"
#include "SpecialObject.generated.h"

UCLASS()
class CHECKOUT_V3_API ASpecialObject : public AObjectBase	//��Ҫʹ��ʶ�����������Obj
{
	GENERATED_BODY()
		ASpecialObject();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FStructSpecialObjectRayRecongnition RayStruct;	//ʶ����ṹ��
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RecognitionActor")	//��Ҫ���м���Actor
		TSubclassOf<AActor> RecognitionActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DefaultRayLenght;	//���߳���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDebugLine;	//�Ƿ����Debug����
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		virtual void InitRayStruct();	//���ݵ�ǰObj������������ʶ����
	UFUNCTION(BlueprintCallable)
		void ReadJSON();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)	//����ʶ���¼� ����Casher�б����ã�
		void RayRecognition_Event();

	UFUNCTION(BlueprintCallable)
		bool CreateRaysRecognition(FStructSpecialObjectRayRecongnition &StructSpecial);

	UFUNCTION(BlueprintCallable)
		bool RecognitionUseMultiLine(FVector StartLocation, FVector EndLocation, float RayLenght, TSubclassOf<UObject> RecognitionObject);

	virtual void Tick(float DeltaTime) override;

};
