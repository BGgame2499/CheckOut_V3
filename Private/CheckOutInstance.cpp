// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckOutInstance.h"
#include "Engine.h"
#include "Saves/CheckOutSave.h"

void UCheckOutInstance::Init()
{
	SaveGames = Cast<UCheckOutSave>(UGameplayStatics::LoadGameFromSlot("CheckOut", 0));	//加载存档 如果加载失败则创建新的SaveGame
	if (!SaveGames)
	{
		SaveGames = Cast<UCheckOutSave>(UGameplayStatics::CreateSaveGameObject(UCheckOutSave::StaticClass()));
	}

}
bool UCheckOutInstance::SaveCheckOut()
{
	if (SaveGames)
	{
		return UGameplayStatics::SaveGameToSlot(SaveGames, "CheckOut", 0);
	}
	return false;
}