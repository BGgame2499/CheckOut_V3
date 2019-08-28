
#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MyStruct.h"
#include "MyEnum.h"
#include "SpawnObjectComponent.generated.h"
class UBoxComponent;
class AObjectBase;
class UStaticMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHECKOUT_V3_API USpawnObjectComponent : public USceneComponent	//���ڴ���obj�����
{
	GENERATED_BODY()

public:	
	USpawnObjectComponent();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector SpawnOffset;			//�ڵ�ǰ����obj��λ�������ƫ��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CreatePriority;		//�������ȼ�����1��n����obj������˳�������Ҫ�������0��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RandomRotationNum;	//���������ֵ0-n���󲿷�����obj�����ת�ϣ�ÿ����һ���������һ�Σ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector RecognitionRayVector;	//����ߵĳ�����������֤�߳���Ҫ�����κ����壬���Ҫ����obj��λ���Ƿ����Ѿ����ڵ�obj�ڵ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ObjectType CurrentCompWillCreateType;	//��ǰ��comp��ָ���������ɵ�obj����������κ�obj������ʹ�á�All����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ObjectType CurrentCompWillUseObjectRotFunType;	//ȷ�ϵ�ǰ���ɵ�objҪʹ���Ѿ��趨�õ�������ת�ǶȺ���

	UPROPERTY(BlueprintReadWrite)
		TArray<AObjectBase*> CurrentCreateObjectArray;//��ǰ�Ѿ�������obj��Cashier����ÿ�����������������

protected:

	UPROPERTY(BlueprintReadWrite)
		FTransform CreateObjectTransform;

	int32 MaxRandomNum;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	

	UFUNCTION(BlueprintCallable)
		bool ChangeObjectLocationUseRay(AObjectBase * object_, FVector offset_);
	UFUNCTION(BlueprintCallable)
		bool ChangeObjectRotationUseRandom(AObjectBase * object_);

	UFUNCTION(BlueprintCallable)
		AObjectBase * CreateObject(TSubclassOf<AObjectBase> ObjectClass, FStructObjectID ObjectID, AActor *Owner);

	UFUNCTION(BlueprintCallable)
		void ClearCurrentObject();	//������е�ǰComp�Ѵ�����Obj�����ò��ֲ���

	/////�Բ�ͬ����obj���е�ת����ݺ���
	UFUNCTION(BlueprintCallable)
		virtual FRotator RandomCountFoodRotation(UStaticMeshComponent * MeshComp);
	UFUNCTION(BlueprintCallable)
		virtual FRotator RandomCountWaterRotation(UStaticMeshComponent * MeshComp);
	UFUNCTION(BlueprintCallable)
		virtual FRotator RandomCountSmokeRotation(UStaticMeshComponent * MeshComp);
	UFUNCTION(BlueprintCallable)
		virtual FRotator RandomCountMilkRotation(UStaticMeshComponent * MeshComp);
	UFUNCTION(BlueprintCallable)
		virtual FRotator RandomCountBucketRotation(UStaticMeshComponent * MeshComp);
	UFUNCTION(BlueprintCallable)
		virtual FRotator RandomCountOtherRotation(UStaticMeshComponent * MeshComp);
};
