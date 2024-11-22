// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW_RoomListRenewal.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"



//FAirjetTotalMissionData ����ü�� ���޹޾� TextBlock�� SetText�ϴ� �Լ�
void UKGW_RoomListRenewal::SetMissionText(FString Aanalyze)
{
		if (TxtBox_Report)
		{
			DisplayedText = "";  // ������� ǥ�õ� �ؽ�Ʈ�� �� ���ڿ��� �ʱ�ȭ
			FullText = Aanalyze;  // ��ü �ؽ�Ʈ ����
			CurrentCharIndex = 0;  // ���� ���� �ε��� �ʱ�ȭ

			// Ÿ�̸� ���� (0.1�� �������� UpdateDisplayedText �Լ� ȣ��)
			GetWorld()->GetTimerManager().SetTimer(TextDisplayTimerHandle, this, &UKGW_RoomListRenewal::UpdateDisplayedText, 0.06f, true);
			UE_LOG(LogTemp, Warning, TEXT("Starting text display animation..."));
		}
	
}

void UKGW_RoomListRenewal::UpdateDisplayedText()
{
	if (CurrentCharIndex < FullText.Len())
	{
		// �� ���ھ� �߰��Ͽ� DisplayedText�� ����
		DisplayedText.AppendChar(FullText[CurrentCharIndex]);
		CurrentCharIndex++;

		// �ؽ�Ʈ ���� ������Ʈ
		if (TxtBox_Report)
		{
			TxtBox_Report->SetText(FText::FromString(DisplayedText));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TxtBox_Report is nullptr! Check your widget binding."));
		}
	}
	else
	{
		// ��� �ؽ�Ʈ�� ��µǾ����� Ÿ�̸� ����
		GetWorld()->GetTimerManager().ClearTimer(TextDisplayTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Finished text display animation."));
	}
}