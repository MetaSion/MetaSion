// Fill out your copyright notice in the Description page of Project Settings.


#include "kgw_ExplainWidget.h"
#include "Components/MultiLineEditableText.h"
#include "JS_CreateRoomWidget.h"

void Ukgw_ExplainWidget::NativeConstruct()
{

	Super::NativeConstruct();



}

void Ukgw_ExplainWidget::SetCreateRoomWidgetReference(UJS_CreateRoomWidget* InCreateRoomWidget)
{
	CreateRoomWidgetRef = InCreateRoomWidget;
}

void Ukgw_ExplainWidget::OnClikExplanation()
{

    if (CreateRoomWidgetRef)
    {
         CurrentText = CreateRoomWidgetRef->GetCurrentText();
        SetExplanation(CurrentText);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CreateRoomWidgetRef가 설정되지 않았습니다."));
    }

}

void Ukgw_ExplainWidget::SetExplanation(const FString& Text)
{

    if (Txt_Explane)
    {
        Txt_Explane->SetText(FText::FromString(Text));
    }

}
