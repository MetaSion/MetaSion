// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpActor.h"
#include "Blueprint/UserWidget.h"
#include "HttpWidget.h"
#include "JS_CreateRoomWidget.h"
#include "JS_RoomWidget.h"
#include "HttpModule.h"
#include "JsonParseLib.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JS_RoomController.h"
#include "JS_TestWidget.h"
#include "Kismet/GameplayStatics.h"
#include "CJS/CJS_BallPlayer.h"
#include "CJS/SessionGameInstance.h"
#include "Engine/Texture2D.h"
#include "KGW_Wbp_WebImage.h"
#include "ImageUtils.h"
#include "KGW/WBP_Image.h"
#include "KGW/KGW_RoomlistActor.h"
#include "Components/WidgetComponent.h"
#include "KGW/KGW_RoomList.h"
#include "JS_SoundActor.h"
#include "CJS/CJS_InnerWorldParticleActor.h"
#include "CJS/CJS_SubObjectActor.h"
#include "KGW/KGW_WBP_Question.h"
#include "KGW/KGW_ChoiceSaveBF.h"


// Sets default values
AHttpActor::AHttpActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHttpActor::BeginPlay()
{
	Super::BeginPlay();

    pc = Cast<AJS_RoomController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (pc)
    {
        UE_LOG(LogTemp, Error, TEXT("AHttpActor::BeginPlay():: PC exsited"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AHttpActor::BeginPlay():: No PC"));
    }

    // SessionGameInstance 할당
    SessionGI = Cast<USessionGameInstance>(GetGameInstance());
    if (SessionGI)
    {
        UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance is set"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("USessionGameInstance is not set"));
    }

    // Find and reference the Sound Actor in the level
    SoundActor = Cast<AJS_SoundActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_SoundActor::StaticClass()));
    if (!SoundActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("SoundActor not found in the level."));
    }

    // Find and reference the Roomlist Actor in the level
    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    if (LevelName.Contains("Main_Sky"))
    {
        UE_LOG(LogTemp, Warning, TEXT("AJS_RoomController::BeginPlay() LevelName.Contains->Main_LV"));
        MyWorldPlayer = Cast<AKGW_RoomlistActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AKGW_RoomlistActor::StaticClass()));
        if (!MyWorldPlayer)
        {
            UE_LOG(LogTemp, Error, TEXT("AHttpActor::BeginPlay() MyWorldPlayer not found in the level."));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("AHttpActor::BeginPlay() MyWorldPlayer in the level."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AHttpActor::BeginPlay() LevelName : %s"), *LevelName);
    }
}

// Called every frame
void AHttpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Tick 함수에서 MyWorldPlayer를 다시 찾기 시도
	/*if (!MyWorldPlayer)
	{
		MyWorldPlayer = Cast<AKGW_RoomlistActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AKGW_RoomlistActor::StaticClass()));

		if (MyWorldPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("MyWorldPlayer successfully found in Tick."));
		}
	}*/
}
//Login -------------------------------------------------------------
void AHttpActor::LoginReqPost(FString url, FString json)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::LoginReqPost()"));
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // ������� �Լ��� ����
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::LoginResPost);
    // ������ ��û
    req->ProcessRequest();
}
void AHttpActor::LoginResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::LoginResPost()"));

    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }

    //FString result1 = Response->GetContentAsString();
    //UE_LOG(LogTemp, Warning, TEXT("AHttpActor::LoginResPost() result: %s"), *result1);
   //
    // ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    UE_LOG(LogTemp, Warning, TEXT("Response code : %d"), Response->GetResponseCode());
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        FString result = Response->GetContentAsString();
        //FString result = "{\"userId\": \"testuser\", \"userpass\": \"testpassword\"}";  <--- 올바른 JSON 형식
        UE_LOG(LogTemp, Warning, TEXT("Login Post Request Success: %s"), *result);

        //UJsonParseLib::Login_Convert_JsonToStruct(result);
        FLogin LoginData = UJsonParseLib::Login_Convert_JsonToStruct(result); // <---- 추가
        FString userid = LoginData.userId;
        SessionGI->UserId = userid;
        UE_LOG(LogTemp, Warning, TEXT("LoginData.userid: %s"), *userid);
        if (SessionGI)
        {
            SessionGI->InitSessionName(userid);
            UE_LOG(LogTemp, Warning, TEXT("Session Name set to UserId: %s"), *SessionGI->MySessionName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AHttpActor::LoginResPost():: No SessionGM"));
        }
      
        if (SessionGI) {
            UE_LOG(LogTemp, Warning, TEXT("AHttpActor::LoginResPost():: SessionGI exsited"));
            SessionGI->bSuccess = true; // GameInstance에 상태 저장
            //UGameplayStatics::OpenLevel(this, FName("Main_Sky"));

            UGameplayStatics::OpenLevel(this, FName("Main_Question"));
        }
        else
        {
			UE_LOG(LogTemp, Error, TEXT("AHttpActor::LoginResPost():: No SessionGI"));
        }
       
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("LoginResPost() Failed..."));
    }
}
//Login End-------------------------------------------------------------

//Sign UP -------------------------------------------------------------
void AHttpActor::SignUpReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // ������� �Լ��� ����
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::SignUpResPost);
    // ������ ��û
    req->ProcessRequest();
}

void AHttpActor::SignUpResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    // ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // ������ ���ڿ��� ��������
        FString result = Response->GetContentAsString();
        UJsonParseLib::SignUp_Convert_JsonToStruct(result);
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
//Sign UP End -------------------------------------------------------------

//WallPaper -------------------------------------------------------------
void AHttpActor::WallPaperReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::WallPaperResPost);

    req->ProcessRequest();
}

void AHttpActor::WallPaperResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    FString ResponseString = Response->GetContentAsString();
    UE_LOG(LogTemp, Warning, TEXT("%s"), *ResponseString);

    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        FString result = Response->GetContentAsString();
        UE_LOG(LogTemp, Warning, TEXT("Server Response: %s"), *result);
        
        FWallPaperData WallPaperData = UJsonParseLib::WallPaperData_Convert_JsonToStruct(result);
        UE_LOG(LogTemp, Warning, TEXT("Parsed roomNum: %s"), *WallPaperData.RoomData.RoomNum);

        
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            AJS_RoomController* RoomController = Cast<AJS_RoomController>(PlayerController);
            if (RoomController && RoomController->R_UI)
            {
                RoomController->R_UI->SetIndex(WallPaperData.UpdatedWallpaperNum, 100);
                RoomController->GetWorldTimerManager().SetTimer(RoomUIWaitTimerHandle, RoomController, &AJS_RoomController::HideRoomUI, 1.0f, false);
            }
        }

    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
//ChangeIndex End-------------------------------------------------------------

//CompleteRoomSend and Reception -------------------------------------------------------------
void AHttpActor::RoomSendDataReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::RoomSendDataResPost);

    // ������ ��û
    req->ProcessRequest();
}


