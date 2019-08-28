
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
void ACashier_A::OnConstruction(const FTransform & Transform)	//�ڵ���CompʱΪ���������
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

		WillCreateNum = FMath::RandRange(1, RandomCreateObjectNum);	//�����趨�������Ʒ������������ó���Ҫ����������

		for (int i = 0; i < WillCreateNum; i++)
		{
			CurrentObjectMaskNum++;
			//�õ���Ҫ����Obj��ID��Ϣ
			FStructObjectID CurrentObjID = GetStringIDArrayRandomToStructObjectID(isQueue, CurrentObjectUEIDArray, CurrentObjectClassIDArray,
				CurrentObjectUEIDArray, CurrentObjectTypeArray, CurrentObjectMaskNum, true, SameProbability);

				WillCreateObjectIDArray.Add(CurrentObjID);	//�洢�������ID
		}

		GetWorldTimerManager().SetTimer(TimeHand_DelayCreateObject, this, &ACashier_A::DelayCreateObject, 0.1, true);	//ÿ����ʱ0.1��ȥ����һ��Obj
	}
}
void ACashier_A::DelayCreateObject_Implementation()
{
	if (WillCreateNum > 0 && WillCreateObjectClass)
	{
		int32 Index = WillCreateObjectIDArray.Num() - WillCreateNum;	//�����ȡID������±�

		if (WillCreateObjectIDArray.IsValidIndex(Index))
		{
			AObjectBase * CurrentObject;
			CurrentObject = ChooseCreateObjectComp(WillCreateNum, WillCreateObjectIDArray[Index]);	//ѡ��Ҫ���ĸ�Component������Obj

			if (CurrentObject)
			{
				SaveObjectArray.Add(CurrentObject);

			}

			WillCreateNum--;

		}
	}
	else   //���ȫ������������� ����������¼�
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

	for (int Index_1 = 0; Index_1 < ArrayComp.Num(); Index_1++)	//�Ȳ����Ƿ��ж�Ӧ���͵�comp
	{
		USpawnObjectComponent * SpawnComp = Cast<USpawnObjectComponent>(ArrayComp[Index_1]);
		if (SpawnComp->CurrentCompWillCreateType == ObjID.Type)
		{
			ArrayObjectComp.Add(SpawnComp);
		}

	}

	if (ArrayObjectComp.Num() <= 0)	//���δ�ҵ���Ӧ���ͱ�����������������͵�comp
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

	if (ArrayObjectComp.Num() <= 0)	//�����Ȼû�����������
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

		//����Ҳ�����Ӧ��������ȼ�������������
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

	for (int index = 0;index < SaveObjectArray.Num();index++)	//Ϊ������ÿ��Obj��������ģ��
	{
		SaveObjectArray[index]->SetObjPhysical(true);
	}

	GetWorldTimerManager().SetTimer(TimeHand_Export, this, &ACashier_A::BeginExportEvent, ObjectPhysicalTime,false);

}
void ACashier_A::BeginExportEvent_Implementation()
{
	for (int index = 0; index < SaveObjectArray.Num(); index++)	//���������ObjΪ��Ҫ����ʶ��������������߼���¼�
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
		//����CID��Ϣ������
		SaveStringArrayToFile(GetObjectArrayCIDtoStrArray(SaveObjectArray), "Content/Export/json_labels/total");

		ExportImageEvent();
		/*TakeHighResShot(1920, 1080, true, "Content/Export2/imgs");			//cpp��ʵ����ʱ��ͼ������ ��ʱ������ͼʱ��

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
void ACashier_A::LoopLoadAllMesh()	//���ټ�������Obj
{
	//˳��õ�ȫ���б�����
	WillCreateObjectIDArray = GetStringIDArrayQueueToStructObjectID(CurrentObjectUEIDArray, CurrentObjectClassIDArray, CurrentObjectUEIDArray, CurrentObjectTypeArray, CurrentObjectMaskNum);
	WillCreateNum = WillCreateObjectIDArray.Num();

	//���ٴ�����ɾ��
	GetWorldTimerManager().SetTimer(TimeHand_DelayCreateObject, this, &ACashier_A::DelayCreateObject, 0.1, true);
	GetWorldTimerManager().SetTimer(TimeHand_ClearAllSpawnObjCompArray, this, &ACashier_A::ClearAllSpawnObjectCompArray, 0.2, true);
}



void ACashier_A::CashierReloat()	//����Cashier�������
{
	CurrentObjectMaskNum = 1;
	SaveObjectArray.Empty();
	ClearAllSpawnObjectCompArray();
	CreateTimes++;
	SpawnSwitch = true;
}
