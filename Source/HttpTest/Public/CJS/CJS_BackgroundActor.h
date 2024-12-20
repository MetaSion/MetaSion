// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CJS_BackgroundActor.generated.h"

UCLASS()
class HTTPTEST_API ACJS_BackgroundActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACJS_BackgroundActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString BGObject;

	UFUNCTION(BlueprintCallable)
	void SetBGObject();
	UFUNCTION(BlueprintCallable)
	FString GetBGObject();

};
