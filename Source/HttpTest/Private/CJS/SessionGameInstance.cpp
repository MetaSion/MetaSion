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



void USessionGameInstance::Init()	// 게임 인스턴스 초기화 함수로, 온라인 서브시스템을 얻어 세션 인터페이스를 초기화. 각종 델리게이트를 설정하여 세션 관련 이벤트에 반응할 수 있도록 함.
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance::Init()"));

	// UserId로 방이름 초기화
	//MySessionName = PlayerState->GetUserId();
	//GetWorld()->GetTimerManager().SetTimerForNextTick(this, &USessionGameInstance::AssignSessionNameFromPlayerState);

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
		GetWorld()->ServerTravel(TEXT("/Game/CJS/Maps/CJS_LobbyMap?listen"));
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

	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (!SessionSearch.IsValid())
	{
		PRINTLOG(TEXT("FindSessions: Failed to create SessionSearch!"));
		return;
	}

	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(FName("HOST_NAME"), MySessionName, EOnlineComparisonOp::Equals);
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 40;


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

	if (!SessionSearch.IsValid())
	{
		PRINTLOG(TEXT("OnMyFindSessionCompleteDelegate: SessionSearch is not valid!"));
		return;
	}

	if (bWasSuccessful)
	{
		TArray<FOnlineSessionSearchResult> results = SessionSearch->SearchResults;
		PRINTLOG(TEXT("Found %d sessions"), results.Num());
		bool bJoinSession = false;

		for (int32 i = 0; i < results.Num(); ++i) // i < results.Num() 으로 수정
		{
			FOnlineSessionSearchResult& ret = results[i]; // 참조를 사용하여 불필요한 복사를 피함
			if (!ret.IsValid())
			{
				continue;
			}

			FString HostName;
			ret.Session.SessionSettings.Get(FName("HOST_NAME"), HostName);
			PRINTLOG(TEXT("Session %d: HostName=%s"), i, *HostName);

			if (HostName == MySessionName)
			{
				PRINTLOG(TEXT("Join Session Call"));
				JoinSession(i);
				bJoinSession = true;
				break;
			}
		}

		if (!bJoinSession)
		{
			CreateMySession();
		}
	}
	else
	{
		PRINTLOG(TEXT("UNetTPSGameInstance::OnMyFindSessionCompleteDelegate()::bWasSuccessful is false"));
	}
}

// 방 조인 요청 (주어진 인덱스의 세션에 참가. 세션 검색 결과를 사용하여 해당 세션에 연결을 시도)
void USessionGameInstance::JoinSession(int32 index)
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
			PRINTLOG(TEXT("Travelling to session URL: %s"), *url);
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
