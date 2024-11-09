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

    UPROPERTY()
	class ACJS_InnerWorldParticleActor* EffectActor;
    
    // Function to update material color
    UFUNCTION()
    void SetMaterialColor(FLinearColor newColor);

    UFUNCTION()
    void ShowMyWorldUI();
    UFUNCTION()
    void HideMyWorldUI();

    UFUNCTION()
    void ShowMyWorldParticle();
    UFUNCTION()
    void HideMyWorldParticle();

};
