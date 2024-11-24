// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_RefRoomInfoWidget.h"
#include "CJS/CJS_BallPlayer.h"

#include "Components/TextBlock.h"
#include "Components/MultiLineEditableTextBox.h"



void UCJS_RefRoomInfoWidget::NativeConstruct()
{
    Super::NativeConstruct();

	if (Txt_Name)
	{
		UE_LOG(LogTemp, Log, TEXT("Txt_Name is valid: %s"), *Txt_Name->GetText().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Txt_Name is null! Check your blueprint bindings."));
	}
	if (Txt_Owner)
	{
		UE_LOG(LogTemp, Log, TEXT("Txt_Owner is valid: %s"), *Txt_Owner->GetText().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Txt_Owner is null! Check your blueprint bindings."));
	}
	if (Txt_Percent)
	{
		UE_LOG(LogTemp, Log, TEXT("Txt_Percent is valid: %s"), *Txt_Percent->GetText().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Txt_Percent is null! Check your blueprint bindings."));
	}
	if (Txt_Description)
	{
		UE_LOG(LogTemp, Log, TEXT("Txt_Description is valid: %s"), *Txt_Description->GetText().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Txt_Description is null! Check your blueprint bindings."));
	}
	if (Txt_Recommand)
	{
		UE_LOG(LogTemp, Log, TEXT("Txt_Recommand is valid: %s"), *Txt_Recommand->GetText().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Txt_Recommand is null! Check your blueprint bindings."));
	}
}


void UCJS_RefRoomInfoWidget::SetRefWorldInfo(const FString& name, const FString& owner, const FString& description, const FString& percent, const FString& reason)
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_RefRoomInfoWidget::SetRefWorldInfo()"));
	
	ShowRefRoomInfoUI();

	if (Txt_Name && Txt_Owner && Txt_Percent && Txt_Description && Txt_Recommand)
	{
		Txt_Name->SetText(FText::FromString(name));
		Txt_Owner->SetText(FText::FromString(owner));
		Txt_Percent->SetText(FText::FromString(percent));
		Txt_Description->SetText(FText::FromString(description));
		Txt_Recommand->SetText(FText::FromString(reason));

		UE_LOG(LogTemp, Log, TEXT("RefWorldInfo set: Name=%s, Owner=%s, Percent=%s, Description=%s, Reason=%s"),
			*name, *owner, *percent, *description, *reason);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("One or more text widgets are null!"));
	}

	if (GetWorld()) {
		GetWorld()->GetTimerManager().SetTimer(HideRefWorldInfoHandler, this, &UCJS_RefRoomInfoWidget::HideRefRoomInfoUI, 5.0f, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() returned NULL!"));
	}
}


void UCJS_RefRoomInfoWidget::ShowRefRoomInfoUI()
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_RefRoomInfoWidget::ShowRefRoomInfoUI()"));
	// 현재 Visibility가 Hidden 상태라면 Visible로 변경
	if (GetVisibility() == ESlateVisibility::Hidden)
	{
		SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp, Log, TEXT("UCJS_LobbyWidget was hidden. Changed to visible."));
	}
}
void UCJS_RefRoomInfoWidget::HideRefRoomInfoUI()
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_RefRoomInfoWidget::HideRefRoomInfoUI()"));
	// 현재 Visibility가 Visible 상태라면 Hidden로 변경
	if (GetVisibility() == ESlateVisibility::Visible)
	{
		SetVisibility(ESlateVisibility::Hidden);
		UE_LOG(LogTemp, Log, TEXT("UCJS_LobbyWidget was hidden. Changed to visible."));
	}

	// BallPlayer를 찾아서 bShowRefRoomInfoUI를 false로 설정
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			ACJS_BallPlayer* BallPlayer = Cast<ACJS_BallPlayer>(PlayerController->GetPawn());
			if (BallPlayer)
			{
				BallPlayer->bShowRefRoomInfoUI = false;
				UE_LOG(LogTemp, Log, TEXT("Successfully reset bShowRefRoomInfoUI to false."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to cast Pawn to ACJS_BallPlayer."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No PlayerController found in the world."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() returned NULL!"));
	}
}
