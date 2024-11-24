// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/SessionGameInstance.h"
#include "CJS/CJS_BallPlayerState.h"
#include "../HttpTest.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

#include "GenericPlatform/GenericPlatformMisc.h"
#include "Kismet/GameplayStatics.h"
#include "string"
#include "CJS/CJS_BallPlayer.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"



void USessionGameInstance::Init()	// 게임 인스턴스 초기화 함수로, 온라인 서브시스템을 얻어 세션 인터페이스를 초기화. 각종 델리게이트를 설정하여 세션 관련 이벤트에 반응할 수 있도록 함.
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::Init()"));
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance Initialized at %p"), this);
	UE_LOG(LogTemp, Warning, TEXT("Initial WorldSetting Suggest List Count: %d"), WorldSetting.suggest_list.Num());

	IOnlineSubsystem* subSystem = IOnlineSubsystem::Get();

	if (subSystem)
	{
		PRINTLOG(TEXT("OnlineSubsystem: %s"), *subSystem->GetSubsystemName().ToString());
		SessionInterface = subSystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			PRINTLOG(TEXT("SessionInterface is valid"));
		}
		else
		{
			PRINTLOG(TEXT("SessionInterface is invalid"));
		}
	}
	else
	{
		PRINTLOG(TEXT("No OnlineSubsystem found"));
	}

	if(subSystem)
	{
		// 방 생성 요청 -> 응답
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USessionGameInstance::OnCreateSessionComplete);

		// 방 찾기 요청 -> 응답
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USessionGameInstance::OnMyFindSessionCompleteDelegate);

		// 방 조인 요청 -> 응답
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USessionGameInstance::OnMyJoinSessionComplete);
	}

	GEngine->OnNetworkFailure().AddUObject(this, &USessionGameInstance::OnNetworkFailure);
}

// 방 생성 요청 (새로운 세션을 생성. 션 설정(FOnlineSessionSettings)을 정의하고, 여러 조건(전용 서버 여부, LAN 매치 여부, 공개 여부 등)을 설정. 그리고 세션 인터페이스를 통해 세션 생성을 요청)
void USessionGameInstance::CreateMySession()
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::CreateMySession()"));
	PRINTLOG(TEXT("CreateMySession()"));

	FOnlineSessionSettings settings;

	// 1. 전용 서버를 사용하는가?
	settings.bIsDedicated = false;

	// 2. 랜선(Lan)으로 매치하는가?
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	settings.bIsLANMatch = subsysName == "NULL";

	// 3. 매칭이 공개(true)냐, 비공개(false)냐?
	settings.bShouldAdvertise = true;

	// 4. 유저의 상태 정보(온라인/자리비움/ 등등) 사용 여부
	settings.bUsesPresence = true;

	// 5. 중간에 난입 가능한가?
	settings.bAllowJoinViaPresence = true;
	settings.bAllowJoinInProgress = true;

	settings.bAllowJoinViaPresenceFriendsOnly = false;
	settings.bAntiCheatProtected = false;
	settings.bUseLobbiesIfAvailable = true;

	// 6. 최대 몇명?
	settings.NumPublicConnections = 100;

	// 7. 커스텀 정보
	//settings.Set(FName("ROOM_NAME"), sessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	settings.Set(FName("HOST_NAME"), MySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	PRINTLOG(TEXT("Attempting to create session with name: %s"), *MySessionName);
	PRINTLOG(TEXT("Create Session Start hostName : %s"), *MySessionName);

	//FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
	SessionInterface->CreateSession(0, FName(MySessionName), settings);
}

// 방 생성 응답 (세션 생성 요청의 결과를 처리. 성공 시 서버를 특정 맵으로 이동(ServerTravel)시킴)
void USessionGameInstance::OnCreateSessionComplete(FName sessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::OnCreateSessionComplete()"));

	if (bWasSuccessful)
	{
		PRINTLOG(TEXT("OnCreateSessionComplete is Successes"));
		PRINTLOG(TEXT("Session created successfully with name: %s"), *sessionName.ToString());

		UE_LOG(LogTemp, Warning, TEXT("Before Level Travel: WorldSetting Suggest List Count: %d"), WorldSetting.suggest_list.Num());
		GetWorld()->ServerTravel(TEXT("/Game/Junguk/Maps/Lobby?listen"));
		UE_LOG(LogTemp, Warning, TEXT("After Level Travel: WorldSetting Suggest List Count: %d"), WorldSetting.suggest_list.Num());
		
	}
	else
	{
		PRINTLOG(TEXT("Failed to create session with name: %s"), *sessionName.ToString());
		PRINTLOG(TEXT("OnCreateSessionComplete is Failed"));
	}
}

