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


	// GameInstance ��������
	GameInstance = Cast<USessionGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance is null in UKGW_RoomListRenewal"));
	}
// 	btn_Test->OnClicked.AddDynamic(this, &UKGW_RoomListRenewal::OnclickTest);
	
}

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
