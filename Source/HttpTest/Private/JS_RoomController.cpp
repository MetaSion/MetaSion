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
#include "CJS/CJS_JS_WidgetFunction.h"
#include "KGW/KGW_RoomlistActor.h"
#include "CJS/CJS_InnerWorldSettingWidget.h"
#include "CJS/CJS_LoginActor.h"
#include "KGW/KGW_RoomList.h"
#include "CineCameraActor.h"
#include "JS_ExplainWidget.h"
#include "CJS/CJS_ChatWidget.h"

AJS_RoomController::AJS_RoomController()
{
    PrimaryActorTick.bCanEverTick = true; // Tick Ȱ��ȭ

    UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::AJS_RoomController()"));
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

    UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay()"));
    HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
    if (HttpActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay() Set HttpActor"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AJS_RoomController::BeginPlay() No HttpActor"));
    }

	LoginActor = Cast<ACJS_LoginActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACJS_LoginActor::StaticClass()));
	if (!LoginActor)
	{
		UE_LOG(LogTemp, Error, TEXT("LoginActor not found in the level."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay() Set LoginActor"));
	}
    
    InitializeUIWidgets();
    CheckDate();
    SetInputMode(FInputModeGameOnly());


    SessionGI = Cast<USessionGameInstance>(GetGameInstance());
    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    if (LevelName.Contains("Main_LV"))
    {
        UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay() LevelName.Contains->Main_LV"));
        if (SessionGI) {
            UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay() LevelName.Contains->Main_LV-> SessionGI exsited"));
            InitInnerWorldSetting();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AJS_RoomController::BeginPlay() NO SessionGI"));
        }
    }
    else if (LevelName.Contains("Main_Sky"))
    {
        UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay() LevelName.Contains->Sky"));
        PlayUIAnimation();
        ShowExplainUI();
        //if (SessionGI && SessionGI->bSuccess) {
        //    UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay() LevelName.Contains->Sky-> SessionGI exsited"));
        //    //if (HttpActor) {
        //    //    //HttpActor->ShowQuestionUI();
        //    //}
        //    else
        //    {
        //        UE_LOG(LogTemp, Error, TEXT("AJS_RoomController::BeginPlay() No HttpActor"));
        //    }
        //    SessionGI->bSuccess = false; // 사용 후 상태 초기화
        //}
		/*else
		{
			UE_LOG(LogTemp, Error, TEXT("AJS_RoomController::BeginPlay() NO SessionGI"));
		}*/
    }
  //  else if (LevelName.Contains("Sky"))
  //  {
  //      UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay() LevelName.Contains->Sky"));
  //      //if (SessionGI && SessionGI->bSuccess) {
  //      //    UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay() LevelName.Contains->Sky-> SessionGI exsited"));
  //      //    //if (HttpActor) {
  //      //    //    //HttpActor->ShowQuestionUI();
  //      //    //}
  //      //    else
  //      //    {
  //      //        UE_LOG(LogTemp, Error, TEXT("AJS_RoomController::BeginPlay() No HttpActor"));
  //      //    }
  //      //    SessionGI->bSuccess = false; // 사용 후 상태 초기화
  //      //}
		///*else
		//{
		//	UE_LOG(LogTemp, Error, TEXT("AJS_RoomController::BeginPlay() NO SessionGI"));
		//}*/
  //  }
    else if (LevelName.Contains("Main_Login"))
    {
        UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay() LevelName.Contains->Login"));
        ShowLoginUI();
		if (LoginActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay() Set LoginActor"));
			LoginActor->ShowLoginUI();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AJS_RoomController::BeginPlay() NO LoginActor"));
		}
    }
    if (LevelName == "Main_Sky" || LevelName == "Main_Login" || LevelName == "Main_Question") {
        GetWorldTimerManager().SetTimer(LevelCheckTimerHandle, this, &AJS_RoomController::SpawnAndSwitchToCamera, 0.01f, false);
    }
    else {
        GetWorldTimerManager().SetTimer(OtherRoomCheckTimerHandle, this, &AJS_RoomController::SwitchToCamera, 0.01f, false);
    }
    // ------------------------------------------------------------------------------------------------

    //FTimerHandle TimerHandle;
    //GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AJS_RoomController::SetActorLocationAfterLevelLoad, 1.0f, false);
    if (LevelName == "Main_Sky") {
        //다음 틱에 액터 위치 변경 실행
        GetWorldTimerManager().SetTimerForNextTick(this, &AJS_RoomController::SetActorLocationAfterLevelLoad);
    }
}

void AJS_RoomController::SetupInputComponent()
{
    Super::SetupInputComponent();
    UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::SetupInputComponent()"));

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
         UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::SetupPlayerInputComponent UEnhancedInputComponent set"));
         // Mouse Click Event
         EnhancedInputComponent->BindAction(IA_LeftMouse, ETriggerEvent::Triggered, this, &AJS_RoomController::OnMouseClick);
         // Inner World Setting UI
         EnhancedInputComponent->BindAction(IA_SettingUI, ETriggerEvent::Started, this, &AJS_RoomController::ShowSettingUI);
    }
}

