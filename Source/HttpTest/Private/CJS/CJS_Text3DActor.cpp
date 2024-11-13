// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_Text3DActor.h"
#include "Text3DComponent.h"


void ACJS_Text3DActor::StartText3DShowing()
{
    // Text3DComponent의 가시성을 켭니다
    if (UText3DComponent* TextComponent = GetText3DComponent())
    {
        TextComponent->SetVisibility(true);
        UE_LOG(LogTemp, Log, TEXT("Text3D is now visible"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Text3DComponent not found"));
    }
}
void ACJS_Text3DActor::EndText3DShowing()
{
    // Text3DComponent의 가시성을 끕니다
    if (UText3DComponent* TextComponent = GetText3DComponent())
    {
        TextComponent->SetVisibility(false);
        UE_LOG(LogTemp, Log, TEXT("Text3D is now hidden"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Text3DComponent not found"));
    }
}


