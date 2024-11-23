// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_LobbyWidget.h"
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
#include "Components/VerticalBox.h"
#include "Animation/WidgetAnimation.h"

void UCJS_LobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// List Panel
	Btn_MyWorld->OnClicked.AddDynamic(this, &UCJS_LobbyWidget::OnClickMyWorld);
	Btn_RecList->OnClicked.AddDynamic(this, &UCJS_LobbyWidget::OnClickRecList);
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

void UCJS_LobbyWidget::ShowLobbyUIZeroOrder()
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::ShowLobbyUIZeroOrder"));
	SwitchToWidget(0);
	if (SlideLeft) // 애니메이션이 유효한지 확인
	{
		PlayAnimation(SlideLeft);
		UE_LOG(LogTemp, Log, TEXT("Playing widget animation: %s"), *SlideLeft->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BtnSlide is null. Make sure it's assigned."));
	}

	if (GetWorld()) {
		GetWorld()->GetTimerManager().SetTimer(HideRefWorldInfoHandler, this, &UCJS_LobbyWidget::HideLobbyUIZeroOrder, 5.0f, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() returned NULL!"));
	}
}
void UCJS_LobbyWidget::HideLobbyUIZeroOrder()
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::HideLobbyUIZeroOrder"));
	SwitchToWidget(0);
	if (SlideRight) // 애니메이션이 유효한지 확인
	{
		PlayAnimation(SlideRight);
		UE_LOG(LogTemp, Log, TEXT("Playing widget animation: %s"), *SlideRight->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BtnSlide is null. Make sure it's assigned."));
	}
}

// Info Panel End --------------------------------------------------------------------------------------------------------


// List Panel Start ------------------------------------------------------------------------------------------------------
void UCJS_LobbyWidget::ShowLobbyUIFirstOrder()
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::ShowLobbyUIFirstOrder"));
	SwitchToWidget(1);
	Box_List->SetVisibility(ESlateVisibility::Hidden);
	SetRecList();
	if (BtnSlide) // 애니메이션이 유효한지 확인
	{
		PlayAnimation(BtnSlide);
		UE_LOG(LogTemp, Log, TEXT("Playing widget animation: %s"), *BtnSlide->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BtnSlide is null. Make sure it's assigned."));
	}
}
void UCJS_LobbyWidget::HideLobbyUIFirstOrder()
{
	if (BtnSlide) // 애니메이션이 유효한지 확인
	{
		PlayAnimationReverse(BtnSlide);
		UE_LOG(LogTemp, Log, TEXT("Playing widget animation: %s"), *BtnSlide->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BtnSlide is null. Make sure it's assigned."));
	}
}

void UCJS_LobbyWidget::OnClickRecList()
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::OnClickRecList"));
	if (!SetListUp)
	{
		Box_List->SetVisibility(ESlateVisibility::Visible);
		if (ListUp) // 애니메이션이 유효한지 확인
		{
			PlayAnimationForward(ListUp);
			UE_LOG(LogTemp, Log, TEXT("Playing widget animation: %s"), *ListUp->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BtnSlide is null. Make sure it's assigned."));
		}
		SetListUp = true;
	}
	else
	{
		Box_List->SetVisibility(ESlateVisibility::Hidden);
		if (ListUp) // 애니메이션이 유효한지 확인
		{
			PlayAnimationReverse(ListUp);
			UE_LOG(LogTemp, Log, TEXT("Playing widget animation: %s"), *ListUp->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BtnSlide is null. Make sure it's assigned."));
		}
		SetListUp = false;
	}	
}

// 추천방 20개 목록 표시
void UCJS_LobbyWidget::SetRecList()
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::SetRecList()"));
	if (TextWidgetFactory)
	{
		// RefRoomList 불러오기 + 값 할당하기
		USessionGameInstance* SessionGI = Cast<USessionGameInstance>(GetGameInstance());
		if (SessionGI)
		{
			/*
			for (int32 Index = 0; Index < SessionGI->WorldSetting.suggest_list.Num(); ++Index)
			{
				FString name = SessionGI->WorldSetting.suggest_list[Index].room_name;
				worldNameText->Txt_Msg->SetText(FText::FromString(name));
			}*/

			const TArray<FMySuggest_List>& SuggestList = SessionGI->WorldSetting.suggest_list;
			UE_LOG(LogTemp, Warning, TEXT("Suggest list contains %d items."), SuggestList.Num());

			for (int32 Index = 0; Index < SuggestList.Num(); ++Index)
			//for (int32 Index = 0; Index < 20; ++Index)  // 테스트용
			{
				// 새로운 ChatTextWidget 생성
				UCJS_ChatTextWidget* worldNameText = CreateWidget<UCJS_ChatTextWidget>(this, TextWidgetFactory);
				if (!worldNameText || !worldNameText->Txt_Msg)
				{
					UE_LOG(LogTemp, Error, TEXT("UCJS_LobbyWidget::SetRecList() Failed to create ChatTextWidget or Txt_Msg is null!"));
					return;
				}

				// 텍스트 설정 
				//FString name = FString::Printf(TEXT("world name %d"), Index);  // 테스트용
				FString name = SessionGI->WorldSetting.suggest_list[Index].room_name;
				UE_LOG(LogTemp, Warning, TEXT("Index %d: room_name = %s"), Index, *name);
				worldNameText->Txt_Msg->SetText(FText::FromString(name));

				// ScrollBox에 추가
				if (!Scroll_List)
				{
					UE_LOG(LogTemp, Error, TEXT("UCJS_LobbyWidget::SetRecList() Scroll_List is null!"));
					return;
				}

				Scroll_List->AddChild(worldNameText);

				int32 NumChildren = Scroll_List->GetChildrenCount();
				UE_LOG(LogTemp, Warning, TEXT("Scroll_List now contains %d children."), NumChildren);
			}

			// 스크롤을 맨 아래로 이동
			Scroll_List->ScrollToEnd();
			UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::SetRecList() worldNameText added successfully"));
		}
		UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::SetRecList() List added successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UCJS_LobbyWidget::SetRecList() TextWidgetFactory is null!"));
	}	
}

void UCJS_LobbyWidget::OnClickMyWorld()
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_LobbyWidget::OnClickMyWorld"));
}
// List Panel End --------------------------------------------------------------------------------------------------------
