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

    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

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
}

// Called every frame
void AHttpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Login -------------------------------------------------------------
void AHttpActor::LoginReqPost(FString url, FString json)
{
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
        UE_LOG(LogTemp, Warning, TEXT("Login Post Request Success: %s"), *result);
        if (pc) {
            if (SessionGI) {
                SessionGI->bSuccess = true; // GameInstance에 상태 저장
            }
            UGameplayStatics::OpenLevel(this, FName("Main_Sky"));
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

//User -------------------------------------------------------------
void AHttpActor::UserReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // ������� �Լ��� ����
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::UserResPost);
    // ������ ��û
    req->ProcessRequest();
}

void AHttpActor::UserResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        FString result = Response->GetContentAsString();
        UJsonParseLib::User_Convert_JsonToStruct(result);
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
//User End-------------------------------------------------------------

//ChangeIndex -------------------------------------------------------------
void AHttpActor::ChangeIndexReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::ChangeIndexResPost);

    // ������ ��û
    req->ProcessRequest();
}

void AHttpActor::ChangeIndexResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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
        
        FChangeIndex ChangerIndexData = UJsonParseLib::ChangeIndex_Convert_JsonToStruct(result);
        UE_LOG(LogTemp, Warning, TEXT("Parsed room_num: %s"), *ChangerIndexData.room_num);

        
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            AJS_RoomController* RoomController = Cast<AJS_RoomController>(PlayerController);
            if (RoomController && RoomController->R_UI)
            {
                RoomController->R_UI->SetIndex(ChangerIndexData.updatedWallpaperNum, 100);
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

//MyRoomInfo -------------------------------------------------------------
void AHttpActor::MyRoomInfoReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::MyRoomInfoResPost);

    // ������ ��û
    req->ProcessRequest();
}

void AHttpActor::MyRoomInfoResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    //if (!Response.IsValid())
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
    //    return;
    //}
    //// ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    //if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    //{
    //    // ������ ���ڿ��� ��������
    //    FString result = Response->GetContentAsString();
    //    FMyRoomInfo MyRoomInfoData = UJsonParseLib::MyRoomInfo_Convert_JsonToStruct(result);

    //    // �������� ��ȯ�� �����͸� �α׷� ���
    //    UE_LOG(LogTemp, Log, TEXT("Response Received: RoomName = %s, room_pp = %s"),
    //        *MyRoomInfoData.RoomName,
    //        MyRoomInfoData.room_pp ? TEXT("true") : TEXT("false"));
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    //}
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }

    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        FString JsonResponse = Response->GetContentAsString();
        RoomData = UJsonParseLib::RoomData_Convert_JsonToStruct(JsonResponse);

        UE_LOG(LogTemp, Warning, TEXT("RoomData initialized: %s"), *RoomData.userMusic);

        // RoomData가 초기화되었음을 알리기 위해 델리게이트 호출
        OnRoomDataInitialized.Broadcast(RoomData);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Request Failed: %d"), Response->GetResponseCode());
    }
}
//MyRoomInfo End-------------------------------------------------------------

//MyCreateRoomInfo -------------------------------------------------------------
void AHttpActor::MyCreateRoomInfoReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::MyCreateRoomInfoResPost);

    // ������ ��û
    req->ProcessRequest();
}

void AHttpActor::MyCreateRoomInfoResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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
        FMyCreateRoomInfo MyCreateRoomInfoData = UJsonParseLib::FMyCreateRoomInfo_Convert_JsonToStruct(result);

        // �������� ��ȯ�� �����͸� �α׷� ���
        UE_LOG(LogTemp, Log, TEXT("Response Received: userId = %s,RoomNum = %d, RoomName = %s"),
            *MyCreateRoomInfoData.UserId,
             MyCreateRoomInfoData.RoomNum,
            *MyCreateRoomInfoData.RoomName);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}

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
    // ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // ������ ���ڿ��� ��������
        FString result = Response->GetContentAsString();
        FRoomData LocalRoomData = UJsonParseLib::RoomData_Convert_JsonToStruct(result);

        // �������� ��ȯ�� �����͸� �α׷� ���
        UE_LOG(LogTemp, Log, TEXT("Response RoomName = %s"), *LocalRoomData.LikeNum);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
