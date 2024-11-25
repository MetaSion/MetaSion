// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_RoomButton.h"
#include "JS_OnClickRoomUI.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UJS_OnClickRoomUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void  NativeConstruct() override;

	UPROPERTY(EditAnywhere)
    class AJS_RoomController* pc;
	UPROPERTY(meta = (BindWidget))
	class UButton* SG_ImageButton;
	UPROPERTY(meta = (BindWidget))
	class UImage* SG_RoomImage;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SG_RoomName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SG_RoomOwner;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SG_Similarity;
	UPROPERTY(meta = (BindWidget))
	class UMultiLineEditableTextBox* SG_RoomComment;
	UPROPERTY(meta = (BindWidget))
	class UMultiLineEditableTextBox* SG_ReasonForRecommendation;
	
	FString ImagePath = "";
	//경로 세팅
	void SettingImagePath();
	void SettingData(FString TexturePath, UJS_RoomButton* Button);
	UFUNCTION()
	void OnImageClicked();
};
