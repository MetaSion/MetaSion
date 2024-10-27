// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"						// 언리얼 엔진의 기본 헤더 파일로, 다양한 유틸리티와 기본 타입을 포함
#include "Engine/GameInstance.h"				// 게임 인스턴스 클래스(UGameInstance)를 포함, 이 클래스는 게임이 실행되는 동안 유지되는 단일 인스턴스를 나타냄
#include "Interfaces/OnlineSessionInterface.h"	//온라인 세션을 관리하기 위한 인터페이스. 세션 생성, 검색, 참가와 같은 기능을 제공
#include "Delegates/DelegateCombinations.h"		// 델리게이트(이벤트 처리기)를 결합하기 위한 헤더 파일
#include "SessionGameInstance.generated.h"		// 언리얼 엔진의 리플렉션 시스템에서 사용되는 자동 생성된 헤더 파일


USTRUCT(BlueprintType)							// 블루프린트에서 사용할 수 있도록 구조체를 정의
struct FRoomInfo								// 세션(방)의 정보를 저장하는 구조체로, 방 이름, 호스트 이름, 최대/현재 플레이어 수, 핑 정보를 포함
{
	GENERATED_BODY()

    // 방 이름
	UPROPERTY(BlueprintReadOnly)
	FString roomName;
	// 호스트 이름
	UPROPERTY(BlueprintReadOnly)
	FString hostName;
	UPROPERTY(BlueprintReadOnly)
	// 최대 플레이어 수
	int32 maxPlayerCount;
	UPROPERTY(BlueprintReadOnly)
	// 입장한 플레이어 수 (최대 - 남은)
	int32 currentPlayerCount;
	UPROPERTY(BlueprintReadOnly)
	// 핑 정보
	int32 pingMS;

	int32 index;

	FString ToString()							// 방 정보를 문자열로 변환하는 메서드
	{
		return FString::Printf(TEXT("%d) [%s] [%s] %d / %d - ping : %dms"), index, *roomName, *hostName, currentPlayerCount, maxPlayerCount, pingMS);
	}
};

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature, const struct FRoomInfo&, info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSessionSearchComplete);

/**
 * 
 */
UCLASS()
class HTTPTEST_API USessionGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	// 온라인 세션 인터페이스를 기억하고 싶다 (비동기 방식)
	IOnlineSessionPtr SessionInterface;			// 온라인 세션 인터페이스를 가리키는 포인터로, 세션 생성 및 관리를 위해 사용

	// 호스트 이름 (세션 이름)
	UPROPERTY()
	FString MySessionName = TEXT("User1");
	//FString MySessionName;

	// 방 생성 요청 (새로운 세션을 생성하는 함수)
	UFUNCTION()
	void CreateMySession();
	// 방 생성 응답 (세션 생성이 완료되었을 때 호출되는 콜백 함수)
	void OnCreateSessionComplete(FName sessionName, bool bWasSuccessful);

	// 찾을 방의 목록 (세션 검색 결과를 저장하는 스마트 포인터)
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	// 방 찾기 요청 (세션을 찾는 함수)
	UFUNCTION()
	void FindSessions();
	// 방 찾기 응답 (션 검색이 완료되었을 때 호출되는 콜백 함수)
	void OnMyFindSessionCompleteDelegate(bool bWasSuccessful);

	// 방 조인 요청 (특정 인덱스의 세션에 참가하는 함수)
	UFUNCTION()
	void JoinSession(int32 index);
	// 방 조인 응답 (세션 참가가 완료되었을 때 호출되는 콜백 함수)
	void OnMyJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type);

	// 네트워크 연결 실패 시 (네트워크 연결 실패 시 호출되는 함수로, 오류 메시지를 출력하거나 특정 동작을 수행)
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	
	
	// UserId 할당
	//UPROPERTY()
	//class ACJS_BallPlayerState* PlayerState;
	// PlayerState의 UserId를 받아서 MySessionName에 지정
	void AssignSessionNameFromPlayerState();
	bool ValidateSessionInterfaceAndSearch() const;

};

