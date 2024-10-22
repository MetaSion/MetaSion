// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_GameInstance.h"
#include "HttpModule.h"
#include "JsonParseLib.h"
#include "JsonObjectConverter.h"

void UJS_GameInstance::ReqPostTest(FString url, FString json)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// 요청할 정보를 설정
	req->SetURL(url);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(json);

	// 응답받을 함수를 연결
	req->OnProcessRequestComplete().BindUObject(this, &UJS_GameInstance::OnResPostTest);
	// 서버에 요청
	req->ProcessRequest();
}
void UJS_GameInstance::OnResPostTest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    // 요청이 성공적으로 완료되었는지 확인
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // 응답을 문자열로 가져오기
        FString result = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);

        // JSON을 파싱하여 FJsonObject로 변환
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(result);
        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            // "login" 필드를 구조체로 변환
            if (JsonObject->HasTypedField<EJson::Object>(TEXT("login")))
            {
                TSharedPtr<FJsonObject> LoginObject = JsonObject->GetObjectField(TEXT("login"));
                FLogin loginStruct;
                FJsonObjectConverter::JsonObjectToUStruct(LoginObject.ToSharedRef(), FLogin::StaticStruct(), &loginStruct, 0, 0);
                UE_LOG(LogTemp, Warning, TEXT("UserID: %s, UserPW: %s"), *loginStruct.UserID, *loginStruct.UserPW);
            }

            // "sign_up" 필드를 구조체로 변환
            if (JsonObject->HasTypedField<EJson::Object>(TEXT("sign_up")))
            {
                TSharedPtr<FJsonObject> SignUpObject = JsonObject->GetObjectField(TEXT("sign_up"));
                FSign_up signUpStruct;
                FJsonObjectConverter::JsonObjectToUStruct(SignUpObject.ToSharedRef(), FSign_up::StaticStruct(), &signUpStruct, 0, 0);
                UE_LOG(LogTemp, Warning, TEXT("UserNickName: %s, UserID: %s, UserPW: %s"), *signUpStruct.UserNickName, *signUpStruct.UserID, *signUpStruct.UserPW);
            }

            // "user" 필드를 구조체로 변환
            if (JsonObject->HasTypedField<EJson::Object>(TEXT("user")))
            {
                TSharedPtr<FJsonObject> UserObject = JsonObject->GetObjectField(TEXT("user"));
                FUser userStruct;
                FJsonObjectConverter::JsonObjectToUStruct(UserObject.ToSharedRef(), FUser::StaticStruct(), &userStruct, 0, 0);
                UE_LOG(LogTemp, Warning, TEXT("UserID: %s, MoodScore: %d"), *userStruct.UserID, userStruct.MoodScore);
            }

            // "user_like" 필드를 구조체로 변환
            if (JsonObject->HasTypedField<EJson::Object>(TEXT("user_like")))
            {
                TSharedPtr<FJsonObject> UserLikeObject = JsonObject->GetObjectField(TEXT("user_like"));
                FUser_like userLikeStruct;
                FJsonObjectConverter::JsonObjectToUStruct(UserLikeObject.ToSharedRef(), FUser_like::StaticStruct(), &userLikeStruct, 0, 0);
                UE_LOG(LogTemp, Warning, TEXT("Feedback: %s, FeedbackTime: %s"), *userLikeStruct.Feedback, *userLikeStruct.FeedbackTime.ToString());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to parse JSON..."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}


// 음성 파일을 바이너리 데이터로 변환하는 함수-----------------------------------------
//bool UJS_GameInstance::ReqBGMDateFromFile(const FString& FilePath, TArray<uint8>& OutBgmData)
//{
//	if (FFileHelper::LoadFileToArray(OutBgmData, *FilePath)) {
//		UE_LOG(LogTemp, Log, TEXT("Successfully loaded voice data from: %s"), *FilePath);
//		return true;
//	}
//	else {
//		UE_LOG(LogTemp, Error, TEXT("Failed to load voice data from: %s"), *FilePath);
//		return false;
//	}
//
//}
////바이너리 데이터를 서버로 전송하는 함수
//void UJS_GameInstance::SendBGMDataToServer(const TArray<uint8>& bgmData, const FString& serverUrl)
//{
//	//HTTP 요청 생성
//	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
//	Request->SetURL(serverUrl);
//	Request->SetVerb("POST");
//	//바이너리 데이터 설정
//	Request->SetHeader(TEXT("content-type"), TEXT("application/octet-stream"));
//	Request->SetContent(bgmData);
//
//	//요청 완료 콜백
//	Request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
//		if (bWasSuccessful && Response.IsValid()) {
//			UE_LOG(LogTemp, Log, TEXT("Successfully sent voice data to server"));
//		}
//		else {
//			UE_LOG(LogTemp, Error, TEXT("Failed to send voice data to server"));
//		}
//		});
//
//	//요청 전송
//	Request->ProcessRequest();
//}
//
//void UJS_GameInstance::UploadBGMData(const FString& BgmFilePath, const FString& ServerUrl)
//{
//	TArray<uint8> bgmData;
//
//	//음성 파일 로드
//	if (ReqBGMDateFromFile(BgmFilePath, bgmData)) {
//		//서버로 전송
//		SendBGMDataToServer(bgmData, ServerUrl);
//	}
//	else {
//		UE_LOG(LogTemp, Error, TEXT("Voice data upload failed due to file loading error"));
//	}
//}