// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HttpWidget.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UHttpWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere)
	class UJS_GameInstance* JS_gi;

	UPROPERTY(EditAnywhere)
	class AHttpActor* HttpActor;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextLog;

	UPROPERTY(meta=(BindWidget))
	class UEditableText* UserID;

	UPROPERTY(meta=(BindWidget))
	class UEditableText* UserPW;

	UPROPERTY(meta=(BindWidget))
	class UButton* ButtonPostSend;

	UPROPERTY(meta=(BindWidget))
	class UButton* btn_User;

	UPROPERTY(meta=(BindWidget))
	class UButton* btn_UserLike;

	UPROPERTY(meta=(BindWidget))
	class UButton* btn_SignUp;
	
	UFUNCTION()
	void SendLoginData();

	UFUNCTION()
	void SendSignUpData();

	UFUNCTION()
	void SendUserData();

	UFUNCTION()
	void SendUserLikeData();
	/*UPROPERTY(meta=(BindWidget))
	class UButton* ButtonGetWebImage;

	UFUNCTION()
	void OnMyClickGetWebImage();

	UPROPERTY(meta=(BindWidget))
	class UImage* ImageWeb;

	void SetWebImage(class UTexture2D* newTexture);*/

	void SetTextLog(FString log);

	void SetHttpActor(class AHttpActor* actor);


	
	FString ServerURL = "https://jsonplaceholder.typicode.com/posts";

	//FString URL = "https://apis.data.go.kr/4050000/libnewbk/getLibnewbk";
	/*FString Key = "1B4jOfk0801JYukDA2FApT%2Bs0VOwrTVSE5qPJFlZ1mgXYs1UhkQ53Zj23EbsaJAITIcsaLGVB2gDGEMSk6IaDA%3D%3D";
	FString WebImageURL = "https://t1.daumcdn.net/news/202105/25/koreadognews/20210525045023050wquu.jpg";*/
};
