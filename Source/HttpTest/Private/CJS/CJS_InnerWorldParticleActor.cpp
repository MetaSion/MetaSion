// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_InnerWorldParticleActor.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

// Sets default values
ACJS_InnerWorldParticleActor::ACJS_InnerWorldParticleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create DefaultSceneRoot and set it as the RootComponent
    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DefaultSceneRoot;

    // Initialize and position 6 Point Light Components
    FVector LightPositions[6] = {
        FVector(0.0f, 500.0f, 0.0f),   // Light 0
        FVector(0.0f, -500.0f, 0.0f),  // Light 1
        FVector(-500.0f, 0.0f, 0.0f),  // Light 2
        FVector(500.0f, 0.0f, 0.0f),   // Light 3
        FVector(0.0f, 0.0f, -500.0f),  // Light 4
        FVector(0.0f, 0.0f, 500.0f)    // Light 5
    };

    for (int32 i = 0; i < 6; ++i)
    {
        UPointLightComponent* PointLight = CreateDefaultSubobject<UPointLightComponent>(FName(*FString::Printf(TEXT("PointLight%d"), i)));
        PointLight->SetupAttachment(RootComponent);
        PointLight->SetRelativeLocation(LightPositions[i]);
        PointLights.Add(PointLight);
    }

    // Initialize Niagara System Component
    InnerWorldParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    InnerWorldParticle->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACJS_InnerWorldParticleActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACJS_InnerWorldParticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


    // 테스트용
    // Update elapsed time
    //ElapsedTime += DeltaTime;

    // Check if 2 seconds have passed
    //if (ElapsedTime >= 2.0f)
    //{
        // Reset the elapsed time
        //ElapsedTime = 0.0f;

        
        // 1. Update the color of the current Point Light
        //if (PointLights.IsValidIndex(CurrentColorIndex) && PointLights[CurrentColorIndex])
        //{
        //    FLinearColor NewColor = Colors[CurrentColorIndex % Colors.Num()];
        //    UpdateInnerWorldPointLights(NewColor, CurrentColorIndex);

        //    // Log the color change
        //    UE_LOG(LogTemp, Log, TEXT("PointLight%d color changed to (%f, %f, %f)"), CurrentColorIndex, NewColor.R, NewColor.G, NewColor.B);
        //}

        //// Move to the next Point Light and color
        //CurrentColorIndex = (CurrentColorIndex + 1) % PointLights.Num();

        // 2.Update the Niagara particle effect
        //UpdateInnerWorldNiagaraAsset(CurrentNiagaraIndex);

        // Move to the next Niagara asset index
        //CurrentNiagaraIndex = (CurrentNiagaraIndex + 1) % 2; // Assuming there are 2 particle assets (Particle_Num1, Particle_Num2)
    //}
}


void ACJS_InnerWorldParticleActor::UpdateInnerWorldPointLights(FLinearColor lightColor, int32 lightIndex)
{
    UE_LOG(LogTemp, Warning, TEXT("ACJS_InnerWorldParticleActor::UpdateInnerWorldPointLights()"));
    if (PointLights.IsValidIndex(lightIndex) && PointLights[lightIndex])
    {
        PointLights[lightIndex]->SetLightColor(lightColor);

        // Log the color change
        UE_LOG(LogTemp, Warning, TEXT("UpdateInnerWorldPointLights: PointLight%d color set to (%f, %f, %f)"), lightIndex, lightColor.R, lightColor.G, lightColor.B);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to Change light color"));
    }
}

void ACJS_InnerWorldParticleActor::UpdateInnerWorldNiagaraAsset(int32 NiagaraAssetIndex)
{
    UE_LOG(LogTemp, Warning, TEXT("ACJS_InnerWorldParticleActor::UpdateInnerWorldNiagaraAsset()"));
    if (InnerWorldParticle)
    {
		FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
        UE_LOG(LogTemp, Warning, TEXT("ACJS_InnerWorldParticleActor::UpdateInnerWorldNiagaraAsset() LevelName : %s"), *LevelName);
		FString AssetPath;
        if (LevelName.Contains("Winter"))
        {
            UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay() LevelName.Contains->Main_LV_Winter"));
            // Construct the asset path based on NiagaraAssetIndex
            AssetPath = FString::Printf(TEXT("/Content/Main/Assets/Emotions/Particle_Num%d.Particle_Num%d"), NiagaraAssetIndex, NiagaraAssetIndex);
        }
        else if (LevelName.Contains("Sky"))
        {
            UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay() LevelName.Contains->Main_Sky"));
            // Construct the asset path based on NiagaraAssetIndex
            AssetPath = FString::Printf(TEXT("/Content/Main/Assets/NoSound/Particle_Num%d.Particle_Num%d"), NiagaraAssetIndex, NiagaraAssetIndex);
        }

        // Load the Niagara System dynamically from the constructed path
        UNiagaraSystem* SelectedNiagaraSystem = Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), nullptr, *AssetPath));

        // Check if the asset was loaded successfully
        if (SelectedNiagaraSystem)
        {
            InnerWorldParticle->SetAsset(SelectedNiagaraSystem); // Set the asset to the NiagaraComponent
            InnerWorldParticle->Activate(); // Activate the Niagara system if needed
            UE_LOG(LogTemp, Warning, TEXT("ACJS_InnerWorldParticleActor::UpdateInnerWorldNiagaraAsset() Successfully set Niagara asset: %s"), *AssetPath);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("ACJS_InnerWorldParticleActor::UpdateInnerWorldNiagaraAsset() Failed to load Niagara asset at path: %s"), *AssetPath);
        }
    }
}

