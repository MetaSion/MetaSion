// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW_ChoiceSaveBF.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"


TArray<FChoiceData> UKGW_ChoiceSaveBF::ChoiceList;

bool UKGW_ChoiceSaveBF::SaveChoicesToJsonFile(const FString& FileName)
{

    // JSON �迭�� ����
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    TArray<TSharedPtr<FJsonValue>> JsonArray;

    // �� �������� JSON ��ü�� ��ȯ�Ͽ� �迭�� �߰�
    for (const FChoiceData& ChoiceData : ChoiceList)
    {
        TSharedPtr<FJsonObject> ChoiceObject = MakeShareable(new FJsonObject);
        ChoiceObject->SetStringField(TEXT("Question"), ChoiceData.Question);
        ChoiceObject->SetStringField(TEXT("SelectedChoice"), ChoiceData.SelectedChoice);

        JsonArray.Add(MakeShareable(new FJsonValueObject(ChoiceObject)));
    }

    // JSON ��ü�� ������ �迭 �߰�
    JsonObject->SetArrayField(TEXT("Choices"), JsonArray);

    // JSON ���ڿ��� ����ȭ
    FString json;
    TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<TCHAR>::Create(&json);
    if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
    {
        // ������ ���� ��� ���� (Saved ���丮)
        FString FullPath = FPaths::ProjectSavedDir() + FileName;
        return FFileHelper::SaveStringToFile(json, *FullPath);
    }

    return false;
}


void UKGW_ChoiceSaveBF::AddChoice(const FChoiceData& ChoiceData)
{
    ChoiceList.Add(ChoiceData);

}

TArray<FColorData> UKGW_ChoiceSaveBF::ParseJsonToRGB(const FString& JsonString)
{
    // ����� ��ȯ�� �迭 ����
    TArray<FColorData> Colors;

    // JSON �����͸� �б� ���� Reader ����
    TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonString);

    // �Ľ� ����� ���� JsonObject ����
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

    // JSON �Ľ� �õ�
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        // JSON �迭�� "items" �ʵ� ��������
        TArray<TSharedPtr<FJsonValue>> ItemsArray = JsonObject->GetArrayField(TEXT("items"));

        // �迭 ��ȸ�ϸ鼭 �� ��ü���� �����͸� ����
        for (TSharedPtr<FJsonValue> Item : ItemsArray)
        {
            TSharedPtr<FJsonObject> ItemObject = Item->AsObject();

            // FColorData ����ü ����
            FColorData ColorData;

            // RGB �� ����
            ColorData.R = ItemObject->GetNumberField(TEXT("R"));
            ColorData.G = ItemObject->GetNumberField(TEXT("G"));
            ColorData.B = ItemObject->GetNumberField(TEXT("B"));

            // ��� �迭�� �߰�
            Colors.Add(ColorData);
        }
    }

    // RGB ���� ����� �迭 ��ȯ
    return Colors;
}

// FString UKGW_ChoiceSaveBF::ParseJsonToColorData(const FString& json)
// {
//     TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
//     // �Ľ� ����� ���� ���� ����
//     TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());
//     // �ؼ��� �Ѵ�.
//     FString returnValue;
//     TArray<FColorData> Colors;
// 
//     if (FJsonSerializer::Deserialize(reader, result))
//     {
//         TArray<TSharedPtr<FJsonValue>> ColorObject = result->GetArrayField(TEXT("items"));
//         for (TSharedPtr<FJsonValue> data : ColorObject)
//         {
//             
// //  // RGB �� ����
// //             float R = data->AsObject()->GetNumberField(TEXT("R"));
// //             float G = data->AsObject()->GetNumberField(TEXT("G"));
// //             float B = data->AsObject()->GetNumberField(TEXT("B"));
// // 
// //             // ������ �����͸� ��� �������� �߰�
// //             returnValue.Append(FString::Printf(TEXT("R: %.2f / G: %.2f / B: %.2f\n"), R, G, B));
//             TSharedPtr<FJsonObject> ItemObject = data->AsObject();
// 
//             // FColorData ����ü ����
//             FColorData ColorData;
// 
//             // RGB �� ����
//             ColorData.R = ItemObject->GetNumberField(TEXT("R"));
//             ColorData.G = ItemObject->GetNumberField(TEXT("G"));
//             ColorData.B = ItemObject->GetNumberField(TEXT("B"));
// 
//             // ��� �迭�� �߰�
//             Colors.Add(ColorData);
//         }
//     }
//     
//     // ��ȯ�Ѵ�.
//     return Colors;
// 
// }