void AHttpActor::RoomSendDataResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
//     if (!Response.IsValid())
//     {
//         UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
//         return;
//     }
// 
//     if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
//     {
// 
//         FString JsonResponse = Response->GetContentAsString();
//         UE_LOG(LogTemp, Warning, TEXT("Received JSON: %s"), *JsonResponse);
//         FRoomReceptionData RoomReceptionData = UJsonParseLib::RoomReceptionData_Convert_JsonToStruct(JsonResponse);
// 
//         UE_LOG(LogTemp, Warning, TEXT("UserId : %s, RoomName : %s, RoomNum : %s, 작품 설명 : %s, 파티클 번호 : %s, 음악 이름 : %s"),
//         *RoomReceptionData.UserData.UserId, 
//         *RoomReceptionData.RoomData.RoomName,
//         *RoomReceptionData.RoomData.RoomNum, 
//         *RoomReceptionData.AIAnalysisData.Result,
//         *RoomReceptionData.AIAnalysisData.ParticleNum,
//         *RoomReceptionData.AIRecommendationData.Music);
// 
//         //UPDate Data
//         SessionGI->WorldSetting.ParticleNum = RoomReceptionData.AIAnalysisData.ParticleNum;
//         SessionGI->WorldSetting.Result = RoomReceptionData.AIAnalysisData.Result;
//         SessionGI->WorldSetting.UserMusic = RoomReceptionData.AIRecommendationData.Music;
//         UE_LOG(LogTemp, Warning, TEXT(" ParticleNum : %s, Result : %s, UserMusic : %s"), *SessionGI->WorldSetting.ParticleNum, *SessionGI->WorldSetting.Result, *SessionGI->WorldSetting.UserMusic);
// 
//         for (const FRGBColorData& ColorData : RoomReceptionData.AIAnalysisData.ArrayColorData) {
//             FMyRGBColor MyColor;
//             MyColor.R = ColorData.R;
//             MyColor.G = ColorData.G;
//             MyColor.B = ColorData.B;
// 
//             SessionGI->WorldSetting.RGB18.Add(MyColor);
// 
//             UE_LOG(LogTemp, Warning, TEXT("Added Color - R: %f, G: %f, B: %f"), MyColor.R, MyColor.G, MyColor.B);
//         }
//         //ReqPostChoice(EnteryLobbyURL, JsonResponse);
//     }
//     else
//     {
//         UE_LOG(LogTemp, Warning, TEXT("Request Failed: %d"), Response->GetResponseCode());
//     }
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::RoomSendDataResPost()"));
    if (bConnectedSuccessfully && Response.IsValid())
    {
        // ���������� ������ �޾��� ��
        FString ResponseContent = Response->GetContentAsString();
        UE_LOG(LogTemp, Warning, TEXT("POST Response: %s"), *ResponseContent);
        StoredJsonResponse = ResponseContent;
        UE_LOG(LogTemp, Warning, TEXT("Stored JSON Response: %s"), *StoredJsonResponse);
        //StoredJsonResponse = StoredJsonResponsetest;    // <----- 여기부터 수정  (임의값 넣고 확인해 보기)

        // JSON 문자열을 JSON 객체로 파싱
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(StoredJsonResponse);

        USessionGameInstance* SessionGameInstance = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
        if (!SessionGameInstance)
        {
            UE_LOG(LogTemp, Error, TEXT("AHttpActor::RoomSendDataResPost() SessionGameInstance is null! Failed to store data."));
            return;
        }
        /*AKGW_RoomlistActor* ListActor = Cast<AKGW_RoomlistActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AKGW_RoomlistActor::StaticClass()));
        UWidgetComponent* WidgetComp = ListActor->FindComponentByClass<UWidgetComponent>();
        UKGW_RoomList* Showlist = Cast<UKGW_RoomList>(WidgetComp->GetUserWidgetObject());*/

        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            // 1.요소마다 받아서 SessionGameInstance에 저장을 한다.
            FMyWorldSetting WorldSetting;
            // Parse RGB
            if (JsonObject->HasTypedField<EJson::Object>(TEXT("RGB")))
            {
                TSharedPtr<FJsonObject> RGBObject = JsonObject->GetObjectField(TEXT("RGB"));
                WorldSetting.RGB.R = FCString::Atof(*RGBObject->GetStringField(TEXT("R")));
                WorldSetting.RGB.G = FCString::Atof(*RGBObject->GetStringField(TEXT("G")));
                WorldSetting.RGB.B = FCString::Atof(*RGBObject->GetStringField(TEXT("B")));
            }
            // Parse RGB18
            if (JsonObject->HasTypedField<EJson::Object>(TEXT("RGB18")))
            {
                TSharedPtr<FJsonObject> RGB18Object = JsonObject->GetObjectField(TEXT("RGB18"));
                for (int32 i = 1; i <= 6; i++)
                {
                    FMyRGBColor Color;
                    Color.R = FCString::Atof(*RGB18Object->GetStringField(FString::Printf(TEXT("R%d"), i)));
                    Color.G = FCString::Atof(*RGB18Object->GetStringField(FString::Printf(TEXT("G%d"), i)));
                    Color.B = FCString::Atof(*RGB18Object->GetStringField(FString::Printf(TEXT("B%d"), i)));
                    WorldSetting.RGB18.Add(Color);
                }
            }
            // Parse UserMusic
            WorldSetting.UserMusic = JsonObject->GetStringField(TEXT("userMusic"));
            // Parse Quadrant
            WorldSetting.Quadrant = JsonObject->GetStringField(TEXT("Quadrant"));
            // Parse Initial Room Info
            WorldSetting.TimeOfDay = JsonObject->GetStringField(TEXT("UltraSky_TimeOfDay"));
            WorldSetting.CloudCoverage = JsonObject->GetStringField(TEXT("UltraWeather_CloudCoverage"));
            WorldSetting.Fog = JsonObject->GetStringField(TEXT("UltraWeather_Fog"));
            WorldSetting.Rain = JsonObject->GetStringField(TEXT("UltraWeather_Rain"));
            WorldSetting.Snow = JsonObject->GetStringField(TEXT("UltraWeather_Snow"));
            WorldSetting.Dust = JsonObject->GetStringField(TEXT("UltraWeather_Dust"));
            WorldSetting.Thunder = JsonObject->GetStringField(TEXT("UltraWeather_Thunder"));
            WorldSetting.MainObject = JsonObject->GetStringField(TEXT("MainObject"));
            WorldSetting.SubObject = JsonObject->GetStringField(TEXT("SubObject"));
            WorldSetting.Background = JsonObject->GetStringField(TEXT("Background"));
            // Parse ParticleNum
            WorldSetting.ParticleNum = JsonObject->GetStringField(TEXT("Particle_num"));
            // Parse Result
            WorldSetting.Result = JsonObject->GetStringField(TEXT("result"));
            WorldSetting.Result2 = JsonObject->GetStringField(TEXT("result2"));
            WorldSetting.Result3 = JsonObject->GetStringField(TEXT("result3"));
            if (SessionGI) {
                SessionGI->AIResult = JsonObject->GetStringField(TEXT("result"));
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("SessionGI Fail From HttpActorLine : 479 "));
            }
            // Parse Rooms
            if (JsonObject->HasTypedField<EJson::Array>(TEXT("rooms")))
            {
                TArray<TSharedPtr<FJsonValue>> RoomsArray = JsonObject->GetArrayField(TEXT("rooms"));
                for (const TSharedPtr<FJsonValue>& RoomValue : RoomsArray)
                {
                    TSharedPtr<FJsonObject> RoomObject = RoomValue->AsObject();
                    if (RoomObject.IsValid())
                    {
                        FMyWorldRoomInfo RoomInfo;
                        RoomInfo.MyRoomNum = RoomObject->GetIntegerField(TEXT("room_num"));
                        RoomInfo.MyRoomName = RoomObject->GetStringField(TEXT("room_name"));
                        WorldSetting.MyRooms.Add(RoomInfo);
                    }
                }
            }
            //Suggest_List JS 추가 추천 방 리스트 파싱
            if (JsonObject->HasTypedField<EJson::Array>(TEXT("suggest_list")))
            {
                TArray<TSharedPtr<FJsonValue>> SuggestListArray = JsonObject->GetArrayField(TEXT("suggest_list"));
                for (const TSharedPtr<FJsonValue>& SuggestValue : SuggestListArray)
                {
                    TSharedPtr<FJsonObject> SuggestObject = SuggestValue->AsObject();
                    if (SuggestObject.IsValid())
                    {
                        FMySuggest_List SuggestList;
                        SuggestList.percent_message = SuggestObject->GetStringField(TEXT("percent_message"));
                        SuggestList.reason_message = SuggestObject->GetStringField(TEXT("reason_message"));
                        SuggestList.room_id = SuggestObject->GetStringField(TEXT("room_id"));
                        SuggestList.room_num = SuggestObject->GetStringField(TEXT("room_num"));
                        SuggestList.room_name = SuggestObject->GetStringField(TEXT("room_name"));
                        WorldSetting.suggest_list.Add(SuggestList);
                    }
                }
            }

            // Store the parsed data in GameInstance or other persistent storage
            if (UGameInstance* GameInstance = GetGameInstance())
            {
                //                 USessionGameInstance* SessionGameInstance = Cast<USessionGameInstance>(GameInstance);
                if (SessionGameInstance)
                {
                    SessionGameInstance->WorldSetting = WorldSetting;
                    //SessionGameInstance->bmyWorldPageOn = true;  // 마이페이지 UI 

                    // 로그 출력
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting RGB: R=%f, G=%f, B=%f"),
                        WorldSetting.RGB.R, WorldSetting.RGB.G, WorldSetting.RGB.B);
                    for (int32 i = 0; i < WorldSetting.RGB18.Num(); i++)
                    {
                        const FMyRGBColor& Color = WorldSetting.RGB18[i];
                        UE_LOG(LogTemp, Warning, TEXT("WorldSetting RGB18[%d]: R=%f, G=%f, B=%f"), i, Color.R, Color.G, Color.B);
                    }
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting UserMusic: %s"), *WorldSetting.UserMusic);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Quadrant: %s"), *WorldSetting.Quadrant);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting TimeOfDay: %s"), *WorldSetting.TimeOfDay);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting CloudCoverage: %s"), *WorldSetting.CloudCoverage);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Fog: %s"), *WorldSetting.Fog);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Rain: %s"), *WorldSetting.Rain);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Snow: %s"), *WorldSetting.Snow);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Dust: %s"), *WorldSetting.Dust);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Thunder: %s"), *WorldSetting.Thunder);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting MainObject: %s"), *WorldSetting.MainObject);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting SubObject: %s"), *WorldSetting.SubObject);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Background: %s"), *WorldSetting.Background);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting ParticleNum: %s"), *WorldSetting.ParticleNum);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Result: %s"), *WorldSetting.Result);
                    for (const FMyWorldRoomInfo& Room : WorldSetting.MyRooms)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Room Number: %d, Room Name: %s"), Room.MyRoomNum, *Room.MyRoomName);
                    }
                }
            }
            // 전체 리스트 크기 확인 및 각 항목 출력
            UE_LOG(LogTemp, Warning, TEXT("Total Suggest Lists Added: %d"), WorldSetting.suggest_list.Num());
            for (int32 Index = 0; Index < WorldSetting.suggest_list.Num(); ++Index)
            {
                const FMySuggest_List& SuggestList = WorldSetting.suggest_list[Index];
                UE_LOG(LogTemp, Log, TEXT("Index %d:"), Index);
                UE_LOG(LogTemp, Log, TEXT("  Percent Message: %s"), *SuggestList.percent_message);
                UE_LOG(LogTemp, Log, TEXT("  Reason Message: %s"), *SuggestList.reason_message);
                UE_LOG(LogTemp, Log, TEXT("  Room ID: %s"), *SuggestList.room_id);
                UE_LOG(LogTemp, Log, TEXT("  Room Num: %s"), *SuggestList.room_num);
                UE_LOG(LogTemp, Log, TEXT("  Room Name: %s"), *SuggestList.room_name);
            }
            UE_LOG(LogTemp, Warning, TEXT("Successfully parsed and stored WorldSetting"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AHttpActor::RoomSendDataResPost() Failed to parse JSON data."));
        }
    }
    else
    {
        // ��û�� �������� ��
        UE_LOG(LogTemp, Warning, TEXT("AHttpActor::RoomSendDataResPost() POST Request Failed"));
    }


}
void AHttpActor::ShowQuestionUI()
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::ShowQuestionUI()"));
    // MyWidgetClass�� ��ȿ���� Ȯ��
    if (QuestionUIFactory && !QuestionUI)
    {
        // UI ���� �ν��Ͻ��� ����
        QuestionUI = CreateWidget<UKGW_WBP_Question>(GetWorld(), QuestionUIFactory);
        if (QuestionUI)
        {
            // ȭ�鿡 �߰�
            QuestionUI->AddToViewport();
            QuestionUI->PlayLateAppearAnimation();
            UE_LOG(LogTemp, Warning, TEXT("AHttpActor::ShowQuestionUI() QuestionUI->AddToViewport()"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AHttpActor::ShowQuestionUI() NO QuestionUI"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AHttpActor::ShowQuestionUI() NO QuestionUIFactory && QuestionUI"));
    }
}
void AHttpActor::HidQuestionUI()
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::HidQuestionUI()"));
    if (QuestionUIFactory && QuestionUI)
    {
        QuestionUI->RemoveFromParent();
        //SetMyWorldUIOn();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AHttpActor::HidQuestionUI() NO QuestionUIFactory && QuestionUI"));
    }

}
//CompleteRoomSend and Reception -------------------------------------------------------------


