// Fill out your copyright notice in the Description page of Project Settings.
#include "JS_RoomController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "JsonParseLib.h"
#include "Blueprint/UserWidget.h"
#include "JS_CreateRoomWidget.h"
#include "JS_RoomWidget.h" 
#include "JS_TestWidget.h"
#include "HttpWidget.h"
#include "Components/WidgetComponent.h"
#include "KGW/WBP_Image.h"
#include "Camera/CameraActor.h"
#include "../../../../Plugins/Experimental/PythonScriptPlugin/Source/PythonScriptPlugin/Public/IPythonScriptPlugin.h"
#include "HighResScreenshot.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include "CJS/SessionGameInstance.h"
#include "Components/VerticalBox.h"
#include "CJS/CJS_BallPlayer.h"
#include "HttpActor.h"
#include "HttpActor.h"
#include "CJS/CJS_JS_WidgetFunction.h"
#include "KGW/KGW_RoomlistActor.h"

AJS_RoomController::AJS_RoomController()
{
    PrimaryActorTick.bCanEverTick = true; // Tick Ȱ��ȭ
}

void AJS_RoomController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FHitResult HitResult;
    bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);

    AActor* HoveredActor = bHitSuccessful ? HitResult.GetActor() : nullptr;

    if (HoveredActor != CurrentHoveredActor)
    {
        if (CurrentHoveredActor)
        {
            OnMouseHoverEnd(CurrentHoveredActor);
        }

        if (HoveredActor)
        {
            OnMouseHover(HoveredActor); 
        }

        CurrentHoveredActor = HoveredActor;
    }
}

void AJS_RoomController::BeginPlay()
{
    Super::BeginPlay();

    /*USessionGameInstance* SessionGI = Cast<USessionGameInstance>(GetGameInstance());
    if (SessionGI)
    {
        UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay()::USessionGameInstance is set"));
        SessionGI->HandleMapChange(GetWorld());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("USessionGameInstance is not set"));
    }*/
    HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
    InitializeUIWidgets();

    CheckDate();
    SetInputMode(FInputModeGameOnly());

    GetWorldTimerManager().SetTimer(LevelCheckTimerHandle, this, &AJS_RoomController::SpawnAndSwitchToCamera, 0.01f, true);

    USessionGameInstance* SessionGI = Cast<USessionGameInstance>(GetGameInstance());
    if (SessionGI && SessionGI->bSuccess) {
        if (HttpActor) {
            HttpActor->ShowQuestionUI();
        }
        SessionGI->bSuccess = false; // 사용 후 상태 초기화
    }


    // 마이월드 테스트용 -------------------------------------------------------------   추후 삭제해용!! 
    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    if (LevelName == "Main_Sky")
    {
        SpawnAndSwitchToCamera();
    }
    // ------------------------------------------------------------------------------------------------

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AJS_RoomController::SetActorLocationAfterLevelLoad, 1.0f, false);

}

void AJS_RoomController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        //Subsystem->ClearAllMappings();
        if (Subsystem)  // 예외 처리 추가
        {
            UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::SetupPlayerInputComponent"));
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(IMC_Controller, 0);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to get EnhancedInputLocalPlayerSubsystem."));
        }
    }
    // EnhancedInputComponent ����
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
         EnhancedInputComponent->BindAction(IA_LeftMouse, ETriggerEvent::Triggered, this, &AJS_RoomController::OnMouseClick);
    }
}

void AJS_RoomController::CheckDate()
{
    FDateTime CurrentTime = FDateTime::Now();
    FDateTime MidnightToday = FDateTime(CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay());


    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

    if (LevelName == "Main_Login" && LevelName != "Main_Lobby" && LevelName != "Main_Room" && LastCheckDate < MidnightToday) {
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
        ShowLoginUI();
        LastCheckDate = MidnightToday; 
    }
    else if(LevelName == "Main_Room" && LastCheckDate < MidnightToday) { // 방 이름이 메인 룸이고 처음 접속 했거나 00시가 지났을 경우
        bShowMouseCursor = true;
        bEnableClickEvents = true;
        bEnableMouseOverEvents = true;
    }
    else if(LevelName == "Main_Sky" && LastCheckDate < MidnightToday) {
        bShowMouseCursor = true;
        bEnableClickEvents = true;
        bEnableMouseOverEvents = true;
    }
    else {
        bShowMouseCursor = false;
        bEnableClickEvents = false;
        bEnableMouseOverEvents = false;
    }
}

