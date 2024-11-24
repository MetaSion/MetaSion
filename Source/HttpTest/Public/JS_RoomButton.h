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
	UJS_RoomButton();
	FString RoomNum = "";
	
	void SetIndex(FString NewIndex);
	FString GetIndex() const;
};
