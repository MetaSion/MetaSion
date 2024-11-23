// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_LobbyWidget.h"
#include "CJS/CJS_ChatWidget.h"
#include "CJS/CJS_BallPlayer.h"
#include "CJS/SessionGameInstance.h"
#include "CJS/CJS_ChatTextWidget.h"

#include "JS_RoomController.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MultiLineEditableText.h"
#include "Components/TextBlock.h"

void UCJS_LobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// List Panel
	Btn_MyPage->OnClicked.AddDynamic(this, &UCJS_LobbyWidget::OnClickMypage);
	Btn_RecList->OnClicked.AddDynamic(this, &UCJS_LobbyWidget::OnClickRecList);

	// World Panel
	Btn_RecList->OnClicked.AddDynamic(this, &UCJS_LobbyWidget::OnClickRecList);
	if (ChatUI && ChatUI->Btn_Send)
	{
		// Btn_Send 클릭 이벤트 바인딩
		ChatUI->Btn_Send->OnClicked.AddDynamic(this, &UCJS_LobbyWidget::HandleSendButtonClicked);
	}

	pc = Cast<AJS_RoomController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

// Widget Switch Start ---------------------------------------------------------------------------------------------------
void UCJS_LobbyWidget::SwitchToWidget(int32 index)
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::SwitchToWidget()"));
	if (Lobby_WidgetSwitcher) {
		UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::SwitchToWidget() UCJS_LobbyWidget exsied"));
		Lobby_WidgetSwitcher->SetActiveWidgetIndex(index);
		UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::SwitchToWidget() UCJS_LobbyWidget 3 Set"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UJS_CreateRoomWidget::SwitchToWidget() CR_WidgetSwitcher No"));
	}
}
// Widget Switch End -----------------------------------------------------------------------------------------------------



// Info Panel Start ------------------------------------------------------------------------------------------------------
void UCJS_LobbyWidget::SetRefWorldInfo()
{
	
}
// Info Panel End --------------------------------------------------------------------------------------------------------



// List Panel Start ------------------------------------------------------------------------------------------------------
void UCJS_LobbyWidget::OnClickMypage()
{

}
void UCJS_LobbyWidget::OnClickRecList()
{

}
// List Panel End --------------------------------------------------------------------------------------------------------



// World Panel Start ------------------------------------------------------------------------------------------------------
void UCJS_LobbyWidget::OnClickInfo()
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::OnClickInfo()"));
	// 월드 설명
	//SetInfomation();
}
void UCJS_LobbyWidget::OnClickWallPaper()
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::OnClickWallPaper()"));
	if (pc)
	{
		pc->OnClickButtonImage();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UCJS_LobbyWidget::OnClickWallPaper() can't be captured"));
	}
}
void UCJS_LobbyWidget::OnClickLobby()
{
	// 로비로 이동
}
void UCJS_LobbyWidget::SetInfomation(const FString& Text)
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::SetInfomation()"));
	if (ChatUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::SetInfomation ChatUI exsited"))
		ChatUI->SetEdit_RoomInfo(Text);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UCJS_LobbyWidget::SetInfomation ChatUI is null!"));
	}
}

void UCJS_LobbyWidget::HandleSendButtonClicked()
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
void UCJS_LobbyWidget::AddChatMessage(const FString& msg)
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::AddChatMessage()"));
	// WBP_CJS_ChatTextWidget 생성
	if (!ChatTextWidgetFactory)
	{
		UE_LOG(LogTemp, Error, TEXT("UCJS_LobbyWidget::AddChatMessage() ChatTextWidgetFactory is null!"));
		return;
	}

	// WBP_CJS_ChatTextWidget 생성
	UCJS_ChatTextWidget* NewChatText = CreateWidget<UCJS_ChatTextWidget>(this, ChatTextWidgetFactory);
	if (!NewChatText || !NewChatText->Txt_Msg)
	{
		UE_LOG(LogTemp, Error, TEXT("UCJS_LobbyWidget::AddChatMessage() Failed to create ChatTextWidget or Txt_Msg is null!"));
		return;
	}

	// Txt_Msg에 텍스트 설정
	UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::AddChatMessage() Setting message: %s"), *msg);
	NewChatText->Txt_Msg->SetText(FText::FromString(msg));

	// ScrollBox에 추가
	if (!ChatUI || !ChatUI->Scroll_MsgList)
	{
		UE_LOG(LogTemp, Error, TEXT("UCJS_LobbyWidget::AddChatMessage() Scroll_MsgList is null!"));
		return;
	}

	// 생성된 ChatTextWidget을 Scroll_MsgList에 추가
	ChatUI->Scroll_MsgList->AddChild(NewChatText);
	ChatUI->Scroll_MsgList->ScrollToEnd(); // 스크롤 맨 아래로 이동

	UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::AddChatMessage() Message added successfully"));
}
// World Panel End --------------------------------------------------------------------------------------------------------