// RoomData ------------------------------------------------------------
void AHttpActor::RoomDataReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::RoomDataResPost);

    // ������ ��û
    req->ProcessRequest();
}
void AHttpActor::RoomDataResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        FString JsonResponse = Response->GetContentAsString();
        //RecommendationData = UJsonParseLib::AIRecommendation_Convert_JsonToStruct(JsonResponse);

        //UE_LOG(LogTemp, Warning, TEXT("RecommendationData initialized: %s"), *RecommendationData.Music);

        //// RoomData가 초기화되었음을 알리기 위해 델리게이트 호출
        //OnAIRecommendationInitialized.Broadcast(RecommendationData);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Request Failed: %d"), Response->GetResponseCode());
    }
}
// RoomData -------------------------------------------------------------------------------

// MyWorld Setting Data Start--------------------------------------------------------------
void AHttpActor::ReqPostChoice(FString url, FString json)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::ReqPostChoice()"));
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostChoice);

    // 요청 처리
    if (!req->ProcessRequest())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to process HTTP request"));
    }

    req->ProcessRequest();
}
void AHttpActor::OnResPostChoice(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::OnResPostChoice()"));
    if (bConnectedSuccessfully && Response.IsValid())
    {
        // ���������� ������ �޾��� ��
        FString ResponseContent = Response->GetContentAsString();
        UE_LOG(LogTemp, Warning, TEXT("POST Response: %s"), *ResponseContent);
        StoredJsonResponse = ResponseContent;
        UE_LOG(LogTemp, Warning, TEXT("Stored JSON Response: %s"), *StoredJsonResponse);
        //StoredJsonResponse = StoredJsonResponsetest;    // <----- 여기부터 수정  (임의값 넣고 확인해 보기)

        // JSON 문자열을 JSON 객체로 파싱
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(StoredJsonResponse);

        USessionGameInstance* SessionGameInstance = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
        if (!SessionGameInstance)
        {
            UE_LOG(LogTemp, Error, TEXT("AHttpActor::OnResPostChoice() SessionGameInstance is null! Failed to store data."));
            return;
        }
		/*AKGW_RoomlistActor* ListActor = Cast<AKGW_RoomlistActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AKGW_RoomlistActor::StaticClass()));
		UWidgetComponent* WidgetComp = ListActor->FindComponentByClass<UWidgetComponent>();
		UKGW_RoomList* Showlist = Cast<UKGW_RoomList>(WidgetComp->GetUserWidgetObject());*/

        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            // 1.요소마다 받아서 SessionGameInstance에 저장을 한다.
            FMyWorldSetting WorldSetting;
            // Parse RGB
            if (JsonObject->HasTypedField<EJson::Object>(TEXT("RGB")))
            {
                TSharedPtr<FJsonObject> RGBObject = JsonObject->GetObjectField(TEXT("RGB"));
                WorldSetting.RGB.R = FCString::Atof(*RGBObject->GetStringField(TEXT("R")));
                WorldSetting.RGB.G = FCString::Atof(*RGBObject->GetStringField(TEXT("G")));
                WorldSetting.RGB.B = FCString::Atof(*RGBObject->GetStringField(TEXT("B")));
            }
            // Parse RGB18
            if (JsonObject->HasTypedField<EJson::Object>(TEXT("RGB18")))
            {
                TSharedPtr<FJsonObject> RGB18Object = JsonObject->GetObjectField(TEXT("RGB18"));
                for (int32 i = 1; i <= 6; i++)
                {
                    FMyRGBColor Color;
                    Color.R = FCString::Atof(*RGB18Object->GetStringField(FString::Printf(TEXT("R%d"), i)));
                    Color.G = FCString::Atof(*RGB18Object->GetStringField(FString::Printf(TEXT("G%d"), i)));
                    Color.B = FCString::Atof(*RGB18Object->GetStringField(FString::Printf(TEXT("B%d"), i)));
                    WorldSetting.RGB18.Add(Color);
                }
            }
            // Parse UserMusic
            WorldSetting.UserMusic = JsonObject->GetStringField(TEXT("userMusic"));
            // Parse Quadrant
            WorldSetting.Quadrant = JsonObject->GetStringField(TEXT("Quadrant"));
            // Parse Initial Room Info
            WorldSetting.TimeOfDay = JsonObject->GetStringField(TEXT("UltraSky_TimeOfDay"));
            WorldSetting.CloudCoverage = JsonObject->GetStringField(TEXT("UltraWeather_CloudCoverage"));
            WorldSetting.Fog = JsonObject->GetStringField(TEXT("UltraWeather_Fog"));
            WorldSetting.Rain = JsonObject->GetStringField(TEXT("UltraWeather_Rain"));
            WorldSetting.Snow = JsonObject->GetStringField(TEXT("UltraWeather_Snow"));
            WorldSetting.Dust = JsonObject->GetStringField(TEXT("UltraWeather_Dust"));
            WorldSetting.Thunder = JsonObject->GetStringField(TEXT("UltraWeather_Thunder"));
            WorldSetting.MainObject = JsonObject->GetStringField(TEXT("MainObject"));
            WorldSetting.SubObject = JsonObject->GetStringField(TEXT("SubObject"));
            WorldSetting.Background = JsonObject->GetStringField(TEXT("Background"));
            // Parse ParticleNum
            WorldSetting.ParticleNum = JsonObject->GetStringField(TEXT("Particle_num"));
            // Parse Result
            WorldSetting.Result = JsonObject->GetStringField(TEXT("result"));
            WorldSetting.Result2 = JsonObject->GetStringField(TEXT("result2"));
            WorldSetting.Result3 = JsonObject->GetStringField(TEXT("result3"));
            if (SessionGI) {
                SessionGI->AIResult = JsonObject->GetStringField(TEXT("result"));
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("SessionGI Fail From HttpActorLine : 479 "));
            }
            // Parse Rooms
            if (JsonObject->HasTypedField<EJson::Array>(TEXT("rooms")))
            {
                TArray<TSharedPtr<FJsonValue>> RoomsArray = JsonObject->GetArrayField(TEXT("rooms"));
                for (const TSharedPtr<FJsonValue>& RoomValue : RoomsArray)
                {
                    TSharedPtr<FJsonObject> RoomObject = RoomValue->AsObject();
                    if (RoomObject.IsValid())
                    {
                        FMyWorldRoomInfo RoomInfo;
                        RoomInfo.MyRoomNum = RoomObject->GetIntegerField(TEXT("room_num"));
                        RoomInfo.MyRoomName = RoomObject->GetStringField(TEXT("room_name"));
                        WorldSetting.MyRooms.Add(RoomInfo);
                    }
                }
            }
            //Suggest_List JS 추가 추천 방 리스트 파싱
            if (JsonObject->HasTypedField<EJson::Array>(TEXT("suggest_list")))
            {
                TArray<TSharedPtr<FJsonValue>> SuggestListArray = JsonObject->GetArrayField(TEXT("suggest_list"));
                for (const TSharedPtr<FJsonValue>& SuggestValue : SuggestListArray)
                {
                    TSharedPtr<FJsonObject> SuggestObject = SuggestValue->AsObject();
                    if (SuggestObject.IsValid())
                    {
                        FMySuggest_List SuggestList;
                        SuggestList.percent_message = SuggestObject->GetStringField(TEXT("percent_message"));
                        SuggestList.reason_message = SuggestObject->GetStringField(TEXT("reason_message"));
                        SuggestList.room_id = SuggestObject->GetStringField(TEXT("room_id"));
                        SuggestList.room_num = SuggestObject->GetStringField(TEXT("room_num"));
                        SuggestList.room_name = SuggestObject->GetStringField(TEXT("room_name"));
                        WorldSetting.suggest_list.Add(SuggestList);
                    }
                }
            }
            
            // Store the parsed data in GameInstance or other persistent storage
            if (UGameInstance* GameInstance = GetGameInstance())
            {
//                 USessionGameInstance* SessionGameInstance = Cast<USessionGameInstance>(GameInstance);
                if (SessionGameInstance)
                {
                    SessionGameInstance->WorldSetting = WorldSetting;
                    //SessionGameInstance->bmyWorldPageOn = true;  // 마이페이지 UI 

                    // 로그 출력
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting RGB: R=%f, G=%f, B=%f"),
                        WorldSetting.RGB.R, WorldSetting.RGB.G, WorldSetting.RGB.B);
                    for (int32 i = 0; i < WorldSetting.RGB18.Num(); i++)
                    {
                        const FMyRGBColor& Color = WorldSetting.RGB18[i];
                        UE_LOG(LogTemp, Warning, TEXT("WorldSetting RGB18[%d]: R=%f, G=%f, B=%f"), i, Color.R, Color.G, Color.B);
                    }
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting UserMusic: %s"), *WorldSetting.UserMusic);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Quadrant: %s"), *WorldSetting.Quadrant);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting TimeOfDay: %s"), *WorldSetting.TimeOfDay);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting CloudCoverage: %s"), *WorldSetting.CloudCoverage);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Fog: %s"), *WorldSetting.Fog);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Rain: %s"), *WorldSetting.Rain);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Snow: %s"), *WorldSetting.Snow);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Dust: %s"), *WorldSetting.Dust);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Thunder: %s"), *WorldSetting.Thunder);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting MainObject: %s"), *WorldSetting.MainObject);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting SubObject: %s"), *WorldSetting.SubObject);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Background: %s"), *WorldSetting.Background);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting ParticleNum: %s"), *WorldSetting.ParticleNum);
                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Result: %s"), *WorldSetting.Result);
                    for (const FMyWorldRoomInfo& Room : WorldSetting.MyRooms)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Room Number: %d, Room Name: %s"), Room.MyRoomNum, *Room.MyRoomName);
                    }
                }
            }
            // 전체 리스트 크기 확인 및 각 항목 출력
            UE_LOG(LogTemp, Warning, TEXT("Total Suggest Lists Added: %d"), WorldSetting.suggest_list.Num());
            for (int32 Index = 0; Index < WorldSetting.suggest_list.Num(); ++Index)
            {
                const FMySuggest_List& SuggestList = WorldSetting.suggest_list[Index];
                UE_LOG(LogTemp, Log, TEXT("Index %d:"), Index);
                UE_LOG(LogTemp, Log, TEXT("  Percent Message: %s"), *SuggestList.percent_message);
                UE_LOG(LogTemp, Log, TEXT("  Reason Message: %s"), *SuggestList.reason_message);
                UE_LOG(LogTemp, Log, TEXT("  Room ID: %s"), *SuggestList.room_id);
                UE_LOG(LogTemp, Log, TEXT("  Room Num: %s"), *SuggestList.room_num);
                UE_LOG(LogTemp, Log, TEXT("  Room Name: %s"), *SuggestList.room_name);
            }
            UE_LOG(LogTemp, Warning, TEXT("Successfully parsed and stored WorldSetting"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AHttpActor::OnResPostChoice() Failed to parse JSON data."));
        }
    }
    else
    {
        // ��û�� �������� ��
        UE_LOG(LogTemp, Warning, TEXT("AHttpActor::OnResPostChoice() POST Request Failed"));
    }
    
}
// void AHttpActor::ShowQuestionUI()
// {    
//     UE_LOG(LogTemp, Warning, TEXT("AHttpActor::ShowQuestionUI()"));
//     // MyWidgetClass�� ��ȿ���� Ȯ��
//     if (QuestionUIFactory && !QuestionUI)
//     {
//         // UI ���� �ν��Ͻ��� ����
//         QuestionUI = CreateWidget<UKGW_WBP_Question>(GetWorld(), QuestionUIFactory);
//         if (QuestionUI)
//         {
//             // ȭ�鿡 �߰�
//             QuestionUI->AddToViewport();
//             QuestionUI->PlayLateAppearAnimation();
//             UE_LOG(LogTemp, Warning, TEXT("AHttpActor::ShowQuestionUI() QuestionUI->AddToViewport()"));
//         }
//         else
//         {
//             UE_LOG(LogTemp, Error, TEXT("AHttpActor::ShowQuestionUI() NO QuestionUI"));
//         }
//     }
//     else
//     {
//         UE_LOG(LogTemp, Error, TEXT("AHttpActor::ShowQuestionUI() NO QuestionUIFactory && QuestionUI"));
//     }
// }
// void AHttpActor::HidQuestionUI()
// {
//     UE_LOG(LogTemp, Warning, TEXT("AHttpActor::HidQuestionUI()"));
//     if (QuestionUIFactory && QuestionUI)
//     {
//         QuestionUI->RemoveFromParent();
//         //SetMyWorldUIOn();
//     }
//     else
//     {
//         UE_LOG(LogTemp, Error, TEXT("AHttpActor::HidQuestionUI() NO QuestionUIFactory && QuestionUI"));
//     }
// }

