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
#include "CJS/CJS_InnerWorldParticleActor.h"

void UKGW_WBP_Question::NativeConstruct()
{
	Super::NativeConstruct();

	BT_Select->OnClicked.AddDynamic(this, &UKGW_WBP_Question::OnClickSelect);
}

void UKGW_WBP_Question::OnClickSelect()
{
	UE_LOG(LogTemp, Warning, TEXT("UKGW_WBP_Question::OnClickSelect()"));
	// QuestionUI Hide
	HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
	if (HttpActor)
	{
	 	HttpActor->HidQuestionUI();
	}

// 	FString UserId;
// 	USessionGameInstance* GameInstance = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
// 	if (GameInstance)
// 	{
// 		//UserId = GameInstance->MySessionName;
// 		UserId = "testuser";
// 		UE_LOG(LogTemp, Warning, TEXT("Assigned UserId from MySessionName: %s"), *UserId);
// 	}
// 
// 	// ����� �����͸� �ʿ� �߰�
// 	TMap<FString, FString> MyRoomData;
// 	MyRoomData.Add("userId", UserId);
// 
// 	// JSON �������� ��ȯ
// 	FString Json = UJsonParseLib::MakeJson(MyRoomData);
// 
// 	// �α� ��� (������)
// 	UE_LOG(LogTemp, Warning, TEXT("userId: %s"), *UserId);
// 	UE_LOG(LogTemp, Warning, TEXT("Json Request: %s"), *Json);
// 	HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
// 	// ������ ��û ����
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
// 		// StoredRoomInfos�� ����Ͽ� �ʿ��� �۾� ����
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("GameInstance is null! Make sure it is initialized properly."));
// 
// 	}	ACJS_InnerWorldParticleActor
 	
	EnterRoomActor = Cast<AKGW_EnterRoomActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AKGW_EnterRoomActor::StaticClass()));
	if (EnterRoomActor)
	{
		FVector NewRoomLocation(-470930.0f, 643100.0f, 648150.0f);
		EnterRoomActor->SetActorLocation(NewRoomLocation, true, nullptr, ETeleportType::TeleportPhysics);
	}	
	EffectActor = Cast<ACJS_InnerWorldParticleActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACJS_InnerWorldParticleActor::StaticClass()));
	if (EffectActor)
	{
		FVector NewRoomLocation(-471220.0f, 644140.0f, 648530.0f);
		EffectActor->SetActorLocation(NewRoomLocation, true, nullptr, ETeleportType::TeleportPhysics);
	}
}

void UKGW_WBP_Question::PlayLateAppearAnimation()
{
	UE_LOG(LogTemp, Warning, TEXT("UKGW_WBP_Question::PlayLateAppearAnimation()"));
	if (LateAppearAnim)
	{	
		PlayAnimation(LateAppearAnim);
		UE_LOG(LogTemp, Warning, TEXT("UKGW_WBP_Question::PlayLateAppearAnimation() PlayAnimation(LateAppearAnim);"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UKGW_WBP_Question::PlayLateAppearAnimation() Cannt Play Anim"));
	}
}
