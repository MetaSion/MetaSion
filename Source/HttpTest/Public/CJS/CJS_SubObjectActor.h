// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CJS_SubObjectActor.generated.h"

UCLASS()
class HTTPTEST_API ACJS_SubObjectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACJS_SubObjectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString FlowerObject;

	//UFUNCTION()
	//void SetSubObject(FString value);
	UFUNCTION(BlueprintCallable)
	void SetSubObject();
	UFUNCTION(BlueprintCallable)
	FString GetSubObject();
	UFUNCTION(BlueprintCallable)
	FString InitSubObject(FString value);
};
