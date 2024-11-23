// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_RoomWidget.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UJS_RoomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(Meta=(BindWidget))
    class UTextBlock* txt_index;

    UPROPERTY(Meta=(BindWidget))
    class UTextBlock* txt_absindex;

	 // �������Ʈ���� ������ �ִϸ��̼� ����
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* CameraSutterEffect;

	UPROPERTY(Meta=(BindWidget))
    class UVerticalBox* VTB_Heart;

	UPROPERTY(Meta=(BindWidget))
    class UTextBlock* txt_HeartNum;

	UPROPERTY(EditAnywhere)
	class AHttpActor* httpActor;

	UFUNCTION()
	void SendWallPaperData();

	/*UFUNCTION()
	void SendHeartCount();*/

	void SetIndex(FString roomNumber, int absWallPaperIndex);
	//void SetHeartCount(FString HeartCount);
};
