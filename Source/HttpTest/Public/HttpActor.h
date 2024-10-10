// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpActor.generated.h"

USTRUCT(BlueprintType) 
struct FLoginStr
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Login/Struct")
	FString UserID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Login/Struct")
	FString UserPW;

	//�⺻ ������
	FLoginStr() : UserID(TEXT("")), UserPW(TEXT("")) {}
};

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

	UPROPERTY()
	class UHttpWidget* HttpUI;


	//Login -----------------------------------------------------------
	// ButtonSendPost�� ������ �� ��û�� �Լ�
	void ReqPostTest(FString url, FString json);
	// ������� �Լ�
	void OnResPostTest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	//Struct To Json : Req
	FString Login_Convert_StructToJson(FString userID, FString userPW);
	//Json To Struct : Response
	FLoginStr Login_Convert_JsonToStruct(const FString& JsonString);

	//Login End-----------------------------------------------------------

	// ButtonGetWebImage�� ������ �� ��û�� �Լ�
	void ReqGetWebImage(FString url);
	// ������� �Լ�
	void OnResGetWebImage(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// ���̹����� �鿣�� ������ �۽�
	void ReqGetWebImageToServer(FString url, TArray<uint8> resource);
	// ���̹����� �鿣�� �������� ����
	void OnResGetWebImageFromServer(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
};
