// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_RoomButton.h"

UJS_RoomButton::UJS_RoomButton()
{
	RoomNum = FString::FromInt(-1);
}

void UJS_RoomButton::SetIndex(FString NewIndex)
{
	RoomNum = NewIndex;
}

FString UJS_RoomButton::GetIndex() const
{
	return RoomNum;
}
