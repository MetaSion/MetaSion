// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_CreateRoomWidget.h"
#include "JS_RoomController.h"
#include "JsonParseLib.h"
#include "JS_WidgetFunction.h"
#include "HttpActor.h"

#include "CJS/CJS_BallPlayer.h"
#include "CJS/CJS_InnerWorldParticleActor.h"
#include "CJS/CJS_MultiRoomActor.h"
#include "CJS/CJS_ChatWidget.h"
#include "CJS/CJS_ChatTextWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"
#include "Components/MultiLineEditableText.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "KGW/KGW_RoomList.h"
#include "JS_RoomButton.h"


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

	btn_Good->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::OnClickGood);
	btn_Bad->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::OnClikBad);


	if (ChatUI && ChatUI->Btn_Send)
	{
		// Btn_Send 클릭 이벤트 바인딩
		ChatUI->Btn_Send->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::HandleSendButtonClicked);
	}

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
	if (pc) {
		pc->OnClickButtonImage();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("captured"));
	}
}
void UJS_CreateRoomWidget::OnClikExplanation()
{
	UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClikExplanation()"));
	SessionGI = Cast<USessionGameInstance>(GetGameInstance());
	if (SessionGI)
	{
		UE_LOG(LogTemp, Warning, TEXT(" UJS_CreateRoomWidget::OnClikExplanation() SessionGI exised"));
		UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClikExplanation() SessionGI->GetbRefRoomUIMultiOn() %d"), SessionGI->GetbRefRoomUIMultiOn());
		if (SessionGI->GetbRefRoomUIMultiOn())
		{
			//멀티방 인덱스를 받아와서
			APlayerController* OwningPlayer = GetWorld()->GetFirstPlayerController();
			if (OwningPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClikExplanation() Set APlayerController"));
				ACJS_BallPlayer* player = Cast<ACJS_BallPlayer>(OwningPlayer->GetPawn());
				if (player)
				{
					UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClikExplanation() Set ACJS_BallPlayer"));
					FString text = player->ClosestRoom->RoomInfo.roomdescription;
					UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClikExplanation() text %s"), *text);
					//방 설명 정보 삽입
					SetExplanation(text);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::OnClikExplanation() No BallPlayer"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::OnClikExplanation() OwningPlayer is null"));
			}		
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClikExplanation() SessionGI->GetbMyRoomWatchingOn() %d"), SessionGI->GetbMyRoomWatchingOn());
			if (SessionGI->GetbMyRoomWatchingOn())
			{
				//내방 목록 중 클릭 시, UI 이너월드 정보 삽입
				UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClikExplanation() Set roomBtn"));
				int32 idx = SessionGI->GetLastMyWolrdBtnIdx();
				UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClikExplanation() SessionGI->GetLastMyWolrdBtnIdx() %d"), idx);
				int32 ArraySize = SessionGI->WorldSetting.MyRooms.Num();
				UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClikExplanation() SessionGI->WorldSetting.MyRooms.Num() %d"), ArraySize);
				if (idx >= 0 && idx < ArraySize)
				{
					FString text = SessionGI->WorldSetting.MyRooms[idx].MyRoomName;
					UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClikExplanation() roomName: %s"), *text);
					SetExplanation(text);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::OnClikExplanation() Invalid index: %d, MyRooms size: %d"), idx, ArraySize);
					int32 changedIdx = 0;  // <--- 버튼 번호가 20보다 클 경우 고정값
					FString Changedtext = SessionGI->WorldSetting.MyRooms[changedIdx].MyRoomName;
					UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::OnClikExplanation() changedIdx: %d"), changedIdx);
					SetExplanation(Changedtext);
				}
			}
			else
			{
				// 이너월드 생성 사용자가 작성한 문구
				UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClikExplanation() CurrentText %s"), *CurrentText);
				SetExplanation(CurrentText);
			}		
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::OnClikExplanation() SessionGI is null!"));
		return;
	}
}
void UJS_CreateRoomWidget::OnClikMypage()
{
	UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClikMypage()"));
	SwitchToWidget(4);
}
void UJS_CreateRoomWidget::OnClickGood()
{
// 	SendCompleteRoomData();
	UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClickGood()"));
	SessionGI = Cast<USessionGameInstance>(GetGameInstance());
	APlayerController* OwningPlayer = GetWorld()->GetFirstPlayerController();
	ACJS_BallPlayer* player = Cast<ACJS_BallPlayer>(OwningPlayer->GetPawn());

	if (SessionGI)
	{
		UE_LOG(LogTemp, Warning, TEXT(" UJS_CreateRoomWidget::OnClickGood() SessionGI exised"));
		if (SessionGI->GetbRefRoomUIMultiOn())
		{
			UE_LOG(LogTemp, Warning, TEXT(" UJS_CreateRoomWidget::OnClickGood() SessionGI->GetbRefRoomUIMultiOn() : %d"), SessionGI->GetbRefRoomUIMultiOn());
			if (OwningPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() Set APlayerController"));				
				if (player)
				{
					UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() Set ACJS_BallPlayer"));
					player->RequestMoveLobby(OwningPlayer);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() No BallPlayer"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() OwningPlayer is null"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(" UJS_CreateRoomWidget::OnClickGood() SessionGI->GetbMyRoomWatchingOn() : %d"), SessionGI->GetbMyRoomWatchingOn());
			if (SessionGI->GetbMyRoomWatchingOn())
			{
				SessionGI->SetbtMyRoomWatchingOn(false);
				UJS_RoomButton* ImageButton = NewObject<UJS_RoomButton>(this);
				ImageButton->Initialize();   // <-- 수정 필요!!
				UE_LOG(LogTemp, Warning, TEXT(" UJS_CreateRoomWidget::OnClickGood() Change SessionGI->SetbMyRoomWatchingOn(false) : %d"), SessionGI->GetbMyRoomWatchingOn());
				httpActor->SetBackgroundSound();
			}
			UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClickGood() Move From InnerWorld to Main_Sky"));
			/*SessionGI->PlayMusic(SessionGI->);*/
			UGameplayStatics::OpenLevel(this, FName("Main_Sky"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() SessionGI is null"));
	}
}
void UJS_CreateRoomWidget::OnClikBad()
{
// 	SendCompleteRoomData();
	UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClikBad()"));

	SessionGI = Cast<USessionGameInstance>(GetGameInstance());
	if (SessionGI)
	{
		UE_LOG(LogTemp, Warning, TEXT(" UJS_CreateRoomWidget::OnClickGood() SessionGI exised"));
		if (SessionGI->GetbRefRoomUIMultiOn())
		{
			UE_LOG(LogTemp, Warning, TEXT(" UJS_CreateRoomWidget::OnClickGood() SessionGI->GetbRefRoomUIMultiOn() : %d"), SessionGI->GetbRefRoomUIMultiOn());
			APlayerController* OwningPlayer = GetWorld()->GetFirstPlayerController();
			if (OwningPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() Set APlayerController"));
				ACJS_BallPlayer* player = Cast<ACJS_BallPlayer>(OwningPlayer->GetPawn());
				if (player)
				{
					UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() Set ACJS_BallPlayer"));
					player->RequestMoveLobby(OwningPlayer);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() No BallPlayer"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() OwningPlayer is null"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(" UJS_CreateRoomWidget::OnClickGood() SessionGI->GetbMyRoomWatchingOn() : %d"), SessionGI->GetbMyRoomWatchingOn());
			if (SessionGI->GetbMyRoomWatchingOn())
			{
				SessionGI->SetbtMyRoomWatchingOn(false);
				UJS_RoomButton* ImageButton = NewObject<UJS_RoomButton>(this);
				ImageButton->Initialize();  // <-- 수정 필요!!
				UE_LOG(LogTemp, Warning, TEXT(" UJS_CreateRoomWidget::OnClickGood() Change SessionGI->SetbMyRoomWatchingOn(false) : %d"), SessionGI->GetbMyRoomWatchingOn());
				httpActor->SetBackgroundSound();

			}
			UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::OnClickGood() Move From InnerWorld to Main_Sky"));
			UGameplayStatics::OpenLevel(this, FName("Main_Sky"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() SessionGI is null"));
	}
}
void UJS_CreateRoomWidget::SetExplanation(const FString& Text)
{ 	
	UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::SetExplanation()"));
	//Txt_Explane->SetText(FText::FromString(Text));
	if (ChatUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::SetExplanation() ChatUI exsited"));
		ChatUI->SetEdit_RoomInfo(Text);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::SetExplanation() ChatUI is null!"));
	}
}

void UJS_CreateRoomWidget::SetParticle()
{
	UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::SetParticle()"));
	if (!ParticleFactory)
	{
		UE_LOG(LogTemp, Error, TEXT("ParticleFactory is not set!"));
		return;
	}
// 	CleanParticle();
// 	(X = 102.397720, Y = 303.903661, Z = -316.892022)
	FVector Location = FVector(-177.602280f, 303.903656, -316.892029f);
	FRotator Rotation = FRotator::ZeroRotator;

	auto* CurrentParticleActor = GetWorld()->SpawnActor<ACJS_InnerWorldParticleActor>(
		ParticleFactory, Location, Rotation);

	// 4.파티클 색을 변경한다 +  감정 파티클을 변경한다.
	if (httpActor) {
		httpActor->ApplyMyWorldPointLightColors();
		httpActor->ApplyMyWorldNiagaraAssets();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("740 : Setting HttpActor Fail..."));
	}
}


// Send 버튼 클릭 시
void UJS_CreateRoomWidget::HandleSendButtonClicked()
{
	if (!ChatUI || !ChatUI->Edit_InputText || !ChatUI->Scroll_MsgList || !ChatTextWidgetFactory)
	{
		UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() ChatUI or required components are not initialized!"));
		return;
	}

	// Edit_InputText에서 텍스트 가져오기
	FString InputText = ChatUI->Edit_InputText->GetText().ToString();
	if (InputText.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() Input text is empty, ignoring send."));
		return;
	}

	SessionGI = Cast<USessionGameInstance>(GetGameInstance());
	if (SessionGI)
	{
		UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() SessionGI set."));
		UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() SessionGI->bRefRoomUIMultiOn : %d"), SessionGI->GetbRefRoomUIMultiOn());
		if (!SessionGI->GetbRefRoomUIMultiOn())  // InnerWorld (싱글 플레이)
		{
			UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() Call AddChatMessage()"));
			AddChatMessage(InputText);
		}
		else  // MultiWorld (멀티 플레이)
		{
			UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() Call ServerRPC"));
			APlayerController* OwningPlayer = GetWorld()->GetFirstPlayerController();
			if (OwningPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() Set APlayerController"));
				ACJS_BallPlayer* player = Cast<ACJS_BallPlayer>(OwningPlayer->GetPawn());
				if (player)
				{
					UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() Set ACJS_BallPlayer"));
					player->ServerRPC_Chat(InputText);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() No BallPlayer"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() OwningPlayer is null"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::HandleSendButtonClicked() No SessionGI"));
	}

	// Edit_InputText 초기화
	ChatUI->Edit_InputText->SetText(FText::GetEmpty());
}

void UJS_CreateRoomWidget::AddChatMessage(const FString& msg)
{
	UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::AddChatMessage()"));
	// WBP_CJS_ChatTextWidget 생성
	if (!ChatTextWidgetFactory)
	{
		UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::AddChatMessage() ChatTextWidgetFactory is null!"));
		return;
	}

	// WBP_CJS_ChatTextWidget 생성
	UCJS_ChatTextWidget* NewChatText = CreateWidget<UCJS_ChatTextWidget>(this, ChatTextWidgetFactory);
	if (!NewChatText || !NewChatText->Txt_Msg)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create ChatTextWidget or Txt_Msg is null!"));
		return;
	}

	// Txt_Msg에 텍스트 설정
	UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::AddChatMessage() Setting message: %s"), *msg);
	NewChatText->Txt_Msg->SetText(FText::FromString(msg));

	// ScrollBox에 추가
	if (!ChatUI || !ChatUI->Scroll_MsgList)
	{
		UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::AddChatMessage() Scroll_MsgList is null!"));
		return;
	}

	// 생성된 ChatTextWidget을 Scroll_MsgList에 추가
	ChatUI->Scroll_MsgList->AddChild(NewChatText);
	ChatUI->Scroll_MsgList->ScrollToEnd(); // 스크롤 맨 아래로 이동

	UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::AddChatMessage() Message added successfully"));
}

//widget Switch
void UJS_CreateRoomWidget::SwitchToWidget(int32 index)
{
	UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::SwitchToWidget()"));
	if (CR_WidgetSwitcher) {
		UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::SwitchToWidget() CR_WidgetSwitcher exsied"));
		CR_WidgetSwitcher->SetActiveWidgetIndex(index);
		UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::SwitchToWidget() CR_WidgetSwitcher 3 Set"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::SwitchToWidget() CR_WidgetSwitcher No"));
	}
}
void UJS_CreateRoomWidget::SetRoomInfo()
{

	RoomName = ED_RoomName->GetText().ToString();
	RoomDescription = ED_MultiText->GetText().ToString();

	// �߰��� ��: bPrivate -> RoomPP ��ȯ
	RoomPP = FString::FromInt(bPrivate);

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

	SetParticle();
	httpActor->SetNewBackGroundSound();

}
void UJS_CreateRoomWidget::DelayedSwitchToWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::DelayedSwitchToWidget()"));
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
// 	RoomName = ED_RoomName->GetText().ToString();
// 	RoomDescription = ED_MultiText->GetText().ToString();
// 
// 	// �߰��� ��: bPrivate -> RoomPP ��ȯ
// 	RoomPP = FString::FromInt(bPrivate);

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