FString AHttpActor::StoreJsonResponse()
{
    FString JsonString = StoredJsonResponsetest;

    return JsonString;
}

void AHttpActor::ReqPostClickMultiRoom(FString url, FString json)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::ReqPostClickMultiRoom()"));
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);
    req->SetTimeout(60.0f); // 타임아웃 설정

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostClickMultiRoom);

    if (req->ProcessRequest())
    {
        UE_LOG(LogTemp, Warning, TEXT("Http Request processed successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Http Request failed to process"));
    }
}

void AHttpActor::OnResPostClickMultiRoom(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::OnResPostClickMultiRoom()"));

    /*if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }

    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        FString result = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);
    }
    else
    {
        int32 ResponseCode = Response->GetResponseCode();
        UE_LOG(LogTemp, Warning, TEXT("OnResPostClickMultiRoom() Failed... Response Code: %d, Connected Successfully: %s"),
            ResponseCode, bConnectedSuccessfully ? TEXT("True") : TEXT("False"));
    }*/

    if (bConnectedSuccessfully)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPosLogin OK... %d"), Response->GetResponseCode());

        int32 res = Response->GetResponseCode();
        if (res == 200)
        {
            UE_LOG(LogTemp, Warning, TEXT("Response ... OK!! "));
            FString LocalJsonData = Response->GetContentAsString();  // <-- 여기 통신에 값 들어오는 거 보고 수정 예정
            UE_LOG(LogTemp, Warning, TEXT(" %s"), *LocalJsonData);


            // 방 정보 처리 로직 <-- 추가 예정
            /*
            UltraSky_TimeOfDay
			UltraWeather_CloudCoverage
			UltraWeather_Fog
			UltraWeather_Rain
			UltraWeather_Snow
			UltraWeather_Dust
			UltraWeather_Thunder
			Particle_num1
			Particle_num2
			Particle_num3
			Particle_num4
            */


            // 체험자 수(playerNum) 처리 로직 
            TSharedPtr<FJsonObject> JsonObject;
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(LocalJsonData);

            if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
            {
                // JSON에서 playerNum을 FString으로 가져옵니다
                FString playerNum = JsonObject->GetStringField(TEXT("playerNum")); // JSON에서 playerNum을 문자열로 가져옴
                UE_LOG(LogTemp, Warning, TEXT("체험자 수: %s"), *playerNum);

                if(APlayerController * PlayerController = GetWorld()->GetFirstPlayerController())
                {
                    ACJS_BallPlayer* player = Cast<ACJS_BallPlayer>(PlayerController->GetPawn());
                    if (player)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer assigned"));
                        player->FindMultiRoomList(player->ClickedRoomNum, playerNum);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Failed to cast PlayerController's Pawn to ACJS_BallPlayer."));
                    }
                }
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to get PlayerController."));
				}
            
            }

            // 플레이어 캐스팅 및 RequestMoveMultiRoom 호출
            /*if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
            {
                ACJS_BallPlayer* player = Cast<ACJS_BallPlayer>(PlayerController->GetPawn());
                if (player)
                {
                    player->RequestMoveMultiRoom(PlayerController);
                    UE_LOG(LogTemp, Warning, TEXT("RequestMoveMultiRoom called successfully."));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to cast PlayerController's Pawn to ACJS_BallPlayer."));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to get PlayerController."));
            }*/
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Response ... not OK!! "));
		}
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed...%d"), Response->GetResponseCode());
    }
}


