// Fill out your copyright notice in the Description page of Project Settings.

#include "KGW/KGW_RoomList.h"
#include "KGW/KGW_UserRoomName.h"
#include "HttpActor.h"
#include "JS_RoomController.h"
#include "JS_ShowColorActor.h"
#include "JS_RoomButton.h"
#include "CJS/CJS_InnerWorldParticleActor.h"
#include "CJS/CJS_InnerWorldSettingWidget.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"
#include "Components/WidgetSwitcher.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SizeBox.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/SceneCapture2D.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"


void UKGW_RoomList::NativeConstruct()
{
    Super::NativeConstruct();

    btn_ShowCharacter->OnClicked.AddDynamic(this, &UKGW_RoomList::ShowCharacterUI);
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
   /* SettingImagePath();*/
    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    if (LevelName == "Main_Sky") {
        //GetWorld()->GetTimerManager().SetTimer(SpawnBallTimerHandle, this, &UKGW_RoomList::SpawnBall, 3.2f, false);
        GetWorld()->GetTimerManager().SetTimer(SpawnBallTimerHandle, this, &UKGW_RoomList::ShowCharacterUI, 3.2f, false);
    }
}
void UKGW_RoomList::SpawnBall()
{
    UE_LOG(LogTemp, Warning, TEXT("UKGW_RoomList::SpawnBall()"));
    if (!SpawnShowColorActorFactory)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnShowColorActorFactory is not set!"));
        return;
    }
    // ���� ���� ����
    /*if (CurrentBallActor)
    {
        CurrentBallActor->Destroy();
        CurrentBallActor = nullptr;
    }*/
    FVector Location = FVector(-470356, 643770.0, 648165);
    FRotator Rotation = FRotator::ZeroRotator;

    FMyRGBColor RGB = SGI->WorldSetting.RGB;
    FLinearColor ColorToSet(RGB.R, RGB.G, RGB.B);
    CurrentBallActor = GetWorld()->SpawnActor<AJS_ShowColorActor>(SpawnShowColorActorFactory, Location, Rotation);
    if (CurrentBallActor)
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully spawned ball at Location: %s"), *Location.ToString());

        CurrentBallActor->SetMaterialColor(ColorToSet);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn ball!"));
    }

    FMyWorldSetting WorldSetting = SGI->WorldSetting;
    FString explain = WorldSetting.Result2;
    UE_LOG(LogTemp, Warning, TEXT("UKGW_RoomList::SpawnBall() explain %s"), *explain);
    if (txt_SG_ColorReason)
    {
        UE_LOG(LogTemp, Warning, TEXT("txt_SG_ColorReason exsited"));
        txt_SG_ColorReason->SetText(FText::FromString(explain));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("txt_SG_ColorReason nullptr"));
    }
}
void UKGW_RoomList::DestroyBall()
{
    UE_LOG(LogTemp, Warning, TEXT("UKGW_RoomList::DestroyBall()"));
    if (CurrentBallActor)
    {
        CurrentBallActor->Destroy();
        CurrentBallActor = nullptr;
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
            // 캐릭터 데이터 받아서 보여주기
            if (CurrentParticleActor) CleanParticle();
            SpawnBall();
            break;
        case 2:
            //파티클 스폰
            if (CurrentBallActor) DestroyBall();
            SpawnParticle();     
            break;
        case 3:
            //AI 결과 저장해서 보여주기
            if (CurrentBallActor) DestroyBall();
            if (CurrentParticleActor) CleanParticle();
            break;
        case 4:
            // 내방목록 데이터 받아서 보여주기
            if (CurrentBallActor) DestroyBall();
            if (CurrentParticleActor) CleanParticle();
            break;
        case 5:
            // 전체방목록 데이터 받아서 보여주기
            if (CurrentBallActor) DestroyBall();
            if (CurrentParticleActor) CleanParticle();
            break;
        default:
            if (CurrentParticleActor) CleanParticle();
            if (CurrentBallActor) DestroyBall();
            break;
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("ChangeCanvas WS_RoomList nullptr"));
    }
}
void UKGW_RoomList::ShowMenuUI()
{
    //ChangeCanvas(1);
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
        RoomNumberCounter = 0; // 카운터 리셋
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
        RoomNumberCounter = 0; // 카운터 리셋
        SettingPath();
        for (int32 i = 0; i < 21; i++) {
            FString RandomPath = GetRandomPath();
            RoomNumber = i;
            AddImageToGrid(RandomPath);
        }
    }
    ChangeCanvas(5);
}
void UKGW_RoomList::ShowCharacterUI()
{
    ChangeCanvas(1);
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
    SizeBox->SetWidthOverride(1200.0f);
    SizeBox->SetHeightOverride(450.0f);

    // 버튼 생성
    UJS_RoomButton* ImageButton = NewObject<UJS_RoomButton>(this);
    ImageButton->Initialize();
    ImageButton->RL = this;

    // 버튼의 인덱스 할당
    ImageButton->SetIndex(RoomNumberCounter);
    RoomNumberCounter++; // RoomNumber 카운터 증가

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

    // 이미지를 버튼의 자식으로 추가
    ImageButton->AddChild(NewImage);

    // SizeBox에 버튼을 추가
    SizeBox->AddChild(ImageButton);

    // 버튼 이벤트 바인딩
    ImageButton->OnHovered.AddDynamic(this, &UKGW_RoomList::OnImageHovered);
    ImageButton->OnUnhovered.AddDynamic(this, &UKGW_RoomList::OnImageUnhovered);

    // RoomList UI에 버튼 추가
    if (bRoomList)
    {
        int32 RowCount = UGP_RoomList->GetChildrenCount() / 3;
        int32 ColCount = UGP_RoomList->GetChildrenCount() % 3;
        UGP_RoomList->AddChildToUniformGrid(SizeBox, RowCount, ColCount);
    }
    if (bMultiRoomList)
    {
        int32 RowCount = UGP_Multi_RoomList->GetChildrenCount() / 3;
        int32 ColCount = UGP_Multi_RoomList->GetChildrenCount() % 3;
        UGP_Multi_RoomList->AddChildToUniformGrid(SizeBox, RowCount, ColCount);
    }

    // 디버깅 로그 추가
    UE_LOG(LogTemp, Warning, TEXT("AddImageToGrid: Added button with index: %d"), ImageButton->GetIndex());
}

