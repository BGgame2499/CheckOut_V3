// Fill out your copyright notice in the Description page of Project Settings.


#include "SmokeObject.h"

void ASmokeObject::BeginPlay()
{
	LoadNewMeahWithMaterialToObjectMesh("SmokeSKU/SKU/MeshData/","SmokeSKU/SKU/MatData/");
}

void ASmokeObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

