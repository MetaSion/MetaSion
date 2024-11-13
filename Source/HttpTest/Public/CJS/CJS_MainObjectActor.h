// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CJS_MainObjectActor.generated.h"

UCLASS()
class HTTPTEST_API ACJS_MainObjectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACJS_MainObjectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString TreeObject;

	UFUNCTION(BlueprintCallable)
	void SetMainObject();
	UFUNCTION(BlueprintCallable)
	FString GetMainObject();

};
