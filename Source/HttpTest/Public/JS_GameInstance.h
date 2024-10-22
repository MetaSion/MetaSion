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
	// ButtonSendPost를 눌렀을 때 요청할 함수
	void ReqPostTest(FString url, FString json);
	// 응답받을 함수
	void OnResPostTest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	
	//// 음성 파일을 바이너리 데이터로 변환하는 함수
	//bool ReqBGMDateFromFile(const FString& FilePath, TArray<uint8>& OutBgmData);
	//// 바이너리 데이터를 서버로 전송하는 함수
	//void SendBGMDataToServer(const TArray<uint8>& bgmData, const FString& serverUrl);
	//void UploadBGMData(const FString& BgmFilePath, const FString& ServerUrl);
};
