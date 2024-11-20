// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_ShowColorActor.generated.h"

UCLASS()
class HTTPTEST_API AJS_ShowColorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_ShowColorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* meshComp;

	// Function to update material color
    UFUNCTION()
    void SetMaterialColor(FLinearColor newColor);

	 //파티클 효과 ----------------------------------------------------------
    UPROPERTY(EditAnyWhere, Category = "VFX")
	class UNiagaraComponent* CircleNiagara;
    UFUNCTION()
    void DeactivateNiagaraEffect();
};
