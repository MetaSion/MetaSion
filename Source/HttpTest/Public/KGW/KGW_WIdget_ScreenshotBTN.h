// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KGW_WIdget_ScreenshotBTN.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UKGW_WIdget_ScreenshotBTN : public UUserWidget
{
	GENERATED_BODY()

public: 
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CaptureImage;

;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_MyPage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	class AJS_RoomController* Controller;

	virtual void NativeConstruct() override;

	void OnClickCaptureImage();
	
	void OnClikMypage();

};
