// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "JS_CreateRoomWidget.generated.h"
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class HTTPTEST_API UJS_CreateRoomWidget : public UUserWidget
{
    GENERATED_BODY()
   
private:
    static const int32 MAX_CHARACTER_COUNT = 51; // �ѱ� ���� 17����
    FString LastValidText;

public:
    virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere)
	class AHttpActor* httpActor;
    UPROPERTY(EditAnywhere)
	class AJS_WidgetFunction* widgetActor;
	UPROPERTY(EditAnywhere)
	class AJS_RoomController* pc;



    UPROPERTY(meta=(BindWidget))
    class UButton* btn_CreateRoom_Yes;   
    UPROPERTY(meta=(BindWidget))
    class UButton* btn_CreateRoom_No;
    UPROPERTY(meta=(BindWidget))
    class UButton* btn_CompleteCreateRoom;  
    UPROPERTY(meta = (BindWidget))
    class UEditableText* ED_RoomName; 
    UPROPERTY(meta = (BindWidget))
    class UMultiLineEditableText* ED_MultiText;
    UPROPERTY(meta=(BindWidget))
    class UButton* btn_CreateRoom_Private;
    UPROPERTY(meta=(BindWidget))
    class UButton* btn_CreateRoom_Public;    
    UPROPERTY(meta=(BindWidget))
    class UWidgetSwitcher* CR_WidgetSwitcher;
    UPROPERTY(meta=(BindWidget))
    class UWidgetSwitcher* CR_WidgetSwitcherPP;   
    UPROPERTY(meta=(BindWidget))
    class UVerticalBox* VB_CreateRoom_Q1; 
    UPROPERTY(meta=(BindWidget))
    class UVerticalBox* VB_CreateRoom_Q2; 
    UPROPERTY(meta=(BindWidget))
    class UBorder* CreateRoomCompelete;

    UPROPERTY(meta=(BindWidget))
    class UMultiLineEditableText* Txt_Explane;

    UPROPERTY(EditAnywhere)
    FString RoomDescription;
    UPROPERTY(EditAnywhere)
    FString CurrentText;
    UPROPERTY(BlueprintReadWrite)
    int32 bPrivate = 0;
    UPROPERTY(EditAnywhere)
    int32 textSize = 10000;
    FTimerHandle Handler;

    UFUNCTION(BlueprintCallable)
    void SwitchToWidget(int32 index);  
    UFUNCTION(BlueprintCallable)
    void SwitchToWidget_PP(int32 index);
    UFUNCTION()
    void CreateRoomChooseYes();   
    UFUNCTION()
    void CreateRoomChooseNo();
    UFUNCTION()
    void CompleteCreateRoom();
    void DelayedSwitchToWidget();
    UFUNCTION()
    void SetPrivate();
    UFUNCTION(BlueprintCallable)
    void ShowUIForLimitedTime(float DurationInSeconds);   
    UFUNCTION()
    void HideUI();
    //Send CompleteRoom
    UFUNCTION()
	void SendCompleteRoomData();
    UFUNCTION()
    void OnTextChanged_SingleLine(const FText& Text);
   UFUNCTION()
    void OnTextChanged_MultiLine(const FText& Text);
    UFUNCTION()
    void OnTextCommitted_MultiLine(const FText& Text, ETextCommit::Type CommitMethod);



    //KGW ==================================
    UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CaptureImage;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_MyPage;
    UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Explanation;
    UPROPERTY(meta = (BindWidgetAnim),BlueprintReadWrite, Transient)
    class UWidgetAnimation* appear;   

        UPROPERTY(meta=(BindWidget))
    class UButton* btn_Good;

        UPROPERTY(meta=(BindWidget))
    class UButton* btn_Bad;

    UFUNCTION(BlueprintCallable)
    void OnClickCaptureImage();
    UFUNCTION(BlueprintCallable)
    void OnClikMypage();
    UFUNCTION(BlueprintCallable)
    void OnClikExplanation();

        UFUNCTION(BlueprintCallable)
    void OnClickGood();

        UFUNCTION(BlueprintCallable)
    void OnClikBad();


    UFUNCTION(BlueprintCallable)
    void SetExplanation(const FString& Text);




    //CJS ==================================
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