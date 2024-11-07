// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KGW_UserRoomName.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UKGW_UserRoomName : public UUserWidget
{
	GENERATED_BODY()

public:

 		UPROPERTY(meta=(BindWidget))
	class UTextBlock* Text_RoomName;

	void UpdateInfo(const struct FMyWorldRoomInfo& info);

};
