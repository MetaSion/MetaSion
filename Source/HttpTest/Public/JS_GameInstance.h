// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JS_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UJS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// ButtonSendPost�� ������ �� ��û�� �Լ�
	void ReqPostTest(FString url, FString json);
	// ������� �Լ�
	void OnResPostTest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	
	//// ���� ������ ���̳ʸ� �����ͷ� ��ȯ�ϴ� �Լ�
	//bool ReqBGMDateFromFile(const FString& FilePath, TArray<uint8>& OutBgmData);
	//// ���̳ʸ� �����͸� ������ �����ϴ� �Լ�
	//void SendBGMDataToServer(const TArray<uint8>& bgmData, const FString& serverUrl);
	//void UploadBGMData(const FString& BgmFilePath, const FString& ServerUrl);
};
