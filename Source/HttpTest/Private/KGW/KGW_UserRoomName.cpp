// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_UserRoomName.h"
#include "Components/TextBlock.h"
#include "JsonParseLib.h"
#include "CJS/SessionGameInstance.h"

void UKGW_UserRoomName::UpdateInfo(const FMyWorldRoomInfo& info)
{

    if (Text_RoomName)
    {
        // RoomName¸¸ Ç¥½Ã
        Text_RoomName->SetText(FText::FromString(info.MyRoomName));
        UE_LOG(LogTemp, Log, TEXT("RoomName set to: %s"), *info.MyRoomName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Text_RoomName is null! Make sure it is set in the widget blueprint."));
    }
}
