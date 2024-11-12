// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CJS_InnerWorldParticleActor.generated.h"

UCLASS()
class HTTPTEST_API ACJS_InnerWorldParticleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACJS_InnerWorldParticleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Root Component
	UPROPERTY(VisibleAnywhere, Category = "Particle")
	class USceneComponent* DefaultSceneRoot;

    // Array to hold the Point Light components
    UPROPERTY(VisibleAnywhere, Category = "Particle")
    TArray<class UPointLightComponent*> PointLights;

    // Niagara System Component
    UPROPERTY(VisibleAnywhere, Category = "Particle")
    class UNiagaraComponent* InnerWorldParticle;
	 // Array to hold multiple Niagara System assets
    UPROPERTY(EditAnywhere, Category = "Particle")
    TArray<class UNiagaraSystem*> NiagaraSystemAssets;


	// Function to update Point Light colors and Niagara System
	void UpdateInnerWorldPointLights(FLinearColor lightColor, int32 lightIndex);
	void UpdateInnerWorldNiagaraAsset(int32 NiagaraAssetIndex);



	// 테스트용
	float ElapsedTime = 0.0f;
	int32 CurrentColorIndex = 0;
	TArray<FLinearColor> Colors = { FLinearColor::Red, FLinearColor::Blue, FLinearColor::Green };

	int32 CurrentNiagaraIndex = 0;
};
