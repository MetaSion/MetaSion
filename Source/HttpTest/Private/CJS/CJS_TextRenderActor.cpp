// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_TextRenderActor.h"
#include "Components/TextRenderComponent.h"

void ACJS_TextRenderActor::SetText(const FString& NewText)
{
	// UTextRenderComponent 가져오기
	UTextRenderComponent* TextRenderComponent = GetTextRender();

	if (TextRenderComponent)
	{
		// 텍스트 설정
		TextRenderComponent->SetText(FText::FromString(NewText));
		UE_LOG(LogTemp, Warning, TEXT("Text updated to: %s"), *NewText);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TextRenderComponent not found"));
	}
}
