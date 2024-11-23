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

    // 현재 스폰된 액터들을 추적
    UPROPERTY()
    AJS_ShowColorActor* CurrentBallActor;
    UPROPERTY()
    AActor* CurrentParticleActor;

    // 액터 수명 타이머
    FTimerHandle ActorLifetimeTimer;
    // 액션 타이밍을 위한 타이머 핸들
    FTimerHandle ActionTimerHandle;
    // 현재 재생 중인 애니메이션 인덱스를 추적
    int32 CurrentAnimationIndex;
    // 레벨 넘어갈 때 기다리는 타이머 핸들
    FTimerHandle LevelTimerHandle;

    // 액션 실행 함수
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
