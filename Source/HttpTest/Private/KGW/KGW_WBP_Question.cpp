// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_WBP_Question.h"
#include "Components/Button.h"
#include "KGW/KGW_RoomList.h"
#include "JsonParseLib.h"
#include "HttpActor.h"
#include "Kismet/GameplayStatics.h"
#include "CJS/SessionGameInstance.h"

void UKGW_WBP_Question::NativeConstruct()
{
	Super::NativeConstruct();

	BT_Select->OnClicked.AddDynamic(this, &UKGW_WBP_Question::OnClickSelect);

}

void UKGW_WBP_Question::OnClickSelect()
{

	FString UserId;
	USessionGameInstance* GameInstance = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		//UserId = GameInstance->MySessionName;
		UserId = "testuser";
		UE_LOG(LogTemp, Warning, TEXT("Assigned UserId from MySessionName: %s"), *UserId);
	}

	// ����� �����͸� �ʿ� �߰�
	TMap<FString, FString> MyRoomData;
	MyRoomData.Add("userId", UserId);

	// JSON �������� ��ȯ
	FString JsonRequest = UJsonParseLib::MakeJson(MyRoomData);

	// �α� ��� (������)
	UE_LOG(LogTemp, Warning, TEXT("userId: %s"), *UserId);
	UE_LOG(LogTemp, Warning, TEXT("Json Request: %s"), *JsonRequest);

	// ������ ��û ����
	if (HttpActor)
	{
		HttpActor->ReqPostRoomList(HttpActor->MyRoomURL, JsonRequest);
	}



	

// 	Showlist->AddSessionSlotWidget();
	
}
