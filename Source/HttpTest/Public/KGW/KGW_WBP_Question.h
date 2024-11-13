// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KGW_WBP_Question.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UKGW_WBP_Question : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UButton* BT_Select;

	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickSelect();


		UPROPERTY()
	class UKGW_RoomList* Showlist;

	FString ListURL = "mirrora.duckdns.org:3326/api/auth/userRooms";
			UPROPERTY()
	class AHttpActor* HttpActor;

				UPROPERTY()
	class AKGW_RoomlistActor* ListActor;

					UPROPERTY()
	class AKGW_EnterRoomActor* EnterRoomActor;
						UPROPERTY()
	class ACJS_InnerWorldParticleActor* EffectActor;


	
	//늦게 나타나는 애니메이션 ------------------------------------------------------------
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* LateAppearAnim;
	UFUNCTION()
	void PlayLateAppearAnimation();


};
