// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JS_WidgetFunction.h"
#include "KGW_RoomlistActor.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API AKGW_RoomlistActor : public AJS_WidgetFunction
{
	GENERATED_BODY()
	
public:
    // Sets default values for this actor's properties
    AKGW_RoomlistActor();

protected:
    virtual void BeginPlay() override;

public:
    // Function to update material color
    void SetMaterialColor(FLinearColor newColor);

};
