// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_RoomWidget.h"
#include "Components/Button.h"
#include "JsonParseLib.h"
#include "Components/TextBlock.h"
#include "JS_RoomController.h"
#include "HttpActor.h"
#include "Kismet/GameplayStatics.h"

void UJS_RoomWidget::NativeConstruct()

{
    Super::NativeConstruct();
   
   httpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
}
//index 요청
void UJS_RoomWidget::SendChangeIndexData()
{
    if (!httpActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("httpActor is null in SendChangeIndexData"));
        return;
    }

    FChangeIndex ChangeIndexData;
    //지금은 서버가 없어서 임시로 임의의 값을 넣음.
    ChangeIndexData.room_num = "3";

    FString json = UJsonParseLib::ChangeIndex_Convert_StructToJson(ChangeIndexData);
    UE_LOG(LogTemp, Warning, TEXT("JSON Request Sent: %s"), *json);

    httpActor->ChangeIndexReqPost(httpActor->WallPaperURL, json);
}

void UJS_RoomWidget::SendHeartCount()
{
    if (!httpActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("httpActor is null in SendChangeIndexData"));
        return;
    }

    FRoomData RoomData;
    RoomData.LikeNum += FString::FromInt(1);
    FString json = UJsonParseLib::RoomData_Convert_StructToJson(RoomData);
    UE_LOG(LogTemp, Warning, TEXT("JSON Request Sent: %s"), *json);

    httpActor->ChangeIndexReqPost(httpActor->HeartURL, json);
}

void UJS_RoomWidget::SetIndex(FString roomNumber, int absWallPaperIndex)
{
    // roomNumber는 이미 FString이므로 바로 설정합니다.
    txt_index->SetText(FText::FromString(roomNumber));
    // absWallPaperIndex를 FString으로 변환한 후 설정합니다.
    txt_absindex->SetText(FText::FromString(FString::FromInt(absWallPaperIndex)));
}

void UJS_RoomWidget::SetHeartCount(FString HeartCount)
{
    txt_HeartNum->SetText(FText::FromString(HeartCount));
}
