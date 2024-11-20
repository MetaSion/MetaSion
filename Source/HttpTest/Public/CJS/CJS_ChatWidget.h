// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CJS_ChatWidget.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UCJS_ChatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta=(BindWidget))
	class UScrollBox* Scroll_MsgList;
	UPROPERTY(meta=(BindWidget))
    class UEditableText* Edit_RoomInfo;
	UPROPERTY(meta=(BindWidget))
    class UMultiLineEditableText* Edit_InputText;
	UPROPERTY(meta=(BindWidget))
    class UButton* Btn_Send;


	UFUNCTION()
	void SetEdit_RoomInfo(FString text);
};
