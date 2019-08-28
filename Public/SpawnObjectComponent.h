
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
class CHECKOUT_V3_API USpawnObjectComponent : public USceneComponent	//用于创建obj的组件
{
	GENERATED_BODY()

public:	
	USpawnObjectComponent();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector SpawnOffset;			//在当前生成obj的位置上添加偏移量（）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CreatePriority;		//创建优先级（从1到n生成obj的优先顺序，如果需要随机则填0）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RandomRotationNum;	//随机数的数值0-n（大部分用在obj随机旋转上，每创建一次物体更新一次）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector RecognitionRayVector;	//检测线的长度向量（保证线长度要碰到任何物体，检查要创建obj的位置是否有已经存在的obj遮挡）

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ObjectType CurrentCompWillCreateType;	//当前的comp所指定允许生成的obj（如果允许任何obj生成则使用“All”）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ObjectType CurrentCompWillUseObjectRotFunType;	//确认当前生成的obj要使用已经设定好的哪种旋转角度函数

	UPROPERTY(BlueprintReadWrite)
		TArray<AObjectBase*> CurrentCreateObjectArray;//当前已经创建的obj（Cashier有在每次生成完后调用清除）

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
		void ClearCurrentObject();	//清除所有当前Comp已创建的Obj并重置部分参数

	/////对不同种类obj进行的转向操纵函数
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
