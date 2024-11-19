// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_ExplainWidget.h"
#include "Components/WidgetSwitcher.h"
#include "JS_RoomController.h"
#include "Kismet/GameplayStatics.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"

void UJS_ExplainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	/*Ex_WidgetSwitcher->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::CreateRoomChooseYes);*/
    pc = Cast<AJS_RoomController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    //Animation array reset
    Animations.Add(TextAnimation_1);
    Animations.Add(TextAnimation_2);
    Animations.Add(TextAnimation_3);
    Animations.Add(TextAnimation_4);

    // Check the array size
    UE_LOG(LogTemp, Log, TEXT("Animations array size: %d"), Animations.Num());

    if (Ex_WidgetSwitcher && Animations.Num() > 0)
    {
        Ex_WidgetSwitcher->SetActiveWidgetIndex(0);
        if (Animations[0])
        {
            PlayAnimation(Animations[0]);
        }
        Nextindex = 1;
    }
}
void UJS_ExplainWidget::SwitchToWidget()
{
	if (Ex_WidgetSwitcher && Nextindex < Animations.Num()) {
        Ex_WidgetSwitcher->SetActiveWidgetIndex(Nextindex);
        if (Animations[Nextindex]) {
            PlayAnimation(Animations[Nextindex]); // Animation sequential play
        }
        HandleActionForIndex(Nextindex); // Check the NextIndex and Play Action  
        Nextindex++;
    }
    else {
        pc->HideExplainUI();
        GetWorld()->GetTimerManager().ClearTimer(UISwitchTimerHandle);
        Nextindex = 0;
    }
}

void UJS_ExplainWidget::NextSwitchWidget()
{
    if (pc && Nextindex < 4 && !GetWorld()->GetTimerManager().IsTimerActive(UISwitchTimerHandle)) // pc and Nextindex and Timer Check
    {
        pc->GetWorld()->GetTimerManager().SetTimer(UISwitchTimerHandle, this, &UJS_ExplainWidget::SwitchToWidget, 5.0f, true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GetWorld() returned nullptr! Ensure the widget is added to the viewport."));
    }
}

void UJS_ExplainWidget::HandleActionForIndex(int32 index)
{
    switch (index)
    {
    case 0:
        // 볼이 스폰되고 통신으로 받은 색상을 보여줌
        SpawnBall();
        break;
    case 1:
        // 파티클이 스폰되고 움직이는 걸 보여줌
        SpawnParticle();
        break;
    case 2:
        // 통신으로 받은 AI 분석내용을 순차적으로 보여줌
        ShowAIAnalysis();
        break;
    case 3:
        // 애니메이션이 끝날 때 마이페이지로 이동
        ShowMyPage();
        break;
    default:
        break;
    }
}

//Ball Spawn
void UJS_ExplainWidget::SpawnBall()
{
    FVector Location = FVector(-470047.589317, 643880.89814, 648118.610643);
    FRotator Rotation = FRotator(9.157953, 200.435537, 0.000001);
    AKGW_RoomlistActor* SpawnedActor = GetWorld()->SpawnActor<AKGW_RoomlistActor>(SpawnBallActorFactory, Location, Rotation);
    if (!SpawnedActor) {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn actor at location: %s"), *Location.ToString());
    }
}

void UJS_ExplainWidget::SpawnParticle()
{
    //if (!ParticleActorFactory) {
    //    UE_LOG(LogTemp, Error, TEXT("ParticleActorFactory is not set!"));
    //    return;
    //}

    //FVector Location = FVector(-470047.589317, 643880.89814, 648118.610643);
    //FRotator Rotation = FRotator::ZeroRotator;
    //FVector Scale = FVector(1.f);

    //// 파티클 액터 스폰
    //AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ParticleActorFactory, Location, Rotation);
    //if (SpawnedActor){
    //    UE_LOG(LogTemp, Log, TEXT("Successfully spawned particle actor."));
    //}
    //else {
    //    UE_LOG(LogTemp, Error, TEXT("Failed to spawn particle actor."));
    //}
}

void UJS_ExplainWidget::ShowAIAnalysis()
{
    
}

void UJS_ExplainWidget::ShowMyPage()
{
    
}
