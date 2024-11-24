// Fill out your copyright notice in the Description page of Project Settings.

#include "KGW/KGW_RoomList.h"
#include "KGW/KGW_UserRoomName.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "HttpActor.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "CJS/CJS_InnerWorldSettingWidget.h"
#include "Components/WidgetSwitcher.h"
#include "JS_RoomController.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/SceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "JS_OnClickRoomUI.h"
#include "CJS/CJS_InnerWorldParticleActor.h"
#include "JS_ShowColorActor.h"

void UKGW_RoomList::NativeConstruct()
{
    Super::NativeConstruct();

    btn_ShowParticle->OnClicked.AddDynamic(this, &UKGW_RoomList::ShowParticleUI);
	btn_MyRoom_List->OnClicked.AddDynamic(this, &UKGW_RoomList::ShowMyRoomListUI);
	btn_List_of_all_rooms->OnClicked.AddDynamic(this, &UKGW_RoomList::ShowListOfAllRooms);
    btn_MyRoom->OnClicked.AddDynamic(this, &UKGW_RoomList::OnClickInnerWorld);
    btn_MultiWorld->OnClicked.AddDynamic(this, &UKGW_RoomList::OnClickMultiWorld);

    pc = Cast<AJS_RoomController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!pc) {
        UE_LOG(LogTemp, Error, TEXT("KGW_RoomList PlayerController not found in the level!"));
        return;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("KGW_RoomList::BeginPlay() No pc"));
    }
    HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
    if (HttpActor){
        UE_LOG(LogTemp, Warning, TEXT("KGW_RoomList::BeginPlay() Set HttpActor"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("KGW_RoomList::BeginPlay() No HttpActor"));
    }
    SGI = Cast<USessionGameInstance>(GetGameInstance());
    if (SGI) {
        UE_LOG(LogTemp, Warning, TEXT("KGW_RoomList::BeginPlay() Set SessionGI"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("KGW_RoomList::BeginPlay() No SessionGI"));
    }
    InitializeOnClickRoomUI();
    GetWorld()->GetTimerManager().SetTimer(SpawnBallTimerHandle, this, &UKGW_RoomList::SpawnBall, 3.2f, false);
}
void UKGW_RoomList::SpawnBall()
{
    if (!SpawnShowColorActorFactory)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnShowColorActorFactory is not set!"));
        return;
    }

    // ���� ���� ����
    if (CurrentBallActor)
    {
        CurrentBallActor->Destroy();
        CurrentBallActor = nullptr;
    }
    FVector Location = FVector(-470356, 643870, 648165);
    FRotator Rotation = FRotator::ZeroRotator;

    CurrentBallActor = GetWorld()->SpawnActor<AJS_ShowColorActor>(SpawnShowColorActorFactory, Location, Rotation);
    if (CurrentBallActor)
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully spawned ball at Location: %s"), *Location.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn ball!"));
    }
}
void UKGW_RoomList::ChangeCanvas(int32 index)
{
    if (WS_RoomList) {
        int32 WidgetIndex = index - 1;
        WS_RoomList->SetActiveWidgetIndex(WidgetIndex);
        switch (index)
        {
        case 1:
            //처음 아무것도 없는 화면 세팅
            HideOnClickRoomUI();
            ShowMenuUI();
        case 2:
            //파티클 스폰
            HideOnClickRoomUI();
            SpawnParticle();
            break;
        case 3:
            //AI 결과 저장해서 보여주기
            HideOnClickRoomUI();
            if (CurrentParticleActor) CleanParticle();
            break;
        case 4:
            HideOnClickRoomUI();
            // 전체방목록 데이터 받아서 보여주기
            if (CurrentParticleActor) CleanParticle();
            // 내방목록 데이터 받아서 보여주기
            break;
        case 5:
            HideOnClickRoomUI();
            // 전체방목록 데이터 받아서 보여주기
            if (CurrentParticleActor) CleanParticle();
            break;
        default:
            HideOnClickRoomUI();
            if (CurrentParticleActor) CleanParticle();
            break;
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("ChangeCanvas WS_RoomList nullptr"));
    }
}
void UKGW_RoomList::ShowMenuUI()
{
    ChangeCanvas(1);
}
void UKGW_RoomList::ShowParticleUI()
{
    ChangeCanvas(2);
}
//void UKGW_RoomList::ShowAIAnalysisUI()
//{
//    ChangeCanvas(3);
//}
void UKGW_RoomList::ShowMyRoomListUI()  
{
    if (!bIsListMyRooms) {
        bIsListMyRooms = true;
        bRoomList = true;
        bMultiRoomList = false;
        SettingPath();
        for (int32 i = 0; i < 21; i++) {
            FString RandomPath = GetRandomPath();
            AddImageToGrid(RandomPath);
        }
    }
    ChangeCanvas(4);
}
void UKGW_RoomList::ShowListOfAllRooms()
{
    if (!bIsListAllRooms) {
        bIsListAllRooms = true;
        bRoomList = false;
        bMultiRoomList = true;
        SettingPath();
        for (int32 i = 0; i < 21; i++) {
            FString RandomPath = GetRandomPath();
            AddImageToGrid(RandomPath);
        }
    }
    ChangeCanvas(5);
}
// GridPanel 부분 -------------------------------------------------------------
void UKGW_RoomList::AddImageToGrid(FString TexturePath)
{
    // 텍스처 로드
    UTexture2D* ImageTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *TexturePath));

    if (!ImageTexture)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load texture from path: %s"), *TexturePath);
        return;
    }

    // SizeBox 생성
    USizeBox* SizeBox = NewObject<USizeBox>(this);
    SizeBox->SetWidthOverride(600.0f);
    SizeBox->SetHeightOverride(500.0f);

    // 버튼 생성
    UButton* ImageButton = NewObject<UButton>(this);

    // 버튼의 배경 스타일 설정
    FButtonStyle ButtonStyle;
    FSlateBrush NormalBrush;
    NormalBrush.TintColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f); // RGBA where A=0 means fully transparent
    ButtonStyle.SetNormal(NormalBrush);
    ButtonStyle.SetHovered(NormalBrush);
    ButtonStyle.SetPressed(NormalBrush);
    ImageButton->SetStyle(ButtonStyle);

    // 이미지 생성 및 설정
    UImage* NewImage = NewObject<UImage>(this);
    NewImage->SetBrushFromTexture(ImageTexture, true);
    NewImage->SetDesiredSizeOverride(FVector2D(600, 500));

    // 이미지의 배경 브러시 설정
    FSlateBrush ImageBrush = NewImage->Brush;
    ImageBrush.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f); // 이미지 자체는 완전 불투명
    NewImage->SetBrush(ImageBrush);

    // 이미지를 버튼의 자식으로 추가
    ImageButton->AddChild(NewImage);

    // SizeBox에 버튼을 추가
    SizeBox->AddChild(ImageButton);

    // 버튼 이벤트 바인딩
    ImageButton->OnHovered.AddDynamic(this, &UKGW_RoomList::OnImageHovered);
    ImageButton->OnUnhovered.AddDynamic(this, &UKGW_RoomList::OnImageUnhovered);
    ImageButton->OnClicked.AddDynamic(this, &UKGW_RoomList::OnImageClicked);

    if (bRoomList) {
        int32 RowCount = UGP_RoomList->GetChildrenCount() / 3;
        int32 ColCount = UGP_RoomList->GetChildrenCount() % 3;
        UGP_RoomList->AddChildToUniformGrid(SizeBox, RowCount, ColCount);
    }
    if (bMultiRoomList) {
        int32 RowCount = UGP_Multi_RoomList->GetChildrenCount() / 3;
        int32 ColCount = UGP_Multi_RoomList->GetChildrenCount() % 3;
        UGP_Multi_RoomList->AddChildToUniformGrid(SizeBox, RowCount, ColCount);
    }
}

