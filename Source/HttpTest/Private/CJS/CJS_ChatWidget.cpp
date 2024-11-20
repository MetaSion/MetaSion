// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_ChatWidget.h"
#include "Components/MultiLineEditableText.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "CJS/CJS_ChatTextWidget.h"
#include "Components/TextBlock.h"
#include "JS_CreateRoomWidget.h"


void UCJS_ChatWidget::SetEdit_RoomInfo(FString text)
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_ChatWidget::SetEdit_RoomInfo()"));
	UE_LOG(LogTemp, Warning, TEXT("UJS_CreateRoomWidget::SetExplanation() - CurrentText: %s"), *text);

	if (!Edit_RoomInfo)
	{
		UE_LOG(LogTemp, Error, TEXT("UCJS_ChatWidget::SetEdit_RoomInfo() - Edit_RoomInfo is null"));
		return;
	}
	else
	{
		Edit_RoomInfo->SetText(FText::FromString(text));
		UE_LOG(LogTemp, Warning, TEXT("UCJS_ChatWidget::SetEdit_RoomInfo() Edit_RoomInfo->SetText()"));
	}
}
