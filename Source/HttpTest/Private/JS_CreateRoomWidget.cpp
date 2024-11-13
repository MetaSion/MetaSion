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

	pc = Cast<AJS_RoomController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	httpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
	widgetActor = Cast<AJS_WidgetFunction>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_WidgetFunction::StaticClass()));

	if (ED_MultiText)
	{
		ED_MultiText->SetAutoWrapText(true);
		ED_MultiText->SetWrapTextAt(550.0f); // 적절한 값으로 조정하세요
		LastValidText = TEXT("");
	}
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
		//이쪽에 보내는 로직 추가
		SwitchToWidget(2);
		ShowUIForLimitedTime(3);
		if (widgetActor) {
			widgetActor->SetActorVisibilityVisible();
		}
		SendCompleteRoomData();
	}
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

	// 입력된 값들을 수집
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
	FString RoomDescription = ED_MultiText->GetText().ToString();

	// 추가된 값: bPrivate -> RoomPP 변환
	FString RoomPP = FString::FromInt(bPrivate);

	// 추가된 값: Quadrant (추정된 값, 직접 설정해 주세요)
	FString Quadrant = "1";  // 예시 값, 실제 값을 넣어주세요.

	// 수동으로 JSON 문자열을 생성
	FString json = FString::Printf(TEXT(
		"{"
		"\"UserId\": \"%s\","
		"\"RoomName\": \"%s\","
		"\"UltraSky_TimeOfDay\": \"%s\","
		"\"UltraWheather_CloudCoverage\": \"%s\","
		"\"UltraWheather_Fog\": \"%s\","
		"\"UltraWheather_Rain\": \"%s\","
		"\"UltraWheather_Snow\": \"%s\","
		"\"UltraWheather_Dust\": \"%s\","
		"\"UltraWheather_Thunder\": \"%s\","
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
		*RoomPP,      // RoomPP 값 추가
		*Quadrant     // Quadrant 값 추가
	);

	//FString json = UJsonParseLib::RoomSendData_Convert_StructToJson(RoomSendData);

	if (httpActor) {
		httpActor->RoomSendDataReqPost(httpActor->SaveRoomData, json);
	}
}

void UJS_CreateRoomWidget::OnTextChanged_SingleLine(const FText& Text)
{
	FString CurrentText = Text.ToString();
	if (CurrentText.Len() > textSize)
	{
		// 30자 초과 시 잘라내기
		FString TrimmedText = CurrentText.Left(textSize/10);
		ED_RoomName->SetText(FText::FromString(TrimmedText));
	}
}



void UJS_CreateRoomWidget::OnTextChanged_MultiLine(const FText& Text)
{
	FString CurrentText = Text.ToString();
	int32 CharacterCount = 0;

	// 텍스트의 실제 길이를 계산
	for (const TCHAR& Char : CurrentText)
	{
		CharacterCount += (Char <= 0x007F) ? 1 : 3; // 한글은 3로 계산
		//UE_LOG(LogTemp, Warning, TEXT("%c"), Char);
	}

	// 글자 수 제한을 넘었을 때 경고 메시지 표시
	if (CharacterCount > MAX_CHARACTER_COUNT)
	{
		// 로그 출력으로 경고
		UE_LOG(LogTemp, Warning, TEXT("글자 수 제한을 초과했습니다!"));
	}
	else
	{
		// 제한을 넘지 않은 경우에만 유효한 텍스트로 저장
		LastValidText = CurrentText;
	}
}

void UJS_CreateRoomWidget::OnTextCommitted_MultiLine(const FText& Text, ETextCommit::Type CommitMethod)
{
	FString CurrentText = Text.ToString();
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
//	// 문자열의 실제 길이를 계산 (UTF-16 문자 단위로)
//	int32 CharacterCount = 0;
//	for (const TCHAR& Char : CurrentText)
//	{
//		// ASCII 범위의 문자는 1로, 그 외(한글 등)는 2로 카운트
//		CharacterCount += (Char <= 0x007F) ? 1 : 2;
//	}
//
//	// 설정한 제한을 초과하면 잘라내기
//	if (CharacterCount > 34) // 17글자 * 2 = 34 (한글 기준)
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
