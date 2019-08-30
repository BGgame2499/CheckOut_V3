
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyStruct.h"
#include "ObjectBase.generated.h"


class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class CHECKOUT_V3_API AObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AObjectBase();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comp")
		USceneComponent* RootScene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comp")
		UStaticMeshComponent* ObjectMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn), Category = "StrObjID")	//Obj信息ID
		FStructObjectID ObjectIDStruct;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")	//Mesh大小测量
		FVector minSpanVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
		FVector maxSpanVector;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
		FVector nowMinSpanVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
		FVector nowMaxSpanVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
		FVector MeshBoundVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialRandom")	//随机材质变化（效果暂时有问题）
		bool isRandomMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialRandom")
		float MaxRandomRoughness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialRandom")
		float MinRandomRoughness;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialRandom")
		float MaxRandomMetal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialRandom")
		float MinRandomMetal;


public:
	UFUNCTION(BlueprintCallable)
		bool LoadNewMeahWithMaterialToObjectMesh(FString InMeshPath = "SKU/MeshData/", FString InMatPath = "SKU/MatData/");	//加载模型和材质
	UFUNCTION(BlueprintCallable)
		bool LoadNewMeahToObjectMesh(FString InMeshPath = "SKU/MeshData/");
	UFUNCTION(BlueprintCallable)
		TArray<FString> GetMatPathList(FString MatPath, FString Format);	//查找材质路径
	UFUNCTION(BlueprintCallable)
		static FString GetMeshPathNameByClassID(FString MeshBasePath, TArray<FString> MeshNameList, int32 classID);	//查找模型路径

	UFUNCTION(BlueprintCallable)
		void SetAndCountCustomDepth(int32 DepthNum = 60);


	UFUNCTION(BlueprintCallable)
		void SetObjPhysical(bool is);
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
