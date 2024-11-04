// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_RoomList.h"
#include "KGW/KGW_UserRoomName.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "JsonParseLib.h"

void UKGW_RoomList::AddSessionSlotWidget(const TArray<FString>& RoomNames)
{

	TMap<FString, FString> UserData;
	FLogin UserIDData;

	FString TextID = UserIDData.userId;
	UserData.Add("userId", TextID);

	FString json = UJsonParseLib::MakeJson(UserData);


	for (const FString& RoomName : RoomNames)
	{
		// UKGW_UserRoomName ���� ����
		auto* RoomSlot = CreateWidget<UKGW_UserRoomName>(this, UserRoomNameFactory);
		if (RoomSlot)
		{
			// roomName ������ ������ ������Ʈ
// 			Slot->UpdateInfo(RoomName);

			// ScrollBox�� ���� �߰�
			ScrollBox->AddChild(RoomSlot);
		}
	}
	


}

void UKGW_RoomList::SetFindActive(bool value)
{
	//ã�Ⱑ ������ Empty �ؽ�Ʈ �Ⱥ��̰� �ϰ�ʹ�.
	if (value) {
		Text_Finding->SetVisibility(ESlateVisibility::Visible);

	}
	else {
		Text_Finding->SetVisibility(ESlateVisibility::Hidden);

		
	}


}
