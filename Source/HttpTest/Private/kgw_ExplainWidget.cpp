// Fill out your copyright notice in the Description page of Project Settings.


#include "kgw_ExplainWidget.h"
#include "Components/MultiLineEditableText.h"

void Ukgw_ExplainWidget::NativeConstruct()
{

	Super::NativeConstruct();



}

void Ukgw_ExplainWidget::Initialize(UJS_CreateRoomWidget* InCreateRoomWidget)
{
	CreateRoomWidgetRef = InCreateRoomWidget;
}

void Ukgw_ExplainWidget::OnClikExplanation()
{

    if (CreateRoomWidgetRef)
    {
        FString CurrentText = CreateRoomWidgetRef->GetCurrentText();
        SetExplanation(CurrentText);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CreateRoomWidgetRef�� �������� �ʾҽ��ϴ�."));
    }

}

void Ukgw_ExplainWidget::SetExplanation(const FString& Text)
{

    if (Txt_Explane)
    {
        Txt_Explane->SetText(FText::FromString(Text));
    }

}
