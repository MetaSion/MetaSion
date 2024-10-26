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

void UHttpWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//login
	ButtonPostSend->OnClicked.AddDynamic(this, &UHttpWidget::SendLoginData);
	//sign_up
	btn_SignUp->OnClicked.AddDynamic(this, &UHttpWidget::SendSignUpData);
	//user
	btn_User->OnClicked.AddDynamic(this, &UHttpWidget::SendUserData);
	//user_like
	btn_UserLike->OnClicked.AddDynamic(this, &UHttpWidget::SendUserLikeData);
}

//Login
void UHttpWidget::SendLoginData()
{
	// ����ü�� �ִ� ������ UI �ؽ�Ʈ ���� �־���� ��.
	FLogin loginData;
	loginData.UserID = UserID->GetText().ToString();
	loginData.UserPW = UserPW->GetText().ToString();

	//�����ͺ� textblock���� �������� ������� ����
	FString json = UJsonParseLib::Login_Convert_StructToJson(loginData);
	//Login��û
	//JS_gi->LoginReqPost(ServerURL, json);
}

void UHttpWidget::SendSignUpData()
{
	FSign_up SignUpData;
	SignUpData.UserNickName = TEXT("sol");
	SignUpData.UserID = TEXT("123456");
	SignUpData.UserPW = TEXT("010010");

	//�����ͺ� textblock���� �������� ������� ����
	FString json = UJsonParseLib::SignUp_Convert_StructToJson(SignUpData);
	//Login��û
	//JS_gi->SignUpReqPost(ServerURL, json);
}

//User
void UHttpWidget::SendUserData()
{
	// ����ü�� �ִ� ������ UI �ؽ�Ʈ ���� �־���� ��.
	FUser UserData;
	UserData.UserID = TEXT("121212");
	UserData.LoginTime = FDateTime::Now();
	UserData.MoodScore = 1;
	UserData.EnergyScore = 2;
	UserData.StabilityScore = 3;
	UserData.WeatherChoice = TEXT("cloud");
	UserData.MainAsset = TEXT("Wood");
	UserData.SecondaryAsset = TEXT("Sky");
	UserData.BackgroundColor = TEXT("Green");
	UserData.FloorMaterial = TEXT("gress");
	UserData.ClusterId = 4;
	UserData.RecommendedRoomId = TEXT("RCOMID");
	UserData.Feedback = TEXT("True");
	UserData.FeedbackTime = FDateTime::Now();
	UserData.UpdateTime = FDateTime::Now();
	UserData.IsRecommended = true;
	
	FString json = UJsonParseLib::User_Convert_StructToJson(UserData);
	//Login��û
	//JS_gi->UserReqPost(ServerURL, json);
}

//UserLike
void UHttpWidget::SendUserLikeData()
{
	// ����ü�� �ִ� ������ UI �ؽ�Ʈ ���� �־���� ��.
	FUser_like UserLikeData;

	UserLikeData.FeedbackId = 1;
	UserLikeData.UserId = TEXT("2323123");
	UserLikeData.VisitedRoomId = TEXT("VisitedRoomID");
	UserLikeData.VisitedUserId = TEXT("VisitedUserID");
	UserLikeData.Feedback = TEXT("Feedback");
	UserLikeData.FeedbackTime = FDateTime::Now();
	UserLikeData.Mood = TEXT("red");
	UserLikeData.Weather = TEXT("sunny");
	UserLikeData.MainAsset = TEXT("sky");
	UserLikeData.BackgroundColor = TEXT("yellow");
	UserLikeData.FloorMaterial = TEXT("stone");
	
	FString json = UJsonParseLib::UserLike_Convert_StructToJson(UserLikeData);
	//Login��û
	//JS_gi->UserLikeReqPost(ServerURL, json);
}

void UHttpWidget::SetTextLog(FString log)
{
	TextLog->SetText(FText::FromString(log));
}

void UHttpWidget::SetHttpActor(AHttpActor* actor)
{
	HttpActor = actor;
}
