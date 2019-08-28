
#include "Cashier_A.h"
#include "TimerManager.h"
#include "SpawnObjectComponent.h"
#include "ObjectBase.h"
#include "SpecialObject.h"
#include "Engine/PostProcessVolume.h"
#include "Engine.h"

ACashier_A::ACashier_A()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnSwitch = true;
	ObjectPhysicalTime = 0.3f;
}

void ACashier_A::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 6, FColor::Red, this->GetName());
}
void ACashier_A::OnConstruction(const FTransform & Transform)	//在调整Comp时为其绘制射线
{
	Super::OnConstruction(Transform);

	GiveAllObjectComponentDrawnLine();
}
void ACashier_A::GiveAllObjectComponentDrawnLine()
{
	TArray<UActorComponent*> ArrayComp;
	ArrayComp = GetComponentsByClass(SpawnObjectComp_1->GetClass());

	for (int Index = 0; Index < ArrayComp.Num(); Index++)
	{
		USpawnObjectComponent * SpawnComp = Cast<USpawnObjectComponent>(ArrayComp[Index]);

		DrawDebugLine(GetWorld(), SpawnComp->GetComponentLocation(), SpawnComp->GetComponentLocation() + SpawnComp->RecognitionRayVector, FColor(255, 100, 0), false, 10.0f, 0.0f, 3.0f);
	}

}
void ACashier_A::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!isLoadAllMesh && SpawnSwitch)
	{
		SpawnSwitch = false;

		WillCreateNum = FMath::RandRange(1, RandomCreateObjectNum);	//根据设定的最大物品生成数量随机得出将要创建的数量

		for (int i = 0; i < WillCreateNum; i++)
		{
			CurrentObjectMaskNum++;
			//得到将要创建Obj的ID信息
			FStructObjectID CurrentObjID = GetStringIDArrayRandomToStructObjectID(isQueue, CurrentObjectUEIDArray, CurrentObjectClassIDArray,
				CurrentObjectUEIDArray, CurrentObjectTypeArray, CurrentObjectMaskNum, true, SameProbability);

				WillCreateObjectIDArray.Add(CurrentObjID);	//存储随机到的ID
		}

		GetWorldTimerManager().SetTimer(TimeHand_DelayCreateObject, this, &ACashier_A::DelayCreateObject, 0.1, true);	//每次延时0.1秒去创建一个Obj
	}
}
void ACashier_A::DelayCreateObject_Implementation()
{
	if (WillCreateNum > 0 && WillCreateObjectClass)
	{
		int32 Index = WillCreateObjectIDArray.Num() - WillCreateNum;	//算出读取ID数组的下标

		if (WillCreateObjectIDArray.IsValidIndex(Index))
		{
			AObjectBase * CurrentObject;
			CurrentObject = ChooseCreateObjectComp(WillCreateNum, WillCreateObjectIDArray[Index]);	//选择要用哪个Component来创建Obj

			if (CurrentObject)
			{
				SaveObjectArray.Add(CurrentObject);

			}

			WillCreateNum--;

		}
	}
	else   //如果全部创建完则清除 并调用输出事件
	{

		GetWorldTimerManager().ClearTimer(TimeHand_DelayCreateObject);
		GetWorldTimerManager().ClearTimer(TimeHand_ClearAllSpawnObjCompArray);

		ExportEvent();
		WillCreateObjectIDArray.Empty();
		
	}


}

