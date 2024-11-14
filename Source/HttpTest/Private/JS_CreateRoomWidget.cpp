// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_CreateRoomWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"
#include "JS_RoomController.h"
#include "Kismet/GameplayStatics.h"
#include "HttpActor.h"
#include "JsonParseLib.h"
#include "JS_WidgetFunction.h"
#include "Components/MultiLineEditableText.h"
#include "Components/MultiLineEditableTextBox.h"

void UJS_CreateRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//Choose Yes or No
	btn_CreateRoom_Yes->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::CreateRoomChooseYes);
	btn_CreateRoom_No->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::CreateRoomChooseNo);
	//Toggle Private
	btn_CreateRoom_Private->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::SetPrivate);
	btn_CreateRoom_Public->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::SetPrivate);
	//Complete CR
	btn_CompleteCreateRoom->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::CompleteCreateRoom);
	// RoomName
	ED_RoomName->OnTextChanged.AddDynamic(this, &UJS_CreateRoomWidget::OnTextChanged_SingleLine);
	// Description
	ED_MultiText->OnTextChanged.AddDynamic(this, &UJS_CreateRoomWidget::OnTextChanged_MultiLine);
	ED_MultiText->OnTextCommitted.AddDynamic(this, &UJS_CreateRoomWidget::OnTextCommitted_MultiLine);

	Btn_CaptureImage->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::OnClickCaptureImage);
	Btn_MyPage->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::OnClikMypage);
	Btn_Explanation->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::OnClikExplanation);


	pc = Cast<AJS_RoomController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	httpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
	widgetActor = Cast<AJS_WidgetFunction>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_WidgetFunction::StaticClass()));

	if (ED_MultiText)
	{
		ED_MultiText->SetAutoWrapText(true);
		ED_MultiText->SetWrapTextAt(550.0f); // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï¼ï¿½ï¿½ï¿½
		LastValidText = TEXT("");
	}
}
void UJS_CreateRoomWidget::OnClickCaptureImage()
{
	pc->OnClickButtonImage();
	UE_LOG(LogTemp, Error, TEXT("captured"));

}
void UJS_CreateRoomWidget::OnClikMypage()
{
	SendCompleteRoomData();
	UGameplayStatics::OpenLevel(this, FName("Main_Sky"));
}
void UJS_CreateRoomWidget::OnClikExplanation()
{
	SetExplanation(CurrentText);
}
void UJS_CreateRoomWidget::SetExplanation(const FString& Text)
{ 
	
	Txt_Explane->SetText(FText::FromString(Text));
		
	
}
//widget Switch
void UJS_CreateRoomWidget::SwitchToWidget(int32 index)
{
	if (CR_WidgetSwitcher) {
		CR_WidgetSwitcher->SetActiveWidgetIndex(index);
	}
}
//private, public
void UJS_CreateRoomWidget::SwitchToWidget_PP(int32 index)
{
	if (CR_WidgetSwitcherPP) {
		CR_WidgetSwitcherPP->SetActiveWidgetIndex(index);
	}
}
void UJS_CreateRoomWidget::CreateRoomChooseYes()
{
	SwitchToWidget(1);
}

void UJS_CreateRoomWidget::CreateRoomChooseNo()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UJS_CreateRoomWidget::CompleteCreateRoom()
{
	if (ED_RoomName && !ED_RoomName->GetText().IsEmpty()) {
		SwitchToWidget(2);
// 		ShowUIForLimitedTime(1.5);
// 		if (widgetActor) {
// 			widgetActor->SetActorVisibilityVisible();
// 		}
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UJS_CreateRoomWidget::DelayedSwitchToWidget, 0.7f, false);
// 		SwitchToWidget(3);
	}
}
void UJS_CreateRoomWidget::DelayedSwitchToWidget()
{
	SwitchToWidget(3);
	PlayAnimation(appear);

}
void UJS_CreateRoomWidget::SetPrivate()
{
	bPrivate = 1 - bPrivate; // 0 -> 1 -> 0 
	SwitchToWidget_PP(bPrivate);
	/*SendSetPrivateRoom(bPrivate);*/
}

void UJS_CreateRoomWidget::ShowUIForLimitedTime(float DurationInSeconds)
{
	//Timer
	SetVisibility(ESlateVisibility::Visible);
	if (GetWorld()){
		GetWorld()->GetTimerManager().SetTimer(Handler, this, &UJS_CreateRoomWidget::HideUI, DurationInSeconds, false);
	}
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GetWorld() returned NULL!"));
    }
}

