// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_GameInstance.h"
#include "HttpModule.h"
#include "JsonParseLib.h"
#include "JsonObjectConverter.h"
#include "KGW/KGW_ChoiceSaveBF.h"

void UJS_GameInstance::ReqPostTest(FString url, FString json)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// ��û�� ������ ����
	req->SetURL(url);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(json);

	// ������� �Լ��� ����
	req->OnProcessRequestComplete().BindUObject(this, &UJS_GameInstance::OnResPostTest);
	// ������ ��û
	req->ProcessRequest();
}
void UJS_GameInstance::OnResPostTest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    // ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // ������ ���ڿ��� ��������
        FString result = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);

        // JSON�� �Ľ��Ͽ� FJsonObject�� ��ȯ
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(result);
        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            // "login" �ʵ带 ����ü�� ��ȯ
            if (JsonObject->HasTypedField<EJson::Object>(TEXT("login")))
            {
                TSharedPtr<FJsonObject> LoginObject = JsonObject->GetObjectField(TEXT("login"));
                FLogin loginStruct;
                FJsonObjectConverter::JsonObjectToUStruct(LoginObject.ToSharedRef(), FLogin::StaticStruct(), &loginStruct, 0, 0);
                UE_LOG(LogTemp, Warning, TEXT("UserID: %s, UserPW: %s"), *loginStruct.UserID, *loginStruct.UserPW);
            }

            // "sign_up" �ʵ带 ����ü�� ��ȯ
            if (JsonObject->HasTypedField<EJson::Object>(TEXT("sign_up")))
            {
                TSharedPtr<FJsonObject> SignUpObject = JsonObject->GetObjectField(TEXT("sign_up"));
                FSign_up signUpStruct;
                FJsonObjectConverter::JsonObjectToUStruct(SignUpObject.ToSharedRef(), FSign_up::StaticStruct(), &signUpStruct, 0, 0);
                UE_LOG(LogTemp, Warning, TEXT("UserNickName: %s, UserID: %s, UserPW: %s"), *signUpStruct.UserNickName, *signUpStruct.UserID, *signUpStruct.UserPW);
            }

            // "user" �ʵ带 ����ü�� ��ȯ
            if (JsonObject->HasTypedField<EJson::Object>(TEXT("user")))
            {
                TSharedPtr<FJsonObject> UserObject = JsonObject->GetObjectField(TEXT("user"));
                FUser userStruct;
                FJsonObjectConverter::JsonObjectToUStruct(UserObject.ToSharedRef(), FUser::StaticStruct(), &userStruct, 0, 0);
                UE_LOG(LogTemp, Warning, TEXT("UserID: %s, MoodScore: %d"), *userStruct.UserID, userStruct.MoodScore);
            }

            // "user_like" �ʵ带 ����ü�� ��ȯ
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
void UJS_GameInstance::ReqPostChoice(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // ������� �Լ��� ����
    req->OnProcessRequestComplete().BindUObject(this, &UJS_GameInstance::OnResPostChoice);
    // ������ ��û

    req->ProcessRequest();


}
void UJS_GameInstance::OnResPostChoice(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (bConnectedSuccessfully && Response.IsValid())
    {
        // ���������� ������ �޾��� ��
        FString ResponseContent = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("POST Response: %s"), *ResponseContent);

        // JSON ���信�� RGB ������ �Ľ�
        TArray<FColorData> Colors = UKGW_ChoiceSaveBF::ParseJsonToRGB(ResponseContent);

//         // �Ľ̵� RGB ���� ���� ��� ó��
//         if (Colors.Num() > 0)
//         {
//             // ù ��° RGB �� ��� ���� (��Ƽ���� ����)
//             FColorData FirstColor = Colors[0];
// 
//             UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CharacterMaterial, this);
//             if (DynamicMaterial)
//             {
//                 DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor(FirstColor.R, FirstColor.G, FirstColor.B));
//                 CharacterMesh->SetMaterial(0, DynamicMaterial);
//             }
// 
//             // �߰� ó�� ����
//         }
    }
    else
    {
        // ��û�� �������� ��
        UE_LOG(LogTemp, Warning, TEXT("POST Request Failed"));
    }
}




// ���� ������ ���̳ʸ� �����ͷ� ��ȯ�ϴ� �Լ�-----------------------------------------
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
////���̳ʸ� �����͸� ������ �����ϴ� �Լ�
//void UJS_GameInstance::SendBGMDataToServer(const TArray<uint8>& bgmData, const FString& serverUrl)
//{
//	//HTTP ��û ����
//	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
//	Request->SetURL(serverUrl);
//	Request->SetVerb("POST");
//	//���̳ʸ� ������ ����
//	Request->SetHeader(TEXT("content-type"), TEXT("application/octet-stream"));
//	Request->SetContent(bgmData);
//
//	//��û �Ϸ� �ݹ�
//	Request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
//		if (bWasSuccessful && Response.IsValid()) {
//			UE_LOG(LogTemp, Log, TEXT("Successfully sent voice data to server"));
//		}
//		else {
//			UE_LOG(LogTemp, Error, TEXT("Failed to send voice data to server"));
//		}
//		});
//
//	//��û ����
//	Request->ProcessRequest();
//}
//
//void UJS_GameInstance::UploadBGMData(const FString& BgmFilePath, const FString& ServerUrl)
//{
//	TArray<uint8> bgmData;
//
//	//���� ���� �ε�
//	if (ReqBGMDateFromFile(BgmFilePath, bgmData)) {
//		//������ ����
//		SendBGMDataToServer(bgmData, ServerUrl);
//	}
//	else {
//		UE_LOG(LogTemp, Error, TEXT("Voice data upload failed due to file loading error"));
//	}
//}