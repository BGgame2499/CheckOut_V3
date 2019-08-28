
#include "CashierBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "FileHelper.h"
#include "ObjectBase.h"
#include "HighResScreenshot.h"
#include "SpawnObjectComponent.h"
#include "TimerManager.h"
#include "CheckOutInstance.h"
#include "Saves/CheckOutSave.h"
#include "Engine.h"

#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"

EImageFormatType format;
int32 quality;
FString FileName;
FDelegateHandle delegateHandle;

ACashierBase::ACashierBase()
{
	PrimaryActorTick.bCanEverTick = true;
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
	CashierMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CashierMeshComp"));

	SpawnObjectComp_1 = CreateDefaultSubobject<USpawnObjectComponent>(TEXT("SpawnComp1"));
	SpawnObjectComp_1->SetupAttachment(CashierMesh);



	CreateTimes = 1;


	isLoadAllMesh = false;
	isClearCreateTimes = false;
	isQueue = false;
	isExport = false;
	RandomCreateObjectNum = 3;
	CurrentObjectMaskNum = 1;
	SameProbability = 3;


	WillCreateNum = 0;

	ReadTxtFileName = "20190805_UE4ID.txt";
}

int32 LastRandomNum;	//为GetStringIDArrayRandomToStructObjectID函数所用
int32 QueueNum;

void ACashierBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<FString> AllTxtFile = GetTxtFileOnPath("Content", ReadTxtFileName);
	GetTxtStringToObjectID(AllTxtFile, "", "", "", OnlyReadType);	//读取列表并初始化各项ObjectInformation

	LastRandomNum = -1;
	QueueNum = 0;

	UCheckOutInstance * GameInst = Cast<UCheckOutInstance>(GetGameInstance());
	if (GameInst){CreateTimes = GameInst->SaveGames->CreateTimes;}	//读取保存的CreateTimes数量
	if (isClearCreateTimes) { ReloadCreateTimes(); }
	if (isLoadAllMesh){LoopLoadAllMesh();}


}

void ACashierBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACashierBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SaveData();
}

bool ACashierBase::SaveStringArrayToFile(TArray<FString> StrArray, FString SavePath)
{
	if (StrArray.Num())
	{
		/*FString SavePathAndName = FPaths::ProjectDir() + SavePath + "/" + FString::FromInt(CreateTimes) + "_json" + "/"
			"label_"+ FString::FromInt(CreateTimes) + ".txt";*/
		FString SavePathAndName = FPaths::ProjectDir() + SavePath + "/" + FString::FromInt(CreateTimes) + "_json" + "/"
			"label_names" + ".txt";
		FFileHelper::SaveStringArrayToFile(StrArray, *SavePathAndName);

	}

	return false;
}
void ACashierBase::TakeScreenShot(const FString fineName, const FString finePath, EImageFormatType _format,
	int32 Quality, bool bInsertTimeStampToFileName, bool bInShowUI, bool bAddFilenameSuffix)
{
	FString filename = fineName;

	format = _format;

	quality = Quality;


	if (bInsertTimeStampToFileName)  //名字后面加上现在的时间
	{
		filename += "_" + FDateTime::Now().ToString();
	}


	FileName = FPaths::ProjectDir() + finePath  + "/" + FString::FromInt(CreateTimes) + filename;


	delegateHandle = UGameViewportClient::OnScreenshotCaptured().AddStatic(&OnScreenshotCapturedInternal);


	FScreenshotRequest::RequestScreenshot(filename, bInShowUI, bAddFilenameSuffix);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("** TakeScreenShot **"));



	//GEngine->GameViewport->Exec(nullptr, TEXT("1080x720"), *GLog);


}
/////////////////////////////////////////////////////////////////////////

bool ACashierBase::TakeHighResShot(const int32 Width, const int32 Height, const bool Fullscreen, const FString finePath)
{
	if (GEngine)
	{
		UGameUserSettings* Settings = GEngine->GetGameUserSettings();
		if (!Settings)
		{
			return false;
		}

		EWindowMode::Type WindowMode = Fullscreen ? EWindowMode::Fullscreen : EWindowMode::Windowed;

		GScreenshotResolutionX = Width;
		GScreenshotResolutionY = Height;
		FString filename = FPaths::ProjectDir() + finePath + "/" + FString::FromInt(CreateTimes) +  ".png";
		FHighResScreenshotConfig& HighResScreenshotConfig = GetHighResScreenshotConfig();
		HighResScreenshotConfig.FilenameOverride = filename;
		HighResScreenshotConfig.SetResolution(Width, Height);
		//GIsHighResScreenshot = true;
		FScreenshotRequest::RequestScreenshot(filename, false, true);
		//FString fileNameNextScreenshot = FScreenshotRequest::GetFilename();
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *fileNameNextScreenshot);		
		return true;
	}

	return false;
}




