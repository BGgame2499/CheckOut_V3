// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnObjectComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ObjectBase.h"
#include "Engine.h"

USpawnObjectComponent::USpawnObjectComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	RecognitionRayVector.Z = -400.0f;
	SpawnOffset.Z = -40.0f;
	RandomRotationNum = 6;
	CreatePriority = 0;

	CurrentCompWillCreateType = ObjectType::All;
	CurrentCompWillUseObjectRotFunType = ObjectType::All;

}

void USpawnObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	MaxRandomNum = RandomRotationNum;
	
}

FRotator USpawnObjectComponent::RandomCountFoodRotation(UStaticMeshComponent * MeshComp)
{
	if (!MeshComp)
	{
		return FRotator(0, 0, 0);
	}

	MeshComp->AddWorldRotation(FRotator(0, FMath::RandRange(-360, 360), 0),true);

	return MeshComp->GetComponentRotation();
}
FRotator USpawnObjectComponent::RandomCountWaterRotation(UStaticMeshComponent * MeshComp)
{
	if (!MeshComp)
	{
		return FRotator(0, 0, 0);
	}

	MeshComp->AddWorldRotation(FRotator(0, FMath::RandRange(-360, 360), 0));

	MeshComp->AddWorldRotation(FRotator(70, 0, 0),true);
	switch (RandomRotationNum)
	{
	case 0:
		MeshComp->AddWorldRotation(FRotator(0, 90, 0), true);
		break;
	case 1:
		MeshComp->AddWorldRotation(FRotator(0, -90, 0), true);
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}

	return MeshComp->GetComponentRotation();
}
FRotator USpawnObjectComponent::RandomCountSmokeRotation(UStaticMeshComponent* MeshComp)
{
	
	if (!MeshComp)
	{
		return FRotator(0,0,0);
	}

	MeshComp->AddWorldRotation(FRotator(0, 90, 0), true);

	MeshComp->AddWorldRotation(FRotator(90, 0, 0), true);
	switch (RandomRotationNum)
	{
	case 0:
		MeshComp->AddWorldRotation(FRotator(180, 0, 0), true);
		MeshComp->AddWorldRotation(FRotator(0, 0, 90), true);
		break;
	case 1:
		MeshComp->AddWorldRotation(FRotator(-180, 0, 0), true);
		MeshComp->AddWorldRotation(FRotator(0, 180, 0), true);
		break;
	case 2:
		MeshComp->AddWorldRotation(FRotator(0, 0, 180), true);
		MeshComp->AddWorldRotation(FRotator(-90, 0, 0), true);
		break;
	case 3:
		MeshComp->AddWorldRotation(FRotator(0, 180, 0), true);
		break;
	default:
		break;
	}

	return MeshComp->GetComponentRotation();

}
FRotator USpawnObjectComponent::RandomCountMilkRotation(UStaticMeshComponent * MeshComp)
{
	if (!MeshComp)
	{
		return FRotator(0, 0, 0);
	}

	MeshComp->AddWorldRotation(FRotator(0, 90, 0));

	MeshComp->AddWorldRotation(FRotator(90, 0, 0));
	switch (RandomRotationNum)
	{
	case 0:
		MeshComp->AddWorldRotation(FRotator(90, 0, 0));
		break;
	case 1:
		MeshComp->AddWorldRotation(FRotator(-90, 0, 0));
		break;
	case 2:
		MeshComp->AddWorldRotation(FRotator(0, 0, 90));
		break;
	case 3:
		break;
	default:
		break;
	}
	return MeshComp->GetComponentRotation();
}
FRotator USpawnObjectComponent::RandomCountBucketRotation(UStaticMeshComponent * MeshComp)
{
	if (!MeshComp)
	{
		return FRotator(0, 0, 0);
	}

	MeshComp->AddWorldRotation(FRotator(0, FMath::RandRange(-360, 360), 0));

	switch (RandomRotationNum)
	{
	case 0:
		MeshComp->AddWorldRotation(FRotator(0, 90, 0), true);
		break;
	case 1:
		MeshComp->AddWorldRotation(FRotator(0, -90, 0), true);
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}

	return MeshComp->GetComponentRotation();
}
FRotator USpawnObjectComponent::RandomCountOtherRotation(UStaticMeshComponent * MeshComp)
{
	if (!MeshComp)
	{
		return FRotator(0, 0, 0);
	}

	MeshComp->AddWorldRotation(FRotator(FMath::RandRange(-360, 360), FMath::RandRange(-360, 360), FMath::RandRange(-360, 360)));

	return MeshComp->GetComponentRotation();
}

