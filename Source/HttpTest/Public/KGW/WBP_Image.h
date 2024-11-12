// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WBP_Image.generated.h"

/**
 *
 */
UCLASS()
class HTTPTEST_API UWBP_Image : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadOnly)
	UWidgetAnimation* ShowImage;


	UFUNCTION(BlueprintCallable)
	void PlayShowImageAnimation();

	UPROPERTY(meta = (BindWidget))
	class UImage* ImageWeb;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Explain;



	void SetImage(class UTexture2D* newTexture);

	FString WebImageURL /*= "http://"*/;
	FString TextURL /*= "http://"*/;


	void GetImage();

// 	void SetTextLog(FString explain);

	void SendTextPost();


	UPROPERTY()
	class AHttpActor* HttpActor;


};