//JS ReWrite 내방 통신 추가 부분
void AHttpActor::ReqPostClickMyRoom(FString url, FString json)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::ReqPostClickMyRoom()"));
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);
    req->SetTimeout(60.0f); // 타임아웃 설정

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostClickMyRoom);

    if (req->ProcessRequest())
    {
        UE_LOG(LogTemp, Warning, TEXT("Http Request processed successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Http Request failed to process"));
    }
}

void AHttpActor::OnResPostClickMyRoom(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }

    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // 응답에서 JSON 문자열 얻기
        FString JsonResponse = Response->GetContentAsString();
        UE_LOG(LogTemp, Warning, TEXT("MyRoom Response JSON: %s"), *JsonResponse);
       
        // FRoomData 구조체로 변환
        //RecommendationData = UJsonParseLib::AIRecommendation_Convert_JsonToStruct(JsonResponse);
		/* SessionGI->RoomMusicData = RecommendationData.Music;
		 UE_LOG(LogTemp, Warning, TEXT("RecommendedMusic: %s"), *SessionGI->RoomMusicData);*/
     
        //MyRoom으로 이동
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController) {
            PlayerController->ClientTravel("/Game/Main/Maps/Main_Room", ETravelType::TRAVEL_Absolute);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Request Failed: %d"), Response->GetResponseCode());
    }
}
void AHttpActor::ReqPostClickMultiWorld(FString url, FString json)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::ReqPostClickMultiWorld()"));
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostClickMultiWorld);

    req->ProcessRequest();
}
void AHttpActor::OnResPostClickMultiWorld(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::OnResPostClickMultiWorld()"));
    if (bConnectedSuccessfully && Response.IsValid())
    {
        FString ResponseContent = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("POST Response: %s"), *ResponseContent);
        StoredJsonResponse = ResponseContent;  // <-- 실제 통신 시
        UE_LOG(LogTemp, Warning, TEXT("Stored JSON Response: %s"), *StoredJsonResponse);  
        CallStartMakeSession(StoredJsonResponse);
    }
}
void AHttpActor::CallStartMakeSession(FString result)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::CallStartMakeSession()"));
    StoredJsonResponse = StoredJsonResponsetest;  // <-- 테스트 시   
    if (SessionGI)
    {
        UE_LOG(LogTemp, Warning, TEXT("SessionGM is OK"));
        SessionGI->SetNetInfoCharacterTOLobby(result);
        SessionGI->FindSessions();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SessionGM is NULL"));
    }
}


