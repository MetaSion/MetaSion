// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CJS/SessionGameInstance.h"
#include "JsonParseLib.generated.h"

USTRUCT(BlueprintType) 
struct FLogin
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Login/Struct")
	FString userId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Login/Struct")
	FString userpass;

	FLogin() : 
			  userId(TEXT("")), 
			  userpass(TEXT(""))
	{}
};

USTRUCT(BlueprintType) 
struct FSign_up
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString userId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString userpass;

	FSign_up() : 
				userId(TEXT("")), 
				userpass(TEXT("")) 
	{}
};
USTRUCT(BlueprintType)
struct FRGBColorData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
    float R;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
    float G;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
    float B;

    FRGBColorData() : R(0.f), G(0.f), B(0.f) {}

};

USTRUCT(BlueprintType)
struct FUserInfoData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User")
    FString UserId;

};

USTRUCT(BlueprintType)
struct FRoomData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
    FString RoomName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
    FString RoomNum;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
    FString RoomDescription;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
    FString RoomPP;
};

USTRUCT(BlueprintType)
struct FEnvironmentData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FString UltraSky_TimeOfDay;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FString UltraWeather_CloudCoverage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FString UltraWeather_Fog;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FString UltraWeather_Rain;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FString UltraWeather_Snow;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FString UltraWeather_Dust;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FString UltraWeather_Thunder;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FString MainObject;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FString SubObject;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FString Background;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FString WeatherParticle;
};

USTRUCT(BlueprintType)
struct FAIRecommendation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Recommendation")
    FString Music;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Recommendation")
    FString Quadrant;
};

USTRUCT(BlueprintType)
struct FAIAnalysisData
{
    GENERATED_BODY()
                         
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Analysis")
    TArray<FRGBColorData> ArrayColorData;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Analysis")
    FString ParticleNum;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Analysis")
    FString Result;
};

// 복합 구조체들은 기본 구조체들 이후에 선언
USTRUCT(BlueprintType)
struct FFullRoomData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unified Data")
    FUserInfoData UserData;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unified Data")
    FRoomData RoomData;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unified Data")
    FEnvironmentData EnvironmentData;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unified Data")
    FAIRecommendation AIRecommendation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unified Data")
    FAIAnalysisData AIAnalysisData;
};
USTRUCT(BlueprintType) 
struct FWallPaperData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallPaper/Struct")
	FRoomData RoomData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallPaper/Struct")
    FString UpdatedWallpaperNum; 

	FWallPaperData() : UpdatedWallpaperNum(TEXT("")) {}
};

USTRUCT(BlueprintType) 
struct FRoomSendData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SendRoomData/Struct")
	FUserInfoData UserData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SendRoomData/Struct")
    FEnvironmentData EnvironmentData;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SendRoomData/Struct")
    FRoomData RoomInfoData;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SendRoomData/Struct")
    FAIRecommendation AIRecommedationData;
};
USTRUCT(BlueprintType) 
struct FRoomReceptionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReceptionRoomData/Struct")
	FUserInfoData UserData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReceptionRoomData/Struct")
    FRoomData RoomData;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReceptionRoomData/Struct")
    FAIRecommendation AIRecommendationData; 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReceptionRoomData/Struct")
    FAIAnalysisData AIAnalysisData;

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
    static FString FullRoomData_Convert_StructToJson(const FFullRoomData& FullRoomDataStruct);
    //Json To Struct : Response
    static FFullRoomData FullRoomData_Convert_JsonToStruct(const FString& JsonString);

    //Struct To Json : Req
    static FString UserInfoData_Convert_StructToJson(const FUserInfoData& UserDataStruct);
    //Json To Struct : Response
    static FUserInfoData UserInfoData_Convert_JsonToStruct(const FString& JsonString);

    //Struct To Json : Req
    static FString RoomData_Convert_StructToJson(const FRoomData& RoomInfoStruct);
    //Json To Struct : Response
    static FRoomData RoomData_Convert_JsonToStruct(const FString& JsonString);

    //Struct To Json : Req
    static FString EnvironmentData_Convert_StructToJson(const FEnvironmentData& EnvironmentDataStruct);
    //Json To Struct : Response
    static FEnvironmentData EnvironmentData_Convert_JsonToStruct(const FString& JsonString);

    //Struct To Json : Req
    static FString AIRecommendation_Convert_StructToJson(const FAIRecommendation& AIRecommendationStruct);
    //Json To Struct : Response
    static FAIRecommendation AIRecommendation_Convert_JsonToStruct(const FString& JsonString);

    //Struct To Json : Req
    static FString AIAnalysisData_Convert_StructToJson(const FAIAnalysisData& AIAnalysisDataStruct);
    //Json To Struct : Response
    static FAIAnalysisData AIAnalysisData_Convert_JsonToStruct(const FString& JsonString);

    //Struct To Json : Req RoomSendData
    static FString RoomSendData_Convert_StructToJson(const FRoomSendData& RoomSendDataStruct);
    //Json To Struct : Response RoomReceptionData
    static FRoomReceptionData RoomReceptionData_Convert_JsonToStruct(const FString& JsonString);


    //Struct To Json : Req
    static FString WallPaperData_Convert_StructToJson(const FWallPaperData& WallPaperDataStruct);
    //Json To Struct : Response
    static FWallPaperData WallPaperData_Convert_JsonToStruct(const FString& JsonString);
};