void UKGW_RoomList::SettingPath()
{
    ImagePath.Empty(); // 기존 배열 초기화
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/Thunder"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/BG"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/cloudy"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/partlysunny"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/Rainy"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/snow"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/storm"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/sunny"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/Thunder"));
}

FString UKGW_RoomList::GetRandomPath()
{
    if (ImagePath.Num() == 0) {
        UE_LOG(LogTemp, Warning, TEXT("ImagePath array is empty!"));
        return FString(); // 빈 문자열 반환
    }

    int32 RandomIndex = FMath::RandRange(0, ImagePath.Num() - 1); // 랜덤 인덱스 선택
    return ImagePath[RandomIndex];
}
void UKGW_RoomList::InitializeOnClickRoomUI()
{
    OnClickRoomUI = CreateWidget<UJS_OnClickRoomUI>(this, OnClickRoomUIFactory);
    if (OnClickRoomUI)
    {
        UE_LOG(LogTemp, Warning, TEXT(" AJS_RoomController::InitializeUIWidgets() OnClickRoomUI set"));
        OnClickRoomUI->AddToViewport();
        OnClickRoomUI->SetVisibility(ESlateVisibility::Hidden);
    }
}
void UKGW_RoomList::ShowOnClickRoomUI()
{
    if (OnClickRoomUI) {
        OnClickRoomUI->SetVisibility(ESlateVisibility::Visible);
    }
}
void UKGW_RoomList::HideOnClickRoomUI()
{
    if (OnClickRoomUI) {
        OnClickRoomUI->SetVisibility(ESlateVisibility::Hidden);
    }
}
void UKGW_RoomList::OnImageHovered()
{
    // 호버 시 댓글 UI 표시
    ShowCommentUI(nullptr);
}
void UKGW_RoomList::OnImageUnhovered()
{
    // 호버 해제 시 댓글 UI 숨김
    HideCommentUI();
}
void UKGW_RoomList::OnImageClicked()
{
    // 특정 레벨로 이동 여기에 루트 정보 해야함.
    if (bRoomList) {
        UGameplayStatics::OpenLevel(GetWorld(), FName("Main_LV_Fall"));
    }
    else if (bMultiRoomList) {
        // 새로운 UI가 뜨고 뜬 UI에서 방이름, 방유사도, 코멘트 넣어서 보여주기
        ShowOnClickRoomUI();
        pc->HideRoomListUI();
        if (OnClickRoomUI) {
            OnClickRoomUI->SettingData(OnClickRoomUI->ImagePath);
        }
    }
}
void UKGW_RoomList::ShowCommentUI(UImage* Image)
{
    // 이미지에 대한 댓글 UI 생성 및 표시 로직
}
void UKGW_RoomList::HideCommentUI()
{
    // 댓글 UI 숨기는 로직
}
// GridPanel 부분 End -------------------------------------------------------------

