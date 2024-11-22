// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KGW/KGW_RoomList.h"
#include "KGW_RoomListRenewal.generated.h"


struct FAirjetTotalMissionData;

/**
 * 
 */
UCLASS()
class HTTPTEST_API UKGW_RoomListRenewal : public UKGW_RoomList
{
	GENERATED_BODY()

public:

	FString DisplayedText; //현재까지 표시된 텍스트들
	FString FullText; //파싱한 전체 텍스트
	int32 CurrentCharIndex; //표시텍스트 인덱스
	FTimerHandle TextDisplayTimerHandle; //텍스트 애니메이션 핸들
	FTimerHandle InvisibleTimerHandle; //UI Invisible 핸들

	//FAirjetTotalMissionData 구조체를 전달받아 TextBlock에 SetText하는 함수
	void SetMissionText(FString Aanalyze);
	//ActingText 애니메이션 함수
	void UpdateDisplayedText();
	
};

	
};