// RoomData ------------------------------------------------------------


void AHttpActor::ReqPostChoice(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // ������� �Լ��� ����
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostChoice);
    // ������ ��û

    req->ProcessRequest();

}
void AHttpActor::OnResPostChoice(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (bConnectedSuccessfully && Response.IsValid())
    {
        // ���������� ������ �޾��� ��
        FString ResponseContent = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("POST Response: %s"), *ResponseContent);
        //StoredJsonResponse = ResponseContent;
        UE_LOG(LogTemp, Warning, TEXT("Stored JSON Response: %s"), *StoredJsonResponse);
        StoredJsonResponse = StoredJsonResponsetest;    // <-------------------------------------------- 여기부터 수정
//         if (SessionGI)
//         {
//             UE_LOG(LogTemp, Warning, TEXT("SessionGM is OK"));
//             SessionGI->SetNetInfoCharacterTOLobby(StoredJsonResponse);
//             SessionGI->FindSessions();
//         }
//         else
//         {
//             UE_LOG(LogTemp, Error, TEXT("SessionGM is NULL"));
//         }

        


        // JSON ���信�� RGB ������ �Ľ�
//         ParsedColors = UKGW_ChoiceSaveBF::ParseJsonToRGB(ResponseContent);
        //         // �Ľ̵� RGB ���� ���� ��� ó��
        //         if (Colors.Num() > 0)
        //         {
        //             // ù ��° RGB �� ��� ���� (��Ƽ���� ����)
        //             FColorData FirstColor = Colors[0];
        // 
        //             UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CharacterMaterial, this);
        //             if (DynamicMaterial)
        //             {
        //                 DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor(FirstColor.R, FirstColor.G, FirstColor.B));
        //                 CharacterMesh->SetMaterial(0, DynamicMaterial);
        //             }
        // 
        //             // �߰� ó�� ����
        //         }
        //         if (ParsedColors.Num() > 0)
        //         {
        //             // �Ľ̵� �����Ͱ� ���� ��� ù ��° ���� �α׷� ��� (������)
        //             FColorData FirstColor = ParsedColors[0];
        //             UE_LOG(LogTemp, Log, TEXT("Parsed Color - R: %f, G: %f, B: %f"), FirstColor.R, FirstColor.G, FirstColor.B);
        //         }
        //         else
        //         {
        //             UE_LOG(LogTemp, Warning, TEXT("No valid RGB data found in response."));
        //         }
        }
        else
        {
            // ��û�� �������� ��
            UE_LOG(LogTemp, Warning, TEXT("POST Request Failed"));
        }

}
void AHttpActor::ShowQuestionUI()
{
    // MyWidgetClass�� ��ȿ���� Ȯ��
    if (QuestionUIFactory && !QuestionUI)
    {
        // UI ���� �ν��Ͻ��� ����
        QuestionUI = CreateWidget<UUserWidget>(GetWorld(), QuestionUIFactory);

        if (QuestionUI)
        {
            // ȭ�鿡 �߰�
            QuestionUI->AddToViewport();
        }
    }
}
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
			UltraWheather_CloudCoverage
			UltraWheather_Fog
			UltraWheather_Rain
			UltraWheather_Snow
			UltraWheather_Dust
			UltraWheather_Thunder
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
        RoomData = UJsonParseLib::RoomData_Convert_JsonToStruct(JsonResponse);
        SessionGI->RoomMusicData = RoomData.userMusic;
		UE_LOG(LogTemp, Warning, TEXT("RecommendedMusic: %s"), *SessionGI->RoomMusicData);
     
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

