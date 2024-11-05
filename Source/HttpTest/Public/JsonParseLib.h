// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonParseLib.generated.h"

USTRUCT(BlueprintType) 
struct FLogin //�α���
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Login/Struct")
	FString userId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Login/Struct")
	FString userpass;

	//�⺻ ������
	FLogin() : 
			  userId(TEXT("")), 
			  userpass(TEXT(""))
	{}
};

USTRUCT(BlueprintType) 
struct FSign_up // ȸ������
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString userId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString userpass;

	//�⺻ ������
	FSign_up() : 
				userId(TEXT("")), 
				userpass(TEXT("")) 
	{}
};

USTRUCT(BlueprintType)
struct FUser // ���� ���̺�
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString userId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FDateTime LoginTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	int32 MoodScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	int32 EnergyScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	int32 StabilityScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString WeatherChoice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString MainAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString SecondaryAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString BackgroundColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString FloorMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	int32 ClusterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString RecommendedRoomId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString Feedback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FDateTime FeedbackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FDateTime UpdateTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	bool IsRecommended;

	// �⺻ ������
	FUser() 
		: userId(TEXT("")), 
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
struct FChangeIndex //ChangeIndex
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallPaper/Struct")
	FString room_num;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallPaper/Struct")
    FString updatedWallpaperNum;
	//�⺻ ������
	FChangeIndex() : room_num(TEXT("")), updatedWallpaperNum(TEXT("")) {}
};

USTRUCT(BlueprintType) 
struct FMyRoomInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyRoom/Struct")
    FString UltraSky_TimeOfDay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyRoom/Struct")
    FString UltraWeather_CloudCoverage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyRoom/Struct")
    FString UltraWeather_Fog;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyRoom/Struct")
    FString UltraWeather_Rain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyRoom/Struct")
    FString UltraWeather_Snow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyRoom/Struct")
    FString UltraWeather_Dust;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyRoom/Struct")
    FString UltraWeather_WindIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyRoom/Struct")
    FString UltraWeather_Thunder;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyRoom/Struct")
    FString RoomName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyRoom/Struct")
    FString userDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyRoom/Struct")
    FString room_pp;

    // �⺻ ������
	FMyRoomInfo()
        : UltraSky_TimeOfDay(TEXT(""))
        , UltraWeather_CloudCoverage(TEXT(""))
        , UltraWeather_Fog(TEXT(""))
        , UltraWeather_Rain(TEXT(""))
        , UltraWeather_Snow(TEXT(""))
        , UltraWeather_Dust(TEXT(""))
        , UltraWeather_WindIntensity(TEXT(""))
        , UltraWeather_Thunder(TEXT(""))
        , RoomName(TEXT(""))
        , userDescription(TEXT(""))
        , room_pp(TEXT(""))
    {}
};

USTRUCT(BlueprintType)
struct FMyCreateRoomInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCreateRoomInfo/Struct")
	FString UserId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCreateRoomInfo/Struct")
	int32 RoomNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCreateRoomInfo/Struct")
	FString RoomName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCreateRoomInfo/Struct")
	FString RecommendedMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCreateRoomInfo/Struct")
	FString EmotionImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCreateRoomInfo/Struct")
	FString Analysiscontent;

	// �⺻ ������
	FMyCreateRoomInfo()
		: UserId(TEXT("DefaultUser"))        
		, RoomNum(0)                          
		, RoomName(TEXT("Default Room"))       
		, RecommendedMusic(TEXT("None"))       
		, EmotionImage(TEXT("DefaultImage"))   
		, Analysiscontent(TEXT("No Analysis"))
	{}
};

USTRUCT(BlueprintType)
struct FRoomData
{
    GENERATED_BODY()

public:
    // �� ��ȣ
    UPROPERTY(BlueprintReadWrite, Category="Room Data")
    FString userMusic;


    // �⺻ ������
    FRoomData()
        : userMusic(TEXT(""))
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

	static FString MakeJson(const TMap<FString, FString>& source);

	static FString JsonParseRoomList(const FString& json);

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
	static FString ChangeIndex_Convert_StructToJson(const FChangeIndex& FWallPaperIndexStruct);
	//Json To Struct : Response
	static FChangeIndex ChangeIndex_Convert_JsonToStruct(const FString& JsonString);

	//Struct To Json : Req
	static FString MyRoomInfo_Convert_StructToJson(const FMyRoomInfo& MyRoomInfo);
	//Json To Struct : Response
	static FMyRoomInfo MyRoomInfo_Convert_JsonToStruct(const FString& JsonString);

	//Struct To Json : Req
	static FString FMyCreateRoomInfo_Convert_StructToJson(const FMyCreateRoomInfo& MyCreateRoomInfo);
	//Json To Struct : Response
	static FMyCreateRoomInfo FMyCreateRoomInfo_Convert_JsonToStruct(const FString& JsonString);

	//Json To Struct : Response
	static FRoomData RoomData_Convert_JsonToStruct(const FString& JsonString);
};