void AJS_RoomController::InitializeUIWidgets()
{
    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    if (LoginUIFactory) {
        LoginUI = CreateWidget<UHttpWidget>(this, LoginUIFactory);
        if (LoginUI) {
            LoginUI->AddToViewport();
            LoginUI->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    if (CR_UIFactory) {
        CR_UI = CreateWidget<UJS_CreateRoomWidget>(this, CR_UIFactory);
        if (CR_UI) {
            CR_UI->AddToViewport();
            CR_UI->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    if (R_UIFactory)
    {
        R_UI = CreateWidget<UJS_RoomWidget>(this, R_UIFactory);
        if (R_UI)
        {
            R_UI->AddToViewport();
            R_UI->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}
void AJS_RoomController::ShowLoginUI()
{
    if (LoginUI)
    {
        LoginUI->SetVisibility(ESlateVisibility::Visible);
    }
}
void AJS_RoomController::HideLoginUI()
{
    if (LoginUI)
    {
        LoginUI->SetVisibility(ESlateVisibility::Hidden);
    }
}
//CreateRoom --------------------------------------------------------------------------
//��
void AJS_RoomController::ShowCreateRoomUI()
{
    if (CR_UI)
    {
        CR_UI->SetVisibility(ESlateVisibility::Visible);
    }
}
//��
void AJS_RoomController::HideCreateRoomUI()
{
    UE_LOG(LogTemp, Log, TEXT(" AJS_RoomController::HideCreateRoomUI()"));

    if (CR_UI)
    {
        CR_UI->SetVisibility(ESlateVisibility::Hidden);
    }
}
//CreateRoom --------------------------------------------------------------------------

//Room --------------------------------------------------------------------------
void AJS_RoomController::ShowRoomUI()
{
    UE_LOG(LogTemp, Log, TEXT(" AJS_RoomController::ShowRoomUI()"));

    if (R_UI)
    {
        R_UI->SetVisibility(ESlateVisibility::Visible);
    }
}
//���� ����
void AJS_RoomController::HideRoomUI()
{
    if (R_UI)
    {
        R_UI->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AJS_RoomController::PlayUIAnimation()
{
    UE_LOG(LogTemp, Log, TEXT(" AJS_RoomController::PlayUIAnimation()"));

    if (R_UI) {
        R_UI->PlayAnimation(R_UI->CameraSutterEffect);
    }
}
void AJS_RoomController::ShowHeartUITimer()
{
    if (R_UI) {
        //R_UI->VTB_Heart->SetVisibility(ESlateVisibility::Visible);
    }
    
}
//Room --------------------------------------------------------------------------

//myWorld -> MultiWorld:: Make Session
void AJS_RoomController::OpenMultiWorld()
{
    HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
    HttpActor->StartHttpMultyWorld();
}
void AJS_RoomController::SetActorLocationAfterLevelLoad()
{
    AKGW_RoomlistActor* ListActor = Cast<AKGW_RoomlistActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AKGW_RoomlistActor::StaticClass()));
    if (ListActor)
    {
        FVector NewListLocation(-470990.0f, 643490.0f, 648180.0f);
        ListActor->SetActorLocation(NewListLocation, true, nullptr, ETeleportType::TeleportPhysics);
        UE_LOG(LogTemp, Log, TEXT("ListActor location set successfully."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ListActor not found in the new level."));
    }
}



//Mouse Interaction --------------------------------------------------------------------------
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
            UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s at Location: %s"), *HitActor->GetName(), *HitResult.Location.ToString());
            if (HitActor->ActorHasTag(TEXT("WallPaper")))
            {
                UE_LOG(LogTemp, Warning, TEXT("-----------------------------"));
                //if (bShowUI) {
                    UE_LOG(LogTemp, Log, TEXT("bShowUI true"));
                    HideCreateRoomUI();
                    ShowRoomUI();
                    //hide until end anim
                    //R_UI->VTB_Heart->SetVisibility(ESlateVisibility::Hidden);
                    PlayUIAnimation();
                    ScreenCapture();
                    R_UI->SendChangeIndexData();
                    GetWorld()->GetTimerManager().SetTimer(HeartUITimer, this, &AJS_RoomController::ShowHeartUITimer, 1.0f, false);
               // }
            }
            else if (HitActor->ActorHasTag(TEXT("Sky")))
            {
				UE_LOG(LogTemp, Warning, TEXT("Lobby Hit - Loading lobby level"));
                UGameplayStatics::OpenLevel(this, FName("Main_Sky"));
                SetActorLocationAfterLevelLoad();

                 // 서버가 있는 로비로 돌아가기 위한 ClientTravel 사용
			   /* APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (PlayerController)
				{
					PlayerController->ClientTravel("/Game/Main/Maps/Main_Lobby", ETravelType::TRAVEL_Relative);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("AJS_RoomController::OnMouseClick():: No PlayerController"));
				}*/

                // GameInstance를 가져와서 JoinSession() 호출
                //USessionGameInstance* GameInstance = Cast<USessionGameInstance>(GetGameInstance());
                //if (GameInstance)
                //{
                //    GameInstance->FindSessions();  // 세션을 찾고, 성공 시 JoinSession을 호출
                //}
                //else
                //{
                //    UE_LOG(LogTemp, Error, TEXT("Failed to get SessionGameInstance"));
                //}
                //UGameplayStatics::OpenLevel(this, FName("Main_Lobby"));
            }
            else if (HitActor->ActorHasTag(TEXT("EnterCreateRoom")))
            {

                UE_LOG(LogTemp, Warning, TEXT("Lobby Hit - Loading lobby level"));
//              UGameplayStatics::OpenLevel(this, FName("Main_Lobby"));
                OpenMultiWorld();

            }
            else if (HitActor->ActorHasTag(TEXT("ChatWidget")))  //  <-- 채팅 위젯 추가
            {
                UE_LOG(LogTemp, Warning, TEXT("ChatWidget Hit - Loading Chat Widget"));
                if (ChatActorFactory)
                {
                    // ChatActorFactory가 ACJS_JS_WidgetFunction 타입임을 보장
                    ACJS_JS_WidgetFunction* ChatFunction = Cast<ACJS_JS_WidgetFunction>(ChatActorFactory);
                    if (ChatFunction)
                    {
                        ChatFunction->ToggleChatUIVisible();
                    }
                }
            }

        }
    }
}
//Mouse Interaction --------------------------------------------------------------------------
void AJS_RoomController::OnMouseHover(AActor* HoveredActor)
{
    if (HoveredActor)
    {
        if (HoveredActor->ActorHasTag(TEXT("ShowImage")))
        {
            UWidgetComponent* WidgetComp = HoveredActor->FindComponentByClass<UWidgetComponent>();
            if (WidgetComp)
            {
                UE_LOG(LogTemp, Log, TEXT("Hovered:ShowImage "));

                // ������ ���̵��� ����
                WidgetComp->SetVisibility(true);

                // ������ �ִϸ��̼��� ���?
                UUserWidget* Widget = WidgetComp->GetWidget();
                if (Widget)
                {

                    if (UWBP_Image* WBPImage = Cast<UWBP_Image>(Widget))
                    {
                        if (!WBPImage->IsAnimationPlaying(WBPImage->ShowImage)) // �̹� �ִϸ��̼��� ���?������ Ȯ��
                        {
                            WBPImage->PlayShowImageAnimation();
                            UE_LOG(LogTemp, Log, TEXT("play ShowImage "));

                        }
                    }
                }
            }

        }
        if (HoveredActor->ActorHasTag(TEXT("Showlist")))
        {
            UWidgetComponent* WidgetComp = HoveredActor->FindComponentByClass<UWidgetComponent>();
            if (WidgetComp)
            {
                UE_LOG(LogTemp, Log, TEXT("Hovered:Showlist "));

                // ������ ���̵��� ����
                WidgetComp->SetVisibility(true);

                // ������ �ִϸ��̼��� ���
                UUserWidget* Widget = WidgetComp->GetWidget();
                if (Widget)
                {

                    if (UWBP_Image* WBPImage = Cast<UWBP_Image>(Widget))
                    {
                        if (!WBPImage->IsAnimationPlaying(WBPImage->ShowImage)) // �̹� �ִϸ��̼��� ��� ������ Ȯ��
                        {
                            WBPImage->PlayShowImageAnimation();
                            WBPImage->GetImage();
                            UE_LOG(LogTemp, Log, TEXT("play Showlist "));

                        }
                    }
                }
            }

        }

    }
}

void AJS_RoomController::OnMouseHoverEnd(AActor* HoveredActor)
{
    if (HoveredActor)
    {
        if (HoveredActor->ActorHasTag(TEXT("ShowImage")))
        {
            UE_LOG(LogTemp, Log, TEXT("Hovere end:ShowImage "));

            // Widget Component ��������
            UWidgetComponent* WidgetComp = HoveredActor->FindComponentByClass<UWidgetComponent>();
            if (WidgetComp)
            {
                // ������ �����?
                WidgetComp->SetVisibility(false);
                //             UE_LOG(LogTemp, Log, TEXT("get  ShowImage "));

            }
        }
        if (HoveredActor->ActorHasTag(TEXT("Showlist")))
        {
            UE_LOG(LogTemp, Log, TEXT("Hovere end:Showlist "));

            // Widget Component ��������
            UWidgetComponent* WidgetComp = HoveredActor->FindComponentByClass<UWidgetComponent>();
            if (WidgetComp)
            {
                // ������ �����
                WidgetComp->SetVisibility(false);
                //             UE_LOG(LogTemp, Log, TEXT("get  ShowImage "));

            }
        }

    }
}


void AJS_RoomController::SpawnAndSwitchToCamera()
{
    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

    FVector CameraLocation;
    FRotator CameraRotation;

    if (LevelName == "Main_Sky")
    {
        // �ϴ� ���� ��ġ�� ȸ�� ����
        CameraLocation = FVector(-470047.589317, 643880.89814, 648118.610643);
        CameraRotation = FRotator(9.157953, 200.435537, 0.000001); 
        //CameraRotation = FRotator(0, 0, 0);
    }
    else if (LevelName == "Main_Room")
    {
        // �� ���� ��ġ�� ȸ�� ����
        CameraLocation = FVector(3004.710844, -40.193309, 83.381573);
        CameraRotation = FRotator(4.510870, 1980.785016, 0);
    }
    else
    {
        return;  // ���ǿ� ���� ������ ��ȯ
    }

    // ī�޶� ���͸� ����
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    ACameraActor* TargetCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), CameraLocation, CameraRotation, SpawnParams);

    if (LevelName == "Main_Room") {
        TargetCamera->GetCameraComponent()->SetFieldOfView(90);
    }
    if (TargetCamera)
    {
        SetViewTarget(TargetCamera);
        UE_LOG(LogTemp, Log, TEXT("Camera view switched to target camera successfully."));

        // ī�޶� ��ȯ�� �Ϸ�Ǹ� Ÿ�̸� ����
        GetWorldTimerManager().ClearTimer(LevelCheckTimerHandle);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn target camera."));
    }
}
//Screen Capture Start ---------------------------------------------------------------------------------------
void AJS_RoomController::ScreenCapture()
{
    UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::ScreenCapture()"));

    // ?�일 경로 ?�정 (?? ?�로?�트 ?�렉?�리 ??Screenshots ?�더)
    FString ScreenshotPath = FPaths::ProjectDir() + TEXT("Screenshots/ScreenCapture.png");

    // ?�크린샷 캡처 ?�청
    FScreenshotRequest::RequestScreenshot(ScreenshotPath, false, false);

    UE_LOG(LogTemp, Warning, TEXT("Screenshot saved at: %s"), *ScreenshotPath);

    ExecuteWallPaperPython();
}
//Screen Capture End -----------------------------------------------------------------------------------------

//Wallpaper Python Auto Execute Start ------------------------------------------------------------------------
void AJS_RoomController::ExecuteWallPaperPython()
{
    UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::ExecuteWallPaperPython()"));

    // ?�이???�일 경로 ?�정
    FString ScriptPath = FPaths::ProjectContentDir() + TEXT("Python/Wallpaper.py");

    // ?�이???�크립트 ?�행
    IPythonScriptPlugin* PythonPlugin = IPythonScriptPlugin::Get();
    if (PythonPlugin && PythonPlugin->IsPythonAvailable())
    {
        PythonPlugin->ExecPythonCommand(*ScriptPath);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Python is not available in this build."));
    }
}
//Wallpaper Python Auto Execute End ------------------------------------------------------------------------

