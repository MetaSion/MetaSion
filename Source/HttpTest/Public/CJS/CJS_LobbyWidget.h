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
	UPROPERTY(meta = (BindWidgetAnim), Transient)
    class UWidgetAnimation* SlideLeft;   
	UPROPERTY(meta = (BindWidgetAnim), Transient)
    class UWidgetAnimation* SlideRight;

	FTimerHandle HideRefWorldInfoHandler;

	UFUNCTION()
	void SetRefWorldInfo();
	UFUNCTION()
	void ShowLobbyUIZeroOrder();
	UFUNCTION()
	void HideLobbyUIZeroOrder();

	

	// --------------------------------------------------------------------------



	// List Panel ----------------------------------------------------------------
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* Scroll_List;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_RecList;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_MyWorld;
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* Box_List;
    
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UCJS_ChatTextWidget> TextWidgetFactory;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
    class UWidgetAnimation* BtnSlide;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
    class UWidgetAnimation* ListUp;

	bool SetListUp = false;

	UFUNCTION()
	void ShowLobbyUIFirstOrder();
	UFUNCTION()
	void HideLobbyUIFirstOrder();

	UFUNCTION()
    void OnClickMyWorld();
    UFUNCTION()
    void OnClickRecList();

	UFUNCTION()
    void SetRecList();


	
	// ---------------------------------------------------------------------------


};
