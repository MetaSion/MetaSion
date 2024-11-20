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
//WallPaper -> RoomData -> RoomNum ��û
void UJS_RoomWidget::SendWallPaperData()
{
    if (!httpActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("httpActor is null in SendWallPaperData"));
        return;
    }

    FWallPaperData WallPaperData;
    WallPaperData.RoomData.RoomNum = "3";
   /* FString json = UJsonParseLib::WallPaperData_Convert_StructToJson(WallPaperData);*/
    // JSON ���ڿ��� �������� ����
	FString json = FString::Printf(TEXT(
		"{"
		"\"RoomNum\": \"%s\""
		"}"),
		*WallPaperData.RoomData.RoomNum
	);

    // ������ JSON�� �α׷� ���
    UE_LOG(LogTemp, Warning, TEXT("Generated JSON: %s"), *json);

    // ���� HTTP ��û ��� json ��� ����
    if (httpActor)
    {
        httpActor->WallPaperReqPost(httpActor->WallPaperURL, json);
    }
    /*FString json = UJsonParseLib::WallPaperData_Convert_StructToJson(WallPaperData);
    UE_LOG(LogTemp, Warning, TEXT("JSON Request Sent: %s"), *json);

    httpActor->WallPaperReqPost(httpActor->WallPaperURL, json);*/
}

//void UJS_RoomWidget::SendHeartCount()
//{
//    if (!httpActor)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("httpActor is null in SendWallPaperData"));
//        return;
//    }
//
//    FRoomData RoomData;
//    RoomData.LikeNum += FString::FromInt(1);
//    FString json = UJsonParseLib::RoomData_Convert_StructToJson(RoomData);
//    UE_LOG(LogTemp, Warning, TEXT("JSON Request Sent: %s"), *json);
//
//    httpActor->WallPaperReqPost(httpActor->HeartURL, json);
//}

void UJS_RoomWidget::SetIndex(FString roomNumber, int absWallPaperIndex)
{
    // roomNumber�� �̹� FString�̹Ƿ� �ٷ� �����մϴ�.
    txt_index->SetText(FText::FromString(roomNumber));
    // absWallPaperIndex�� FString���� ��ȯ�� �� �����մϴ�.
    txt_absindex->SetText(FText::FromString(FString::FromInt(absWallPaperIndex)));
}

//void UJS_RoomWidget::SetHeartCount(FString HeartCount)
//{
//    txt_HeartNum->SetText(FText::FromString(HeartCount));
//}