void UJS_CreateRoomWidget::HideUI()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	if (pc) {
		pc->bShowUI = true;
	}
}

void UJS_CreateRoomWidget::SendCompleteRoomData()
{
	if (!httpActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("httpActor is null in SendSetPrivateRoom"));
		return;
	}
	/*FRoomSendData RoomSendData;

	RoomSendData.UserData.UserId = "testuser";
	RoomSendData.EnvironmentData.UltraSky_TimeOfDay = "1200";
	RoomSendData.EnvironmentData.UltraWeather_CloudCoverage = "1";
	RoomSendData.EnvironmentData.UltraWeather_Fog = "1";
	RoomSendData.EnvironmentData.UltraWeather_Rain = "1";
	RoomSendData.EnvironmentData.UltraWeather_Snow = "1";
	RoomSendData.EnvironmentData.UltraWeather_Dust = "1";
	RoomSendData.EnvironmentData.UltraWeather_Thunder = "1";
	RoomSendData.EnvironmentData.MainObject = "1";
	RoomSendData.EnvironmentData.SubObject = "1";
	RoomSendData.EnvironmentData.Background = "1";
	RoomSendData.EnvironmentData.WeatherParticle = "particle_maple";
	RoomSendData.RoomInfoData.RoomName = ED_RoomName->GetText().ToString();
	RoomSendData.RoomInfoData.RoomDescription = ED_MultiText->GetText().ToString();*/

	// ÀÔ·ÂµÈ °ªµéÀ» ¼öÁý
	FString UserId = "testuser";
	FString UltraSky_TimeOfDay = "1200";
	FString UltraWeather_CloudCoverage = "1";
	FString UltraWeather_Fog = "1";
	FString UltraWeather_Rain = "1";
	FString UltraWeather_Snow = "1";
	FString UltraWeather_Dust = "1";
	FString UltraWeather_Thunder = "1";
	FString MainObject = "1";
	FString SubObject = "1";
	FString Background = "1";
	FString WeatherParticle = "particle_maple";
	FString RoomName = ED_RoomName->GetText().ToString();
	RoomDescription = ED_MultiText->GetText().ToString();

	// Ãß°¡µÈ °ª: bPrivate -> RoomPP º¯È¯
	FString RoomPP = FString::FromInt(bPrivate);

	// Ãß°¡µÈ °ª: Quadrant (ÃßÁ¤µÈ °ª, Á÷Á¢ ¼³Á¤ÇØ ÁÖ¼¼¿ä)
	FString Quadrant = "1";  // ¿¹½Ã °ª, ½ÇÁ¦ °ªÀ» ³Ö¾îÁÖ¼¼¿ä.

	// ¼öµ¿À¸·Î JSON ¹®ÀÚ¿­À» »ý¼º
	FString json = FString::Printf(TEXT(
		"{"
		"\"UserId\": \"%s\","
		"\"RoomName\": \"%s\","
		"\"UltraSky_TimeOfDay\": \"%s\","
		"\"UltraWeather_CloudCoverage\": \"%s\","  // ¿ÀÅ¸ ¼öÁ¤
		"\"UltraWeather_Fog\": \"%s\","
		"\"UltraWeather_Rain\": \"%s\","
		"\"UltraWeather_Snow\": \"%s\","
		"\"UltraWeather_Dust\": \"%s\","
		"\"UltraWeather_Thunder\": \"%s\","
		"\"MainObject\": \"%s\","
		"\"SubObject\": \"%s\","
		"\"Background\": \"%s\","
		"\"Particle_num\": \"%s\","
		"\"RoomDescription\": \"%s\","
		"\"RoomPP\": \"%s\","
		"\"Quadrant\": \"%s\""
		"}"),
		*UserId,
		*RoomName,
		*UltraSky_TimeOfDay,
		*UltraWeather_CloudCoverage,
		*UltraWeather_Fog,
		*UltraWeather_Rain,
		*UltraWeather_Snow,
		*UltraWeather_Dust,
		*UltraWeather_Thunder,
		*MainObject,
		*SubObject,
		*Background,
		*WeatherParticle,
		*RoomDescription,
		*RoomPP,
		*Quadrant
	);

	if (httpActor) {
		httpActor->RoomSendDataReqPost(httpActor->SaveRoomData, json);

	}
}

