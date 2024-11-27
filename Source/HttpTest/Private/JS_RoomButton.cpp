// Fill out your copyright notice in the Description page of Project Settings.

#include "JS_RoomButton.h"
#include "Kismet/GameplayStatics.h"
#include "KGW/KGW_RoomList.h"
#include "JS_RoomController.h"

void UJS_RoomButton::Initialize()
{
    //Ŭ���Լ� ���ε�
    this->OnClicked.AddDynamic(this, &UJS_RoomButton::OnImageClicked);
    //RoomNum�ʱⰪ
    RoomNum = -1;
}

void UJS_RoomButton::SetIndex(int32 NewIndex)
{
	RoomNum = NewIndex;
}

int32 UJS_RoomButton::GetIndex() const
{
	return RoomNum;
}

void UJS_RoomButton::OnImageClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("UJS_RoomButton::OnImageClicked()"));

    if (!RL)
    {
        UE_LOG(LogTemp, Error, TEXT("RL is null in OnImageClicked!"));
        return; // RL�� nullptr�̸� �Լ� ����
    }
    
    // 마지막 누른 방 번호 기억
    LastBtnIdx = GetIndex();
    UE_LOG(LogTemp, Error, TEXT("UJS_RoomButton::OnImageClicked() LastBtnIdx %d"), LastBtnIdx);

    USessionGameInstance* SessionGI = Cast<USessionGameInstance>(GetGameInstance());
    if (SessionGI)
    {
        UE_LOG(LogTemp, Warning, TEXT("UJS_RoomButton::OnImageClicked() Set SessionGI"));
        UE_LOG(LogTemp, Warning, TEXT("UJS_RoomButton::OnImageClicked() Get SessionGI->bMyRoomWatchingOn() %d"), SessionGI->GetbMyRoomWatchingOn());
        if (!SessionGI->GetbMyRoomWatchingOn())
        {
            SessionGI->SetbtMyRoomWatchingOn(true);
            SessionGI->SetLastMyWolrdBtnIdx(LastBtnIdx);
            UE_LOG(LogTemp, Warning, TEXT("UJS_RoomButton::OnImageClicked() After Set SessionGI->bMyRoomWatchingOn() %d"), SessionGI->GetbMyRoomWatchingOn());
            UE_LOG(LogTemp, Warning, TEXT("UJS_RoomButton::OnImageClicked() After Set SessionGI->LastMyWorldBnIdx() %d"), SessionGI->GetLastMyWolrdBtnIdx());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UJS_RoomButton::OnImageClicked() SessionGI->bMyRoomWatchingOn() true right now %d"), SessionGI->GetbMyRoomWatchingOn());
        }
    }

    /*OnRoomClicked.Broadcast(RoomNum);*/
    if (RL->bRoomList) {
        RL->bIsListMyRooms = false;
        SessionGI->StopMusic();
        UGameplayStatics::OpenLevel(GetWorld(), FName("Main_LV_Fall"));
    }
    else if (RL->bMultiRoomList) {
        RL->bIsListAllRooms = false;
        //OnClickShowUI 했던부분
        if (RL) {
			RL->SettingData(RL->SettingImagePath(), this);
        }
    }
}


