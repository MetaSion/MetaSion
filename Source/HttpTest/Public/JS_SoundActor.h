// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_SoundActor.generated.h"

UCLASS()
class HTTPTEST_API AJS_SoundActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_SoundActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class USessionGameInstance* SessionGI;
	UPROPERTY(EditAnywhere)
	class AHttpActor* HttpActor;

	UPROPERTY()
	UAudioComponent* AudioComponent;  // ���带 ����ϴ� ����� ������Ʈ

	// �������� ���� ���� �̸��� ���� ������� �����ϴ� �Լ�
	void SetBackgroundSoundByFileName(const FString& FileName);

	UFUNCTION()
    void OnAIRecommendationInitialized(); // RoomData �ʱ�ȭ �� ȣ��Ǵ� �Լ�

};
