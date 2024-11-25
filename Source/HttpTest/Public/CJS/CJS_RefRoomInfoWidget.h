// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CJS_RefRoomInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UCJS_RefRoomInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
    class UTextBlock* Txt_Name;
	UPROPERTY(meta=(BindWidget))
    class UTextBlock* Txt_Owner;
	UPROPERTY(meta=(BindWidget))
    class UTextBlock* Txt_Percent;
	UPROPERTY(meta=(BindWidget))
    class UTextBlock* Txt_Description;
	UPROPERTY(meta=(BindWidget))
    class UMultiLineEditableTextBox* Txt_Recommand;
	//UPROPERTY(meta = (BindWidgetAnim), Transient)
 //   class UWidgetAnimation* SlideLeft;   
	//UPROPERTY(meta = (BindWidgetAnim), Transient)
 //   class UWidgetAnimation* SlideRight;

	FTimerHandle HideRefWorldInfoHandler;

	UFUNCTION()
	void SetRefWorldInfo(const FString& name, const FString& owner, const FString& description, const FString& percent, const FString& reason);
	UFUNCTION()
	void ShowRefRoomInfoUI();
	UFUNCTION()
	void HideRefRoomInfoUI();
};
