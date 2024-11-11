// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CJS/SessionGameInstance.h"
#include "KGW_RoomList.generated.h"

/**
 *
 */
UCLASS()
class HTTPTEST_API UKGW_RoomList : public UUserWidget
{
	GENERATED_BODY()

public:

	//  		UPROPERTY(meta=(BindWidget))
	// 	class UTextBlock* Text_Finding;
	virtual void  NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_RecomendRoom;

	UPROPERTY(meta = (BindWidget))
	class UMultiLineEditableTextBox* TxtBox_Report;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_InnerWorld;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_MultiWorld;

	

	void AddSessionSlotWidget(const TArray<FMyWorldRoomInfo>& RoomInfos);

	// 	void SetRecomendRoomName(const )

	// 	void SetFindActive(bool value);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UKGW_UserRoomName> UserRoomNameFactory;


	void SetTextLog(FString explain);

	UFUNCTION()
	void OnClickInnerWorld();

	UFUNCTION()
	void OnClickMultiWorld();

	FString WheatherNum;

	void SetWheaterNumb(FString TempNUmb);

	UPROPERTY(VisibleAnywhere)
	class AHttpActor* OpenActor;

		UPROPERTY(VisibleAnywhere)
	class UCJS_InnerWorldSettingWidget* InnerWorldWidget;


	

};
