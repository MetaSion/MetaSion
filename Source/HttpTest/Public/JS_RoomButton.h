// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "JS_RoomButton.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UJS_RoomButton : public UButton
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
	class UKGW_RoomList* RL;

	int32 RoomNum = 1;

	void Initialize();
	void SetIndex(int32 NewIndex);
	int32 GetIndex() const;
	UFUNCTION()
	void OnImageClicked();

	int32 LastBtnIdx = -1;
	
};
