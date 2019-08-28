
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn), Category = "StrObjID")
		FStructObjectID ObjectIDStruct;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
		FVector minSpanVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
		FVector maxSpanVector;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
		FVector nowMinSpanVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
		FVector nowMaxSpanVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
		FVector MeshBoundVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialRandom")
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
		bool LoadNewMeahWithMaterialToObjectMesh(FString InMeshPath = "SKU/MeshData/", FString InMatPath = "SKU/MatData/");
	UFUNCTION(BlueprintCallable)
		bool LoadNewMeahToObjectMesh(FString InMeshPath = "SKU/MeshData/");
	UFUNCTION(BlueprintCallable)
		TArray<FString> GetMatPathList(FString MatPath, FString Format);
	UFUNCTION(BlueprintCallable)
		static FString GetMeshPathNameByClassID(FString MeshBasePath, TArray<FString> MeshNameList, int32 classID);

	UFUNCTION(BlueprintCallable)
		void SetAndCountCustomDepth(int32 DepthNum = 60);


	UFUNCTION(BlueprintCallable)
		void SetObjPhysical(bool is);
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
