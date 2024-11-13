// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JS_WidgetFunction.h"
#include "CJS_LoginActor.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API ACJS_LoginActor : public AJS_WidgetFunction
{
	GENERATED_BODY()

public:
	UFUNCTION()
    void ShowLoginUI();
    UFUNCTION()
    void HideLoginUI();
	
};