FDelegateHandle GCaptureHandle;
void ACashierBase::OnCaptureScreenshotComplete(int32 InWidth, int32 InHeight, const TArray<FColor>& InColors)
{
	FString ScreenShotName = FPaths::ProjectSavedDir() + "BB.png";
	TArray<uint8> CompressedBitmap;
	for (int32 Index = 0; Index < InWidth*InHeight; Index++)
	{
		InColors[Index].Black;
	}
	//FImageUtils::CompressImageArray(InWidth, InHeight, InColors, CompressedBitmap);
	FFileHelper::SaveArrayToFile(CompressedBitmap, *ScreenShotName);
	UGameViewportClient::OnScreenshotCaptured().Remove(GCaptureHandle);
}

void ACashierBase::CaptureScreenshot(const FString finePath, FString fileName)
{
	if (GEngine && GEngine->GameViewport)
	{
		GCaptureHandle = UGameViewportClient::OnScreenshotCaptured().AddStatic(&OnCaptureScreenshotComplete);
		GEngine->GameViewport->Exec(nullptr, TEXT("HighResShot_2 1920x1080"), *GLog);//分辨率可自己设定
	}
}

TArray<FString> ACashierBase::GetObjectArrayCIDtoStrArray(const TArray<AObjectBase*> ObjectArray)
{
	TArray<FString> IntoStrArray;
	IntoStrArray.Add("_background_");

	int32 CurrentMaskID = 1;

	for (int32 Index = 0;Index < ObjectArray.Num();Index++)
	{
		//ObjectArray[Index].SetObjPhysical(false);
		if (ObjectArray[Index])
		{
			IntoStrArray.Add(*FString::FromInt(ObjectArray[Index]->ObjectIDStruct.CID));

			ObjectArray[Index]->ObjectIDStruct.MaskID = CurrentMaskID += 1;	//以防万一重置一次MaskID
			ObjectArray[Index]->SetAndCountCustomDepth();
		}


	}

	return IntoStrArray;
}

/////////////////////////////////////////////////////////////////////////
void ACashierBase::OnScreenshotCapturedInternal(int32 Width, int32 Height, const TArray<FColor>& Bitmap)
{

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));

	TSharedPtr<IImageWrapper> ImageWrapper;

	if (format == EImageFormatType::PNG)
	{

		TArray<uint8> PNGData;
		TArray<FColor> BitmapCopy(Bitmap);
		FImageUtils::CompressImageArray(Width, Height, BitmapCopy, PNGData);


		FileName += ".png";
		FFileHelper::SaveArrayToFile(PNGData, *FileName);

	}

	else
	{
		//默认使用Jpg格式

		ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
		ImageWrapper->SetRaw(Bitmap.GetData(), Bitmap.GetAllocatedSize(), Width, Height, ERGBFormat::BGRA, 8);

		const TArray<uint8>& JPEGData = ImageWrapper->GetCompressed(quality);
		FileName += ".jpg";
		FFileHelper::SaveArrayToFile(JPEGData, *FileName);

	}

	UGameViewportClient::OnScreenshotCaptured().Remove(delegateHandle);
	ImageWrapper.Reset();



}

