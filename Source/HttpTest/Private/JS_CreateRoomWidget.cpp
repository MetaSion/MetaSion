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

	btn_CreateRoom_Yes->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::CreateRoomChooseYes);
	btn_CreateRoom_No->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::CreateRoomChooseNo);

	btn_CreateRoom_Private->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::SetPrivate);
	btn_CreateRoom_Public->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::SetPrivate);

	btn_CompleteCreateRoom->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::CompleteCreateRoom);

	ED_RoomName->OnTextChanged.AddDynamic(this, &UJS_CreateRoomWidget::OnTextChanged_SingleLine);

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
		widgetActor->SetActorVisibilityVisible();
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

	FMyRoomInfo MyRoomInfo;

	MyRoomInfo.UserId = "testuser";
	MyRoomInfo.UltraSky_TimeOfDay = "1200";
	MyRoomInfo.UltraWheather_CloudCoverage = "true";
	MyRoomInfo.UltraWheather_Fog = "true";
	MyRoomInfo.UltraWheather_Rain = "true";
	MyRoomInfo.UltraWheather_Snow = "true";
	MyRoomInfo.UltraWheather_Dust = "true";
	MyRoomInfo.UltraWheather_Thunder = "true";
	MyRoomInfo.MainObject = "true";
	MyRoomInfo.SubObject = "true";
	MyRoomInfo.Background = "true";
	MyRoomInfo.Particle_num = "true";
	MyRoomInfo.RoomName = ED_RoomName->GetText().ToString();
	MyRoomInfo.RoomDescription = "this is hello worlds";
	MyRoomInfo.RoomPP = FString::FromInt(bPrivate);

	FString json = UJsonParseLib::MyRoomInfo_Convert_StructToJson(MyRoomInfo);

	if (httpActor) {
		httpActor->MyRoomInfoReqPost(httpActor->MyRoomURL, json);
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
		UE_LOG(LogTemp, Warning, TEXT("%c"), Char);
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
		UE_LOG(LogTemp, Warning, TEXT("%c"), Char);
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
