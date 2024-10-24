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
    static void AddChoice(const FChoiceData& ChoiceData);

    UFUNCTION(BlueprintCallable, Category = "Save")
    static TArray<FColorData> ParseJsonToRGB(const FString& JsonString);

    UFUNCTION(BlueprintCallable, Category = "Save")
    static void StoreChoice(FString Question, int32 SelectedValue);

    
	static TArray<FChoiceData> ChoiceList;

//     UPROPERTY(EditDefaultsOnly)
//     static class UJS_GameInstance* PostChoice;
    

	
};