void AJS_RoomController::CheckDate()
{
    FDateTime CurrentTime = FDateTime::Now();
    FDateTime MidnightToday = FDateTime(CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay());

    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::CheckDate() LevelName : %s"), *LevelName);

    if (LevelName == "Main_Login" && LevelName != "Main_Lobby" && LevelName != "Main_Room" && LastCheckDate < MidnightToday) {
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
        //ShowLoginUI();
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
    else if (LevelName.Contains("Main_LV")) {
        bShowMouseCursor = true;
        bEnableClickEvents = true;
        bEnableMouseOverEvents = true;
    }
    else if (LevelName.Contains("Question")) {
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
    UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::InitializeUIWidgets()"));
    //FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (LoginUIFactory) {
        UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::InitializeUIWidgets() LoginUIFactory exsited"));
		LoginUI = CreateWidget<UHttpWidget>(this, LoginUIFactory);
		if (LoginUI) {
            UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::InitializeUIWidgets() LoginUI set"));
			LoginUI->AddToViewport();
			LoginUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}
    if (CR_UIFactory) {
        UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::InitializeUIWidgets() CR_UIFactory exsited"));
        CR_UI = CreateWidget<UJS_CreateRoomWidget>(this, CR_UIFactory);
        if (CR_UI) {
            UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::InitializeUIWidgets() CR_UI set"));
            CR_UI->AddToViewport();
            CR_UI->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    if (R_UIFactory)
    {
        UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::InitializeUIWidgets() CR_UIFactory exsited"));
        R_UI = CreateWidget<UJS_RoomWidget>(this, R_UIFactory);
        if (R_UI)
        {
            UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::InitializeUIWidgets() R_UI set"));
            R_UI->AddToViewport();
            R_UI->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    if (Ex_UIFactory)
    {
        Ex_UI = CreateWidget<UJS_ExplainWidget>(this, Ex_UIFactory);
        if (Ex_UI)
        {
            Ex_UI->AddToViewport();
            Ex_UI->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    if (ChatUIFactory)
    {
        UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::InitializeUIWidgets() ChatUIFactory exsited"));
        ChatUI = CreateWidget<UCJS_ChatWidget>(this, ChatUIFactory);
        if (ChatUI)
        {
            UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::InitializeUIWidgets() ChatUI set"));
            ChatUI->AddToViewport();
            ChatUI->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}
void AJS_RoomController::ShowLoginUI()
{
    UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::ShowLoginUI()"));
    if (LoginUI)
    {
        UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::ShowLoginUI() LoginUI exsited"));
        LoginUI->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT(" AJS_RoomController::ShowLoginUI() NO LoginUI"));
    }
}
void AJS_RoomController::HideLoginUI()
{
    if (LoginUI)
    {
        UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::HideLoginUI()"));
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
    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

    if (R_UI && (LevelName == "LV_Spring" || LevelName == "LV_Summer" || LevelName == "LV_Fall" || LevelName == "LV_Winter")) {
        R_UI->PlayAnimation(R_UI->CameraSutterEffect);
    }
    else if (Ex_UI && LevelName == "Main_Sky") {
        Ex_UI->NextSwitchWidget();
    }
}
void AJS_RoomController::ShowHeartUITimer()
{
    if (R_UI) {
        //R_UI->VTB_Heart->SetVisibility(ESlateVisibility::Visible);
    }
}
//Room --------------------------------------------------------------------------

//Explain UI
void AJS_RoomController::ShowExplainUI()
{
    if(Ex_UI) Ex_UI->SetVisibility(ESlateVisibility::Visible);
}
void AJS_RoomController::HideExplainUI()
{
    if (Ex_UI) Ex_UI->SetVisibility(ESlateVisibility::Hidden);
}
//Explain UI End

//myWorld -> MultiWorld:: Make Session
//void AJS_RoomController::OpenMultiWorld()
//{
//    HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
//    HttpActor->StartHttpMultyWorld();
//}
void AJS_RoomController::SetActorLocationAfterLevelLoad()
{
    AKGW_RoomlistActor* ListActor = Cast<AKGW_RoomlistActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AKGW_RoomlistActor::StaticClass()));
    if (ListActor)
    {
        FVector NewListLocation(-470990.0f, 643490.0f, 648180.0f);
        ListActor->SetActorLocation(NewListLocation, true, nullptr, ETeleportType::TeleportPhysics);
        UE_LOG(LogTemp, Warning, TEXT("ListActor location set successfully."));
        SetChangeLevelData();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ListActor not found in the new level."));
    }
}


void AJS_RoomController::OnClickButtonImage()
{
    HideCreateRoomUI();
    ShowRoomUI();
    PlayUIAnimation();
    ScreenCapture();
    R_UI->SendWallPaperData();
    GetWorld()->GetTimerManager().SetTimer(HeartUITimer, this, &AJS_RoomController::ShowHeartUITimer, 1.0f, false);
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
                    R_UI->SendWallPaperData();
                    GetWorld()->GetTimerManager().SetTimer(HeartUITimer, this, &AJS_RoomController::ShowHeartUITimer, 1.0f, false);
               // }
            }
            else if (HitActor->ActorHasTag(TEXT("Sky")))
            {
				UE_LOG(LogTemp, Warning, TEXT("Sky Hit - Loading sky level"));
                // AI쪽에서 다시 추천 받은 R, G, B값
                // 파티클 종류
                // AI설명
                // 방목록
                // 
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
                
            }
            else if (HitActor->ActorHasTag(TEXT("Lobby"))) {
                UGameplayStatics::OpenLevel(this, FName("Main_Lobby"));
                //UGameplayStatics::OpenLevel(this, FName("Main_Lobby"));
            }
            else if (HitActor->ActorHasTag(TEXT("EnterCreateRoom")))
            {

                UE_LOG(LogTemp, Warning, TEXT("Lobby Hit - Loading lobby level"));
//              UGameplayStatics::OpenLevel(this, FName("Main_Lobby"));
                //OpenMultiWorld();

            }
            //else if (HitActor->ActorHasTag(TEXT("ChatWidget")))  //  <-- 채팅 위젯 추가
            //{
            //    UE_LOG(LogTemp, Warning, TEXT("ChatWidget Hit - Loading Chat Widget"));
            //    if (ChatActorFactory)
            //    {
            //        // ChatActorFactory가 ACJS_JS_WidgetFunction 타입임을 보장
            //        ACJS_JS_WidgetFunction* ChatFunction = Cast<ACJS_JS_WidgetFunction>(ChatActorFactory);
            //        if (ChatFunction)
            //        {
            //            ChatFunction->ToggleChatUIVisible();
            //        }
            //    }
            //}

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
    UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::SpawnAndSwitchToCamera()"));
    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::SpawnAndSwitchToCamera() LevelName : %s"), *LevelName);

    FVector CameraLocation;
    FRotator CameraRotation;

    if (LevelName == "Main_Sky" || LevelName == "Main_Login" || LevelName == "Main_Question")
    {
        // �ϴ� ���� ��ġ�� ȸ�� ����
        CameraLocation = FVector(-470047.589317, 643880.89814, 648118.610643);
        CameraRotation = FRotator(9.157953, 200.435537, 0.000001); 
        UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::SpawnAndSwitchToCamera() Set Sky Camera Transform"));
        //CameraRotation = FRotator(0, 0, 0);
    }
    //else if (LevelName == "Main_Room")C:/Project/MetaSion/Content/Junguk/Maps/Template/LV_Winter.umap
    else if (LevelName.Contains("Main_LV"))
    {
        // �� ���� ��ġ�� ȸ�� ����
        //CameraLocation = FVector(3004.710844, -40.193309, 83.381573);
        //CameraRotation = FRotator(4.510870, 1980.785016, 0);
        CameraLocation = FVector(-100.266574, 3428.386539, -455.570113);
        CameraRotation = FRotator(2.600000, -90.800001, 0.0);
        UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::SpawnAndSwitchToCamera() Set Main_LV Camera Transform"));
    }

    // ī�޶� ���͸� ����
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    ACameraActor* TargetCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), CameraLocation, CameraRotation, SpawnParams);

    //if (LevelName == "Main_Room") {
    if (LevelName.Contains("Main_LV"))
    {
        TargetCamera->GetCameraComponent()->SetFieldOfView(50);
    }
    else if (LevelName == "Main_Login"|| LevelName == "Main_Sky" || LevelName == "Main_Question")
    {
        TargetCamera->GetCameraComponent()->SetFieldOfView(90);
    }
    if (TargetCamera)
    {
        SetViewTarget(TargetCamera);
        UE_LOG(LogTemp, Warning, TEXT("Camera view switched to target camera successfully."));

        // ī�޶� ��ȯ�� �Ϸ�Ǹ� Ÿ�̸� ����
        GetWorldTimerManager().ClearTimer(LevelCheckTimerHandle);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn target camera."));
    }
}
void AJS_RoomController::SetChangeLevelData()
{
    AKGW_RoomlistActor* MyWorldPlayer = Cast<AKGW_RoomlistActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AKGW_RoomlistActor::StaticClass()));
   SessionGI->WorldSetting;

   AKGW_RoomlistActor* ListActor = Cast<AKGW_RoomlistActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AKGW_RoomlistActor::StaticClass()));
   UWidgetComponent* WidgetComp = ListActor->FindComponentByClass<UWidgetComponent>();
   UKGW_RoomList* Showlist = Cast<UKGW_RoomList>(WidgetComp->GetUserWidgetObject());

    // 2.추천 음악을 튼다
    HttpActor->SetBackgroundSound();
    // 3.캐릭터 색상을 변경한다.
    if (MyWorldPlayer)
    {
        FMyRGBColor RGB = SessionGI->WorldSetting.RGB;
        FLinearColor ColorToSet(RGB.R, RGB.G, RGB.B);
        UE_LOG(LogTemp, Warning, TEXT("Setting Material Color: R=%f, G=%f, B=%f"), ColorToSet.R, ColorToSet.G, ColorToSet.B);
        MyWorldPlayer->SetMaterialColor(ColorToSet);
    }
    // 4.파티클 색을 변경한다 +  감정 파티클을 변경한다.
    HttpActor->ApplyMyWorldPointLightColors();
    HttpActor->ApplyMyWorldNiagaraAssets();
    // 5.방 목록의 제목을 UI에 넣는다.
    if (SessionGI)
    {
        SessionGI->InitRoomNameNum(SessionGI->WorldSetting.MyRooms); // 데이터가 제대로 저장되었는지 로그로 확인
        UE_LOG(LogTemp, Warning, TEXT("GameInstance->InitRoomInfoList size: %d"), SessionGI->RoomInfoList.Num());
        TArray<FMyWorldRoomInfo> Result;
        Result = SessionGI->GettRoomNameNum(); // 데이터가 제대로 저장되었는지 로그로 확인
        UE_LOG(LogTemp, Warning, TEXT("GameInstance->GEtRoomInfoList size: %d"), Result.Num());
        if (ListActor)
        {
            if (WidgetComp)
            {
                if (Showlist)
                {
                    // RoomInfoList 데이터를 위젯에 추가
                    Showlist->AddSessionSlotWidget(Result);
                    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::OnResPostChoice() Showlist updated successfully."));

                    // 6.AI 분석 결과를 UI에 넣는다.
                    Showlist->SetTextLog(SessionGI->WorldSetting.Result);
                    // move to sugested tmeplate room 방이동
                    Showlist->SetWheaterNumb(SessionGI->WorldSetting.Quadrant);

                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Showlist is null! Make sure the widget is correctly set in BP_ListActor."));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("WidgetComponent not found on BP_ListActor."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("No BP_ListActor."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameInstance is null!"));
    }
}
void AJS_RoomController::SwitchToCamera()
{
    // 월드가 유효한지 확인
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("World is null."));
        return;
    }

    // 월드에서 모든 CineCameraActor 검색
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, ACineCameraActor::StaticClass(), FoundActors);
    FString LevelName = TEXT("LV_Winter");
    for (AActor* Actor : FoundActors)
    {
        if (Actor->GetName() == LevelName) // 이름 일치 확인
        {
            ACineCameraActor* TargetCamera = Cast<ACineCameraActor>(Actor);
            if (TargetCamera)
            {
                // 뷰를 해당 카메라로 전환
                SetViewTarget(TargetCamera);
                UE_LOG(LogTemp, Log, TEXT("View switched to camera: %s"), *LevelName);
                return;
            }
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Camera with name %s not found."), *LevelName);
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

//Initial Inner World Setting Start ------------------------------------------------------------------------
void AJS_RoomController::InitInnerWorldSetting()
{
    UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::InitInnerWorldSetting()"));
    //1. 템플릿에 따른 Setting UI 값 설정 
     timeOfDay = SessionGI->WorldSetting.TimeOfDay;
     cloudCoverage = SessionGI->WorldSetting.CloudCoverage;
     fog = SessionGI->WorldSetting.Fog;
     rain = SessionGI->WorldSetting.Rain;
     snow = SessionGI->WorldSetting.Snow;
     dust = SessionGI->WorldSetting.Dust;
     thunder = SessionGI->WorldSetting.Thunder;
     mainObject = SessionGI->WorldSetting.MainObject;
     subObject = SessionGI->WorldSetting.SubObject;
     background = SessionGI->WorldSetting.Background;

     // 각 변수 값에 대한 로그 출력
     UE_LOG(LogTemp, Warning, TEXT("timeOfDay: %s"), *timeOfDay);
     UE_LOG(LogTemp, Warning, TEXT("cloudCoverage: %s"), *cloudCoverage);
     UE_LOG(LogTemp, Warning, TEXT("fog: %s"), *fog);
     UE_LOG(LogTemp, Warning, TEXT("rain: %s"), *rain);
     UE_LOG(LogTemp, Warning, TEXT("snow: %s"), *snow);
     UE_LOG(LogTemp, Warning, TEXT("dust: %s"), *dust);
     UE_LOG(LogTemp, Warning, TEXT("thunder: %s"), *thunder);
     UE_LOG(LogTemp, Warning, TEXT("mainObject: %s"), *mainObject);
     UE_LOG(LogTemp, Warning, TEXT("subObject: %s"), *subObject);
     UE_LOG(LogTemp, Warning, TEXT("background: %s"), *background);

    if (SettingUI)
    {
        UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::InitInnerWorldSetting() SettingUI is existed"));
        // 블루프린트 이벤트 호출
        //SettingUI->OnValueSet.Broadcast(timeOfDay, cloudCoverage, fog, rain, snow, dust, thunder, mainObject, subObject, background);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT(" AJS_RoomController::InitInnerWorldSetting() No SettingUI"));
    }
    //2. 파티클 색 + 감정 파티클 적용
    HttpActor->ApplyMyWorldPointLightColors();
    HttpActor->ApplyMyWorldNiagaraAssets();
}

/* Inner World UI (After Setting UI Hidden) */
void AJS_RoomController::ShowInnerWorldUIZero()
{
    UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::ShowInnerWorldUIZero()"));
    if (CR_UIFactory)
    {
        if (!CR_UI)
        {
            UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::ShowInnerWorldUIZero() not exsited InnerWorldUI"));
			CR_UI = CreateWidget<UJS_CreateRoomWidget>(GetWorld(), CR_UIFactory);
			if (CR_UI)
			{
				UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::ShowInnerWorldUIZero() InnerWorldUI assigned"));
				CR_UI->SetVisibility(ESlateVisibility::Visible);
                CR_UI->SwitchToWidget(0);
				UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::ShowInnerWorldUIZero() InnerWorldUI set Visible"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT(" AJS_RoomController::ShowInnerWorldUIZero() No InnerWorldUI"));
			}
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT(" AJS_RoomController::ShowInnerWorldUIZero() already exsited InnerWorldUI"));
            CR_UI->SetVisibility(ESlateVisibility::Visible);
            CR_UI->SwitchToWidget(0);
            UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::ShowInnerWorldUIZero() InnerWorldUI set Visible"));
        }

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AJS_RoomController::ShowInnerWorldUIZero() Failed to create CR_UIFactory"));
    }
  

}
void AJS_RoomController::HideInnerWorldUI()
{
    UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::HideInnerWorldUI()"));
    if (CR_UI)
    {
        UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::HideInnerWorldUI() CR_UI exsied"));
       /* UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::HideInnerWorldUI() InnerWorldUI is existed"));
        CR_UI->SetVisibility(ESlateVisibility::Hidden);
        UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::HideInnerWorldUI() InnerWorldUI set Hidden"));*/

        CR_UI->RemoveFromParent();
        CR_UI = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::HideInnerWorldUI() Inner World UI Remove"));
    
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT(" AJS_RoomController::HideInnerWorldUI() No InnerWorldUI"));
    }
}

