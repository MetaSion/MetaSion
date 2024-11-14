// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_Text3DActor.h"
#include "CJS/CJS_NewQuestionWidget.h"

#include "Text3DComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HttpWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"




void ACJS_Text3DActor::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::BeginPlay()"));

    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::BeginPlay() LevelName : %s"), *LevelName);

    if (LevelName.Contains("Question"))
    {
        UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::BeginPlay() LevelName.Contains->Main_Question"));
        // WBP_QuestionRenewal 위젯 생성
        if (WBP_QuestionRenewal)
        {
            NewQuesionUI = CreateWidget<UCJS_NewQuestionWidget>(GetWorld(), WBP_QuestionRenewal);
            if (NewQuesionUI)
            {
                NewQuesionUI->AddToViewport();  // 처음에 추가하여 보이도록 함
                NewQuesionUI->SetVisibility(ESlateVisibility::Hidden);  // 기본적으로 숨김 상태로 설정
                UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::BeginPlay() NewQuesionUI created and hidden"));
            
                FTimerHandle TimerHandle;
                GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACJS_Text3DActor::ShowQuestionWidget, 5.0f, false);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("ACJS_Text3DActor::BeginPlay() Failed to create NewQuesionUI"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("ACJS_Text3DActor::BeginPlay() Failed to create WBP_QuestionRenewal"));
        }
    }
    else if (LevelName.Contains("Login"))
    {
        UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::BeginPlay() LevelName.Contains->Main_Login"));
    }
    
    

    /*LoginWidget = Cast<UHttpWidget>(UGameplayStatics::GetActorOfClass(GetWorld(), UHttpWidget::StaticClass()));
    if (LoginWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::BeginPlay() Set HttpActor"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ACJS_Text3DActor::BeginPlay() No HttpActor"));
    }*/

    //TextComponent = GetText3DComponent();
}


/* Main_Lohin Map */
void ACJS_Text3DActor::ShowLoginText3D()
{
    UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::ShowLoginText3D()"));
    // Text3DComponent의 가시성을 켭니다
    UText3DComponent* InTextComponent = GetText3DComponent();
    if (InTextComponent)
    {
        InTextComponent->SetVisibility(true);
        UE_LOG(LogTemp, Warning, TEXT("Text3D is now visible"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Text3DComponent not found or not initialized"));
    }
}
void ACJS_Text3DActor::HideLoginText3D()
{
    UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::HideLoginText3D()"));
    // Text3DComponent의 가시성을 끕니다
    UText3DComponent* TextComponent = GetText3DComponent();
    if (TextComponent)
    {
        TextComponent->SetVisibility(false);
        UE_LOG(LogTemp, Warning, TEXT("Text3D is now hidden"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Text3DComponent not found or not initialized"));
    }
}
void ACJS_Text3DActor::ReadyReqLoginData()
{
    // TArray를 사용해 화면에 표시된 모든 UHttpWidget을 검색합니다.
    TArray<UUserWidget*> FoundWidgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UHttpWidget::StaticClass(), false);

    if (FoundWidgets.Num() > 0)
    {
        UHttpWidget* LoginWidget = Cast<UHttpWidget>(FoundWidgets[0]);
        if (LoginWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::ReadyReqLoginData() LoginWidget exists"));
            LoginWidget->SendLoginData();
            return;
        }
    }

    UE_LOG(LogTemp, Error, TEXT("ACJS_Text3DActor::ReadyReqLoginData() No LoginWidget"));
}
void ACJS_Text3DActor::StartShowLoginText3D(class UText3DComponent* InTextComponent)
{
    UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::StartShowLoginText3D()"));
    //ShowLoginText3D(InTextComponent);
}


/* Main_Question Map */
void ACJS_Text3DActor::ShowQuestionWidget()
{
    UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::ShowQuestionWidget()"));
    if (NewQuesionUI)
    {
        NewQuesionUI->SetVisibility(ESlateVisibility::Visible);
        UE_LOG(LogTemp, Warning, TEXT("Question Widget is now visible"));  
    }
}
void ACJS_Text3DActor::HideQuestionWidget()
{
    UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::HideQuestionWidget()"));
    if (NewQuesionUI)
    {
        NewQuesionUI->SetVisibility(ESlateVisibility::Hidden);
        UE_LOG(LogTemp, Warning, TEXT("Question Widget is now hidden"));
    }
}

void ACJS_Text3DActor::ShowQuestionText3DActor()
{
    UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::ShowQuestionText3DActor()"));
    //ACJS_Question3DActor* QeustText3DActor = Cast<ACJS_Question3DActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACJS_Text3DActor::StaticClass()));
    //QeustText3DActor->ShowQuesion1();
}

//
//void ACJS_Text3DActor::ShowQuestionActors()
//{
//
//}
//void ACJS_Text3DActor::HideQuestionActors()
//{
//
//}
//
//void ACJS_Text3DActor::ShowCurrentQuestionActors()
//{
//
//}
//void ACJS_Text3DActor::HideCurrentQuestionActors()
//{
//
//}
//void ACJS_Text3DActor::NextQuestion()
//{
//
//}
//
//
//void ACJS_Text3DActor::ShowQuesion1()
//{
//    UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::ShowQuesion1()"));
//    UText3DComponent* InTextComponent = GetText3DComponent();
//    if (InTextComponent)
//    {
//        InTextComponent->SetVisibility(true);
//        UE_LOG(LogTemp, Warning, TEXT("Text3D is now visible"));
//    }
//    else
//    {
//        UE_LOG(LogTemp, Error, TEXT("ACJS_Text3DActor::ShowQuesion1() Text3DComponent not found or not initialized"));
//    }
//}
//void ACJS_Text3DActor::ShowQuesion2()
//{
//    UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::ShowQuesion2()"));
//    UText3DComponent* InTextComponent = GetText3DComponent();
//    if (InTextComponent)
//    {
//        InTextComponent->SetVisibility(true);
//        UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::ShowQuesion2() Text3D is now visible"));
//    }
//    else
//    {
//        UE_LOG(LogTemp, Error, TEXT("Text3DComponent not found or not initialized"));
//    }
//}
//void ACJS_Text3DActor::ShowQuesion3()
//{
//    UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::ShowQuesion3()"));
//    UText3DComponent* InTextComponent = GetText3DComponent();
//    if (InTextComponent)
//    {
//        InTextComponent->SetVisibility(true);
//        UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::ShowQuesion3() Text3D is now visible"));
//    }
//    else
//    {
//        UE_LOG(LogTemp, Error, TEXT("Text3DComponent not found or not initialized"));
//    }
//}
//
//void ACJS_Text3DActor::OnCompleteSelection()
//{
//    UE_LOG(LogTemp, Warning, TEXT("ACJS_Text3DActor::OnCompleteSelection()"));
//}
//
