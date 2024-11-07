// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_WBP_Question.h"
#include "Components/Button.h"
#include "KGW/KGW_RoomList.h"
#include "JsonParseLib.h"
#include "HttpActor.h"
#include "Kismet/GameplayStatics.h"
#include "CJS/SessionGameInstance.h"
#include "KGW/KGW_RoomlistActor.h"
#include "Components/WidgetComponent.h"
#include "KGW/KGW_EnterRoomActor.h"

void UKGW_WBP_Question::NativeConstruct()
{
	Super::NativeConstruct();

	BT_Select->OnClicked.AddDynamic(this, &UKGW_WBP_Question::OnClickSelect);

}

void UKGW_WBP_Question::OnClickSelect()
{

// 	FString UserId;
// 	USessionGameInstance* GameInstance = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
// 	if (GameInstance)
// 	{
// 		//UserId = GameInstance->MySessionName;
// 		UserId = "testuser";
// 		UE_LOG(LogTemp, Warning, TEXT("Assigned UserId from MySessionName: %s"), *UserId);
// 	}
// 
// 	// 사용자 데이터를 맵에 추가
// 	TMap<FString, FString> MyRoomData;
// 	MyRoomData.Add("userId", UserId);
// 
// 	// JSON 형식으로 변환
// 	FString Json = UJsonParseLib::MakeJson(MyRoomData);
// 
// 	// 로그 출력 (디버깅용)
// 	UE_LOG(LogTemp, Warning, TEXT("userId: %s"), *UserId);
// 	UE_LOG(LogTemp, Warning, TEXT("Json Request: %s"), *Json);
// 	HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
// 	// 서버로 요청 전송
// 	if (HttpActor)
// 	{
// 		HttpActor->ReqPostRoomList(HttpActor->MyRoomURL, Json);
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("HttpActor is null! Make sure it is initialized properly."));
// 	}
// 	TArray<FMyCreatedRoom> StoredRoomInfos;
// 	if (GameInstance)
// 	{
// 		StoredRoomInfos = GameInstance->GettRoomNameNum();
// 
// 		UE_LOG(LogTemp, Error, TEXT("GameInstance->StoredRoomInfos size: %d"), StoredRoomInfos.Num());
// 
// 		// StoredRoomInfos를 사용하여 필요한 작업 수행
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("GameInstance is null! Make sure it is initialized properly."));
// 
// 	}	
 	ListActor = Cast<AKGW_RoomlistActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AKGW_RoomlistActor::StaticClass()));
 	if (ListActor)
 	{
 		FVector NewListLocation(-470990.0f, 643490.0f, 648180.0f);
 		ListActor->SetActorLocation(NewListLocation, true, nullptr, ETeleportType::TeleportPhysics);
 	}	
	EnterRoomActor = Cast<AKGW_EnterRoomActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AKGW_EnterRoomActor::StaticClass()));
	if (EnterRoomActor)
	{
		FVector NewRoomLocation(-470930.0f, 643100.0f, 648150.0f);
		EnterRoomActor->SetActorLocation(NewRoomLocation, true, nullptr, ETeleportType::TeleportPhysics);
	}


// 	 ListActor = Cast<AKGW_RoomlistActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AKGW_RoomlistActor::StaticClass()));
// 	if (ListActor)
// 	{
// 		UWidgetComponent* WidgetComp = ListActor->FindComponentByClass<UWidgetComponent>();
// 		if (WidgetComp)
// 		{
// 			Showlist = Cast<UKGW_RoomList>(WidgetComp->GetUserWidgetObject());
// 			if (!Showlist)
// 			{
// 				UE_LOG(LogTemp, Error, TEXT("Showlist is null! Make sure the widget is correctly set in BP_ListActor."));
// 			}
// 			else
// 			{
// 				Showlist->AddSessionSlotWidget(StoredRoomInfos);
// 				UE_LOG(LogTemp, Error, TEXT("Showlist is good."));
// 
// 			}
// 		}
// 		else
// 		{
// 			UE_LOG(LogTemp, Error, TEXT("WidgetComponent not found on BP_ListActor."));
// 
// 		}
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("BP_ListActor not found in the world."));
// 	}

	
}
