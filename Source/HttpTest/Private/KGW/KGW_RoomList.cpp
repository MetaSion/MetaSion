// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_RoomList.h"
#include "KGW/KGW_UserRoomName.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "HttpActor.h"

void UKGW_RoomList::AddSessionSlotWidget(const TArray<FMyCreatedRoom>& RoomInfos)
{

    if (!ScrollBox)
    {
        UE_LOG(LogTemp, Error, TEXT("ScrollBox is null! Make sure it is set correctly in the widget."));
        return;
    }
//     ScrollBox->ClearChildren();
    for (const FMyCreatedRoom& Room : RoomInfos)
    {
        auto* RoomSlot = CreateWidget<UKGW_UserRoomName>(this, UserRoomNameFactory);
        if (RoomSlot)
        {
            // RoomWidget�� ������ ������Ʈ (��: �� �̸� ����)
            RoomSlot->UpdateInfo(Room);
            ScrollBox->AddChild(RoomSlot);
        }
    }  
    UE_LOG(LogTemp, Log, TEXT("Room list updated with %d rooms."), RoomInfos.Num());

}

// void UKGW_RoomList::SetFindActive(bool value)
// {
// 	//ã�Ⱑ ������ Empty �ؽ�Ʈ �Ⱥ��̰� �ϰ�ʹ�.
// 	if (value) {
// 		Text_Finding->SetVisibility(ESlateVisibility::Visible);
// 
// 	}
// 	else {
// 		Text_Finding->SetVisibility(ESlateVisibility::Hidden);
// 
// 		
// 	}
// 
// 
// }
