

#include "ObjectBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "ConstructorHelpers.h"
#include "PlatformFileManager.h"
#include "FileHelper.h"
#include "FileManagerGeneric.h"
#include "Materials/MaterialInstanceDynamic.h"

AObjectBase::AObjectBase()
{
	PrimaryActorTick.bCanEverTick = true;
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
	SetRootComponent(RootScene);
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMeshComp"));
	//ObjectMesh->SetSimulatePhysics(true);
	ObjectMesh->SetupAttachment(RootScene);
	ObjectMesh->SetRenderCustomDepth(true);

	
	isRandomMaterial = false;
	MaxRandomRoughness = 1.2f;
	MinRandomRoughness = 0.8f;
	MaxRandomMetal = 1.2f;
	MinRandomMetal = 0.8f;

}
TArray<FString> AObjectBase::GetMatPathList(FString MatPath, FString Format)
{
	FString SearchStr = MatPath + Format;
	TArray<FString> FileList;
	IFileManager::Get().FindFiles(FileList, *SearchStr, true, false);
	return FileList;
}

bool AObjectBase::LoadNewMeahWithMaterialToObjectMesh(FString InMeshPath, FString InMatPath)
{

	if (LoadNewMeahToObjectMesh(InMeshPath))
	{
		FString postfix = FString::FromInt(ObjectIDStruct.UID) + "_*";
		FString thisMatPath = FPaths::ProjectDir() + "Content/" + InMatPath;
		TArray<FString> FileList = GetMatPathList(thisMatPath, postfix);

		if (FileList.Num())		//是否根据路径找到材质路径
		{

			TArray<FString> FileNameList;
			for (int i = 0; i < FileList.Num(); i++)	//Remove.uasset
			{
				//FileList[i].ParseIntoArray(FileNameList, TEXT("."), true);
				FileNameList.Add(FileList[i].LeftChop(7));
				
			}

			for (int i = 0; i < FileNameList.Num(); i++)
			{
				FString newthisMeshPath = "/Game/" + InMatPath + FileNameList[i];
				ObjectMesh->SetMaterial(i, Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, *FName(*newthisMeshPath).ToString())));
			}



			UMaterialInstanceDynamic* DynMaterial = ObjectMesh->CreateDynamicMaterialInstance(0, ObjectMesh->GetMaterial(0));
			if (DynMaterial && isRandomMaterial)		//随机设置材质变量_
			{
				float orignMetal = 0.f;
				float orignRough = 0.f;
				if (DynMaterial->GetScalarParameterValue(FName(TEXT("M")), orignMetal))
				{
					orignMetal = FMath::FRandRange(MinRandomMetal, MaxRandomMetal);
					DynMaterial->SetScalarParameterValue("M", orignMetal);
				}
				if (DynMaterial->GetScalarParameterValue(FName(TEXT("R")), orignRough))
				{
					orignRough = FMath::FRandRange(MinRandomRoughness,MaxRandomRoughness);
					DynMaterial->SetScalarParameterValue("R", orignRough);
				}
			}
			
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("find Material by FilePath failed!"));
		}

	}

	return false;
}

bool AObjectBase::LoadNewMeahToObjectMesh(FString InMeshPath)
{
	//FString fileName = "/Game/SKU/MeshData/" + InMeshPath;
	FString postfix = FString::FromInt(ObjectIDStruct.UID) + "_*";
	FString thisMeshPath = FPaths::ProjectDir() + "Content/" + InMeshPath;
	TArray<FString> FileList = GetMatPathList(thisMeshPath, postfix);
	TArray<FString> FileNameList;


	if (FileList.Num())		//是否根据路径找到模型路径
	{
		for (int i = 0; i < FileList.Num(); i++)	//Remove.uasset
		{
			FileList[i].ParseIntoArray(FileNameList, TEXT("."), false);
		}

		FString newthisMeshPath = "/Game/" + InMeshPath + FileNameList[0];
		//FString newthisMeshPath = thisMeshPath + FileList[0];
		UStaticMesh* tempMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *FName(*newthisMeshPath).ToString()));
		if (tempMesh)
		{
			ObjectMesh->SetStaticMesh(tempMesh);
			ObjectMesh->SetUseCCD(true);

			SetAndCountCustomDepth();

			MeshBoundVector = ObjectMesh->GetStaticMesh()->GetBounds().GetBox().GetSize();
			nowMinSpanVector = minSpanVector + FVector(MeshBoundVector.GetMax(), MeshBoundVector.GetMax(), 0.0f);
			nowMaxSpanVector = maxSpanVector - FVector(MeshBoundVector.GetMax(), MeshBoundVector.GetMax(), MeshBoundVector.GetMax());

			return true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("find Mesh by FilePath failed!"));
	}
	return false;
}
void AObjectBase::SetAndCountCustomDepth(int32 DepthNum)
{
	
	if (ObjectMesh->GetStaticMesh())
	{
		
		int32 nawDepthNum = DepthNum + (ObjectIDStruct.MaskID * DepthNum / 3);
		ObjectMesh->SetRenderCustomDepth(true);
		ObjectMesh->SetCustomDepthStencilValue(ObjectIDStruct.MaskID);
	}

}
FString AObjectBase::GetMeshPathNameByClassID(FString MeshBasePath, TArray<FString> MeshNameList, int32 classID)
{
	TArray<FString> tempList;
	for (int32 i = 0; i < MeshNameList.Num(); i++)
	{
		MeshNameList[i].ParseIntoArray(tempList, TEXT("_"), false);
		int32 nowClass = FCString::Atoi(*tempList[0]);
		if (nowClass == classID)
		{
			return MeshBasePath + "/" + MeshNameList[i];
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("find mesh name by class ID failed!"));
	return MeshBasePath + "/" + MeshNameList[0];
}



void AObjectBase::SetObjPhysical(bool is)
{
	if (ObjectMesh->GetStaticMesh())
	{
		ObjectMesh->SetSimulatePhysics(is);

	}
}

void AObjectBase::BeginPlay()
{
	Super::BeginPlay();
	LoadNewMeahWithMaterialToObjectMesh();
}

void AObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

