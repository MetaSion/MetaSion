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

void UKGW_RoomList::NativeConstruct()
{
    Super::NativeConstruct();

    btn_ShowParticle->OnClicked.AddDynamic(this, &UKGW_RoomList::ShowParticleUI);
	btn_AIAnalysis->OnClicked.AddDynamic(this, &UKGW_RoomList::ShowAIAnalysisUI);
	btn_MyRoom->OnClicked.AddDynamic(this, &UKGW_RoomList::OnClickInnerWorld);
	btn_MyRoom_List->OnClicked.AddDynamic(this, &UKGW_RoomList::ShowMyRoomListUI);
	btn_List_of_all_rooms->OnClicked.AddDynamic(this, &UKGW_RoomList::ShowListOfAllRooms);
    btn_MultiWorld->OnClicked.AddDynamic(this, &UKGW_RoomList::OnClickMultiWorld);

    pc = Cast<AJS_RoomController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!pc) {
        UE_LOG(LogTemp, Error, TEXT("PlayerController not found in the level!"));
        return;
    }
    //Path Setting
    SettingPath();
    //Setting Random Path 
    for (int32 i = 0; i < 21; i++) {
        FString RandomPath = GetRandomPath(); // 랜덤 경로 선택
        AddImageToGrid(RandomPath); // 무작위 경로를 추가
        UE_LOG(LogTemp, Warning, TEXT("RandomPath : %s"), *RandomPath);
    }
}

void UKGW_RoomList::ChangeCanvas(int32 index)
{
    if (WS_RoomList) {
        WS_RoomList->SetActiveWidgetIndex(index);
        switch (index)
        {
        case 1:
            //파티클 스폰
            SpawnParticle();
            break;
        case 2:
            //AI 결과 저장해서 보여주기
            if(CurrentParticleActor) CleanParticle();

            break;
        case 3:
            // 내방목록 데이터 받아서 보여주기
            if (CurrentParticleActor) CleanParticle();

            break;
        case 4:
            // 전체방목록 데이터 받아서 보여주기
            if (CurrentParticleActor) CleanParticle();

            break;
        default:
            break;
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("ChangeCanvas WS_RoomList nullptr"));
    }
}
void UKGW_RoomList::ShowParticleUI()
{
    ChangeCanvas(0);
}
void UKGW_RoomList::ShowAIAnalysisUI()
{
    ChangeCanvas(1);
}
void UKGW_RoomList::ShowMyRoomListUI()
{
    ChangeCanvas(2);
}
void UKGW_RoomList::ShowListOfAllRooms()
{
    ChangeCanvas(3);
}
// GridPanel 부분 -------------------------------------------------------------
void UKGW_RoomList::AddImageToGrid(FString TexturePath)
{
    // 텍스처 경로에서 UTexture2D 로드
    UTexture2D* ImageTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *TexturePath));

    // 텍스처가 제대로 로드되었는지 확인
    if (!ImageTexture)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load texture from path: %s"), *TexturePath);
        return;
    }

    // SizeBox 생성 (버튼 크기 설정)
    USizeBox* SizeBox = NewObject<USizeBox>(this);
    SizeBox->SetWidthOverride(600.0f);  // 너비 설정
    SizeBox->SetHeightOverride(500.0f); // 높이 설정

    // 버튼 생성
    UButton* ImageButton = NewObject<UButton>(this);

    // 이미지 생성 및 설정
    UImage* NewImage = NewObject<UImage>(this);

    // 로드된 텍스처를 이미지에 적용
    NewImage->SetBrushFromTexture(ImageTexture, true);
    NewImage->SetDesiredSizeOverride(FVector2D(600, 500));  // 이미지의 크기도 맞춤

    // 이미지를 버튼의 자식으로 추가
    ImageButton->AddChild(NewImage);

    // SizeBox에 버튼을 추가
    SizeBox->AddChild(ImageButton);

    // 버튼 이벤트 바인딩
    ImageButton->OnHovered.AddDynamic(this, &UKGW_RoomList::OnImageHovered);
    ImageButton->OnUnhovered.AddDynamic(this, &UKGW_RoomList::OnImageUnhovered);
    ImageButton->OnClicked.AddDynamic(this, &UKGW_RoomList::OnImageClicked);

    // 그리드에 SizeBox 추가 (여기서 SizeBox가 그리드에 들어감)
    int32 RowCount = UGP_RoomList->GetChildrenCount() / 3;
    int32 ColCount = UGP_RoomList->GetChildrenCount() % 3;
    UGP_RoomList->AddChildToUniformGrid(SizeBox, RowCount, ColCount);

    UE_LOG(LogTemp, Log, TEXT("SizeBox width: %f, height: %f"), SizeBox->GetDesiredSize().X, SizeBox->GetDesiredSize().Y);
    UE_LOG(LogTemp, Log, TEXT("ImageButton width: %f, height: %f"), ImageButton->GetDesiredSize().X, ImageButton->GetDesiredSize().Y);
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
    UGameplayStatics::OpenLevel(GetWorld(), FName("Main_LV_Fall"));
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

    FVector Location = FVector(-470990.0f, 643490.0f, 648180.0f);
    FRotator Rotation = FRotator::ZeroRotator;

    CurrentParticleActor = GetWorld()->SpawnActor<AActor>(
        ParticleActorFactory, Location, Rotation);

    if (CurrentParticleActor)
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully spawned particle actor"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn particle actor"));
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

    UGameplayStatics::OpenLevel(this, FName("Main_LV_Winter"));

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
