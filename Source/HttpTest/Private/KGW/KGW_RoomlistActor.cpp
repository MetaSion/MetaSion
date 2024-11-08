// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_RoomlistActor.h"

AKGW_RoomlistActor::AKGW_RoomlistActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AKGW_RoomlistActor::BeginPlay()
{
    Super::BeginPlay();

    FVector NewListLocation(0.0f, 0.0f, 0.0f);
    SetActorLocation(NewListLocation, true, nullptr, ETeleportType::TeleportPhysics);

    
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
        DynamicMaterial->SetVectorParameterValue(FName("TransmitB"), newColor); // "BaseColor"는 실제 파라미터 이름으로 교체해야 합니다
        UE_LOG(LogTemp, Warning, TEXT("Material color set to R: %f, G: %f, B: %f"), newColor.R, newColor.G, newColor.B);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create dynamic material instance for MeshComp."));
    }
}
