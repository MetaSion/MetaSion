// Fill out your copyright notice in the Description page of Project Settings.

#include "JS_RoomButton.h"
#include "Kismet/GameplayStatics.h"
#include "KGW/KGW_RoomList.h"
#include "JS_RoomController.h"
#include "JS_OnClickRoomUI.h"

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
    if (!RL)
    {
        UE_LOG(LogTemp, Error, TEXT("RL is null in OnImageClicked!"));
        return; // RL�� nullptr�̸� �Լ� ����
    }

    /*OnRoomClicked.Broadcast(RoomNum);*/
    if (RL->bRoomList) {
        UGameplayStatics::OpenLevel(GetWorld(), FName("Main_LV_Fall"));
    }
    else if (RL->bMultiRoomList) {
        // ���ο� UI�� �߰� �� UI���� ���̸�, �����絵, �ڸ�Ʈ �־ �����ֱ�
        RL->ShowOnClickRoomUI();
        RL->pc->HideRoomListUI();
        if (RL->OnClickRoomUI) {
            RL->OnClickRoomUI->SettingData(RL->OnClickRoomUI->ImagePath, this);
        }
    }
}


