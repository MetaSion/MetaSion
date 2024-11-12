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

		UPROPERTY(meta=(BindWidget))
	class UScrollBox* ScrollBox;

	 		UPROPERTY(meta=(BindWidget))
	class UTextBlock* Text_RecomendRoom;




	void AddSessionSlotWidget(const TArray<FMyCreatedRoom>& RoomInfos);

// 	void SetRecomendRoomName(const )

// 	void SetFindActive(bool value);

		UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UKGW_UserRoomName> UserRoomNameFactory;






};
