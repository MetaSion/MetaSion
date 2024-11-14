// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Text3DActor.h"
#include "CJS_Text3DActor.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API ACJS_Text3DActor : public AText3DActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    //class UHttpWidget* LoginWidget;

	/* Main_Login Map */
	UFUNCTION(BlueprintCallable, Category="Text")
	//void ShowLoginText3D(class UText3DComponent*InTextComponent);
	void ShowLoginText3D();
	UFUNCTION(BlueprintCallable, Category="Text")
	void HideLoginText3D();
	UFUNCTION(BlueprintCallable, Category="Text")
	void ReadyReqLoginData();

	UFUNCTION()
	void StartShowLoginText3D(class UText3DComponent* InTextComponent);


	/* Main_Question Map */
	// 질문 UI
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> WBP_QuestionRenewal;
	UPROPERTY()
	class UCJS_NewQuestionWidget* NewQuesionUI;
	UFUNCTION()
	void ShowQuestionWidget();
	UFUNCTION()
	void HideQuestionWidget();

	// 액터 가시성 제어
	UFUNCTION(BlueprintCallable, Category="Text")
	void ShowQuestionText3DActor();
	
	


};
