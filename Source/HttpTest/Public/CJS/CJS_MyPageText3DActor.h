// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Text3DActor.h"
#include "CJS_MyPageText3DActor.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API ACJS_MyPageText3DActor : public AText3DActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Text")
	void CallMyPageActor();
	
};