AObjectBase * ACashier_A::ChooseCreateObjectComp(int32 CompNum, FStructObjectID ObjID)
{

	TArray<UActorComponent*> ArrayComp;
	TArray<USpawnObjectComponent*> ArrayObjectComp;
	ArrayComp = GetComponentsByClass(SpawnObjectComp_1->GetClass());

	for (int Index_1 = 0; Index_1 < ArrayComp.Num(); Index_1++)	//先查找是否有对应类型的comp
	{
		USpawnObjectComponent * SpawnComp = Cast<USpawnObjectComponent>(ArrayComp[Index_1]);
		if (SpawnComp->CurrentCompWillCreateType == ObjID.Type)
		{
			ArrayObjectComp.Add(SpawnComp);
		}

	}

	if (ArrayObjectComp.Num() <= 0)	//如果未找到对应类型便查找允许创建所有类型的comp
	{
		for (int Index_2 = 0; Index_2 < ArrayComp.Num(); Index_2++)
		{
			USpawnObjectComponent * SpawnComp = Cast<USpawnObjectComponent>(ArrayComp[Index_2]);
			if (SpawnComp->CurrentCompWillCreateType == ObjectType::All)
			{
				ArrayObjectComp.Add(SpawnComp);
			}

		}
	}

	if (ArrayObjectComp.Num() <= 0)	//如果依然没有则放弃创建
	{
		UE_LOG(LogTemp, Warning, TEXT("NotAvailableComponent!"))
	}
	else
	{
		for (int Index_3 = 0; Index_3 < ArrayObjectComp.Num(); Index_3++)
		{
			if (ArrayObjectComp[Index_3]->CreatePriority == CompNum)
			{
				return ArrayObjectComp[Index_3]->CreateObject(WillCreateObjectClass, ObjID, this);
			}

		}

		//如果找不到对应的组件优先级序号则随机创建
		return ArrayObjectComp[FMath::RandRange(0, ArrayObjectComp.Num()-1)]->CreateObject(WillCreateObjectClass, ObjID, this);
		
	}

	return nullptr;
}

void ACashier_A::ExportImageEvent_Implementation()
{
}

void ACashier_A::ExportEvent_Implementation()
{
	if (ObjectPhysicalTime <= 0.0f)
	{
		BeginExportEvent();
		return;
	}

	for (int index = 0;index < SaveObjectArray.Num();index++)	//为创建的每个Obj开启物理模拟
	{
		SaveObjectArray[index]->SetObjPhysical(true);
	}

	GetWorldTimerManager().SetTimer(TimeHand_Export, this, &ACashier_A::BeginExportEvent, ObjectPhysicalTime,false);

}
void ACashier_A::BeginExportEvent_Implementation()
{
	for (int index = 0; index < SaveObjectArray.Num(); index++)	//如果创建的Obj为需要特殊识别类则调用其射线检测事件
	{
		SaveObjectArray[index]->SetObjPhysical(false);
		ASpecialObject * CurrentSpecialObject = Cast<ASpecialObject>(SaveObjectArray[index]);
		if (CurrentSpecialObject)
		{
			CurrentSpecialObject->RayRecognition_Event();
		}
	}
	if (isExport)
	{
		//保存CID信息到本地
		SaveStringArrayToFile(GetObjectArrayCIDtoStrArray(SaveObjectArray), "Content/Export/json_labels/total");

		ExportImageEvent();
		/*TakeHighResShot(1920, 1080, true, "Content/Export2/imgs");			//cpp中实现延时截图有问题 暂时调用蓝图时间

		if (MaskPostVolume)
		{
			MaskPostVolume->bEnabled = true;

			TakeHighResShot(1920, 1080, true, "Content/Export2/labelImgs");

			MaskPostVolume->bEnabled = false;
		}
*/
		//EndExportEvent();

	}
	else
	{
		EndExportEvent();
	}

}
void ACashier_A::EndExportEvent_Implementation()
{
}
void ACashier_A::LoopLoadAllMesh()	//快速加载所有Obj
{
	//顺序得到全部列表内容
	WillCreateObjectIDArray = GetStringIDArrayQueueToStructObjectID(CurrentObjectUEIDArray, CurrentObjectClassIDArray, CurrentObjectUEIDArray, CurrentObjectTypeArray, CurrentObjectMaskNum);
	WillCreateNum = WillCreateObjectIDArray.Num();

	//快速创建并删除
	GetWorldTimerManager().SetTimer(TimeHand_DelayCreateObject, this, &ACashier_A::DelayCreateObject, 0.1, true);
	GetWorldTimerManager().SetTimer(TimeHand_ClearAllSpawnObjCompArray, this, &ACashier_A::ClearAllSpawnObjectCompArray, 0.2, true);
}



void ACashier_A::CashierReloat()	//重载Cashier各项参数
{
	CurrentObjectMaskNum = 1;
	SaveObjectArray.Empty();
	ClearAllSpawnObjectCompArray();
	CreateTimes++;
	SpawnSwitch = true;
}
