// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_RoomList.h"
#include "KGW/KGW_UserRoomName.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "HttpActor.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UKGW_RoomList::NativeConstruct()
{
    Super::NativeConstruct();
       
    Btn_InnerWorld->OnClicked.AddDynamic(this, &UKGW_RoomList::OnClickInnerWorld);
    Btn_MultiWorld->OnClicked.AddDynamic(this, &UKGW_RoomList::OnClickMultiWorld);      
}

void UKGW_RoomList::AddSessionSlotWidget(const TArray<FMyWorldRoomInfo>& RoomInfos)
{
    UE_LOG(LogTemp, Warning, TEXT("UKGW_RoomList::AddSessionSlotWidget()"));
    if (!ScrollBox)
    {
        UE_LOG(LogTemp, Error, TEXT("ScrollBox is null! Make sure it is set correctly in the widget."));
        return;
    }
//     ScrollBox->ClearChildren();
    for (const FMyWorldRoomInfo& Room : RoomInfos)
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

void UKGW_RoomList::SetTextLog(FString explain)
{
    TxtBox_Report->SetText(FText::FromString(explain));
    
}
void UKGW_RoomList::SetWheaterNumb(FString TempNUmb)
{
    WheatherNum = TempNUmb;
}


void UKGW_RoomList::OnClickInnerWorld()
{   
    UE_LOG(LogTemp, Warning, TEXT("UKGW_RoomList::OnClickInnerWorld()"));
    if (!WheatherNum.IsEmpty())
    { 
    
		if (WheatherNum == TEXT("1"))
		{
			UGameplayStatics::OpenLevel(this, FName("Main_LV_Spring"));
		}
		else if (WheatherNum == TEXT("2"))
		{
			UGameplayStatics::OpenLevel(this, FName("Main_LV_Summer"));

		}
		else if (WheatherNum == TEXT("3"))
		{
			UGameplayStatics::OpenLevel(this, FName("Main_LV_Fall"));

		}
		else if (WheatherNum == TEXT("4"))
		{
			UGameplayStatics::OpenLevel(this, FName("Main_LV_Winter"));

		}
    }
}

void UKGW_RoomList::OnClickMultiWorld()
{
    UE_LOG(LogTemp, Warning, TEXT("UKGW_RoomList::OnClickMultiWorld()"));
    OpenActor->StartHttpMultyWorld();
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
