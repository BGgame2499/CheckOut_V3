// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyStruct.h"
#include "MyEnum.h"
#include "CashierBase.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class AObjectBase;
class UBoxComponent;
class ACamerPawnBase;
class USpawnObjectComponent;
UENUM(BlueprintType)

enum class EImageFormatType : uint8
{

	PNG,

	JPEG,

};

UCLASS()
class CHECKOUT_V3_API ACashierBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ACashierBase();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comp")
		USceneComponent* RootScene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comp")
		UStaticMeshComponent* CashierMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnComp")
		USpawnObjectComponent* SpawnObjectComp_1;

	UPROPERTY(BlueprintReadWrite)
		TArray<AObjectBase*> SaveObjectArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CreateTimes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn")
		ACamerPawnBase * CurrentPawn;

	UPROPERTY(BlueprintReadWrite)
		TArray<FStructObjectID>WillCreateObjectIDArray;

	UPROPERTY(BlueprintReadWrite)
		int32 WillCreateNum;

	//CashierControll参数详细说明可参见文档
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashierControll")
		bool isLoadAllMesh;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashierControll")
		bool isClearCreateTimes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashierControll")
		bool isQueue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashierControll")
		bool isExport;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashierControll")
		int32 RandomCreateObjectNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashierControll")
		int32 SameProbability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashierControll")
		TArray<FString> OnlyReadType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashierControll")
		FString ReadTxtFileName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CashierControll")
		TSubclassOf<AObjectBase> WillCreateObjectClass;
public:

	UPROPERTY(BlueprintReadWrite, Category = "TimeHandle")	//用于创建Obj的时间柄
		FTimerHandle TimeHand_DelayCreateObject;

	UPROPERTY(BlueprintReadWrite, Category = "TimeHandle")
		FTimerHandle TimeHand_ClearAllSpawnObjCompArray;
public:
		//当前读取到的列表全部内容数组
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ObjectInformation")
		TArray<FString> CurrentObjectClassIDArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ObjectInformation")
		TArray<FString> CurrentObjectUEIDArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ObjectInformation")
		TArray<FString> CurrentObjectTypeArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectInformation")
		int32 CurrentObjectMaskNum;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
		bool SaveStringArrayToFile(TArray<FString> StrArray, FString SavePath = "Content/Text/");	//用于保存CID到TXT

	static void OnScreenshotCapturedInternal(int32 Width, int32 Height, const TArray<FColor>& Bitmap);

	/****几种截图方案***/
	UFUNCTION(BlueprintCallable)
		void TakeScreenShot(const FString fineName = FString(TEXT("Image")), const FString finePath = FString(TEXT("Content/Imgs/")), EImageFormatType _format = EImageFormatType::PNG,
			int32 Quality = 100, bool bInsertTimeStampToFileName = false, bool bInShowUI = false, bool bAddFilenameSuffix = false);

	static void OnCaptureScreenshotComplete(int32 InWidth, int32 InHeight, const TArray<FColor>& InColors);

	UFUNCTION(BlueprintCallable)
		void CaptureScreenshot(const FString finePath, FString fileName);

	UFUNCTION(BlueprintCallable)
		bool TakeHighResShot(const int32 Width, const int32 Height, const bool Fullscreen, const FString finePath);

	/*******/


	UFUNCTION(BlueprintCallable)
		TArray<FString> GetObjectArrayCIDtoStrArray(const TArray<AObjectBase*> ObjectArray);

	UFUNCTION(BlueprintCallable)	//通过路径来得到TEXT文件并转化为String
		virtual TArray<FString> GetTxtFileOnPath(FString filePath,FString fileName);

	UFUNCTION(BlueprintCallable)	//将读取到的text内容依次转变成ID数据（可通过类型号来筛选出想要只读的Obj类型）
		virtual bool GetTxtStringToObjectID(TArray<FString> StrArray, FString DeletStrTarget_1, FString DeletStrTarget_2, FString DeletStrTarget_3,TArray<FString> OnlyReadStrType);

	UFUNCTION(BlueprintCallable)	//随机出要创建的ObjID，可以使用isUseQueue来让其顺序读取
		virtual FStructObjectID GetStringIDArrayRandomToStructObjectID(const bool isUseQueue,const TArray<FString> & UIDArray,const TArray<FString> & CIDArray, const TArray<FString> & MIDArray, const TArray<FString> & ObjectTypeArray, const int32 & MaskID, bool isRandomSimilar, int32 SimilarRange);

	UFUNCTION(BlueprintCallable)
		virtual TArray<FStructObjectID> GetStringIDArrayQueueToStructObjectID(const TArray<FString> & UIDArray, const TArray<FString> & CIDArray,
		const TArray<FString> & MIDArray, const TArray<FString> & ObjectTypeArray, const int32 & MaskID);

	UFUNCTION(BlueprintCallable)	//清除所有创建的Obj
		virtual void ClearAllSpawnObjectCompArray();

	UFUNCTION(BlueprintCallable)
		virtual ObjectType ChooseObjectType(int32 Index);

	UFUNCTION(BlueprintCallable)
		virtual void SaveData();

	UFUNCTION(BlueprintCallable)
		virtual void ReloadCreateTimes(int32 Num = 1) { CreateTimes = Num; SaveData(); };

	UFUNCTION(BlueprintCallable)
		virtual void LoopLoadAllMesh() {};



	UFUNCTION(BlueprintCallable)
		virtual void CashierReloat();

};