TArray<FString> ACashierBase::GetTxtFileOnPath(FString filePath, FString fileName)
{
	FString thisPath = FPaths::ProjectDir() + filePath + "/" + fileName;

	TArray<FString> TextArray;

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*thisPath))
	{

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("** Could not Find File **"));

		return TextArray;

	}
	//FFileHelper::LoadFileToString(TextArray, *thisPath);
	FFileHelper::LoadFileToStringArray(TextArray, *thisPath);

	return TextArray;
}
bool ACashierBase::GetTxtStringToObjectID(TArray<FString> StrArray, FString DeletStrTarget_1, FString DeletStrTarget_2, FString DeletStrTarget_3, TArray<FString> OnlyReadStrType)
{
	for (int Index = 0;Index < StrArray.Num();Index++)
	{

		TArray<FString> OutStr;
		const TCHAR * DelimArray(TEXT(" "));
		StrArray[Index].ParseIntoArray(OutStr, DelimArray, true);

		if (OutStr.Num() >= 3)
		{
			if (OnlyReadStrType.Num())
			{
				for (int RedIndex = 0; RedIndex < OnlyReadStrType.Num(); RedIndex++)
				{
					if (OnlyReadStrType[RedIndex] == OutStr[2])
					{
						CurrentObjectUEIDArray.Add(OutStr[0]);
						CurrentObjectClassIDArray.Add(OutStr[1]);
						CurrentObjectTypeArray.Add(OutStr[2]);
					}
				}

			}
			else
			{
				CurrentObjectUEIDArray.Add(OutStr[0]);
				CurrentObjectClassIDArray.Add(OutStr[1]);
				CurrentObjectTypeArray.Add(OutStr[2]);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TXT Miss ID:%d"),Index+1);
		}
	}

	return false;

}
TArray<FStructObjectID> ACashierBase::GetStringIDArrayQueueToStructObjectID(const TArray<FString> & UIDArray, const TArray<FString> & CIDArray,
	const TArray<FString> & MIDArray, const TArray<FString> & ObjectTypeArray, const int32 & MaskID)
{
	TArray<FStructObjectID> IDArray;

	
	for (int32 Index = 0; Index < UIDArray.Num(); Index++)
	{

		FStructObjectID IDStruct;


		IDStruct.UID = FCString::Atoi(*UIDArray[Index]);
		IDStruct.CID = FCString::Atoi(*CIDArray[Index]);
		IDStruct.MID = FCString::Atoi(*MIDArray[Index]);
		IDStruct.MaskID = MaskID;
		IDStruct.Type = ChooseObjectType(FCString::Atoi(*ObjectTypeArray[Index]));

		IDArray.Add(IDStruct);
	}

	return IDArray;

}


void ACashierBase::ClearAllSpawnObjectCompArray()
{
	TArray<UActorComponent*> ArrayComp;
	ArrayComp = GetComponentsByClass(SpawnObjectComp_1->GetClass());

	for (int Index = 0; Index < ArrayComp.Num();Index++)
	{
		USpawnObjectComponent * SpawnComp = Cast<USpawnObjectComponent>(ArrayComp[Index]);
		SpawnComp->ClearCurrentObject();
	}
}


FStructObjectID ACashierBase::GetStringIDArrayRandomToStructObjectID(const bool isUseQueue,const TArray<FString> & UIDArray, const TArray<FString> & CIDArray,
	const TArray<FString> & MIDArray, const TArray<FString> & ObjectTypeArray, const int32 & MaskID,bool isRandomSimilar,int32 SimilarRange)
{
	FStructObjectID IDStruct;

	if (UIDArray.Num() > 0 && CIDArray.Num() > 0 && ObjectTypeArray.Num() > 0)
	{
		int32 ArrayIndex;

		if (isUseQueue)	//如果使用队列顺序输出则改变将要随机的下标为顺序队列 否则使用随机
		{
			ArrayIndex = QueueNum;
			QueueNum++;
		}
		else
		{

			int32 RandomRange = UIDArray.Num();
			ArrayIndex = FMath::FRandRange(0, RandomRange);
		}

		if (LastRandomNum != -1 && isRandomSimilar && !isQueue)	//创造随机生成相同物品的概率
		{
			int32 Num = FMath::FRandRange(1, SimilarRange);
			if (Num == 1)
			{
				ArrayIndex = LastRandomNum;
			}
			
		}

		if (!UIDArray.IsValidIndex(ArrayIndex))
		{
			return IDStruct;
		}
		
		IDStruct.UID = FCString::Atoi(*UIDArray[ArrayIndex]);
		IDStruct.CID = FCString::Atoi(*CIDArray[ArrayIndex]);
		IDStruct.MID = FCString::Atoi(*MIDArray[ArrayIndex]);
		IDStruct.Type = ChooseObjectType(FCString::Atoi(*ObjectTypeArray[ArrayIndex]));
		IDStruct.MaskID = MaskID;
		LastRandomNum = ArrayIndex;

	}
	return IDStruct;
}


void ACashierBase::SaveData()
{
	UCheckOutInstance * GameInst = Cast<UCheckOutInstance>(GetGameInstance());
	if (GameInst)
	{
		GameInst->SaveGames->CreateTimes = CreateTimes;
		GameInst->SaveCheckOut();
	}
}

void ACashierBase::CashierReloat()
{
	CurrentObjectMaskNum = 1;
	SaveObjectArray.Empty();
	ClearAllSpawnObjectCompArray();
	CreateTimes++;
}
ObjectType ACashierBase::ChooseObjectType(int32 Index)	//根据读取的表格来返回Obj的类型
{
	ObjectType CurrentType;
	switch (Index)
	{
	case -1:
		CurrentType = ObjectType::Other;
		break;
	case 0:
		CurrentType = ObjectType::Water;
		break;
	case 1:
		CurrentType = ObjectType::Smoke;
		break;
	case 3:
		CurrentType = ObjectType::Milk;
		break;
	case 2:
		CurrentType = ObjectType::Food;
		break;
	case 4:
		CurrentType = ObjectType::Bucket;
		break;
	case 55:
		CurrentType = ObjectType::LongSmoke;
		break;
	case 5:
		CurrentType = ObjectType::ShortPot;
		break;
	default:
		CurrentType = ObjectType::Other;
		break;
	}

	return CurrentType;
}