// 방 찾기 요청 (다른 세션을 검색. 검색 설정을 정의하고, 온라인 세션 인터페이스를 통해 세션 검색을 시작함)
void USessionGameInstance::FindSessions()
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::FindSessions()"));
	PRINTLOG(TEXT("SessionInterface valid: %s"), SessionInterface.IsValid() ? TEXT("True") : TEXT("False"));

	if (bIsSearching)
	{
		UE_LOG(LogTemp, Warning, TEXT("FindSessions() - A search request is already pending."));
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (!SessionSearch.IsValid())
	{
		PRINTLOG(TEXT("FindSessions: Failed to create SessionSearch!"));
		return;
	}

	bIsSearching = true;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(FName("HOST_NAME"), MySessionName, EOnlineComparisonOp::Equals);
	//SessionSearch->QuerySettings.Set(SEARCH_KEYWORDS, FString(TEXT("testuser")), EOnlineComparisonOp::Equals); // 세션 이름 추가 조건 설정
	//SessionSearch->bIsLanQuery = true;
	SessionSearch->bIsLanQuery = false;
	SessionSearch->MaxSearchResults = 50;


	if (!SessionInterface->FindSessions(0, SessionSearch.ToSharedRef()))
	{
		PRINTLOG(TEXT("FindSessions: Failed to start FindSessions!"));
	}
	else
	{
		PRINTLOG(TEXT("FindSessions: Successfully started FindSessions"));
	}

	//PRINTLOG(TEXT("Searching for sessions with name: %s, IsLAN: %s"), *MySessionName, SessionSearch->bIsLanQuery ? TEXT("True") :
}

// 방 찾기 응답 (세션 검색이 완료되면 호출. 색 결과를 기반으로 세션에 참가하거나, 조건에 맞는 세션이 없다면 새로 생성)
void USessionGameInstance::OnMyFindSessionCompleteDelegate(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::OnMyFindSessionCompleteDelegate()"));

	bIsSearching = false;

	if (SessionSearch.IsValid() && bWasSuccessful)
	{
		PRINTLOG(TEXT("Found %d sessions"), SessionSearch->SearchResults.Num());

		for (const auto& Result : SessionSearch->SearchResults)
		{
			FString HostName;
			Result.Session.SessionSettings.Get(FName("HOST_NAME"), HostName);
			PRINTLOG(TEXT("Session Found - HostName: %s, SessionName: %s"), *HostName, *Result.Session.OwningUserName);
		}
	}
	else
	{
		PRINTLOG(TEXT("No sessions found or session search failed."));
	}

	//if (bWasSuccessful)
	//{
	//	TArray<FOnlineSessionSearchResult> results = SessionSearch->SearchResults;
	//	PRINTLOG(TEXT("Found %d sessions"), results.Num());
	//	bool bJoinSession = false;

	//	for (int32 i = 0; i < results.Num(); ++i) // i < results.Num() 으로 수정
	//	{
	//		FOnlineSessionSearchResult& ret = results[i]; // 참조를 사용하여 불필요한 복사를 피함
	//		if (!ret.IsValid())
	//		{
	//			continue;
	//		}
	//		FString HostName;
	//		ret.Session.SessionSettings.Get(FName("HOST_NAME"), HostName);
	//		PRINTLOG(TEXT("Session %d: HostName=%s"), i, *HostName);

	//		if (HostName == MySessionName)
	//		{
	//			PRINTLOG(TEXT("Join Session Call"));
	//			JoinSession(i);
	//			bJoinSession = true;
	//			break;
	//		}
	//	}
	//	if (!bJoinSession)
	//	{
	//		CreateMySession();
	//	}
	//}
	//else
	//{
	//	PRINTLOG(TEXT("UNetTPSGameInstance::OnMyFindSessionCompleteDelegate()::bWasSuccessful is false"));
	//}

	if (bWasSuccessful && SessionSearch->SearchResults.Num() > 0)
	{
		bool bJoinSession = false;

		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); ++i)
		{
			const FOnlineSessionSearchResult& Result = SessionSearch->SearchResults[i];

			FString HostName;
			Result.Session.SessionSettings.Get(FName("HOST_NAME"), HostName);

			if (HostName == "testuser")  // 정확한 세션 이름 확인
			{
				ReqJoinSession(i);  // 인덱스를 전달
				bJoinSession = true;
				break;
			}
		}
		if (!bJoinSession)
		{
			PRINTLOG(TEXT("No session with the name 'testuser' found, creating a new session."));
			CreateMySession();
		}
	}
	else
	{
		PRINTLOG(TEXT("Failed to find any sessions, creating a new session."));
		CreateMySession();
	}
}

