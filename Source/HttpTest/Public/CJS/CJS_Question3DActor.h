// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Text3DActor.h"
#include "CJS_Question3DActor.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API ACJS_Question3DActor : public AText3DActor
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="Text")
	//void ShowLoginText3D(class UText3DComponent*InTextComponent);
	void ShowQuestionText3D();
	UFUNCTION(BlueprintCallable, Category="Text")
	void HideQuestionText3D();


	// 모든 질문 관련 액터들을 보이거나 숨기는 함수
	//TArray<AActor*> QuestionActors; // 질문 관련 액터들을 저장하는 배열

	//void ShowQuestionActors();
	//void HideQuestionActors();

	////TArray<TArray<AActor*>> QuestionActorsGroups; // 각 질문마다 관련된 액터를 저장하는 배열
 //   void ShowCurrentQuestionActors();
 //   void HideCurrentQuestionActors();
 //   void NextQuestion(); 

	/*UFUNCTION(BlueprintCallable, Category="Text")
	void ShowQuesion1();
	UFUNCTION(BlueprintCallable, Category="Text")
	void ShowQuesion2();
	UFUNCTION(BlueprintCallable, Category="Text")
	void ShowQuesion3();*/

 //   UFUNCTION()
 //   void OnCompleteSelection();    // WBP_QuestionRenewal의 버튼 클릭 이벤트에 연결될 함수
	
};
