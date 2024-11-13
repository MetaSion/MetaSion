// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpWidget.h"
#include "Components/Button.h"
#include "HttpActor.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "JsonParseLib.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "JsonObjectConverter.h"
#include "Kismet/GameplayStatics.h"
#include "JS_RoomController.h"
#include "CJS/SessionGameInstance.h"

void UHttpWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//login
	btn_Login->OnClicked.AddDynamic(this, &UHttpWidget::SendLoginData);
	//sign_up
	btn_SignUp->OnClicked.AddDynamic(this, &UHttpWidget::SendSignUpData);

	pc = Cast<AJS_RoomController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
}

//Login
void UHttpWidget::SendLoginData()
{
	UE_LOG(LogTemp, Warning, TEXT("UHttpWidget::SendLoginData()"));
	// ����ü�� �ִ� ������ UI �ؽ�Ʈ ���� �־���� ��.
	FLogin loginData;
	loginData.userId = userId->GetText().ToString();
	loginData.userpass = userpass->GetText().ToString();

	FString json = UJsonParseLib::Login_Convert_StructToJson(loginData);
	//Login��û
	if (HttpActor) {
		HttpActor->LoginReqPost(HttpActor->LoginURL, json); // <--- 테스트 중에 주석처리
	}

	//USessionGameInstance* SessionGI = Cast<USessionGameInstance>(GetGameInstance());  // <-- 테스트 용
	//if (SessionGI) {
	//	SessionGI->bSuccess = true; // GameInstance에 상태 저장
	//}
	//UGameplayStatics::OpenLevel(this, FName("Main_Sky")); 
}

void UHttpWidget::SendSignUpData()
{
	FSign_up SingupData;
	SingupData.userId = TEXT("123456");
	SingupData.userpass = TEXT("010010");

	//�����ͺ� textblock���� �������� ������� ����
	FString json = UJsonParseLib::SignUp_Convert_StructToJson(SingupData);
	//Login��û
	if (HttpActor) {
		//HttpActor->SignUpReqPost(HttpActor->MultiRoomURL, json);
	}
}