// 방 조인 요청 (주어진 인덱스의 세션에 참가. 세션 검색 결과를 사용하여 해당 세션에 연결을 시도)
void USessionGameInstance::ReqJoinSession(int32 index)
{
	// SessionSearch
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::JoinSession()"));
	PRINTLOG(TEXT("Join Session Called"));

	if (!SessionSearch.IsValid() || SessionSearch->SearchResults.Num() <= index)
	{
		PRINTLOG(TEXT("JoinSession failed: SessionSearch is invalid or no search results."));
		return;
	}

	auto result = SessionSearch->SearchResults[index];
	SessionInterface->JoinSession(0, FName(MySessionName), result);
}

// 방 조인 응답 (세션 참가 요청의 결과를 처리. 성공 시 플레이어 컨트롤러를 사용하여 세션 URL로 이동)
void USessionGameInstance::OnMyJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type EOnJoinSessionCompleteResult)
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::OnMyJoinSessionComplete()"));
	PRINTLOG(TEXT("OnMyJoinSessionComplete"));

	if (EOnJoinSessionCompleteResult == EOnJoinSessionCompleteResult::Success)
	{
		// 서버가 있는 레벨로 여행을 떠나고 싶다.
		auto* pc = GetWorld()->GetFirstPlayerController();

		FString url;
		if (SessionInterface->GetResolvedConnectString(SessionName, url))
		{
			PRINTLOG(TEXT("Travelling to session MultiRoomURL: %s"), *url);
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}
		else
		{
			PRINTLOG(TEXT("Failed to get resolved connect string for session."));
		}
	}
	else
	{
		PRINTLOG(TEXT("JoinSession failed with result: %d"), static_cast<int32>(EOnJoinSessionCompleteResult));
	}
}

// 네트워크 연결 실패 시 (네트워크 오류가 발생했을 때 호출. 오류 내용을 출력하고 필요 시 특정 레벨로 이동하는 등의 동작을 수행)
void USessionGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::OnNetworkFailure()"));
	PRINTLOG(TEXT("Network Failure: %s"), *ErrorString);

	if (FailureType == ENetworkFailure::FailureReceived)
	{
		PRINTLOG(TEXT("Host closed connection, returning to Exit Map"));
		//UGameplayStatics::OpenLevel(this, FName("/Game/ArtProject/LHM/Maps/LHM_Exit"));
	}
}


// 세션 파괴
void USessionGameInstance::ExitSession()
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::ExitSession"));
	//ServerRPC_ExitSession();
}

void USessionGameInstance::ServerRPC_ExitSession_Implementation()
{
	//MulticastRPC_ExitSession();
}

void USessionGameInstance::MulticastRPC_ExitSession_Implementation()
{
	// 방퇴장 요청
	//SessionInterface->DestroySession(FName(MySessionName));
}


void USessionGameInstance::OnMyDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		// 클라이언트가 로비로 여행을 가고싶다.
		auto* pc = GetWorld()->GetFirstPlayerController();
		pc->ClientTravel(TEXT("/Game/NetTPS/Maps/LobbyMap"), ETravelType::TRAVEL_Absolute);
	}
}