void UKGW_RoomList::SpawnParticle()
{
    if (!ParticleActorFactory)
    {
        UE_LOG(LogTemp, Error, TEXT("ParticleActorFactory is not set!"));
        return;
    }
    CleanParticle();
    
    FVector Location = FVector(-470990.0f, 643286.0f, 648362.0f);
    FRotator Rotation = FRotator::ZeroRotator;

    CurrentParticleActor = GetWorld()->SpawnActor<ACJS_InnerWorldParticleActor>(
        ParticleActorFactory, Location, Rotation);

    // 4.파티클 색을 변경한다 +  감정 파티클을 변경한다.
    if (HttpActor) {
        HttpActor->ApplyMyWorldPointLightColors();
        HttpActor->ApplyMyWorldNiagaraAssets();
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("740 : Setting HttpActor Fail..."));
    }
    //파티클 설명 업데이트 부분
    FMyWorldSetting WorldSetting = SGI->WorldSetting;
    SetTextLog(WorldSetting.Result2);

    if (CurrentParticleActor)
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully spawned particle actor"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn particle actor"));
    }
}

void UKGW_RoomList::CleanParticle()
{
    // ���� ���� ����
    if (CurrentParticleActor)
    {
        CurrentParticleActor->Destroy();
        CurrentParticleActor = nullptr;
    }
}

void UKGW_RoomList::AddSessionSlotWidget(const TArray<FMyWorldRoomInfo>& RoomInfos)
{
    UE_LOG(LogTemp, Warning, TEXT("UKGW_RoomList::AddSessionSlotWidget()"));
    if (!ScrollBox)
    {
        UE_LOG(LogTemp, Error, TEXT("ScrollBox is null! Make sure it is set correctly in the widget."));
        return;
    }
    //ScrollBox->ClearChildren();
    for (const FMyWorldRoomInfo& Room : RoomInfos)
    {
        auto* RoomSlot = CreateWidget<UKGW_UserRoomName>(this, UserRoomNameFactory);
        if (RoomSlot)
        {
            // RoomWidget�� ������ ������Ʈ (��: �� �̸� ����)
            RoomSlot->UpdateInfo(Room);
            ScrollBox->AddChild(RoomSlot);
        }
    }  
    UE_LOG(LogTemp, Log, TEXT("Room list updated with %d rooms."), RoomInfos.Num());
}

void UKGW_RoomList::SetTextLog(FString explain)
{
    if (TxtBox_Report) {
        TxtBox_Report->SetText(FText::FromString(explain));
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("TxtBox_Report nullptr"));
    }
    if (txt_SG_ParticleReason) {
        txt_SG_ParticleReason->SetText(FText::FromString(explain));
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("txt_SG_ParticleReason nullptr"));
    }
}
void UKGW_RoomList::SetWheaterNumb(FString TempNUmb)
{
    WheatherNum = TempNUmb;
}