// 추천 음악 틀기
void AHttpActor::SetBackgroundSound()
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::SetBackgroundSound()"));
    if (SessionGI && SoundActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("AHttpActor::SetBackgroundSound() SessionGI && SoundActor OK"));
        // Get UserMusic from WorldSetting and play it
        FString UserMusic = SessionGI->WorldSetting.UserMusic;
        SoundActor->SetBackgroundSoundByFileName(UserMusic);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AHttpActor::SetBackgroundSound() SessionGI && SoundActor NO"));
    }
}

void AHttpActor::ApplyMyWorldPointLightColors()
{
    if (!SessionGI)
    {
        UE_LOG(LogTemp, Error, TEXT("SessionGameInstance is not valid."));
        return;
    }

    // Reference to WorldSetting's RGB18 array
    const TArray<FMyRGBColor>& RGB18Array = SessionGI->WorldSetting.RGB18;

    // Assuming ACJS_InnerWorldParticleActor is an actor in the world, find and reference it
    ACJS_InnerWorldParticleActor* InnerWorldParticleActor = Cast<ACJS_InnerWorldParticleActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACJS_InnerWorldParticleActor::StaticClass()));

    if (InnerWorldParticleActor)
    {
        // Iterate over RGB18 and apply each color to corresponding PointLight
        for (int32 i = 0; i < RGB18Array.Num(); i++)
        {
            if (i < InnerWorldParticleActor->PointLights.Num())  // Check if PointLight index exists
            {
                FLinearColor LightColor(RGB18Array[i].R, RGB18Array[i].G, RGB18Array[i].B);
                InnerWorldParticleActor->UpdateInnerWorldPointLights(LightColor, i);
                UE_LOG(LogTemp, Warning, TEXT("Applying color to PointLight%d: R=%f, G=%f, B=%f"), i, LightColor.R, LightColor.G, LightColor.B);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No corresponding PointLight for RGB18[%d]"), i);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ACJS_InnerWorldParticleActor not found in the level."));
    }
}

void AHttpActor::ApplyMyWorldNiagaraAssets()
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::ApplyMyWorldNiagaraAssets()"));
    if (!SessionGI)
    {
        UE_LOG(LogTemp, Error, TEXT("SessionGameInstance is not valid."));
        return;
    }

    // Reference to WorldSetting's Weather value
    FString ParticleNumString = SessionGI->WorldSetting.ParticleNum;

    // Convert the Weather string to int32
    int32 ParticleIndex = FCString::Atoi(*ParticleNumString);
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::ApplyMyWorldNiagaraAssets() ParticleIndex : %d"), ParticleIndex);

    // Assuming ACJS_InnerWorldParticleActor is an actor in the world, find and reference it
    ACJS_InnerWorldParticleActor* InnerWorldParticleActor = Cast<ACJS_InnerWorldParticleActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACJS_InnerWorldParticleActor::StaticClass()));

    if (InnerWorldParticleActor)
    {
        //ParticleIndex = 2;
        // Call UpdateInnerWorldNiagaraAsset with the converted WeatherIndex
        InnerWorldParticleActor->UpdateInnerWorldNiagaraAsset(ParticleIndex);
        UE_LOG(LogTemp, Warning, TEXT("Calling UpdateInnerWorldNiagaraAsset with ParticleIndex: %d"), ParticleIndex);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ACJS_InnerWorldParticleActor not found in the level."));
    }
}
void AHttpActor::SetMyWorldUIOn()
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::SetMyWorldUIOn()"));
    if (MyWorldPlayer)
    {
        MyWorldPlayer->ShowMyWorldUI();
        MyWorldPlayer->ShowMyWorldParticle();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AHttpActor::SetMyWorldUIOn() MyWorldPlayer is not found in the level."));
    }
}
void AHttpActor::SetMyWorldUIOff()
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::SetMyWorldUIOff()"));
    if (MyWorldPlayer)
    {
        MyWorldPlayer->HideMyWorldUI();
        MyWorldPlayer->HideMyWorldParticle();

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AHttpActor::SetMyWorldUIOff() MyWorldPlayer is not found in the level."));
    }
}

void AHttpActor::CallHttpClickMyRoomList(FString room_num)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::CallHttpClickMyRoomList()"));
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::CallHttpClickMyRoomList()  room_num : %s"), *room_num);

    // room_num을 makeJson()으로 변환
    TMap<FString, FString> MyRoomNum;
    MyRoomNum.Add("room_num", room_num);

    // JSON 형식으로 변환
    FString JsonRequest = UJsonParseLib::MakeJson(MyRoomNum);
    
    ReqPostClickMyRoomList(ClickMyRoomURL, JsonRequest);
}
void AHttpActor::ReqPostClickMyRoomList(FString url, FString json)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::ReqPostClickMyRoomList()"));
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);
    req->SetTimeout(60.0f); // 타임아웃 설정

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostClickMyRoomList);

    if (req->ProcessRequest())
    {
        UE_LOG(LogTemp, Warning, TEXT("Http Request processed successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Http Request failed to process"));
    }
}
void AHttpActor::OnResPostClickMyRoomList(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::OnResPostClickMyRoomList()"));
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }

    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // 응답에서 JSON 문자열 얻기
        FString JsonResponse = Response->GetContentAsString();
        UE_LOG(LogTemp, Warning, TEXT("MyRoom Response JSON: %s"), *JsonResponse);

        /*
        LogTemp: Warning: MyRoom Response JSON: 
{"roomNum":3,
"roomId":"testuser",
"roomName":"얼어붙는 겨울",
"roomMusic":"Music_01",
"userQuestions":"3",
"roomTimeOfDay":"2000",
"roomCloudCoverage":"1",
"roomFog":"2",
"roomRain":"3",
"roomSnow":"4",
"roomDust":"5",
"roomWindIntensity":"2",
"roomThunder":"7",
"roomPp":"true",
"createdAt":"2024-10-27T06:32:15",
"wallpaperNum":"5",
"main_object":null,
"sub_object":null,
"background":null,
"particle_num":null,
"room_description":null,
"r1":null,"
g1":null,"b1":null,"r2":null,"g2":null"b2":null,"r3":null,"g3":null,"b3":null,"r4":null,"g4":null,"b4":null,"r5":null,"g5":null,"b5":null,"r6":null,"g6":null,"b6":null,
"result":null}
        */


        //// FRoomData 구조체로 변환
        //RoomData = UJsonParseLib::RoomData_Convert_JsonToStruct(JsonResponse);
        //SessionGI->RoomMusicData = RoomData.userMusic;
        //UE_LOG(LogTemp, Warning, TEXT("RecommendedMusic: %s"), *SessionGI->RoomMusicData);

        ////MyRoom으로 이동
        //APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        //if (PlayerController) {
        //    PlayerController->ClientTravel("/Game/Main/Maps/Main_Room", ETravelType::TRAVEL_Absolute);
        //}
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Request Failed: %d"), Response->GetResponseCode());
    }
}

