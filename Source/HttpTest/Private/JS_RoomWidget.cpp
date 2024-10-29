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
    httpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
}
//index ��û
void UJS_RoomWidget::SendChangeIndexData()
{
    if (!httpActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("httpActor is null in SendChangeIndexData"));
        return;
    }

    FChangeIndex ChangeIndexData;
    //������ ������ ��� �ӽ÷� ������ ���� ����.

    FString json = UJsonParseLib::ChangeIndex_Convert_StructToJson(ChangeIndexData);
    

    httpActor->ChangeIndexReqPost(httpActor->ServerURL, json);
}

void UJS_RoomWidget::SetIndex(FString roomNumber, int absWallPaperIndex)
{
    // roomNumber�� �̹� FString�̹Ƿ� �ٷ� �����մϴ�.
    txt_index->SetText(FText::FromString(roomNumber));
    // absWallPaperIndex�� FString���� ��ȯ�� �� �����մϴ�.
    txt_absindex->SetText(FText::FromString(FString::FromInt(absWallPaperIndex)));
}
