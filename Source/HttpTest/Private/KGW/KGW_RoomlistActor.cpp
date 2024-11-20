// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_RoomlistActor.h"
#include "CJS/SessionGameInstance.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CJS/CJS_InnerWorldParticleActor.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "Engine/TimerHandle.h"

AKGW_RoomlistActor::AKGW_RoomlistActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // 나이아가라 컴포넌트 추가
    CircleNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CircleNiagara"));
    CircleNiagara->SetupAttachment(meshComp);
    CircleNiagara->bAutoActivate = false; // 처음에는 비활성 상태로 설정
    //CircleNiagara->SetRelativeScale3D(FVector(.5f)); // 필요에 따라 크기 조정
}

void AKGW_RoomlistActor::BeginPlay()
{
    Super::BeginPlay();

    FVector NewListLocation(0.0f, 0.0f, 0.0f);
    SetActorLocation(NewListLocation, true, nullptr, ETeleportType::TeleportPhysics);   
    //HideMyWorldUI();
    EffectActor = Cast<ACJS_InnerWorldParticleActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACJS_InnerWorldParticleActor::StaticClass()));
    if (EffectActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::BeginPlay() Set EffectActor"));
        HideMyWorldParticle();
        HideMyWorldUI();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AKGW_RoomlistActor::BeginPlay() No EffectActor"));
    }

   
    // Circle 이펙트 나이아가라 컴포넌트 비활성화 후 활성화
    //if (CircleNiagara && CircleNiagara->IsActive())
    //{
    //    DeactivateNiagaraEffect();
    //}
    //FVector Location = meshComp->GetComponentLocation();
    //FRotator Rotation = meshComp->GetComponentRotation();
    //if (CircleNiagara) {
    //    // RootComponent의 위치와 회전에 Niagara 이펙트 재생
    //    CircleNiagara->SetWorldLocationAndRotation(Location, Rotation);
    //    CircleNiagara->Activate(); // Niagara 이펙트 활성화
    //    UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::DeactivateNiagaraEffect() CircleNiagara->Activate();"));
    //    // 일정 시간 후에 이펙트 비활성화
    //    FTimerHandle NiagaraTimerHandle;
    //    GetWorldTimerManager().SetTimer(NiagaraTimerHandle, this, &AKGW_RoomlistActor::DeactivateNiagaraEffect, 1.0f, false);
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Error, TEXT("AKGW_RoomlistActor::BeginPlay() No CircleNiagara"));
    //}
}

void AKGW_RoomlistActor::DeactivateNiagaraEffect()
{   
    UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::DeactivateNiagaraEffect()"));
    if (CircleNiagara && CircleNiagara->IsActive())
    {
        CircleNiagara->Deactivate();
        UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::DeactivateNiagaraEffect() CircleNiagara->Deactivate();"));
    }
}

void AKGW_RoomlistActor::SetMaterialColor(FLinearColor newColor)
{
    UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::SetMaterialColor()"));
    if (!meshComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("MeshComp is not valid."));
        return;
    }

    // Create a dynamic material instance and set the color
    UMaterialInstanceDynamic* DynamicMaterial = meshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, meshComp->GetMaterial(0));
    if (DynamicMaterial)
    {
        //DynamicMaterial->SetVectorParameterValue(FName("TransmitB"), newColor); // "BaseColor"는 실제 파라미터 이름으로 교체해야 합니다
        DynamicMaterial->SetVectorParameterValue(FName("InnerColor"), newColor); // "BaseColor"는 실제 파라미터 이름으로 교체해야 합니다
        UE_LOG(LogTemp, Warning, TEXT("Material color set to R: %f, G: %f, B: %f"), newColor.R, newColor.G, newColor.B);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create dynamic material instance for MeshComp."));
    }
}

void AKGW_RoomlistActor::ShowMyWorldUI()
{
    UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::ShowMyWorldUI()"));
	/*USessionGameInstance* GameInstance = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance)
    {
        if(!GameInstance->bmyWorldPageOn)
        {
            GameInstance->bmyWorldPageOn = true;
            UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::ShowMyWorldUI() GameInstance->bmyWorldPageOn = true"));
       */
            // 전체 보이도록 설정하기
            if (boxComp) boxComp->SetVisibility(true);
            if (meshComp) meshComp->SetVisibility(true);
            if (BillboardComp) BillboardComp->SetVisibility(true);
            if (WidgetComponent) WidgetComponent->SetVisibility(true);
            UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::ShowMyWorldUI() - All components are now visible"));
    /*    }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AKGW_RoomlistActor::ShowMyWorldUI() Failed to !GameInstance->bmyWorldPageOn"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AKGW_RoomlistActor::ShowMyWorldUI() Failed to GameInstance"));
    }*/
}
void AKGW_RoomlistActor::HideMyWorldUI()
{
    UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::HideMyWorldUI()"));
   /* USessionGameInstance* GameInstance = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance)
    {
        if (!GameInstance->bmyWorldPageOn)
        {
            GameInstance->bmyWorldPageOn = true;*/
            //UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::ShowMyWorldUI() GameInstance->bmyWorldPageOn = true"));

            // 전체 보이도록 설정하기
            if (boxComp) boxComp->SetVisibility(false);
            if (meshComp) meshComp->SetVisibility(false);
            if (BillboardComp) BillboardComp->SetVisibility(false);
            if (WidgetComponent) WidgetComponent->SetVisibility(false);
            UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::HideMyWorldUI() - All components are now hidden"));
   /*     }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AKGW_RoomlistActor::ShowMyWorldUI() Failed to !GameInstance->bmyWorldPageOn"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AKGW_RoomlistActor::ShowMyWorldUI() Failed to GameInstance"));
    }*/
}

void AKGW_RoomlistActor::ShowMyWorldParticle()
{
    UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::ShowMyWorldParticle()"));
    if (EffectActor)
    {
        EffectActor->SetActorHiddenInGame(false);
        UE_LOG(LogTemp, Warning, TEXT("EffectActor is now hidden"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AKGW_RoomlistActor::BeginPlay() No EffectActor"));
    }
}
void AKGW_RoomlistActor::HideMyWorldParticle()
{
    UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::HideMyWorldParticle()"));    
    if (EffectActor)
    {
        EffectActor->SetActorHiddenInGame(true);
        UE_LOG(LogTemp, Warning, TEXT("EffectActor is now hidden"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AKGW_RoomlistActor::BeginPlay() No EffectActor"));
    }
}
