// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW_RoomListRenewal.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "CJS/SessionGameInstance.h"



void UKGW_RoomListRenewal::NativeConstruct()
{

	Super::NativeConstruct();

	btn_AIAnalysis->OnClicked.AddDynamic(this, &UKGW_RoomListRenewal::ShowAIAnalysisUI);


	// GameInstance 가져오기
	GameInstance = Cast<USessionGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance is null in UKGW_RoomListRenewal"));
	}
// 	btn_Test->OnClicked.AddDynamic(this, &UKGW_RoomListRenewal::OnclickTest);
	
}

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
void UKGW_RoomListRenewal::ShowAIAnalysisUI()
{
	ChangeCanvas(1);
// 	SetMissionText(GameInstance->WorldSetting.Result);
	 	FString explain = TEXT("he beauty of programming lies in its infinite possibilities. Just as an artist wields a brush to create a masterpiece, a developer writes lines of code to bring ideas to life. Whether it's designing a game, building a website, or automating a process, every keystroke ");
		SetMissionText(explain);

}


// void UKGW_RoomListRenewal::OnclickTest()
// {
// 	FString explain = TEXT("he beauty of programming lies in its infinite possibilities. Just as an artist wields a brush to create a masterpiece, a developer writes lines of code to bring ideas to life. Whether it's designing a game, building a website, or automating a process, every keystroke ");
// 	SetMissionText(explain);
// }
