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
		ED_MultiText->SetWrapTextAt(550.0f); // ������ ������ �����ϼ���
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

	// �Էµ� ������ ����
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

	// �߰��� ��: bPrivate -> RoomPP ��ȯ
	FString RoomPP = FString::FromInt(bPrivate);

	// �߰��� ��: Quadrant (������ ��, ���� ������ �ּ���)
	FString Quadrant = "1";  // ���� ��, ���� ���� �־��ּ���.

	// �������� JSON ���ڿ��� ����
	FString json = FString::Printf(TEXT(
		"{"
		"\"UserId\": \"%s\","
		"\"RoomName\": \"%s\","
		"\"UltraSky_TimeOfDay\": \"%s\","
		"\"UltraWeather_CloudCoverage\": \"%s\","  // ��Ÿ ����
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
		// 30�� �ʰ� �� �߶󳻱�
		FString TrimmedText = CurrentText.Left(textSize/10);
		ED_RoomName->SetText(FText::FromString(TrimmedText));
	}
}



void UJS_CreateRoomWidget::OnTextChanged_MultiLine(const FText& Text)
{
	CurrentText = Text.ToString();
	int32 CharacterCount = 0;

	// �ؽ�Ʈ�� ���� ���̸� ���?
	for (const TCHAR& Char : CurrentText)
	{
		CharacterCount += (Char <= 0x007F) ? 1 : 3; // �ѱ��� 3�� ���
		//UE_LOG(LogTemp, Warning, TEXT("%c"), Char);

		CharacterCount += (Char <= 0x007F) ? 1 : 3; // �ѱ��� 3�� ���?
		UE_LOG(LogTemp, Warning, TEXT("%c"), Char);
	}

	// ���� �� ������ �Ѿ��� �� ���?�޽��� ǥ��
	if (CharacterCount > MAX_CHARACTER_COUNT)
	{
		// �α� �������?���?
		UE_LOG(LogTemp, Warning, TEXT("���� �� ������ �ʰ��߽��ϴ�!"));
	}
	else
	{
		// ������ ���� ���� ��쿡��?��ȿ�� �ؽ�Ʈ�� ����
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
//	// ���ڿ��� ���� ���̸� ���?(UTF-16 ���� ������)
//	int32 CharacterCount = 0;
//	for (const TCHAR& Char : CurrentText)
//	{
//		// ASCII ������ ���ڴ� 1��, �� ��(�ѱ� ��)�� 2�� ī��Ʈ
//		CharacterCount += (Char <= 0x007F) ? 1 : 2;
//	}
//
//	// ������ ������ �ʰ��ϸ� �߶󳻱�
//	if (CharacterCount > 34) // 17���� * 2 = 34 (�ѱ� ����)
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
