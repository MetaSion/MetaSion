// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_GameInstance.h"
#include "HttpModule.h"
#include "JsonParseLib.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "KGW/KGW_ChoiceSaveBF.h"

//Login -------------------------------------------------------------
void UJS_GameInstance::LoginReqPost(FString url, FString json)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// ��û�� ������ ����
	req->SetURL(url);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(json);

	// ������� �Լ��� ����
	req->OnProcessRequestComplete().BindUObject(this, &UJS_GameInstance::LoginResPost);
	// ������ ��û
	req->ProcessRequest();
}
void UJS_GameInstance::LoginResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    // ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // ������ ���ڿ��� ��������
        FString result = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);
        UJsonParseLib::Login_Convert_JsonToStruct(result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
//Login End-------------------------------------------------------------

//Sign UP -------------------------------------------------------------
void UJS_GameInstance::SignUpReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // ������� �Լ��� ����
    req->OnProcessRequestComplete().BindUObject(this, &UJS_GameInstance::SignUpResPost);
    // ������ ��û
    req->ProcessRequest();
}

void UJS_GameInstance::SignUpResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    // ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // ������ ���ڿ��� ��������
        FString result = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);
        UJsonParseLib::SignUp_Convert_JsonToStruct(result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}

//Sign UP End -------------------------------------------------------------

//User -------------------------------------------------------------
void UJS_GameInstance::UserReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // ������� �Լ��� ����
    req->OnProcessRequestComplete().BindUObject(this, &UJS_GameInstance::UserResPost);
    // ������ ��û
    req->ProcessRequest();
}

void UJS_GameInstance::UserResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    // ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // ������ ���ڿ��� ��������
        FString result = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);
        UJsonParseLib::UserLike_Convert_JsonToStruct(result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
//User End-------------------------------------------------------------

//User Like-------------------------------------------------------------
void UJS_GameInstance::UserLikeReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // ������� �Լ��� ����
    req->OnProcessRequestComplete().BindUObject(this, &UJS_GameInstance::UserLikeResPost);
    // ������ ��û
    req->ProcessRequest();
}

void UJS_GameInstance::UserLikeResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    // ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // ������ ���ڿ��� ��������
        FString result = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);
        UJsonParseLib::UserLike_Convert_JsonToStruct(result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
//User Like End-------------------------------------------------------------


//void UJS_GameInstance::BindDelegates()
//{
//    LoginDelegate.BindUObject(this, &UJS_GameInstance::LoginDelegateFunction);
//
//    LoginDelegate.Execute();
//}
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
