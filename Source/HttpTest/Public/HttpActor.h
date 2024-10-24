// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpActor.generated.h"

UCLASS()
class HTTPTEST_API AHttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHttpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �������̽��� ���� UI
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> HttpUIFactory;

	/*UPROPERTY()
	class UHttpWidget* HttpUI;*/

	UPROPERTY()
	class UJS_CreateRoomWidget* CR_WidgetUI;
	/*// ButtonGetWebImage�� ������ �� ��û�� �Լ�
	void ReqGetWebImage(FString url);
	// ������� �Լ�
	void OnResGetWebImage(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// ���̹����� �鿣�� ������ �۽�
	void ReqGetWebImageToServer(FString url, TArray<uint8> resource);
	// ���̹����� �鿣�� �������� ����
	void OnResGetWebImageFromServer(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully*/
	
};
