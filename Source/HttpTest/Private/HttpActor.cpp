// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpActor.h"
#include "HttpWidget.h"
#include "HttpModule.h"
#include "JsonParseLib.h"
#include "Engine/Texture2D.h"
#include "ImageUtils.h"
#include "JsonObjectConverter.h"

// Sets default values
AHttpActor::AHttpActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHttpActor::BeginPlay()
{
	Super::BeginPlay();

	// UI�� �����ؼ� ����ϰ�ʹ�.
	HttpUI = Cast<UHttpWidget>(CreateWidget(GetWorld(), HttpUIFactory));
	if (HttpUI)
	{
		HttpUI->AddToViewport();
		HttpUI->SetHttpActor(this);
	}

	auto* pc = GetWorld()->GetFirstPlayerController();
	pc->SetShowMouseCursor(true);
	pc->SetInputMode(FInputModeUIOnly());
}

// Called every frame
void AHttpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//Login -----------------------------------------------------------
void AHttpActor::ReqPostTest(FString url, FString json)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// ��û�� ������ ����
	req->SetURL(url);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(json);

	// ������� �Լ��� ����
	req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostTest);
	// ������ ��û
	req->ProcessRequest();
}
void AHttpActor::OnResPostTest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		// ����
		FString result = Response->GetContentAsString();
		// Json�� ����ü�� ����
		auto login = Login_Convert_JsonToStruct(result);
		UE_LOG(LogTemp, Warning, TEXT("OnResPostTest() OK. Code : %d  Str : %s"), Response->GetResponseCode(), *result);

		// �ʿ��� ������ �̾Ƽ� ȭ�鿡 ����ϰ�ʹ�.
		HttpUI->SetTextLog(result);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
	}
}
FString AHttpActor::Login_Convert_StructToJson(FString userID, FString userPW)
{
	//����� ����ü ������ ����
	FLoginStr loginStruct;
	// �Է� ���� ID�� PW�� ����ü�� �Է�
	loginStruct.UserID = userID; 
	loginStruct.UserPW = userPW;

	//Json ���ڿ��� ������ ����
	FString JsonString;

	//����ü�� JSON ���ڿ��� ��ȯ
	FJsonObjectConverter::UStructToJsonObjectString(loginStruct, JsonString, 0, 0);
	//�ϼ��� Json ��ȯ
	return JsonString;
}
FLoginStr AHttpActor::Login_Convert_JsonToStruct(const FString& JsonString)
{
	FLoginStr loginJson;
	//Json�� ����ü�� ��ȯ
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &loginJson, 0, 0);
	//��ȯ�� ����ü�� ��ȯ
	return loginJson;
}
//Login End-----------------------------------------------------------

void AHttpActor::ReqGetWebImage(FString url)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// ��û�� ������ ����
	req->SetURL(url);
	req->SetVerb(TEXT("GET"));
	req->SetHeader(TEXT("content-type"), TEXT("image/jpeg"));

	// ������� �Լ��� ����
	req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResGetWebImage);
	// ������ ��û

	req->ProcessRequest();
}

void AHttpActor::OnResGetWebImage(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		TArray<uint8> data = Response->GetContent();
		FString fileServerURL = "http://mtvs.helloworldlabs.kr:7771/api/byte";
		ReqGetWebImageToServer(fileServerURL, data);
	}
	else {
		// ����
		UE_LOG(LogTemp, Warning, TEXT("OnResGetWebImage Failed..."));
	}
}

void AHttpActor::ReqGetWebImageToServer(FString url, TArray<uint8> resource)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// ��û�� ������ ����
	req->SetURL(url);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/octet-stream"));
	req->SetContent(resource);

	// ������� �Լ��� ����
	req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResGetWebImageFromServer);
	// ������ ��û

	req->ProcessRequest();

}

void AHttpActor::OnResGetWebImageFromServer(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		TArray<uint8> data = Response->GetContent();
		FString imagePath = FPaths::ProjectPersistentDownloadDir() + "/Cat.jpg";
		FFileHelper::SaveArrayToFile(data, *imagePath);
		UTexture2D* realTexture = FImageUtils::ImportBufferAsTexture2D(data);
		HttpUI->SetWebImage(realTexture);
	}
	else {
		// ����
		UE_LOG(LogTemp, Warning, TEXT("OnResGetWebImageFromServer Failed..."));
	}
}
