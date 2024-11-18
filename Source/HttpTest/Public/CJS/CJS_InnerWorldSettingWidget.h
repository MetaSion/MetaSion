// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CJS_InnerWorldSettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UCJS_InnerWorldSettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class USlider* UDW_Snow_Slider;

	void SetWinterSnowSlider();

	UFUNCTION(BlueprintCallable)
	void SetInnerWorldUI();

	
};