//Getter 함수
//FAIRecommendation AHttpActor::GetAIRecommendation() const
//{
//    FString Rdata = RecommendationData.Music;
//    UE_LOG(LogTemp, Warning, TEXT("RoomData music : %s"), *Rdata);
//    return RecommendationData;
//}
//MyCreateRoomInfo End-------------------------------------------------------------

void AHttpActor::ReqGetWebImage(FString url)
{
    // HTTP 모듈 인스턴스 가져오기
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // 요청 설정
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("Content-Type"), TEXT("image/jpeg"));

    // 응답 처리 함수 바인딩
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResGetWebImage);

    // 요청 실행
    req->ProcessRequest();
}

void AHttpActor::OnResGetWebImage(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (bConnectedSuccessfully && Response.IsValid())
    {
        // 이미지 데이터를 가져옴
        TArray<uint8> data = Response->GetContent();

        // 이미지 파일 저장 경로 설정
        FString imagePath = FPaths::ProjectPersistentDownloadDir() + "/DownloadedImage.jpg";
        FFileHelper::SaveArrayToFile(data, *imagePath);

        // 데이터를 텍스처로 변환
        UTexture2D* realTexture = FImageUtils::ImportBufferAsTexture2D(data);

        // UI에 텍스처 설정
        if (ImageUI && realTexture)
        {
            ImageUI->SetImage(realTexture);
        }

        // 로그 출력
        UE_LOG(LogTemp, Log, TEXT("Image successfully downloaded and set to UI."));
    }
    else
    {
        // 실패한 경우 로그 출력
        UE_LOG(LogTemp, Warning, TEXT("Failed to download image from server."));
    }
}
void AHttpActor::ReqPostTest(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // 요청할 정보를 설정
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // 응답받을 함수를 연결
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostTest);
    // 서버에 요청

    req->ProcessRequest();
}

void AHttpActor::OnResPostTest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (bConnectedSuccessfully)
    {
    }
    else {
        // 실패
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
void AHttpActor::ReqPostRoomList(FString url, FString json)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::ReqPostClickMyRoom()"));
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);
    req->SetTimeout(60.0f); // 타임아웃 설정

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostRoomList);

    if (req->ProcessRequest())
    {
        UE_LOG(LogTemp, Warning, TEXT("Http Request processed successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Http Request failed to process"));
    }
}
void AHttpActor::OnResPostRoomList(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    
//     if (bConnectedSuccessfully && Response.IsValid())
//     {
//         FString ResponseContent = Response->GetContentAsString();
//         UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *ResponseContent);
//         	
// 
//         // JSON 파싱 함수 호출 및 반환 값 저장
//         TArray<FMyCreatedRoom> ParsedResult = UJsonParseLib::JsonParseRoomList(ResponseContent);
// 
//         // 파싱된 결과를 문자열로 변환하여 출력
//         FString ParsedString;
//         for (const FMyCreatedRoom& Room : ParsedResult)
//         {
//             ParsedString.Append(FString::Printf(TEXT("roomNum: %s, roomName: %s\n"), *Room.RoomNum, *Room.RoomName));
//         }
// 
//         // 파싱된 결과 출력
//         UE_LOG(LogTemp, Log, TEXT("Parsed Room Data:\n%s"), *ParsedString);
// 
//         USessionGameInstance* GameInstance = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
//         if (GameInstance)
//         {
//             GameInstance->InitRoomNameNum(ParsedResult); // 데이터가 제대로 저장되었는지 로그로 확인
//             UE_LOG(LogTemp, Error, TEXT("GameInstance->InitRoomInfoList size: %d"), GameInstance->RoomInfoList.Num());
//             TArray<FMyCreatedRoom> Result;
//             Result=  GameInstance->GettRoomNameNum(); // 데이터가 제대로 저장되었는지 로그로 확인
//             UE_LOG(LogTemp, Error, TEXT("GameInstance->GEtRoomInfoList size: %d"), Result.Num());
//             AKGW_RoomlistActor* ListActor = Cast<AKGW_RoomlistActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AKGW_RoomlistActor::StaticClass()));
//             if (ListActor)
//             {
//                 UWidgetComponent* WidgetComp = ListActor->FindComponentByClass<UWidgetComponent>();
//                 if (WidgetComp)
//                 {
//                     UKGW_RoomList* Showlist = Cast<UKGW_RoomList>(WidgetComp->GetUserWidgetObject());
//                     if (Showlist)
//                     {
//                         // RoomInfoList 데이터를 위젯에 추가
//                         Showlist->AddSessionSlotWidget(GameInstance->GettRoomNameNum());
//                         UE_LOG(LogTemp, Log, TEXT("Showlist updated successfully."));
//                     }
//                     else
//                     {
//                         UE_LOG(LogTemp, Error, TEXT("Showlist is null! Make sure the widget is correctly set in BP_ListActor."));
//                     }
//                 }
//                 else
//                 {
//                     UE_LOG(LogTemp, Error, TEXT("WidgetComponent not found on BP_ListActor."));
//                 }
//             }
//         }
//         else
//         {
//             UE_LOG(LogTemp, Error, TEXT("GameInstance is null!"));
//         }
// 
//     }
//     else
//     {
//         UE_LOG(LogTemp, Error, TEXT("Failed to receive a valid response from the server."));
//     }
// 

}

