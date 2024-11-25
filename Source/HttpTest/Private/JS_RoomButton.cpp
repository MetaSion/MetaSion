// Fill out your copyright notice in the Description page of Project Settings.

#include "JS_RoomButton.h"
#include "Kismet/GameplayStatics.h"
#include "KGW/KGW_RoomList.h"
#include "JS_RoomController.h"
#include "JS_OnClickRoomUI.h"

void UJS_RoomButton::Initialize()
{
    //클릭함수 바인드
    this->OnClicked.AddDynamic(this, &UJS_RoomButton::OnImageClicked);
    //RoomNum초기값
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
    if (!RL)
    {
        UE_LOG(LogTemp, Error, TEXT("RL is null in OnImageClicked!"));
        return; // RL이 nullptr이면 함수 종료
    }

    /*OnRoomClicked.Broadcast(RoomNum);*/
    if (RL->bRoomList) {
        UGameplayStatics::OpenLevel(GetWorld(), FName("Main_LV_Fall"));
    }
    else if (RL->bMultiRoomList) {
        // 새로운 UI가 뜨고 뜬 UI에서 방이름, 방유사도, 코멘트 넣어서 보여주기
        RL->ShowOnClickRoomUI();
        RL->pc->HideRoomListUI();
        if (RL->OnClickRoomUI) {
            RL->OnClickRoomUI->SettingData(RL->OnClickRoomUI->ImagePath, this);
        }
    }
}


