// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KGW_ChattingText.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UKGW_ChattingText : public UUserWidget
{
	GENERATED_BODY()
	

	public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_Msg;
	

};
