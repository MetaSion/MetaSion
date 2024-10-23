// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonParseLib.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "JsonObjectConverter.h"
// Login ----------------------------------------------------------------------------
FString UJsonParseLib::Login_Convert_StructToJson(const FLogin& LoginStrcut)
{
	//Json ���ڿ��� ������ ����
	FString JsonString;

	//����ü�� JSON ���ڿ��� ��ȯ
	FJsonObjectConverter::UStructToJsonObjectString(LoginStrcut, JsonString, 0, 0);

	//�ϼ��� Json ��ȯ
	return JsonString;
}


FLogin UJsonParseLib::Login_Convert_JsonToStruct(const FString& JsonString)
{
	FLogin loginJson;

	//Json�� ����ü�� ��ȯ
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &loginJson, 0, 0);

	//��ȯ�� ����ü�� ��ȯ
	return loginJson;
}

// Login End----------------------------------------------------------------------------

// Sign up ----------------------------------------------------------------------------
FString UJsonParseLib::SignUp_Convert_StructToJson(const FSign_up& SignUpStruct)
{
	
	FString JsonString;
	//Json�� ����ü�� ��ȯ
	FJsonObjectConverter::UStructToJsonObjectString(SignUpStruct, JsonString, 0, 0);
	//��ȯ�� ����ü�� ��ȯ
	return JsonString;
}

FSign_up UJsonParseLib::SignUp_Convert_JsonToStruct(const FString& JsonString)
{
	FSign_up signUpStruct;
	//Json�� ����ü�� ��ȯ
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &signUpStruct, 0, 0);
	//��ȯ�� ����ü�� ��ȯ
	return signUpStruct;
}
// Sign up End----------------------------------------------------------------------------

// User ----------------------------------------------------------------------------
FString UJsonParseLib::User_Convert_StructToJson(const FUser& UserStrcut)
{
	//Json ���ڿ��� ������ ����
	FString JsonString;

	//����ü�� JSON ���ڿ��� ��ȯ
	FJsonObjectConverter::UStructToJsonObjectString(UserStrcut, JsonString, 0, 0);

	//�ϼ��� Json ��ȯ
	return JsonString;
}

FUser UJsonParseLib::User_Convert_JsonToStruct(const FString& JsonString)
{
	FUser UserJson;

	//Json�� ����ü�� ��ȯ
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &UserJson, 0, 0);

	//��ȯ�� ����ü�� ��ȯ
	return UserJson;

}
// User End----------------------------------------------------------------------------

// User_Like ----------------------------------------------------------------------------
FString UJsonParseLib::UserLike_Convert_StructToJson(const FUser_like& User_LikeStrcut)
{
	//Json ���ڿ��� ������ ����
	FString JsonString;

	//����ü�� JSON ���ڿ��� ��ȯ
	FJsonObjectConverter::UStructToJsonObjectString(User_LikeStrcut, JsonString, 0, 0);

	//�ϼ��� Json ��ȯ
	return JsonString;

}

FUser_like UJsonParseLib::UserLike_Convert_JsonToStruct(const FString& JsonString)
{
	FUser_like UserLikeJson;
	//Json�� ����ü�� ��ȯ
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &UserLikeJson, 0, 0);
	//��ȯ�� ����ü�� ��ȯ
	return UserLikeJson;

}
// User_Like End ----------------------------------------------------------------------------

FString UJsonParseLib::MakeJson(const TMap<FString, FString> source)
{
	// source�� JsonObject �������� �����.
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	for (TPair<FString, FString> pair : source)
	{
		jsonObject->SetStringField(pair.Key, pair.Value);
	}

	// writer�� ���� JsonObject�� ���ڵ��ؼ� 
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&json);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);
	// ��ȯ�Ѵ�.
	return json;
}