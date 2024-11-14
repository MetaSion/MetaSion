// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_Question3DActor.h"
#include "../../../../Plugins/Experimental/Text3D/Source/Text3D/Public/Text3DComponent.h"
#include "Kismet/GameplayStatics.h"

void ACJS_Question3DActor::BeginPlay()
{
    Super::BeginPlay();

   HideQuestionText3D();

    //HideQuestionText3D();
   
    //// 월드의 모든 액터 검색
    //TArray<AActor*> FoundActors;
    //UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

    //// 이름에 "Question"이 포함된 액터만 배열에 추가
    //for (AActor* Actor : FoundActors)
    //{
    //    if (Actor && Actor->GetName().Contains(TEXT("Question")))
    //    {
    //        QuestionActors.Add(Actor);
    //        UE_LOG(LogTemp, Warning, TEXT("Added %s to QuestionActors"), *Actor->GetName());
    //    }
    //}
}

void ACJS_Question3DActor::ShowQuestionText3D()
{
    UE_LOG(LogTemp, Warning, TEXT("ACJS_Question3DActor::ShowLoginText3D()"));
    // Text3DComponent의 가시성을 켭니다
    UText3DComponent* InTextComponent = GetText3DComponent();
    if (InTextComponent)
    {
        InTextComponent->SetVisibility(true);
        UE_LOG(LogTemp, Warning, TEXT("ACJS_Question3DActor::ShowQuestionText3D() Text3D is now visible"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ACJS_Question3DActor::ShowQuestionText3D() Text3DComponent not found or not initialized"));
    }
}
void ACJS_Question3DActor::HideQuestionText3D()
{
    UE_LOG(LogTemp, Warning, TEXT("ACJS_Question3DActor::ShowLoginText3D()"));
    // Text3DComponent의 가시성을 켭니다
    UText3DComponent* InTextComponent = GetText3DComponent();
    if (InTextComponent)
    {
        InTextComponent->SetVisibility(false);
        UE_LOG(LogTemp, Warning, TEXT("ACJS_Question3DActor::ShowQuestionText3D() Text3D is now visible"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ACJS_Question3DActor::ShowQuestionText3D() Text3DComponent not found or not initialized"));
    }

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACJS_Question3DActor::ShowQuestionText3D, 10.0f, false);
}

void ACJS_Question3DActor::ShowQuesion1()
{
    UE_LOG(LogTemp, Warning, TEXT("CJS_Question3DActor::ShowQuesion1()"));
    ShowQuestionText3D();
}
void ACJS_Question3DActor::ShowQuesion2()
{
    UE_LOG(LogTemp, Warning, TEXT("CJS_Question3DActor::ShowQuesion2()"));
}
void ACJS_Question3DActor::ShowQuesion3()
{
    UE_LOG(LogTemp, Warning, TEXT("CJS_Question3DActor::ShowQuesion3()"));
}
