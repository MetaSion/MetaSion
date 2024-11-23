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
	// 텍스처 경로에서 UTexture2D 로드
	UTexture2D* ImageTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *TexturePath));

	// 텍스처가 제대로 로드되었는지 확인
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
	SG_RoomName->SetText(FText::FromString(TEXT("MyRoomName")));
	//RoomOwner Setting
	SG_RoomOwner->SetText(FText::FromString(TEXT("OwnerName")));
	//Similarity Setting
	SG_Similarity->SetText(FText::FromString(TEXT("MySimilarity : 80%")));
	//ReasonForRecommendation Setting
	SG_ReasonForRecommendation->SetText(FText::FromString(TEXT("Why was this world suggested for you? Because you and the owner of this world are closely similar")));
	//RoomComent Setting
	SG_RoomComment->SetText(FText::FromString(TEXT("Oh So Beautiful this world..!!")));
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
	
