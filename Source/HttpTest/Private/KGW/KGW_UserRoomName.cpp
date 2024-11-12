// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_UserRoomName.h"
#include "CJS/SessionGameInstance.h"
#include "HttpActor.h"
#include "JsonParseLib.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"



void UKGW_UserRoomName::NativeConstruct()
{
    Super::NativeConstruct();

    UE_LOG(LogTemp, Warning, TEXT("UKGW_UserRoomName::NativeConstruct()"));
    Btn_EnterMyRoom->OnClicked.AddDynamic(this, &UKGW_UserRoomName::OnClickEnterMyRoom);
}

void UKGW_UserRoomName::UpdateInfo(const FMyWorldRoomInfo& info)
{
    UE_LOG(LogTemp, Warning, TEXT("UKGW_UserRoomName::UpdateInfo()"));
    if (Text_RoomName)
    {
        // RoomName�� ǥ��
        Text_RoomName->SetText(FText::FromString(info.MyRoomName));
        UE_LOG(LogTemp, Log, TEXT("RoomName set to: %s"), *info.MyRoomName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Text_RoomName is null! Make sure it is set in the widget blueprint."));
    }

    // 방 번호를 RoomNum에 저장
    RoomNum = FString::FromInt(info.MyRoomNum);
}

void UKGW_UserRoomName::OnClickEnterMyRoom()
{
    UE_LOG(LogTemp, Warning, TEXT("UKGW_UserRoomName::OnClickEnterMyRoom()"));

    HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
    if (HttpActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("UKGW_UserRoomName::OnClickEnterMyRoom() Set HttpActor"));
        USessionGameInstance* SessionGI = Cast<USessionGameInstance>(GetGameInstance());
        if (SessionGI)
        {
            UE_LOG(LogTemp, Warning, TEXT("UKGW_UserRoomName::OnClickEnterMyRoom() Set SessionGI"));

            // RoomNum을 인자로 사용하여 CallHttpClickMyRoomList 호출
            HttpActor->CallHttpClickMyRoomList(RoomNum);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UKGW_UserRoomName::OnClickEnterMyRoom() No SessionGI"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UKGW_UserRoomName::OnClickEnterMyRoom() No HttpActor"));
    }
    
}
