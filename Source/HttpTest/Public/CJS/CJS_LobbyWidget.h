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
	class AJS_RoomController* pc;

	// Widget Switcher -----------------------------------------------------
	UPROPERTY(meta=(BindWidget))
    class UWidgetSwitcher* Lobby_WidgetSwitcher;
	UFUNCTION()
    void SwitchToWidget(int32 index);  

	// -----------------------------------------------------------------------


	// Info Panel -------------------------------------------------------------
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
	UPROPERTY(meta = (BindWidgetAnim),BlueprintReadWrite, Transient)
    class UWidgetAnimation* SlideLeft;   
	UPROPERTY(meta = (BindWidgetAnim),BlueprintReadWrite, Transient)
    class UWidgetAnimation* SlideRight;

	//UFUNCTION()
	void SetRefWorldInfo();
	//void ShowRefWorldInfo();
	//UFUNCTION()
	//void HideRefWorldInfo();

	// --------------------------------------------------------------------------



	// List Panel ----------------------------------------------------------------
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* Scroll_List;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_RecList;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_MyPage;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCJS_ChatTextWidget> TextWidgetFactory;

	UPROPERTY(meta = (BindWidgetAnim),BlueprintReadWrite, Transient)
    class UWidgetAnimation* BtnSlide;
	UPROPERTY(meta = (BindWidgetAnim),BlueprintReadWrite, Transient)
    class UWidgetAnimation* ListUp;

	//UFUNCTION()
	//void AddRefWolrdList();

	//UFUNCTION()
    void OnClickMypage();
    //UFUNCTION()
    void OnClickRecList();
	
	// ---------------------------------------------------------------------------



	// World Panel ---------------------------------------------------------------
	 UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Info;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_WallPaper;
    UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Lobby;

	UPROPERTY()
    class USessionGameInstance* SessionGI;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCJS_ChatWidget* ChatUI;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCJS_ChatTextWidget> ChatTextWidgetFactory;


	//UFUNCTION()
    void OnClickInfo();
    //UFUNCTION()
    void OnClickWallPaper();
    //UFUNCTION()
    void OnClickLobby();
    UFUNCTION()
    void SetInfomation(const FString& Text);

    //UFUNCTION()
    void HandleSendButtonClicked();
    UFUNCTION()
    void AddChatMessage(const FString& msg);
	
	// ---------------------------------------------------------------------------
};
