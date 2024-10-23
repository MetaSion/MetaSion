// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonParseLib.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "JsonObjectConverter.h"
// Login ----------------------------------------------------------------------------
FString UJsonParseLib::Login_Convert_StructToJson(const FLogin& LoginStrcut)
{
	//Json 문자열을 저장할 변수
	FString JsonString;

	//구조체를 JSON 문자열로 변환
	FJsonObjectConverter::UStructToJsonObjectString(LoginStrcut, JsonString, 0, 0);

	//완성된 Json 반환
	return JsonString;
}


FLogin UJsonParseLib::Login_Convert_JsonToStruct(const FString& JsonString)
{
	FLogin loginJson;

	//Json을 구조체로 변환
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &loginJson, 0, 0);

	//변환된 구조체를 반환
	return loginJson;
}

// Login End----------------------------------------------------------------------------

// Sign up ----------------------------------------------------------------------------
FString UJsonParseLib::SignUp_Convert_StructToJson(const FSign_up& SignUpStruct)
{
	
	FString JsonString;
	//Json을 구조체로 변환
	FJsonObjectConverter::UStructToJsonObjectString(SignUpStruct, JsonString, 0, 0);
	//변환된 구조체를 반환
	return JsonString;
}

FSign_up UJsonParseLib::SignUp_Convert_JsonToStruct(const FString& JsonString)
{
	FSign_up signUpStruct;
	//Json을 구조체로 변환
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &signUpStruct, 0, 0);
	//변환된 구조체를 반환
	return signUpStruct;
}
// Sign up End----------------------------------------------------------------------------

// User ----------------------------------------------------------------------------
FString UJsonParseLib::User_Convert_StructToJson(const FUser& UserStrcut)
{
	//Json 문자열을 저장할 변수
	FString JsonString;

	//구조체를 JSON 문자열로 변환
	FJsonObjectConverter::UStructToJsonObjectString(UserStrcut, JsonString, 0, 0);

	//완성된 Json 반환
	return JsonString;
}

FUser UJsonParseLib::User_Convert_JsonToStruct(const FString& JsonString)
{
	FUser UserJson;

	//Json을 구조체로 변환
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &UserJson, 0, 0);

	//변환된 구조체를 반환
	return UserJson;

}
// User End----------------------------------------------------------------------------

// User_Like ----------------------------------------------------------------------------
FString UJsonParseLib::UserLike_Convert_StructToJson(const FUser_like& User_LikeStrcut)
{
	//Json 문자열을 저장할 변수
	FString JsonString;

	//구조체를 JSON 문자열로 변환
	FJsonObjectConverter::UStructToJsonObjectString(User_LikeStrcut, JsonString, 0, 0);

	//완성된 Json 반환
	return JsonString;

}

FUser_like UJsonParseLib::UserLike_Convert_JsonToStruct(const FString& JsonString)
{
	FUser_like UserLikeJson;
	//Json을 구조체로 변환
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &UserLikeJson, 0, 0);
	//변환된 구조체를 반환
	return UserLikeJson;

}
// User_Like End ----------------------------------------------------------------------------

FString UJsonParseLib::MakeJson(const TMap<FString, FString> source)
{
	// source를 JsonObject 형식으로 만든다.
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	for (TPair<FString, FString> pair : source)
	{
		jsonObject->SetStringField(pair.Key, pair.Value);
	}

	// writer를 만들어서 JsonObject를 인코딩해서 
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&json);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);
	// 반환한다.
	return json;
}