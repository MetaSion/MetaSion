// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"
#include "Animation/WidgetAnimation.h"
#include "JS_ShowColorActor.h"
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
    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
 
    UPROPERTY(meta=(BindWidget))
    class UWidgetSwitcher* Ex_WidgetSwitcher;

    UPROPERTY(EditAnywhere)
    class AJS_RoomController* pc;

    UPROPERTY(EditAnywhere)
	class AHttpActor* httpActor;

    UPROPERTY(EditAnywhere)
	class USessionGameInstance* SessionGI;
    
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* txt_AIAnalysis;

    // Animation properties
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* TextAnimation_1;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* TextAnimation_2;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* TextAnimation_3;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* TextAnimation_4;

    // Spawn properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TSubclassOf<class AJS_ShowColorActor> SpawnShowColorActorFactory;

    // ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Íµï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
    UPROPERTY()
    AJS_ShowColorActor* CurrentBallActor;
    UPROPERTY()
    AActor* CurrentParticleActor;

    // ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ Å¸ï¿½Ì¸ï¿½
    FTimerHandle ActorLifetimeTimer;
    // ï¿½×¼ï¿½ Å¸ï¿½Ì¹ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ Å¸ï¿½Ì¸ï¿½ ï¿½Úµï¿½
    FTimerHandle ActionTimerHandle;
    // ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ö´Ï¸ï¿½ï¿½Ì¼ï¿½ ï¿½Îµï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
    int32 CurrentAnimationIndex;
    // ·¹º§ ³Ñ¾î°¥ ¶§ ±â´Ù¸®´Â Å¸ÀÌ¸Ó ÇÚµé
    FTimerHandle LevelTimerHandle;

    // ï¿½×¼ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ô¼ï¿½
    void ExecuteActionWithDelay();

     UPROPERTY(EditAnywhere, Category = "Animation Timing")
    float ActionDelay = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TSubclassOf<AActor> ParticleActorFactory;

    TArray<UWidgetAnimation*> Animations;
    int32 Nextindex = 0;

    // Action handlers
    void HandleActionForIndex(int32 index);
    void SpawnBall();
    void SpawnParticle();
    void ShowAIAnalysis();
    void ShowMyPage();
    void CleanupCurrentActors();

    //settext
    void SetAIAnalysis(const FString& Text);
};