void UKGW_RoomList::SettingPath()
{
    ImagePath.Empty(); // 기존 배열 초기화
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/room_1"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/room_2"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/room_3"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/room_4"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/room_5"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/room_6"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/room_7"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/room_8"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/room_9"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/room_10"));
    ImagePath.Add(TEXT("/Game/Main/Assets/UI/room_11"));
//     ImagePath.Add(TEXT("/Game/Main/Assets/UI/room_1"));
//     ImagePath.Add(TEXT("/Game/Main/Assets/UI/room_1"));
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
//void UKGW_RoomList::OnClickedImageRoomList()
//{
//    // 특정 레벨로 이동 여기에 루트 정보 해야함.
//    if (bRoomList) {
//        UGameplayStatics::OpenLevel(GetWorld(), FName("Main_LV_Fall"));
//    }
//    else if (bMultiRoomList) {
//        // 새로운 UI가 뜨고 뜬 UI에서 방이름, 방유사도, 코멘트 넣어서 보여주기
//        ShowOnClickRoomUI();
//        pc->HideRoomListUI();
//        if (OnClickRoomUI) {
//            OnClickRoomUI->SettingData(OnClickRoomUI->ImagePath, ImageButtonTemp);
//        }
//    }
//}
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
    
    //FVector Location = FVector(-470990.0f, 643286.0f, 648362.0f); 
    FVector Location = FVector(-471497.593750f, 643550.875000f, 648328.610643f);
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
    SetTextLog(WorldSetting.Result3);
   
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
// OnClickUI쪽에서 가져온 곳 ----------------------------------------------------------------
FString UKGW_RoomList::SettingImagePath()
{
    SGImagePath = {
            TEXT("/Game/Main/Assets/UI/room_1.room_1"),
            TEXT("/Game/Main/Assets/UI/room_2.room_2"),
            TEXT("/Game/Main/Assets/UI/room_3.room_3"),
            TEXT("/Game/Main/Assets/UI/room_4.room_4"),
            TEXT("/Game/Main/Assets/UI/room_5.room_5"),
            TEXT("/Game/Main/Assets/UI/room_6.room_6"),
            TEXT("/Game/Main/Assets/UI/room_7.room_7"),
            TEXT("/Game/Main/Assets/UI/room_7.room_8"),
            TEXT("/Game/Main/Assets/UI/room_7.room_9"),
            TEXT("/Game/Main/Assets/UI/room_7.room_10"),
            TEXT("/Game/Main/Assets/UI/room_7.room_11")
    };

    int32 RandomIndex = FMath::RandRange(0, SGImagePath.Num() - 1);

    FString SelectedPath = SGImagePath[RandomIndex];

    if (!SelectedPath.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Randomly Selected Path: %s"), *SelectedPath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to select an ImagePath!"));
    }

    return SelectedPath;
}

void UKGW_RoomList::SettingData(FString TexturePath, UJS_RoomButton* Button)
{
    // 초기 null 체크
    if (!pc || !pc->SessionGI || !Button)
    {
        UE_LOG(LogTemp, Warning, TEXT("Required components are null"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("KGW_RoomListUI::SettingData()"));
    // �ؽ�ó ��ο��� UTexture2D �ε�
    UTexture2D* ImageTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *TexturePath));

    // �ؽ�ó�� ����� �ε�Ǿ����� Ȯ��
    if (!ImageTexture)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load texture from path: %s"), *TexturePath);
        return;
    }
    if (!SG_ImageButton) {
        UE_LOG(LogTemp, Error, TEXT("Fail to ImageButton"));
        return;
    }

    //Image Setting
    SG_RoomImage->SetBrushFromTexture(ImageTexture, true);
    // suggest_list 접근
    const TArray<FMySuggest_List>& SuggestList = pc->SessionGI->WorldSetting.suggest_list;
    if (SuggestList.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Suggest list is empty"));
        return;
    }


    // 버튼 인덱스 처리
    int32 ButtonIndex = Button->GetIndex();
    if (ButtonIndex < 0 || ButtonIndex >= SuggestList.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid button index: %d"), ButtonIndex);
        return;
    }
    // 데이터 설정
    const FMySuggest_List& Suggestion = SuggestList[ButtonIndex];

    // UI 컴포넌트 null 체크 후 설정
    if (SG_RoomName)
        SG_RoomName->SetText(FText::FromString(Suggestion.room_name));
    if (SG_RoomOwner)
        SG_RoomOwner->SetText(FText::FromString(Suggestion.room_id));
    if (SG_Similarity)
        SG_Similarity->SetText(FText::FromString(FString::Printf(TEXT("Similarity: %s%"), *Suggestion.percent_message)));
    if (SG_ReasonForRecommendation)
        SG_ReasonForRecommendation->SetText(FText::FromString(Suggestion.reason_message));
    if (SG_RoomComment)
        SG_RoomComment->SetText(FText::FromString(Suggestion.reason_message));
}
// OnClickUI쪽에서 가져온 곳 ----------------------------------------------------------------

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
