// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextRenderActor.h"
#include "CJS_TextRenderActor.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API ACJS_TextRenderActor : public ATextRenderActor
{
	GENERATED_BODY()

public:
	// 텍스트를 동적으로 변경하는 함수 선언
	UFUNCTION(BlueprintCallable, Category="Text")
	void SetText(const FString& NewText);
	
};
