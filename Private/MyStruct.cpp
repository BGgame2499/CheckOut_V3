// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStruct.h"

FStructObjectID::FStructObjectID()
{
	UID = 0;
	CID = 0;
	MID = 0;
	MaskID = 0;
	Type = ObjectType::Other;
}
FStructSpecialObjectRayRecongnition::FStructSpecialObjectRayRecongnition()
{
	FX = true;
	BX = true;
	FY = true;
	BY = true;
	FZ = true;
	BZ = true;
	ChangeCID = 0;
}