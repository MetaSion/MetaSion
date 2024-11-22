// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW_RoomListRenewal.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"



//FAirjetTotalMissionData 구조체를 전달받아 TextBlock에 SetText하는 함수
void UKGW_RoomListRenewal::SetMissionText(FString Aanalyze)
{
		if (TxtBox_Report)
		{
			DisplayedText = "";  // 현재까지 표시된 텍스트를 빈 문자열로 초기화
			FullText = Aanalyze;  // 전체 텍스트 저장
			CurrentCharIndex = 0;  // 현재 문자 인덱스 초기화

			// 타이머 설정 (0.1초 간격으로 UpdateDisplayedText 함수 호출)
			GetWorld()->GetTimerManager().SetTimer(TextDisplayTimerHandle, this, &UKGW_RoomListRenewal::UpdateDisplayedText, 0.06f, true);
			UE_LOG(LogTemp, Warning, TEXT("Starting text display animation..."));
		}
	
}

void UKGW_RoomListRenewal::UpdateDisplayedText()
{
	if (CurrentCharIndex < FullText.Len())
	{
		// 한 글자씩 추가하여 DisplayedText에 저장
		DisplayedText.AppendChar(FullText[CurrentCharIndex]);
		CurrentCharIndex++;

		// 텍스트 블럭에 업데이트
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
		// 모든 텍스트가 출력되었으면 타이머 중지
		GetWorld()->GetTimerManager().ClearTimer(TextDisplayTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Finished text display animation."));
	}
}