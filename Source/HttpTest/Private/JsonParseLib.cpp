// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonParseLib.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "JsonObjectConverter.h"
#include "CJS/SessionGameInstance.h"
// Login ----------------------------------------------------------------------------
FString UJsonParseLib::Login_Convert_StructToJson(const FLogin& LoginStrcut)
{
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(LoginStrcut, JsonString, 0, 0);
	return JsonString;
}

FLogin UJsonParseLib::Login_Convert_JsonToStruct(const FString& JsonString)
{
	FLogin loginJson;
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &loginJson, 0, 0);
	return loginJson;
}

// Login End----------------------------------------------------------------------------

// Sign up ----------------------------------------------------------------------------
FString UJsonParseLib::SignUp_Convert_StructToJson(const FSign_up& SignUpStruct)
{
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(SignUpStruct, JsonString, 0, 0);
	return JsonString;
}

FSign_up UJsonParseLib::SignUp_Convert_JsonToStruct(const FString& JsonString)
{
	FSign_up SingupStruct;
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &SingupStruct, 0, 0);
	return SingupStruct;
}
// Sign up End----------------------------------------------------------------------------

//FullRoomData ////////////////////////////////////////////////////////////////////////////
FString UJsonParseLib::FullRoomData_Convert_StructToJson(const FFullRoomData& FullRoomDataStruct)
{
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(FullRoomDataStruct, JsonString, 0, 0);
	return JsonString;
}

FFullRoomData UJsonParseLib::FullRoomData_Convert_JsonToStruct(const FString& JsonString)
{
	FFullRoomData FullRoomDataStruct;
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &FullRoomDataStruct, 0, 0);
	return FullRoomDataStruct;
}
//FullRoomData End////////////////////////////////////////////////////////////////////////////

//FUserInfoData ////////////////////////////////////////////////////////////////////////////
FString UJsonParseLib::UserInfoData_Convert_StructToJson(const FUserInfoData& UserDataStruct)
{
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(UserDataStruct, JsonString, 0, 0);
	return JsonString;
}
FUserInfoData UJsonParseLib::UserInfoData_Convert_JsonToStruct(const FString& JsonString)
{
	FUserInfoData UserDataStruct;
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &UserDataStruct, 0, 0);
	return UserDataStruct;
}
//FUserInfoData End////////////////////////////////////////////////////////////////////////////

//FRoomData ////////////////////////////////////////////////////////////////////////////
FString UJsonParseLib::RoomData_Convert_StructToJson(const FRoomData& RoomInfoStruct)
{
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(RoomInfoStruct, JsonString, 0, 0);
	return JsonString;
}
FRoomData UJsonParseLib::RoomData_Convert_JsonToStruct(const FString& JsonString)
{
	FRoomData RoomInfoStruct;
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &RoomInfoStruct, 0, 0);
	return RoomInfoStruct;
}

//FRoomData End////////////////////////////////////////////////////////////////////////////

//FEnvironmentData ////////////////////////////////////////////////////////////////////////////
FString UJsonParseLib::EnvironmentData_Convert_StructToJson(const FEnvironmentData& EnvironmentDataStruct)
{
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(EnvironmentDataStruct, JsonString, 0, 0);
	return JsonString;
}
FEnvironmentData UJsonParseLib::EnvironmentData_Convert_JsonToStruct(const FString& JsonString)
{
	FEnvironmentData EnvironmentDataStruct;
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &EnvironmentDataStruct, 0, 0);
	return EnvironmentDataStruct;
}
//FEnvironmentData End ////////////////////////////////////////////////////////////////////////////

//FAIRecommendation ////////////////////////////////////////////////////////////////////////////
FString UJsonParseLib::AIRecommendation_Convert_StructToJson(const FAIRecommendation& AIRecommendationStruct)
{
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(AIRecommendationStruct, JsonString, 0, 0);
	return JsonString;
}
FAIRecommendation UJsonParseLib::AIRecommendation_Convert_JsonToStruct(const FString& JsonString)
{
	FAIRecommendation AIRecommendation;
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &AIRecommendation, 0, 0);
	return AIRecommendation;
}
//FAIRecommendation End////////////////////////////////////////////////////////////////////////////

//FAIAnalysisData ////////////////////////////////////////////////////////////////////////////
FString UJsonParseLib::AIAnalysisData_Convert_StructToJson(const FAIAnalysisData& AIAnalysisDataStruct)
{
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(AIAnalysisDataStruct, JsonString, 0, 0);
	return JsonString;
}
FAIAnalysisData UJsonParseLib::AIAnalysisData_Convert_JsonToStruct(const FString& JsonString)
{
	FAIAnalysisData AIAnalysisData;
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &AIAnalysisData, 0, 0);
	return AIAnalysisData;
}
//FAIAnalysisData End////////////////////////////////////////////////////////////////////////////

//RoomSendData, RoomReceptionData////////////////////////////////////////////////////////////////////////////
FString UJsonParseLib::RoomSendData_Convert_StructToJson(const FRoomSendData& RoomSendDataStruct)
{
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(RoomSendDataStruct, JsonString, 0, 0);
	return JsonString;
}

FRoomReceptionData UJsonParseLib::RoomReceptionData_Convert_JsonToStruct(const FString& JsonString)
{
	FRoomReceptionData RoomReceptionData;
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &RoomReceptionData, 0, 0);
	return RoomReceptionData;
}

//RoomReceptionData End ////////////////////////////////////////////////////////////////////////////

//FWallPaperData ////////////////////////////////////////////////////////////////////////////
FString UJsonParseLib::WallPaperData_Convert_StructToJson(const FWallPaperData& WallPaperDataStruct)
{
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(WallPaperDataStruct, JsonString, 0, 0);
	return JsonString;
}
FWallPaperData UJsonParseLib::WallPaperData_Convert_JsonToStruct(const FString& JsonString)
{
	FWallPaperData WallPaperDataStruct;
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &WallPaperDataStruct, 0, 0);
	return WallPaperDataStruct;
}
//FWallPaperData ////////////////////////////////////////////////////////////////////////////

FString UJsonParseLib::MakeJson(const TMap<FString, FString>& source)
{
	// source�� JsonObject �������� �����?
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	for (TPair<FString, FString> pair : source)
	{
		jsonObject->SetStringField(pair.Key, pair.Value);
	}

	// writer�� �����?JsonObject�� ���ڵ��ؼ� 
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&json);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);
	// ��ȯ�Ѵ�.
	return json;
}

TArray<FMyCreatedRoom>  UJsonParseLib::JsonParseRoomList(const FString& json)
{
	TArray<FMyCreatedRoom> RoomInfos;
	// �����⸦ �����?
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	// �Ľ� �����?���� ���� ����
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());
	// �ؼ��� �Ѵ�.
	FString returnValue;
	if (FJsonSerializer::Deserialize(reader, result))
	{
		TArray<TSharedPtr<FJsonValue>> parseDataList = result->GetArrayField(TEXT("userRooms"));
		for (TSharedPtr<FJsonValue> data : parseDataList)
		{
			
			FString roomNum = data->AsObject()->GetStringField(TEXT("roomNum"));
			FString roomName = data->AsObject()->GetStringField(TEXT("roomName"));
			returnValue.Append(FString::Printf(TEXT("roomNum : %s / roomName : %s\n"), *roomNum, *roomName));
			FMyCreatedRoom RoomInfo;
			RoomInfo.RoomNum = roomNum;
			RoomInfo.RoomName = roomName;
			RoomInfos.Add(RoomInfo);
		}
	}
	// ��ȯ�Ѵ�.
	return RoomInfos;
}