//마이월드 -> 멀티월드 버튼 클릭 시 통신
void AHttpActor::StartHttpMultyWorld()
{
    //JS ReWrite 이쪽에 방 데이터 송수신 하는 부분 넣고 수신 하는 부분에서 방 이동
    FString UserId;
    if (SessionGI)
    {
        UserId = SessionGI->MySessionName;
        UE_LOG(LogTemp, Warning, TEXT("Assigned UserId from MySessionName: %s"), *UserId);
    }

    // 사용자 데이터를 맵에 추가
    TMap<FString, FString> MyRoomData;
    MyRoomData.Add("userId", UserId);

    // JSON 형식으로 변환
    FString JsonRequest = UJsonParseLib::MakeJson(MyRoomData);

    // 로그 출력 (디버깅용)
    UE_LOG(LogTemp, Warning, TEXT("userId: %s"), *UserId);
    UE_LOG(LogTemp, Warning, TEXT("Json Request: %s"), *JsonRequest);

    // 서버로 요청 전송
    ReqPostClickMyRoom(EntryMultiWorldURL, JsonRequest);
}
void AHttpActor::ReqPostClickMultiWorld(FString url, FString json)
{
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
    if (bConnectedSuccessfully && Response.IsValid())
    {
        FString ResponseContent = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("POST Response: %s"), *ResponseContent);
        //StoredJsonResponse = ResponseContent;  // <-- 실제 통신 시
        UE_LOG(LogTemp, Warning, TEXT("Stored JSON Response: %s"), *StoredJsonResponse);
        StoredJsonResponse = StoredJsonResponsetest;  // <-- 테스트 시   
        if (SessionGI)
        {
            UE_LOG(LogTemp, Warning, TEXT("SessionGM is OK"));
            SessionGI->SetNetInfoCharacterTOLobby(StoredJsonResponse);
            SessionGI->FindSessions();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("SessionGM is NULL"));
        }
    }

}

//Getter 함수
FRoomData AHttpActor::GetRoomData() const
{
    FString Rdata = RoomData.userMusic;
    UE_LOG(LogTemp, Warning, TEXT("RoomData music : %s"), *Rdata);
    return RoomData;
}
//MyCreateRoomInfo End-------------------------------------------------------------

void AHttpActor::ReqGetWebImage(FString url)
{
    // HTTP 모듈 인스턴스 가져오기
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // 요청 설정
    req->SetURL(url);
    req->SetVerb(TEXT("Post"));
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
        // 성공
        FString result = Response->GetContentAsString();

        // 필요한 정보만 뽑아서 화면에 출력하고싶다.
        ImageUI->SetTextLog(result);
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
    
    if (bConnectedSuccessfully && Response.IsValid())
    {
        FString ResponseContent = Response->GetContentAsString();
        UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *ResponseContent);
        	

        // JSON 파싱 함수 호출 및 반환 값 저장
        TArray<FMyCreatedRoom> ParsedResult = UJsonParseLib::JsonParseRoomList(ResponseContent);

        // 파싱된 결과를 문자열로 변환하여 출력
        FString ParsedString;
        for (const FMyCreatedRoom& Room : ParsedResult)
        {
            ParsedString.Append(FString::Printf(TEXT("roomNum: %s, roomName: %s\n"), *Room.RoomNum, *Room.RoomName));
        }

        // 파싱된 결과 출력
        UE_LOG(LogTemp, Log, TEXT("Parsed Room Data:\n%s"), *ParsedString);

        USessionGameInstance* GameInstance = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
        if (GameInstance)
        {
            GameInstance->InitRoomNameNum(ParsedResult); // 데이터가 제대로 저장되었는지 로그로 확인
            UE_LOG(LogTemp, Error, TEXT("GameInstance->InitRoomInfoList size: %d"), GameInstance->RoomInfoList.Num());
            TArray<FMyCreatedRoom> Result;
            Result=  GameInstance->GettRoomNameNum(); // 데이터가 제대로 저장되었는지 로그로 확인
            UE_LOG(LogTemp, Error, TEXT("GameInstance->GEtRoomInfoList size: %d"), Result.Num());
            AKGW_RoomlistActor* ListActor = Cast<AKGW_RoomlistActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AKGW_RoomlistActor::StaticClass()));
            if (ListActor)
            {
                UWidgetComponent* WidgetComp = ListActor->FindComponentByClass<UWidgetComponent>();
                if (WidgetComp)
                {
                    UKGW_RoomList* Showlist = Cast<UKGW_RoomList>(WidgetComp->GetUserWidgetObject());
                    if (Showlist)
                    {
                        // RoomInfoList 데이터를 위젯에 추가
                        Showlist->AddSessionSlotWidget(GameInstance->GettRoomNameNum());
                        UE_LOG(LogTemp, Log, TEXT("Showlist updated successfully."));
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
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("GameInstance is null!"));
        }

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to receive a valid response from the server."));
    }


}