void UJS_CreateRoomWidget::OnTextChanged_SingleLine(const FText& Text)
{
	 CurrentText = Text.ToString();
	if (CurrentText.Len() > textSize)
	{
		// 30ï¿½ï¿½ ï¿½Ê°ï¿½ ï¿½ï¿½ ï¿½ß¶ó³»±ï¿½
		FString TrimmedText = CurrentText.Left(textSize/10);
		ED_RoomName->SetText(FText::FromString(TrimmedText));
	}
}



void UJS_CreateRoomWidget::OnTextChanged_MultiLine(const FText& Text)
{
	CurrentText = Text.ToString();
	int32 CharacterCount = 0;

	// ï¿½Ø½ï¿½Æ®ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ì¸ï¿½ ï¿½ï¿½ï¿?
	for (const TCHAR& Char : CurrentText)
	{
		CharacterCount += (Char <= 0x007F) ? 1 : 3; // ÇÑ±ÛÀº 3·Î °è»ê
		//UE_LOG(LogTemp, Warning, TEXT("%c"), Char);

		CharacterCount += (Char <= 0x007F) ? 1 : 3; // ï¿½Ñ±ï¿½ï¿½ï¿½ 3ï¿½ï¿½ ï¿½ï¿½ï¿?
		UE_LOG(LogTemp, Warning, TEXT("%c"), Char);
	}

	// ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ñ¾ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½ï¿?ï¿½Þ½ï¿½ï¿½ï¿½ Ç¥ï¿½ï¿½
	if (CharacterCount > MAX_CHARACTER_COUNT)
	{
		// ï¿½Î±ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿?
		UE_LOG(LogTemp, Warning, TEXT("ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ê°ï¿½ï¿½ß½ï¿½ï¿½Ï´ï¿½!"));
	}
	else
	{
		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ì¿¡ï¿½ï¿?ï¿½ï¿½È¿ï¿½ï¿½ ï¿½Ø½ï¿½Æ®ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
		LastValidText = CurrentText;
	}
}

void UJS_CreateRoomWidget::OnTextCommitted_MultiLine(const FText& Text, ETextCommit::Type CommitMethod)
{
	 CurrentText = Text.ToString();
	int32 CharacterCount = 0;

	for (const TCHAR& Char : CurrentText)
	{
		CharacterCount += (Char <= 0x007F) ? 1 : 3;
	/*	UE_LOG(LogTemp, Warning, TEXT("%c"), Char);*/
	}

	if (CharacterCount > MAX_CHARACTER_COUNT)
	{
		ED_MultiText->SetText(FText::FromString(LastValidText));
	}
	else
	{
		LastValidText = CurrentText;
	}
}

//void UJS_CreateRoomWidget::ApplyTextLimit(const FText& Text)
//{
//	FString CurrentText = Text.ToString();
//
//	// ï¿½ï¿½ï¿½Ú¿ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ì¸ï¿½ ï¿½ï¿½ï¿?(UTF-16 ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½)
//	int32 CharacterCount = 0;
//	for (const TCHAR& Char : CurrentText)
//	{
//		// ASCII ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ú´ï¿½ 1ï¿½ï¿½, ï¿½ï¿½ ï¿½ï¿½(ï¿½Ñ±ï¿½ ï¿½ï¿½)ï¿½ï¿½ 2ï¿½ï¿½ Ä«ï¿½ï¿½Æ®
//		CharacterCount += (Char <= 0x007F) ? 1 : 2;
//	}
//
//	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ê°ï¿½ï¿½Ï¸ï¿½ ï¿½ß¶ó³»±ï¿½
//	if (CharacterCount > 34) // 17ï¿½ï¿½ï¿½ï¿½ * 2 = 34 (ï¿½Ñ±ï¿½ ï¿½ï¿½ï¿½ï¿½)
//	{
//		FString TrimmedText;
//		int32 CurrentCount = 0;
//
//		for (int32 i = 0; i < CurrentText.Len(); ++i)
//		{
//			TCHAR CurrentChar = CurrentText[i];
//			int32 CharSize = (CurrentChar <= 0x007F) ? 1 : 2;
//
//			if (CurrentCount + CharSize > 34)
//				break;
//
//			TrimmedText.AppendChar(CurrentChar);
//			CurrentCount += CharSize;
//		}
//
//		ED_MultiText->SetText(FText::FromString(TrimmedText));
//	}
//}
