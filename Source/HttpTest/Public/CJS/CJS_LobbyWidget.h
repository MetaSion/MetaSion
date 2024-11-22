// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CJS_LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UCJS_LobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;


	UPROPERTY()
    class USessionGameInstance* SessionGI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCJS_ChatWidget* ChatUI;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCJS_ChatTextWidget> ChatTextWidgetFactory;
	//UPROPERTY()
	//class UCJS_ChatTextWidget* ChatTextWidget;

    UFUNCTION()
    void HandleSendButtonClicked();
    UFUNCTION()
    void AddChatMessage(const FString& msg);
	
};
