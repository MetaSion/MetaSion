// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_CreateRoomWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"

void UJS_CreateRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

    // �������� �ùٸ��� ���ε��Ǿ����� Ȯ��
    if (btn_CreateRoom_Yes == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("btn_CreateRoom_Yes is not bound!"));
    }

    if (btn_CreateRoom_No == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("btn_CreateRoom_No is not bound!"));
    }

    if (btn_CreateRoom_Private == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("btn_CreateRoom_Private is not bound!"));
    }

    // �̺�Ʈ ���ε�
    if (btn_CreateRoom_Yes)
    {
        btn_CreateRoom_Yes->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::CreateRoomChooseYes);
    }

    if (btn_CreateRoom_No)
    {
        btn_CreateRoom_No->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::CreateRoomChooseNo);
    }

    if (btn_CreateRoom_Private)
    {
        btn_CreateRoom_Private->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::SetPrivate);
    }

}

void UJS_CreateRoomWidget::SwitchToWidget(int32 index)
{
	if (CR_WidgetSwitcher) {
		// �ε����� ���� Ȱ��ȭ�� ���� ��ȯ
		CR_WidgetSwitcher->SetActiveWidgetIndex(index);
	}
}
//�� �ٹ̱� �Ϸ� �� SwitchToWidget(0)�� ȣ���ϰ� ��������.
void UJS_CreateRoomWidget::CreateRoomChooseYes()
{
	// �� �ٹ̱� �Ϸ� �� �־�� �� ��
	// �� ���� 2�� ° ���� UI ���̱�
	SwitchToWidget(1);
}

void UJS_CreateRoomWidget::CreateRoomChooseNo()
{
	// �� �ٹ̱� �ƴϿ� �� �־�� �� ��
	// �� �ٹ̱� �Ϸ� UI �����
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UJS_CreateRoomWidget::SetPrivate()
{
	bPrivate = !bPrivate;
	SwitchToWidget(2);
	ShowUIForLimitedTime(3);
}

void UJS_CreateRoomWidget::ShowUIForLimitedTime(float DurationInSeconds)
{
	SetVisibility(ESlateVisibility::Visible);
	if (GetWorld()) GetWorld()->GetTimerManager().SetTimer(Handler, this, &UJS_CreateRoomWidget::HideUI, DurationInSeconds, false);
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GetWorld() returned NULL!"));
    }
}

void UJS_CreateRoomWidget::HideUI()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}