void USessionGameInstance::AssignSessionNameFromPlayerState()
{
	PRINTLOG(TEXT("USessionGameInstance::AssignSessionNameFromPlayerState()"));

	APlayerController* PlayerController = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;

	if (PlayerController)
	{
		ACJS_BallPlayerState* PlayerState = Cast<ACJS_BallPlayerState>(PlayerController->PlayerState);
		if (PlayerState)
		{
			MySessionName = PlayerState->GetUserId();
			PRINTLOG(TEXT("MySessionName:: %s"), *MySessionName);
		}
		else
		{
			PRINTLOG(TEXT("AssignSessionNameFromPlayerState: PlayerState is not valid"));
		}
	}
	else
	{
		PRINTLOG(TEXT("AssignSessionNameFromPlayerState: PlayerController is not valid"));
	}
}

bool USessionGameInstance::ValidateSessionInterfaceAndSearch() const
{
	if (!SessionInterface.IsValid())
	{
		PRINTLOG(TEXT("SessionInterface is not valid"));
		return false;
	}

	if (!SessionSearch.IsValid())
	{
		PRINTLOG(TEXT("SessionSearch is not valid"));
		return false;
	}

	return true;
}



void USessionGameInstance::InitSessionName(FString name)
{
	UE_LOG(LogTemp, Warning, TEXT(" USessionGameInstance::InitSessionName()"));
	MySessionName = name;
	UE_LOG(LogTemp, Warning, TEXT(" USessionGameInstance::InitSessionName() MySessionName : %s"), *MySessionName);
}
FString USessionGameInstance::GetMySessionName()
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::GetMySessionName() MySessionName : %s"), *MySessionName);
	return MySessionName;
}

void USessionGameInstance::InitRoomNameNum(TArray<FMyWorldRoomInfo> list)
{
	UE_LOG(LogTemp, Warning, TEXT(" USessionGameInstance::InitRoomNameNum()"));

	RoomInfoList = list;
	UE_LOG(LogTemp, Error, TEXT("GameInstance->StoredRoomInfos size: %d"), RoomInfoList.Num());

}
TArray<FMyWorldRoomInfo> USessionGameInstance::GettRoomNameNum()
{
	UE_LOG(LogTemp, Error, TEXT("USessionGameInstance::GetMySessionName() MySessionName : %d"), RoomInfoList.Num());

	return RoomInfoList;

}

	
//void USessionGameInstance::SetRefMultiRoomInfo(FString json)
//{
//	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::SetRefMultiRoomInfo()"));
//
//	// World 객체에서 첫 번째 PlayerController 가져오기
//		UWorld * World = GetWorld();
//	if (!World)
//	{
//		UE_LOG(LogTemp, Error, TEXT("World is null in SetRefMultiRoomInfo"));
//		return;
//	}
//
//	APlayerController* PlayerController = World->GetFirstPlayerController();
//	if (!PlayerController)
//	{
//		UE_LOG(LogTemp, Error, TEXT("PlayerController is null in SetRefMultiRoomInfo"));
//		return;
//	}
//
//	// PlayerController에서 현재 Pawn을 가져와 ACJS_BallPlayer로 캐스팅
//	ACJS_BallPlayer* Player = Cast<ACJS_BallPlayer>(PlayerController->GetPawn());
//	if (Player)
//	{
//		Player->InitJsonData(json);
//		UE_LOG(LogTemp, Warning, TEXT("Player cast successful and InitJsonData called"));
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("Failed to cast to ACJS_BallPlayer"));
//	}
//}

void USessionGameInstance::SetNetInfoCharacterTOLobby(FString info)
{	
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::SetNetInfoCharacterTOLobby()"));
	NetInfoCharacterTOLobby = info;
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::SetNetInfoCharacterTOLobby() : %s"), *NetInfoCharacterTOLobby);
}
FString USessionGameInstance::GetNetInfoCharacterTOLobby()
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::GetNetInfoCharacterTOLobby()"));
	return NetInfoCharacterTOLobby;
}

// 체험방 UI 멀티 적용 여부 확인 변수
bool USessionGameInstance::GetbRefRoomUIMultiOn()
{
	return bRefRoomUIMultiOn;
}
void USessionGameInstance::SetbRefRoomUIMultiOn(bool value)
{
	bRefRoomUIMultiOn = value;
}

