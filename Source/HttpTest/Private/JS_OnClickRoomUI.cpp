// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_OnClickRoomUI.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "JS_RoomController.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ButtonSlot.h"
#include "CJS/SessionGameInstance.h"

void UJS_OnClickRoomUI::NativeConstruct()
{
	Super::NativeConstruct();

	SettingImagePath();

	pc = Cast<AJS_RoomController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!pc) {
		UE_LOG(LogTemp, Error, TEXT("PlayerController not found in the level!"));
		return;
	}
}

void UJS_OnClickRoomUI::SettingImagePath()
{
	ImagePath = TEXT("/Game/Main/Assets/UI/sunny");
	if (!(ImagePath.IsEmpty())) {
		UE_LOG(LogTemp, Warning, TEXT("SettingPath : %s "), *ImagePath);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("JS_OnClickRoomUI SettingPath Fail.. "));
	}
}

void UJS_OnClickRoomUI::SettingData(FString TexturePath)
{
	UE_LOG(LogTemp, Warning, TEXT("UJS_OnClickRoomUI::SettingData()"));
	// �ؽ�ó ��ο��� UTexture2D �ε�
	UTexture2D* ImageTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *TexturePath));

	// �ؽ�ó�� ����� �ε�Ǿ����� Ȯ��
	if (!ImageTexture)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load texture from path: %s"), *TexturePath);
		return;
	}
	if (!SG_ImageButton) {
		UE_LOG(LogTemp, Error, TEXT("Fail to ImageButton"));
		return;
	}

	//Image Setting
	SG_RoomImage->SetBrushFromTexture(ImageTexture, true);
	
	SG_ImageButton->OnClicked.AddDynamic(this, &UJS_OnClickRoomUI::OnImageClicked);

	//RoomName Setting
	//SG_RoomName->SetText(FText::FromString(TEXT("MyRoomName")));
	////RoomOwner Setting
	//SG_RoomOwner->SetText(FText::FromString(TEXT("OwnerName")));
	////Similarity Setting
	//SG_Similarity->SetText(FText::FromString(TEXT("MySimilarity : 80%")));
	////ReasonForRecommendation Setting
	//SG_ReasonForRecommendation->SetText(FText::FromString(TEXT("Why was this world suggested for you? Because you and the owner of this world are closely similar")));
	////RoomComent Setting
	//SG_RoomComment->SetText(FText::FromString(TEXT("Oh So Beautiful this world..!!")));

	// RoomName Setting   
	if (pc && pc->SessionGI)
	{
		// SessionGI에서 suggest_list 가져오기
		const TArray<FMySuggest_List>& SuggestList = pc->SessionGI->WorldSetting.suggest_list;

		// suggest_list가 비어 있는지 확인
		if (SuggestList.Num() > 0)
		{
			// suggest_list 크기 출력
			UE_LOG(LogTemp, Warning, TEXT("UJS_OnClickRoomUI::SettingData() Suggest list contains %d items."), SuggestList.Num());
			// 모든 항목 출력
			for (int32 Index = 0; Index < SuggestList.Num(); ++Index)
			{
				const FMySuggest_List& Suggest = SuggestList[Index];
				UE_LOG(LogTemp, Warning, TEXT("Index %d:"), Index);
				UE_LOG(LogTemp, Warning, TEXT("  Room Name: %s"), *Suggest.room_name);
				UE_LOG(LogTemp, Warning, TEXT("  Room ID: %s"), *Suggest.room_id);
				UE_LOG(LogTemp, Warning, TEXT("  Room Num: %s"), *Suggest.room_num);
				UE_LOG(LogTemp, Warning, TEXT("  Percent Message: %s"), *Suggest.percent_message);
				UE_LOG(LogTemp, Warning, TEXT("  Reason Message: %s"), *Suggest.reason_message);
				UE_LOG(LogTemp, Log, TEXT("  Room Description: %s"), *Suggest.roomdescription);   // <-- 솔아 이거 추가해~~
			}
			
			// 첫 번째 추천방 정보 가져오기
			const FMySuggest_List& FirstSuggestion = SuggestList[0];

			// RoomName Setting
			SG_RoomName->SetText(FText::FromString(FirstSuggestion.room_name));

			// RoomOwner Setting (owner 정보를 구조체에 추가했다면 사용)
			FString OwnerName = FirstSuggestion.room_id; // 예: room_id를 owner로 사용한다고 가정
			SG_RoomOwner->SetText(FText::FromString(OwnerName));

			// Similarity Setting
			FString Similarity = FirstSuggestion.percent_message; // percent_message가 유사도를 나타낸다고 가정
			SG_Similarity->SetText(FText::FromString(FString::Printf(TEXT("Similarity: %s%%"), *Similarity)));

			// ReasonForRecommendation Setting
			SG_ReasonForRecommendation->SetText(FText::FromString(FirstSuggestion.reason_message));

			// RoomComment Setting (임의 코멘트를 사용하거나 구조체에 추가적으로 정의)
			FString Recommand = FirstSuggestion.reason_message;
			SG_RoomComment->SetText(FText::FromString(Recommand));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UJS_OnClickRoomUI::SettingData() Suggest list is empty in SessionGI."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UJS_OnClickRoomUI::SettingData() PlayerController or SessionGI is null."));
	}
}

void UJS_OnClickRoomUI::OnImageClicked()
{
	if (pc) {
		this->SetVisibility(ESlateVisibility::Hidden);
		pc->ShowRoomListUI();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("JS_OnClickRoomUI pc Fail..."));
	}
}
	
