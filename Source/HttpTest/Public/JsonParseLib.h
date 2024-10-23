// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonParseLib.generated.h"

USTRUCT(BlueprintType) 
struct FLogin //로그인
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Login/Struct")
	FString UserID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Login/Struct")
	FString UserPW;

	//기본 생성자
	FLogin() : 
			  UserID(TEXT("")), 
			  UserPW(TEXT(""))
	{}
};

USTRUCT(BlueprintType) 
struct FSign_up // 회원가입
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString UserNickName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString UserID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString UserPW;

	//기본 생성자
	FSign_up() : 
				UserNickName(TEXT("")), 
				UserID(TEXT("")), 
				UserPW(TEXT("")) 
	{}
};

USTRUCT(BlueprintType)
struct FUser // 유저 테이블
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString UserID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FDateTime LoginTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	int32 MoodScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	int32 EnergyScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	int32 StabilityScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString WeatherChoice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString MainAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString SecondaryAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString BackgroundColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString FloorMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	int32 ClusterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString RecommendedRoomId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString Feedback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FDateTime FeedbackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FDateTime UpdateTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	bool IsRecommended;

	// 기본 생성자
	FUser() 
		: UserID(TEXT("")), 
		  LoginTime(FDateTime::Now()), 
		  MoodScore(0), 
		  EnergyScore(0), 
		  StabilityScore(0), 
		  WeatherChoice(TEXT("")), 
		  MainAsset(TEXT("")), 
		  SecondaryAsset(TEXT("")), 
		  BackgroundColor(TEXT("")), 
		  FloorMaterial(TEXT("")), 
		  ClusterId(0), 
		  RecommendedRoomId(TEXT("")), 
		  Feedback(TEXT("")), 
		  FeedbackTime(FDateTime::Now()), 
		  UpdateTime(FDateTime::Now()), 
		  IsRecommended(false) 
	{}
};

USTRUCT(BlueprintType)
struct FUser_like
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	int32 FeedbackId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString UserId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString VisitedRoomId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString VisitedUserId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString Feedback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FDateTime FeedbackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString Mood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString Weather;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString MainAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString BackgroundColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString FloorMaterial;

	// 기본 생성자
	FUser_like()
		: FeedbackId(0), 
		  UserId(TEXT("")), 
		  VisitedRoomId(TEXT("")), 
		  VisitedUserId(TEXT("")), 
		  Feedback(TEXT("")), 
		  FeedbackTime(FDateTime::Now()), 
		  Mood(TEXT("")), 
		  Weather(TEXT("")), 
		  MainAsset(TEXT("")), 
		  BackgroundColor(TEXT("")), 
		  FloorMaterial(TEXT("")) 
	{}
};
/**
 * 
 */
UCLASS()
class HTTPTEST_API UJsonParseLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	static FString MakeJson(const TMap<FString, FString> source);

	//Struct To Json : Req
	static FString Login_Convert_StructToJson(const FLogin& LoginStrcut);
	//Json To Struct : Response
	static FLogin Login_Convert_JsonToStruct(const FString& JsonString);

	//Struct To Json : Req
	static FString SignUp_Convert_StructToJson(const FSign_up& SignUpStruct);
	//Json To Struct : Response
	static FSign_up SignUp_Convert_JsonToStruct(const FString& JsonString);

	//Struct To Json : Req
	static FString User_Convert_StructToJson(const FUser& UserStrcut);
	//Json To Struct : Response
	static FUser User_Convert_JsonToStruct(const FString& JsonString);

	//Struct To Json : Req
	static FString UserLike_Convert_StructToJson(const FUser_like& User_LikeStrcut);
	//Json To Struct : Response
	static FUser_like UserLike_Convert_JsonToStruct(const FString& JsonString);
};
