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
#include "JS_RoomButton.h"

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

void UJS_OnClickRoomUI::SettingData(FString TexturePath, UJS_RoomButton* Button)
{
	// 초기 null 체크
	if (!pc || !pc->SessionGI || !Button)
	{
		UE_LOG(LogTemp, Warning, TEXT("Required components are null"));
		return;
	}

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
	SG_ImageButton->OnClicked.RemoveDynamic(this, &UJS_OnClickRoomUI::OnImageClicked);
	SG_ImageButton->OnClicked.AddDynamic(this, &UJS_OnClickRoomUI::OnImageClicked);

	// suggest_list 접근
	const TArray<FMySuggest_List>& SuggestList = pc->SessionGI->WorldSetting.suggest_list;
	if (SuggestList.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Suggest list is empty"));
		return;
	}

	// 버튼 인덱스 처리
	int32 ButtonIndex = FCString::Atoi(*Button->GetIndex());
	if (ButtonIndex < 0 || ButtonIndex >= SuggestList.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid button index: %d"), ButtonIndex);
		return;
	}
	// 데이터 설정
    const FMySuggest_List& Suggestion = SuggestList[ButtonIndex];
    
    // UI 컴포넌트 null 체크 후 설정
    if (SG_RoomName)
        SG_RoomName->SetText(FText::FromString(Suggestion.room_name));
    if (SG_RoomOwner)
        SG_RoomOwner->SetText(FText::FromString(Suggestion.room_id));
    if (SG_Similarity)
        SG_Similarity->SetText(FText::FromString(FString::Printf(TEXT("Similarity: %s%%"), *Suggestion.percent_message)));
    if (SG_ReasonForRecommendation)
        SG_ReasonForRecommendation->SetText(FText::FromString(Suggestion.reason_message));
    if (SG_RoomComment)
        SG_RoomComment->SetText(FText::FromString(Suggestion.reason_message));
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
	
