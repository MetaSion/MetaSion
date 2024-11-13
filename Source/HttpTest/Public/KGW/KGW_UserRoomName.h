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
	virtual void  NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_EnterMyRoom;
 	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Text_RoomName;

	UFUNCTION()
	void UpdateInfo(const struct FMyWorldRoomInfo& info);
	UFUNCTION()
	void OnClickEnterMyRoom();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    class AHttpActor* HttpActor;
	// 방 번호를 저장하는 변수
	FString RoomNum;

};