//void AHttpActor::OnResPostBackRoom(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
//{
//     UE_LOG(LogTemp, Warning, TEXT("AHttpActor::OnResPostBackRoom()"));
//    if (bConnectedSuccessfully && Response.IsValid())
//    {
//        // ���������� ������ �޾��� ��
//        FString ResponseContent = Response->GetContentAsString();
//        UE_LOG(LogTemp, Warning, TEXT("POST Response: %s"), *ResponseContent);
//        StoredJsonResponse = ResponseContent;
//        UE_LOG(LogTemp, Warning, TEXT("Stored JSON Response: %s"), *StoredJsonResponse);
//        //StoredJsonResponse = StoredJsonResponsetest;    // <----- 여기부터 수정  (임의값 넣고 확인해 보기)
//
//        // JSON 문자열을 JSON 객체로 파싱
//        TSharedPtr<FJsonObject> JsonObject;
//        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(StoredJsonResponse);
//        USessionGameInstance* SessionGameInstance = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
//        AKGW_RoomlistActor* ListActor = Cast<AKGW_RoomlistActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AKGW_RoomlistActor::StaticClass()));
//        UWidgetComponent* WidgetComp = ListActor->FindComponentByClass<UWidgetComponent>();
//        UKGW_RoomList* Showlist = Cast<UKGW_RoomList>(WidgetComp->GetUserWidgetObject());
//
//        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
//        {
//            // 1.요소마다 받아서 SessionGameInstance에 저장을 한다.
//            FMyWorldSetting WorldSetting;
//            // Parse RGB18
//            if (JsonObject->HasTypedField<EJson::Object>(TEXT("RGB18")))
//            {
//                TSharedPtr<FJsonObject> RGB18Object = JsonObject->GetObjectField(TEXT("RGB18"));
//                for (int32 i = 1; i <= 6; i++)
//                {
//                    FMyRGBColor Color;
//                    Color.R = FCString::Atof(*RGB18Object->GetStringField(FString::Printf(TEXT("R%d"), i)));
//                    Color.G = FCString::Atof(*RGB18Object->GetStringField(FString::Printf(TEXT("G%d"), i)));
//                    Color.B = FCString::Atof(*RGB18Object->GetStringField(FString::Printf(TEXT("B%d"), i)));
//                    WorldSetting.RGB18.Add(Color);
//                }
//            }
//            // Parse UserMusic
//            WorldSetting.UserMusic = JsonObject->GetStringField(TEXT("userMusic"));
//            // Parse Quadrant
//            WorldSetting.Quadrant = JsonObject->GetStringField(TEXT("Quadrant"));
//            // Parse Initial Room Info
//            WorldSetting.TimeOfDay = JsonObject->GetStringField(TEXT("UltraSky_TimeOfDay"));
//            WorldSetting.CloudCoverage = JsonObject->GetStringField(TEXT("UltraWeather_CloudCoverage"));
//            WorldSetting.Fog = JsonObject->GetStringField(TEXT("UltraWeather_Fog"));
//            WorldSetting.Rain = JsonObject->GetStringField(TEXT("UltraWeather_Rain"));
//            WorldSetting.Snow = JsonObject->GetStringField(TEXT("UltraWeather_Snow"));
//            WorldSetting.Dust = JsonObject->GetStringField(TEXT("UltraWeather_Dust"));
//            WorldSetting.Thunder = JsonObject->GetStringField(TEXT("UltraWeather_Thunder"));
//            WorldSetting.MainObject = JsonObject->GetStringField(TEXT("MainObject"));
//            WorldSetting.SubObject = JsonObject->GetStringField(TEXT("SubObject"));
//            WorldSetting.Background = JsonObject->GetStringField(TEXT("Background"));
//            // Parse ParticleNum
//            WorldSetting.ParticleNum = JsonObject->GetStringField(TEXT("Particle_num"));
//            // Parse Result
//            WorldSetting.Result = JsonObject->GetStringField(TEXT("result"));
//            // Parse Rooms
//            if (JsonObject->HasTypedField<EJson::Array>(TEXT("rooms")))
//            {
//                TArray<TSharedPtr<FJsonValue>> RoomsArray = JsonObject->GetArrayField(TEXT("rooms"));
//                for (const TSharedPtr<FJsonValue>& RoomValue : RoomsArray)
//                {
//                    TSharedPtr<FJsonObject> RoomObject = RoomValue->AsObject();
//                    if (RoomObject.IsValid())
//                    {
//                        FMyWorldRoomInfo RoomInfo;
//                        RoomInfo.MyRoomNum = RoomObject->GetIntegerField(TEXT("room_num"));
//                        RoomInfo.MyRoomName = RoomObject->GetStringField(TEXT("room_name"));
//                        WorldSetting.MyRooms.Add(RoomInfo);
//                    }
//                }
//            }
//            // Store the parsed data in GameInstance or other persistent storage
//            if (UGameInstance* GameInstance = GetGameInstance())
//            {
////                 USessionGameInstance* SessionGameInstance = Cast<USessionGameInstance>(GameInstance);
//                if (SessionGameInstance)
//                {
//                    SessionGameInstance->WorldSetting = WorldSetting;
//                    //SessionGameInstance->bmyWorldPageOn = true;  // 마이페이지 UI 
//
//                    // 로그 출력
//                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting RGB: R=%f, G=%f, B=%f"),
//                        WorldSetting.RGB.R, WorldSetting.RGB.G, WorldSetting.RGB.B);
//                    for (int32 i = 0; i < WorldSetting.RGB18.Num(); i++)
//                    {
//                        const FMyRGBColor& Color = WorldSetting.RGB18[i];
//                        UE_LOG(LogTemp, Warning, TEXT("WorldSetting RGB18[%d]: R=%f, G=%f, B=%f"), i, Color.R, Color.G, Color.B);
//                    }
//                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting UserMusic: %s"), *WorldSetting.UserMusic);
//                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Quadrant: %s"), *WorldSetting.Quadrant);
//                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting TimeOfDay: %s"), *WorldSetting.TimeOfDay);
//                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting CloudCoverage: %s"), *WorldSetting.CloudCoverage);
//                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Fog: %s"), *WorldSetting.Fog);
//                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Rain: %s"), *WorldSetting.Rain);
//                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Snow: %s"), *WorldSetting.Snow);
//                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Dust: %s"), *WorldSetting.Dust);
//                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Thunder: %s"), *WorldSetting.Thunder);
//                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting MainObject: %s"), *WorldSetting.MainObject);
//                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting SubObject: %s"), *WorldSetting.SubObject);
//                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Background: %s"), *WorldSetting.Background);
//                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting ParticleNum: %s"), *WorldSetting.ParticleNum);
//                    UE_LOG(LogTemp, Warning, TEXT("WorldSetting Result: %s"), *WorldSetting.Result);
//                    for (const FMyWorldRoomInfo& Room : WorldSetting.MyRooms)
//                    {
//                        UE_LOG(LogTemp, Warning, TEXT("Room Number: %d, Room Name: %s"), Room.MyRoomNum, *Room.MyRoomName);
//                    }
//                }
//            }
//            UE_LOG(LogTemp, Warning, TEXT("Successfully parsed and stored WorldSetting"));
//        
//            // 2.추천 음악을 튼다
//            SetBackgroundSound();
//        
//            // 4.파티클 색을 변경한다 + 감정 파티클을 변경한다.
//            ApplyMyWorldPointLightColors();
//            ApplyMyWorldNiagaraAssets();
//            // 5.방 목록의 제목을 UI에 넣는다.
//            if (SessionGameInstance)
//            {
//                SessionGameInstance->InitRoomNameNum(WorldSetting.MyRooms); // 데이터가 제대로 저장되었는지 로그로 확인
//                UE_LOG(LogTemp, Warning, TEXT("GameInstance->InitRoomInfoList size: %d"), SessionGameInstance->RoomInfoList.Num());
//                TArray<FMyWorldRoomInfo> Result;
//                Result = SessionGameInstance->GettRoomNameNum(); // 데이터가 제대로 저장되었는지 로그로 확인
//                UE_LOG(LogTemp, Warning, TEXT("GameInstance->GEtRoomInfoList size: %d"), Result.Num());
//                if (ListActor)
//                {
//                    if (WidgetComp)
//                    {
//                        if (Showlist)
//                        {
//                            // RoomInfoList 데이터를 위젯에 추가
//                            Showlist->AddSessionSlotWidget(Result);
//                            UE_LOG(LogTemp, Warning, TEXT("AHttpActor::OnResPostChoice() Showlist updated successfully."));
//
//                            // 6.AI 분석 결과를 UI에 넣는다.
//                            Showlist->SetTextLog(WorldSetting.Result);
//                            // move to sugested tmeplate room 방이동
//                            Showlist->SetWheaterNumb(WorldSetting.Quadrant);
//
//                        }
//                        else
//                        {
//                            UE_LOG(LogTemp, Error, TEXT("Showlist is null! Make sure the widget is correctly set in BP_ListActor."));
//                        }
//                    }
//                    else
//                    {
//                        UE_LOG(LogTemp, Error, TEXT("WidgetComponent not found on BP_ListActor."));
//                    }
//                }
//                else
//                {
//                    UE_LOG(LogTemp, Error, TEXT("No BP_ListActor."));
//                }
//            }
//            else
//            {
//                UE_LOG(LogTemp, Error, TEXT("GameInstance is null!"));
//            }
//        }
//        else
//        {
//            UE_LOG(LogTemp, Error, TEXT("AHttpActor::OnResPostChoice() Failed to parse JSON data."));
//        }
//    }
//    else
//    {
//        // ��û�� �������� ��
//        UE_LOG(LogTemp, Warning, TEXT("AHttpActor::OnResPostChoice() POST Request Failed"));
//    }
//}
// MyWorld Setting Data End ------------------------------------------------
