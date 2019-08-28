

#include "CamerPawnBase.h"
#include "Camera/CameraComponent.h"

ACamerPawnBase::ACamerPawnBase()
{
	PrimaryActorTick.bCanEverTick = true;
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
}
void ACamerPawnBase::Visibility__GetRenderedActors(TArray<AActor*>& CurrentlyRenderedActors, float MinRecentTime)
{
	//Empty any previous entries
	CurrentlyRenderedActors.Empty();

	//Iterate Over Actors
	for (TObjectIterator<AActor> Itr; Itr; ++Itr)
	{
		if (Itr->GetLastRenderTime() > MinRecentTime)
		{
			CurrentlyRenderedActors.Add(*Itr);
		}
	}
}
void ACamerPawnBase::Visibility__GetNotRenderedActors(TArray<AActor*>& CurrentlyNotRenderedActors, float MinRecentTime)
{
	//Empty any previous entries
	CurrentlyNotRenderedActors.Empty();

	//Iterate Over Actors
	for (TObjectIterator<AActor> Itr; Itr; ++Itr)
	{
		if (Itr->GetLastRenderTime() <= MinRecentTime)
		{
			CurrentlyNotRenderedActors.Add(*Itr);
		}
	}
}
void ACamerPawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACamerPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACamerPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

