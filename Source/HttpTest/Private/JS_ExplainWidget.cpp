// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_ExplainWidget.h"
#include "Components/WidgetSwitcher.h"
#include "JS_RoomController.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "JS_ShowColorActor.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "HttpActor.h"
#include "CJS/SessionGameInstance.h"

void UJS_ExplainWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Initialize animations array
    Animations = {
        TextAnimation_1,
        TextAnimation_2,
        TextAnimation_3,
        TextAnimation_4
    };

    CurrentBallActor = nullptr;
    CurrentParticleActor = nullptr;
    CurrentAnimationIndex = 0;

    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    if (Ex_WidgetSwitcher && !Animations.IsEmpty() && LevelName == "Test_Main_Sky")
    {
        Ex_WidgetSwitcher->SetActiveWidgetIndex(0);
        if (Animations[0])
        {
            CurrentAnimationIndex = 0;
            PlayAnimation(Animations[0]);

            // 애니메이션 시작 1초 후에 액션 실행
            GetWorld()->GetTimerManager().SetTimer(
                ActionTimerHandle,
                this,
                &UJS_ExplainWidget::ExecuteActionWithDelay,
                ActionDelay,
                false);

            Nextindex = 1;
        }
    }

    FString JsonString = TEXT("{\"values\": [3, 2, 1, \"testuser\"]}");
    httpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
    httpActor->ReqPostChoice(httpActor->EnteryLobbyURL, JsonString);

    SessionGI = Cast<USessionGameInstance>(GetGameInstance());
 }

void UJS_ExplainWidget::ExecuteActionWithDelay()
{
    // 현재 애니메이션 인덱스에 해당하는 액션 실행
    HandleActionForIndex(CurrentAnimationIndex);
}

void UJS_ExplainWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    // 현재 액터들을 정리
    CleanupCurrentActors();

    if (Nextindex < Animations.Num())
    {
        if (Ex_WidgetSwitcher)
        {
            Ex_WidgetSwitcher->SetActiveWidgetIndex(Nextindex);
        }

        if (Animations[Nextindex])
        {
            CurrentAnimationIndex = Nextindex;
            PlayAnimation(Animations[Nextindex]);

            // 다음 애니메이션 시작 1초 후에 액션 실행
            GetWorld()->GetTimerManager().SetTimer(
                ActionTimerHandle,
                this,
                &UJS_ExplainWidget::ExecuteActionWithDelay,
                ActionDelay,
                false);

            Nextindex++;
        }
    }
    else
    {
        if (pc)
        {
            pc->HideExplainUI();
        }
        Nextindex = 0;
        CurrentAnimationIndex = 0;
    }
}

void UJS_ExplainWidget::HandleActionForIndex(int32 index)
{
    switch (index)
    {
    case 0:
        SpawnBall();
        break;
    case 1:
        SpawnParticle();
        break;
    case 2:
        ShowAIAnalysis();
        break;
    case 3:
        ShowMyPage();
        break;
    default:
        break;
    }
}

//Ball Spawn
void UJS_ExplainWidget::SpawnBall()
{
    if (!SpawnShowColorActorFactory)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnShowColorActorFactory is not set!"));
        return;
    }

    // 이전 액터 정리
    if (CurrentBallActor)
    {
        CurrentBallActor->Destroy();
        CurrentBallActor = nullptr;
    }

    FVector Location = FVector(-470270, 643800, 648140);
    FRotator Rotation = FRotator::ZeroRotator;

    CurrentBallActor = GetWorld()->SpawnActor<AJS_ShowColorActor>(
        SpawnShowColorActorFactory, Location, Rotation);

    if (CurrentBallActor)
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully spawned ball at Location: %s"), *Location.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn ball!"));
    }
}

void UJS_ExplainWidget::SpawnParticle()
{
    if (!ParticleActorFactory)
    {
        UE_LOG(LogTemp, Error, TEXT("ParticleActorFactory is not set!"));
        return;
    }

    // 이전 액터 정리
    if (CurrentParticleActor)
    {
        CurrentParticleActor->Destroy();
        CurrentParticleActor = nullptr;
    }

    FVector Location = FVector(-470990.0f, 643490.0f, 648180.0f);
    FRotator Rotation = FRotator::ZeroRotator;

    CurrentParticleActor = GetWorld()->SpawnActor<AActor>(
        ParticleActorFactory, Location, Rotation);

    if (CurrentParticleActor)
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully spawned particle actor"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn particle actor"));
    }
}

void UJS_ExplainWidget::ShowAIAnalysis()
{
    UE_LOG(LogTemp, Error, TEXT("In SHowAIAnalysis"));
    SetAIAnalysis(SessionGI->AIResult);
}

void UJS_ExplainWidget::ShowMyPage()
{
    UGameplayStatics::OpenLevel(this, FName("Main_Sky"));
}

void UJS_ExplainWidget::CleanupCurrentActors()
{
    // 기존 액터 제거
    if (CurrentBallActor)
    {
        CurrentBallActor->Destroy();
        CurrentBallActor = nullptr;
    }

    if (CurrentParticleActor)
    {
        CurrentParticleActor->Destroy();
        CurrentParticleActor = nullptr;
    }

    // 타이머가 아직 실행 중이라면 정리
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(ActionTimerHandle);
    }
}

void UJS_ExplainWidget::SetAIAnalysis(const FString& Text)
{
    txt_AIAnalysis->SetText(FText::FromString(Text));
}