//Initial Inner World Setting End --------------------------------------------------------------------------

//Inner World Setting UI Start -----------------------------------------------------------------------------
void AJS_RoomController::ShowSettingUI()
{
    UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::ShowSettingUI()"));
    // SettingUIFactory가 유효하고 SettingUI가 생성되지 않은 경우에만 생성
    if (SettingUIFactory && !SettingUI)
    {
        SettingUI = CreateWidget<UCJS_InnerWorldSettingWidget>(GetWorld(), SettingUIFactory);

        if (SettingUI)
        {
            // 화면에 추가
            SettingUI->AddToViewport();
            UE_LOG(LogTemp, Warning, TEXT("Setting UI shown"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create Setting UI"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create SettingUIFactory"));
    }
}
void AJS_RoomController::HideSettingUI()
{
    UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::HideSettingUI()"));
    if (SettingUI)
    {
        // SettingUI를 화면에서 제거
        SettingUI->RemoveFromParent();

        // SettingUI 포인터를 nullptr로 설정하여 다시 ShowSettingUI에서 새로 생성할 수 있도록 함
        SettingUI = nullptr;

        UE_LOG(LogTemp, Warning, TEXT("Setting UI hidden"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create Setting UI"));
    }
}
// Inner World Setting UI End ------------------------------------------------------------------------------



