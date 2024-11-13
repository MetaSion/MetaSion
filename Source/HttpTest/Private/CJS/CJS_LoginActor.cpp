// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_LoginActor.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"


void ACJS_LoginActor::ShowLoginUI()
{
    UE_LOG(LogTemp, Warning, TEXT("ACJS_LoginActor::ShowMyWorldUI()"));
    // 전체 보이도록 설정하기
    if (boxComp) boxComp->SetVisibility(true);
    if (meshComp) meshComp->SetVisibility(true);
    if (BillboardComp) BillboardComp->SetVisibility(true);
    if (WidgetComponent) WidgetComponent->SetVisibility(true);
    UE_LOG(LogTemp, Warning, TEXT("ACJS_LoginActor::ShowMyWorldUI() - All components are now visible"));
}

void ACJS_LoginActor::HideLoginUI()
{
    UE_LOG(LogTemp, Warning, TEXT("ACJS_LoginActor::HideMyWorldUI()"));
    // 전체 안 보이도록 설정하기
    if (boxComp) boxComp->SetVisibility(false);
    if (meshComp) meshComp->SetVisibility(false);
    if (BillboardComp) BillboardComp->SetVisibility(false);
    if (WidgetComponent) WidgetComponent->SetVisibility(false);
    UE_LOG(LogTemp, Warning, TEXT("ACJS_LoginActor::HideMyWorldUI() - All components are now hidden"));
}
