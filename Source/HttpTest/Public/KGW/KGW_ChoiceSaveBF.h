// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KGW_ChoiceSaveBF.generated.h"


USTRUCT(BlueprintType)
struct FChoiceData
{
    GENERATED_BODY()


    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Choice")
    FString Q1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Choice")
    FString Q2;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Choice")
    FString Weather;
};
USTRUCT(BlueprintType)
struct FSelectedData
{
    GENERATED_BODY()


    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Select")
    FString Flower;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Select")
    FString Tree;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Select")
    FString BackGround;
};

USTRUCT(BlueprintType)
struct FColorData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color")
    float R;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color")
    float G;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color")
    float B;
};
/**
 * Blueprint function library for saving choices to a JSON file
 */
 UCLASS()
class HTTPTEST_API UKGW_ChoiceSaveBF : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

    public:
UFUNCTION(BlueprintCallable, Category = "Save")
    static void SaveChoicesToJsonFile(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Save")
    static void RoomDataSend(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Save")
    static void AddChoice(const FChoiceData& ChoiceData);

    UFUNCTION(BlueprintCallable, Category = "Save")
    static TArray<FColorData> ParseJsonToRGB(const FString& JsonString);

    UFUNCTION(BlueprintCallable, Category = "Save")
    static void StoreChoice(FString Question, FString SelectedValue);
        UFUNCTION(BlueprintCallable, Category = "Save")

     static void StoreSelectedMyRoom(FString Object, FString SelectedValue);
    //void UKGW_ChoiceSaveBF::StoreChoice(FString Question, FString SelectedValue) <-- 수정 필요
    //static void StoreChoice(FString Question, FString SelectedValue);

    UPROPERTY()
	class USessionGameInstance* SessionID;


    
	static TArray<FChoiceData> ChoiceList;



       UFUNCTION(BlueprintCallable, Category = "Save")
    static void ShowJson();



//     UPROPERTY(EditDefaultsOnly)
//     static class UJS_GameInstance* PostChoice;
    

	//UserId 설정   <-- 이거 추가해야 함.
    //UFUNCTION(BlueprintCallable, Category = "Save")
    //static void GetUserId(USessionGameInstance* SessionGM, FString Question);
};
