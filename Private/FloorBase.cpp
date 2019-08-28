// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorBase.h"

#include "IPlatformFileCachedWrapper.h"
#include "PlatformFileManager.h"
#include "FileHelper.h"
#include "FileManagerGeneric.h"
#include "Paths.h"
#include "TransArray.h"
#include "ImageUtils.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Engine/Texture2D.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "TimerManager.h"
AFloorBase::AFloorBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PlaneMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMeshComp"));
	SetRootComponent(PlaneMeshComp);


	ChangeSpeed = 1.5f;
	MinRandomIndex = 1;
	MaxRandomIndex = 119;

}
UTexture2D* AFloorBase::LoadTexture2D(const FString path, bool& IsValid, int32& OutWidth, int32& OutHeight)
{
	UTexture2D* Texture = nullptr;
	IsValid = false;
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*path))
	{
		return nullptr;
	}
	TArray<uint8> RawFileData;
	if (!FFileHelper::LoadFileToArray(RawFileData, *path))
	{
		return nullptr;
	}
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);;
	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
		const TArray<uint8>* UncompressedRGBA = nullptr;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA
			, 8, UncompressedRGBA))
		{
			Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
			if (Texture != nullptr)
			{
				IsValid = true;
				OutWidth = ImageWrapper->GetWidth();
				OutHeight = ImageWrapper->GetHeight();
				void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, UncompressedRGBA->GetData(), UncompressedRGBA->Num());
				Texture->PlatformData->Mips[0].BulkData.Unlock();
				Texture->UpdateResource();
			}
		}
	}
	return Texture;

}
void AFloorBase::RandomeChangeTexture()	//根据设定的数值范围 随机查找并替换材质贴图
{
	UMaterialInstanceDynamic* DynMaterial = PlaneMeshComp->CreateDynamicMaterialInstance(0, PlaneMeshComp->GetMaterial(0));
	if (DynMaterial)
	{
		int32 Index = FMath::FRandRange(MinRandomIndex, MaxRandomIndex);
		FString thisPath = FPaths::ProjectDir() + "Content/ImageMaterial/" + FString::FromInt(Index) + ".jpg";

		bool IsValid;int32 OutWidth; int32 OutHeight;
		DynMaterial->SetTextureParameterValue("Image", LoadTexture2D(thisPath, IsValid, OutWidth, OutHeight));
	}

}
void AFloorBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(Hand, this, &AFloorBase::RandomeChangeTexture, ChangeSpeed, true, -1.0);
}

void AFloorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

