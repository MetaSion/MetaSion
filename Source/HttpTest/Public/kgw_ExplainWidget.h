// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_CreateRoomWidget.h"
#include "kgw_ExplainWidget.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API Ukgw_ExplainWidget : public UUserWidget
{
	GENERATED_BODY()

public:

    virtual void NativeConstruct() override;


    	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Explanation;

    UPROPERTY(meta = (BindWidgetAnim),BlueprintReadWrite, Transient)
    class UWidgetAnimation* pop;

        UPROPERTY(meta=(BindWidget))
    class UMultiLineEditableText* Txt_Explane;

        	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CaptureImage;

;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_MyPage;
            UPROPERTY(EditAnywhere)
    FString CurrentText;

    UPROPERTY()
 class UJS_CreateRoomWidget* CreateRoomWidgetRef;

 void SetCreateRoomWidgetReference(UJS_CreateRoomWidget* InCreateRoomWidget);



	                UFUNCTION(BlueprintCallable)

    void OnClikExplanation();

    UFUNCTION(BlueprintCallable)

    void SetExplanation(const FString& Text);

};
