// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KGW/KGW_RoomlistActor.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"
#include "JS_ExplainWidget.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UJS_ExplainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
    class UWidgetSwitcher* Ex_WidgetSwitcher;

	UPROPERTY(EditAnywhere)
	class AJS_RoomController* pc;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* TextAnimation_1;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* TextAnimation_2;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* TextAnimation_3;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* TextAnimation_4;

	// Spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<class AKGW_RoomlistActor> SpawnBallActorFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TSubclassOf<AActor> ParticleActorFactory;

	TArray<UWidgetAnimation*> Animations;
	/*UPROPERTY(meta=(BindWidget))
    class UButton* btn_CreateRoom_Public;*/
	FTimerHandle UISwitchTimerHandle;
	int32 Nextindex = 0;

	// switchWidgetAnimation
	void SwitchToWidget();
	UFUNCTION(BlueprintCallable)
    void NextSwitchWidget();

	// HandleIndex
	void HandleActionForIndex(int32 index);
	void SpawnBall();
	void SpawnParticle();
	void ShowAIAnalysis();
	void ShowMyPage();
};