AObjectBase * USpawnObjectComponent::CreateObject(TSubclassOf<AObjectBase> ObjectClass, FStructObjectID ObjectID, AActor *Owner)
{
	AObjectBase * CurrrentObject = nullptr;

	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableComponent")), true);
	TraceParams.bReturnPhysicalMaterial = false;       
	TraceParams.bTraceComplex = true;   
	FHitResult Hit(ForceInit);		//���·������߼���Ƿ��д��Objectģ���赲 ��������������Object
	if (GetWorld()->LineTraceSingleByChannel(Hit, GetComponentLocation(), GetComponentLocation() + RecognitionRayVector, ECollisionChannel::ECC_Camera, TraceParams))
	{

		if (Cast<AObjectBase>(Hit.Actor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Barrier!"));
			return nullptr;
		}

		FActorSpawnParameters parameters;
		parameters.Owner = Owner;
		parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		CreateObjectTransform.SetLocation(GetComponentTransform().GetLocation());	//���õ�ǰ����λ��
		CurrrentObject = GetWorld()->SpawnActorDeferred<AObjectBase>(ObjectClass, CreateObjectTransform,Owner,nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	}
	
	
	
	if (CurrrentObject)	//�������Obj�ɹ����ʼ����ID��Ϣ�͸���λ�ú������תֵ
	{
		CurrrentObject->ObjectIDStruct = ObjectID;
		UGameplayStatics::FinishSpawningActor(CurrrentObject, CreateObjectTransform);	//��ʱ����Obj����ʼ�������


		ChangeObjectLocationUseRay(CurrrentObject, SpawnOffset);
		ChangeObjectRotationUseRandom(CurrrentObject);

		CurrentCreateObjectArray.Add(CurrrentObject);
		return CurrrentObject;
	}


	return nullptr;
}

void USpawnObjectComponent::ClearCurrentObject()
{
	if (CurrentCreateObjectArray.Num()>0)
	{
		for (int index = 0;index < CurrentCreateObjectArray.Num();index++)
		{
			if (CurrentCreateObjectArray[index])
			{
				CurrentCreateObjectArray[index]->Destroy();
			}
		}

		CurrentCreateObjectArray.Empty();
	}
	RandomRotationNum = FMath::FRandRange(0, MaxRandomNum);

}

void USpawnObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


bool USpawnObjectComponent::ChangeObjectLocationUseRay(AObjectBase * object_, FVector offset_)
{
	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableComponent")), true);
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;   
	
	//DrawDebugLine(GetWorld(), GetComponentLocation(), FVector(GetComponentLocation().X, GetComponentLocation().Y, BarrierRayLength), FColor(10, 200, 0), false, 5.0f, 0.0f, 10.0f);

	FHitResult Hit(ForceInit);	//����Ƿ���Obj�赲 
	if (GetWorld()->LineTraceSingleByChannel(Hit, GetComponentLocation(), GetComponentLocation() + RecognitionRayVector, ECollisionChannel::ECC_Camera, TraceParams))
	{
		
		if (!Cast<AObjectBase>(Hit.Actor))
		{
			//��Obj��Mesh�ĳ��Ⱥ��趨��ƫ����������ObjҪ�ı��λ��
			FVector newLocation = FVector(Hit.Location.X, Hit.Location.Y, (object_->nowMinSpanVector.X + Hit.Location.Z)) - offset_;
			
			object_->SetActorLocation(newLocation, true);

			return true;
		}
	}

	return false;
}

bool USpawnObjectComponent::ChangeObjectRotationUseRandom(AObjectBase * object_)
{
	if (!object_)
	{
		return false;
	}

	switch (CurrentCompWillUseObjectRotFunType)	//���ݵ�ǰComp���õ������ת����������Ҫ���õĺ�����Ĭ��ʹ����άȫ�������
	{
	case ObjectType::Bucket:
		RandomCountBucketRotation(object_->ObjectMesh);
		break;
	case ObjectType::Food:
		RandomCountFoodRotation(object_->ObjectMesh);
		break;
	case ObjectType::Water:
		RandomCountWaterRotation(object_->ObjectMesh);
		break;
	case ObjectType::Smoke:
		RandomCountSmokeRotation(object_->ObjectMesh);
		break;
	case ObjectType::Milk:
		RandomCountMilkRotation(object_->ObjectMesh);
		break;
	case ObjectType::Other:
		RandomCountOtherRotation(object_->ObjectMesh);
		break;
	default:
		RandomCountOtherRotation(object_->ObjectMesh);
		break;
	}

	return true;
}