void UKGW_RoomList::OnClickInnerWorld()
{   
    UE_LOG(LogTemp, Warning, TEXT("UKGW_RoomList::OnClickInnerWorld()"));
//     if (!WheatherNum.IsEmpty())
//     { 
//     
// 		if (WheatherNum == TEXT("1"))
// 		{
// 			UGameplayStatics::OpenLevel(this, FName("Main_LV_Spring"));
// 		}
// 		else if (WheatherNum == TEXT("2"))
// 		{
// 			UGameplayStatics::OpenLevel(this, FName("Main_LV_Summer"));
// 
// 		}
// 		else if (WheatherNum == TEXT("3"))
// 		{
// 			UGameplayStatics::OpenLevel(this, FName("Main_LV_Fall"));
// 
// 		}
// 		else if (WheatherNum == TEXT("4"))
// 		{
//             InnerWorldWidget ->SetWinterSnowSlider();
// 		}
//     }

    UGameplayStatics::OpenLevel(this, FName("Room_2"));

}

void UKGW_RoomList::OnClickMultiWorld()
{
    UE_LOG(LogTemp, Warning, TEXT("UKGW_RoomList::OnClickMultiWorld()"));
    StartHttpMultyWorld();
}

//마이월드 -> 멀티월드 버튼 클릭 시 통신
void UKGW_RoomList::StartHttpMultyWorld()
{
    UE_LOG(LogTemp, Warning, TEXT("UKGW_RoomList::StartHttpMultyWorld()"));
    OpenActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
    if (!OpenActor)
    {
        UE_LOG(LogTemp, Error, TEXT("OpenActor is not initialized!"));
        return;
    }

    USessionGameInstance* SessionGI = Cast<USessionGameInstance>(GetGameInstance());
    if (SessionGI)
    {
        FString UserId = SessionGI->MySessionName;
        TMap<FString, FString> MyRoomData;
        MyRoomData.Add("userId", UserId);
        FString JsonRequest = UJsonParseLib::MakeJson(MyRoomData);

        // 서버로 요청 전송
        //OpenActor->ReqPostClickMultiWorld(OpenActor->EntryMultiWorldURL, JsonRequest);
        FString StoredJsonResponsetest = TEXT("{\"UserId\":\"testuser\",\"R\":1.0,\"G\":0.9225690792809692,\"B\":0.4,\"SimilarUsers\":[{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sunny World\"},{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sol World\"},{\"UserId\":\"abc11\",\"EmotionScore\":81.0,\"RoomName\":\"KW World\"}],\"OppositeUsers\":[{\"UserId\":\"user_1\",\"EmotionScore\":283.5,\"RoomName\":\"JW World\"},{\"UserId\":\"user_3\",\"EmotionScore\":321.0,\"RoomName\":\"DL World\"}]}");
        OpenActor->CallStartMakeSession(StoredJsonResponsetest);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SessionGI is NULL"));
    }
    //if (!OpenActor)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("OpenActor is not initialized!"));
    //    return;
    //}

    //USessionGameInstance* SessionGI = Cast<USessionGameInstance>(GetGameInstance());
    //if (SessionGI)
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance is set"));
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Error, TEXT("USessionGameInstance is not set"));
    //}
    //FString UserId;
    //if (SessionGI)
    //{
    //    UserId = SessionGI->UserId;
    //    UserId = SessionGI->MySessionName;
    //    UE_LOG(LogTemp, Warning, TEXT("Assigned UserId from MySessionName: %s"), *UserId);
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("gi Nullptr"));
    //    return;
    //}

    //// 사용자 데이터를 맵에 추가
    //TMap<FString, FString> MyRoomData;
    //MyRoomData.Add("userId", UserId);

    //// JSON 형식으로 변환
    //FString JsonRequest = UJsonParseLib::MakeJson(MyRoomData);

    //// 로그 출력 (디버깅용)
    //UE_LOG(LogTemp, Warning, TEXT("userId: %s"), *UserId);
    //UE_LOG(LogTemp, Warning, TEXT("Json Request: %s"), *JsonRequest);

    //// 서버로 요청 전송
    //OpenActor->ReqPostClickMultiWorld(OpenActor->EntryMultiWorldURL, JsonRequest);
}
// void UKGW_RoomList::SetFindActive(bool value)
// {
// 	//ã�Ⱑ ������ Empty �ؽ�Ʈ �Ⱥ��̰� �ϰ�ʹ�.
// 	if (value) {
// 		Text_Finding->SetVisibility(ESlateVisibility::Visible);
// 
// 	}
// 	else {
// 		Text_Finding->SetVisibility(ESlateVisibility::Hidden);
// 
// 		
// 	}
// 
// 
// }