void USessionGameInstance::ChangePlayerController(UWorld* World, TSubclassOf<APlayerController> NewControllerClass)
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::ChangePlayerController()"));

	// 현재 로컬 플레이어인지 확인
	APlayerController* CurrentController = World->GetFirstPlayerController();
	if (!CurrentController || !CurrentController->IsLocalController()) return;

	// 이미 변경 중이라면 중단
	if (bIsChangingController || !NewControllerClass) return;

	bIsChangingController = true;

	// 기존 컨트롤러가 존재하면 파괴
	APawn* CurrentPawn = CurrentController->GetPawn();
	CurrentController->UnPossess();
	CurrentController->Destroy();

	// 새로운 컨트롤러 스폰
	FActorSpawnParameters SpawnParams;
	APlayerController* NewController = World->SpawnActor<APlayerController>(NewControllerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	// 로컬 플레이어 컨트롤러인지 확인 후 기존 Pawn을 조종하게 설정
	if (NewController && NewController->IsLocalController() && CurrentPawn)
	{
		NewController->Possess(CurrentPawn);
	}

	bIsChangingController = false; // 플래그 초기화
}

void USessionGameInstance::HandleMapChange(UWorld* World)
{
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::HandleMapChange()"));

	APlayerController* LocalController = World->GetFirstPlayerController();
	if (!LocalController || !LocalController->IsLocalController()) return;

	FString MapName = World->GetMapName();
	if (MapName.Contains("LobbyMap"))
	{
		ChangePlayerController(World, LobbyControllerClass);
	}
	else
	{
		ChangePlayerController(World, RoomControllerClass);
	}
}

void USessionGameInstance::PlayMusic(USoundBase* Music)
{
	if (!Music)
	{
		UE_LOG(LogTemp, Warning, TEXT("Music is null. Cannot play music."));
		return;
	}

	// 사운드 재생
	UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(
		this,                    // 월드 컨텍스트
		Music,                   // 사운드
		1.0f,                    // 볼륨 배수
		1.0f,                    // 피치 배수
		0.0f,                    // 시작 시간
		nullptr,                 // 동시성 설정
		true,                    // 레벨 전환 간 지속 여부
		true                     // 자동 파괴
	);

	// AudioComponent를 MusicSound 변수에 저장
	MusicSound = AudioComponent;
}
void USessionGameInstance::FadeOutAndPlayNewMusic(USoundBase* NewMusic)
{
	if (!NewMusic)
	{
		UE_LOG(LogTemp, Warning, TEXT("NewMusic is null. Cannot play music."));
		return;
	}

	if (MusicSound)
	{
		// 기존 음악이 재생 중이면 페이드 아웃 실행
		FadeOutCurrentMusic(NewMusic);
	}
	else
	{
		// 기존 음악이 없으면 바로 새로운 음악 재생
		PlayMusic(NewMusic);
	}
}
void USessionGameInstance::FadeOutCurrentMusic(USoundBase* NewMusic)
{
	GetWorld()->GetTimerManager().SetTimer(FadeOutTimerHandle, [this, NewMusic]()
		{
			if (MusicSound)
			{
				// 현재 볼륨 가져오기
				float CurrentVolume = MusicSound->VolumeMultiplier;

				// 볼륨이 0보다 크면 서서히 감소
				if (CurrentVolume > 0.0f)
				{
					CurrentVolume = FMath::Clamp(CurrentVolume - (1.0f / FadeOutDuration) * GetWorld()->GetDeltaSeconds(), 0.0f, 1.0f);
					MusicSound->SetVolumeMultiplier(CurrentVolume);
				}
				else
				{
					// 볼륨이 0이 되면 음악 정지
					MusicSound->Stop();
					MusicSound = nullptr;

					// 타이머 정리
					GetWorld()->GetTimerManager().ClearTimer(FadeOutTimerHandle);

					// 새로운 음악 재생
					PlayMusic(NewMusic);
				}
			}
			else
			{
				// MusicSound가 null일 경우 즉시 새로운 음악 재생
				UE_LOG(LogTemp, Warning, TEXT("MusicSound is null during fade out. Playing new music."));
				GetWorld()->GetTimerManager().ClearTimer(FadeOutTimerHandle);
				PlayMusic(NewMusic);
			}
		}, 0.1f, true); // 0.1초 간격으로 타이머 실행
}
