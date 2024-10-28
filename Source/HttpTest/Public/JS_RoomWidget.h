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

	UPROPERTY(meta=(BindWidget))
	class UButton* btn_SignComplete;

	UPROPERTY(Meta=(BindWidget))
    class UTextBlock* txt_index;

    UPROPERTY(Meta=(BindWidget))
    class UTextBlock* txt_absindex;

	 // �������Ʈ���� ������ �ִϸ��̼� ����
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* CameraSutterEffect;

	UPROPERTY(EditAnywhere)
	class AHttpActor* httpActor;

	UFUNCTION()
	void SendChangeIndexData();

	void SetIndex(int WallPaperIndex, int absWallPaperIndex);
};
