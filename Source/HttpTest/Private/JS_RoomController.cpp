// Fill out your copyright notice in the Description page of Project Settings.
#include "JS_RoomController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

AJS_RoomController::AJS_RoomController()
{
    PrimaryActorTick.bCanEverTick = true; // Tick Ȱ��ȭ
}

void AJS_RoomController::BeginPlay()
{
    Super::BeginPlay();

    // ���콺 ���� ����
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    // ���Ӱ� UI �� �� ��ǲ�� ���� �� �ֵ��� ����
    FInputModeGameAndUI InputMode;
    InputMode.SetHideCursorDuringCapture(false); // ĸó �߿� ���콺 Ŀ���� ������ ����
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // ���콺�� ����� ����
    SetInputMode(InputMode);
}

void AJS_RoomController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // EnhancedInputLocalPlayerSubsystem���� InputMappingContext�� �߰��մϴ�.
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        //Subsystem->ClearAllMappings();
        Subsystem->AddMappingContext(IMC_Controller, 0);
    }
    // EnhancedInputComponent ����
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // ���콺 Ŭ�� �׼� ���ε�
        //if (IA_LeftMouse)
        {
           /* EnhancedInputComponent->BindAction(IA_LeftMouse, ETriggerEvent::Started, this, &AJS_RoomController::OnMouseClick);*/
            EnhancedInputComponent->BindAction(IA_LeftMouse, ETriggerEvent::Triggered, this, &AJS_RoomController::OnMouseClick);
        }
    }
}

void AJS_RoomController::OnMouseClick()
{
    FHitResult HitResult;
	bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);
    UE_LOG(LogTemp, Warning, TEXT("Click!!"));

    if (bHitSuccessful) {
        AActor* HitActor = HitResult.GetActor();
        UE_LOG(LogTemp, Warning, TEXT("in bHitSuccessful"));

        if (HitActor)
        {
            UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s at Location: %s"),
                *HitActor->GetName(), *HitResult.Location.ToString());

            // �±� üũ
            if (HitActor->ActorHasTag(TEXT("WallPaper")))
            {
                UE_LOG(LogTemp, Log, TEXT("WallPaper Hit - Processing interaction"));
                // ���⿡ ���� ��� ���� �߰�
            }
            if (HitActor->ActorHasTag(TEXT("Lobby")))
            {
                UE_LOG(LogTemp, Log, TEXT("Lobby Hit - Loading lobby level"));
                UGameplayStatics::OpenLevel(this, FName("JS_Lobby"));
            }
        }
    }
}