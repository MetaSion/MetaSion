// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CJS/SessionGameInstance.h"
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
struct FChangeIndex //Index
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ultra Data")
    FString UserId;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ultra Data")
    FString UltraSky_TimeOfDay;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ultra Data")
    FString UltraWheather_CloudCoverage;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ultra Data")
    FString UltraWheather_Fog;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ultra Data")
    FString UltraWheather_Rain;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ultra Data")
    FString UltraWheather_Snow;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ultra Data")
    FString UltraWheather_Dust;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ultra Data")
    FString UltraWheather_Thunder;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Environment")
    FString MainObject;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Environment")
    FString SubObject;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Environment")
    FString Background;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Particles")
    FString Particle_num;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Room Info")
    FString RoomName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Room Info")
    FString RoomDescription;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Room Info")
    FString RoomPP;

    // �⺻ ������
	FMyRoomInfo()
		: UserId(TEXT(""))
		, UltraSky_TimeOfDay(TEXT(""))
		, UltraWheather_CloudCoverage(TEXT(""))
		, UltraWheather_Fog(TEXT(""))
		, UltraWheather_Rain(TEXT(""))
		, UltraWheather_Snow(TEXT(""))
		, UltraWheather_Dust(TEXT(""))
		, UltraWheather_Thunder(TEXT(""))
		, MainObject(TEXT(""))
		, SubObject(TEXT(""))
		, Background(TEXT(""))
		, Particle_num(TEXT(""))
		, RoomName(TEXT(""))
		, RoomDescription(TEXT(""))
		, RoomPP(TEXT(""))
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
		: UserId(TEXT(""))        
		, RoomNum(0)                          
		, RoomName(TEXT(""))       
		, RecommendedMusic(TEXT(""))       
		, EmotionImage(TEXT(""))   
		, Analysiscontent(TEXT(""))
	{}
};

USTRUCT(BlueprintType)
struct FRoomData
{
    GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomData")
    FString UserId;
	// �� ��ȣ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomData")
    FString RoomNum;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomData")
    FString userMusic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomData")
    FString LikeNum;


    // �⺻ ������
    FRoomData()
        : UserId(TEXT("")),
		RoomNum(TEXT("")),
		userMusic(TEXT("")),
		LikeNum(TEXT(""))
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

	static TArray<FMyCreatedRoom> JsonParseRoomList(const FString& json);

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

	//Struct To Json : Req
	static FString RoomData_Convert_StructToJson(const FRoomData& RoomData);
	//Json To Struct : Response
	static FRoomData RoomData_Convert_JsonToStruct(const FString& JsonString);
};
