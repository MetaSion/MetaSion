// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_RoomList.h"
#include "KGW/KGW_UserRoomName.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "HttpActor.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "CJS/CJS_InnerWorldSettingWidget.h"

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
//     if (!WheatherNum.IsEmpty())
//     { 
//     
// 		if (WheatherNum == TEXT("1"))
// 		{
// 			UGameplayStatics::OpenLevel(this, FName("Main_LV_Spring"));
// 		}
// 		else if (WheatherNum == TEXT("2"))
// 		{
// 			UGameplayStatics::OpenLevel(this, FName("Main_LV_Summer"));
// 
// 		}
// 		else if (WheatherNum == TEXT("3"))
// 		{
// 			UGameplayStatics::OpenLevel(this, FName("Main_LV_Fall"));
// 
// 		}
// 		else if (WheatherNum == TEXT("4"))
// 		{
//             InnerWorldWidget ->SetWinterSnowSlider();
// 		}
//     }

    UGameplayStatics::OpenLevel(this, FName("Main_LV_Winter"));

}

void UKGW_RoomList::OnClickMultiWorld()
{
    UE_LOG(LogTemp, Warning, TEXT("UKGW_RoomList::OnClickMultiWorld()"));
    StartHttpMultyWorld();
}

//마이월드 -> 멀티월드 버튼 클릭 시 통신
void UKGW_RoomList::StartHttpMultyWorld()
{
    OpenActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
    if (!OpenActor)
    {
        UE_LOG(LogTemp, Error, TEXT("OpenActor is not initialized!"));
        return;
    }

    USessionGameInstance* SessionGI = Cast<USessionGameInstance>(GetGameInstance());
    if (SessionGI)
    {
        FString UserId = SessionGI->MySessionName;
        TMap<FString, FString> MyRoomData;
        MyRoomData.Add("userId", UserId);
        FString JsonRequest = UJsonParseLib::MakeJson(MyRoomData);

        // 서버로 요청 전송
        OpenActor->ReqPostClickMultiWorld(OpenActor->EntryMultiWorldURL, JsonRequest);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SessionGI is NULL"));
    }
    //if (!OpenActor)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("OpenActor is not initialized!"));
    //    return;
    //}

    //USessionGameInstance* SessionGI = Cast<USessionGameInstance>(GetGameInstance());
    //if (SessionGI)
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance is set"));
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Error, TEXT("USessionGameInstance is not set"));
    //}
    //FString UserId;
    //if (SessionGI)
    //{
    //    UserId = SessionGI->UserId;
    //    UserId = SessionGI->MySessionName;
    //    UE_LOG(LogTemp, Warning, TEXT("Assigned UserId from MySessionName: %s"), *UserId);
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("gi Nullptr"));
    //    return;
    //}

    //// 사용자 데이터를 맵에 추가
    //TMap<FString, FString> MyRoomData;
    //MyRoomData.Add("userId", UserId);

    //// JSON 형식으로 변환
    //FString JsonRequest = UJsonParseLib::MakeJson(MyRoomData);

    //// 로그 출력 (디버깅용)
    //UE_LOG(LogTemp, Warning, TEXT("userId: %s"), *UserId);
    //UE_LOG(LogTemp, Warning, TEXT("Json Request: %s"), *JsonRequest);

    //// 서버로 요청 전송
    //OpenActor->ReqPostClickMultiWorld(OpenActor->EntryMultiWorldURL, JsonRequest);
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
