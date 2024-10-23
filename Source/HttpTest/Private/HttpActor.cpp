// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpActor.h"
#include "Blueprint/UserWidget.h"
#include "HttpWidget.h"

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


//��� ����
//void SomeFunction()
//{
//	// ���� ���� ��� (����)
//	FString VoiceFilePath = FPaths::ProjectDir() + "YourVoiceFile.wav";
//
//	// ���� URL
//	FString ServerUrl = "http://your-server-address/upload";
//
//	// ���� �����͸� ������ ����
//	UploadVoiceData(VoiceFilePath, ServerUrl);
//}----------------------------------------------------------------------------------------

// image ó��
//void AHttpActor::ReqGetWebImage(FString url)
//{
//	FHttpModule& httpModule = FHttpModule::Get();
//	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
//
//	// ��û�� ������ ����
//	req->SetURL(url);
//	req->SetVerb(TEXT("GET"));
//	req->SetHeader(TEXT("content-type"), TEXT("image/jpeg"));
//
//	// ������� �Լ��� ����
//	req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResGetWebImage);
//	// ������ ��û
//
//	req->ProcessRequest();
//}
//
//void AHttpActor::OnResGetWebImage(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
//{
//	if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
//	{
//		TArray<uint8> data = Response->GetContent();
//		FString fileServerURL = "http://mtvs.helloworldlabs.kr:7771/api/byte";
//		ReqGetWebImageToServer(fileServerURL, data);
//	}
//	else {
//		// ����
//		UE_LOG(LogTemp, Warning, TEXT("OnResGetWebImage Failed..."));
//	}
//}
//
//void AHttpActor::ReqGetWebImageToServer(FString url, TArray<uint8> resource)
//{
//	FHttpModule& httpModule = FHttpModule::Get();
//	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
//
//	// ��û�� ������ ����
//	req->SetURL(url);
//	req->SetVerb(TEXT("POST"));
//	req->SetHeader(TEXT("content-type"), TEXT("application/octet-stream"));
//	req->SetContent(resource);
//
//	// ������� �Լ��� ����
//	req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResGetWebImageFromServer);
//	// ������ ��û
//
//	req->ProcessRequest();
//
//}
//
//void AHttpActor::OnResGetWebImageFromServer(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
//{
//	if (bConnectedSuccessfully)
//	{
//		TArray<uint8> data = Response->GetContent();
//		FString imagePath = FPaths::ProjectPersistentDownloadDir() + "/Cat.jpg";
//		FFileHelper::SaveArrayToFile(data, *imagePath);
//		UTexture2D* realTexture = FImageUtils::ImportBufferAsTexture2D(data);
//		HttpUI->SetWebImage(realTexture);
//	}
//	else {
//		// ����
//		UE_LOG(LogTemp, Warning, TEXT("OnResGetWebImageFromServer Failed..."));
//	}
//}
