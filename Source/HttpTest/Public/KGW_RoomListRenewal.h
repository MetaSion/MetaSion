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

	FString DisplayedText; //������� ǥ�õ� �ؽ�Ʈ��
	FString FullText; //�Ľ��� ��ü �ؽ�Ʈ
	int32 CurrentCharIndex; //ǥ���ؽ�Ʈ �ε���
	FTimerHandle TextDisplayTimerHandle; //�ؽ�Ʈ �ִϸ��̼� �ڵ�
	FTimerHandle InvisibleTimerHandle; //UI Invisible �ڵ�

	//FAirjetTotalMissionData ����ü�� ���޹޾� TextBlock�� SetText�ϴ� �Լ�
	void SetMissionText(FString Aanalyze);
	//ActingText �ִϸ��̼� �Լ�
	void UpdateDisplayedText();
	
};

	
};
