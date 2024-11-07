// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpFwd.h"
#include "JsonParseLib.h"
#include "HttpActor.generated.h"

// RoomData가 초기화되었을 때 호출되는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomDataInitialized, const FRoomData&, RoomData);

UCLASS()
class HTTPTEST_API AHttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHttpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// RoomData 초기화 시 호출되는 델리게이트
    UPROPERTY(BlueprintAssignable, Category="Room Data")
    FOnRoomDataInitialized OnRoomDataInitialized;

	UPROPERTY(EditAnywhere)
	class AJS_RoomController* pc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UHttpWidget> Sign_Factory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> QuestionUIFactory;

	UPROPERTY(EditAnywhere)
	class UUserWidget* QuestionUI;

	UPROPERTY(BlueprintReadWrite, Category = "JSON Data")
	FString StoredJsonResponse;

	FString StoredJsonResponsetest = TEXT("{\"UserId\":\"testuser\",\"R\":1.0,\"G\":0.9225690792809692,\"B\":0.4,\"SimilarUsers\":[{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sunny World\"},{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sol World\"},{\"UserId\":\"abc11\",\"EmotionScore\":81.0,\"RoomName\":\"KW World\"}],\"OppositeUsers\":[{\"UserId\":\"user_1\",\"EmotionScore\":283.5,\"RoomName\":\"JW World\"},{\"UserId\":\"user_3\",\"EmotionScore\":321.0,\"RoomName\":\"DL World\"}]}");

	// Login
	void LoginReqPost(FString url, FString json);
	void LoginResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// Sign_Up
	void SignUpReqPost(FString url, FString json);
	void SignUpResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// User
	void UserReqPost(FString url, FString json);
	void UserResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	
	//ChangeIndex
	void ChangeIndexReqPost(FString url, FString json);
	void ChangeIndexResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	//MyRoomInfo
	void MyRoomInfoReqPost(FString url, FString json);
	void MyRoomInfoResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	//MyCreateRoomInfo
	void MyCreateRoomInfoReqPost(FString url, FString json);
	void MyCreateRoomInfoResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	//RoomData
	void RoomDataReqPost(FString url, FString json);
	void RoomDataResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void ReqPostChoice(FString url, FString json);
	void OnResPostChoice(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void ShowQuestionUI();

	UFUNCTION(BlueprintCallable, Category = "JSON")
    FString StoreJsonResponse();


	FString LoginURL = "mirrora.duckdns.org:3326/api/auth/login";
	FString EnteryLobbyURL = "mirrora.duckdns.org:3326/api/auth/processAndSendData";
	FString MyRoomURL = "mirrora.duckdns.org:3326/api/auth/userRooms";
	FString WallPaperURL = "mirrora.duckdns.org:3326/api/auth/wallpaperupdate";
	FString HeartURL = "jsonplaceholder.typicode.com/posts";
	FString EntryMultiWorldURL = "mirrora.duckdns.org:3326/api/auth/";  // <-- BE 작업 완료 후 추가하기
	

	/* Sunny */
	//캐릭터생성 -> 로비 입장 시 초기 설정
	UPROPERTY()
	class USessionGameInstance* SessionGI;

	//로비 -> 멀티방 입장 시 통신
	void ReqPostClickMultiRoom(FString url, FString json);
	void OnResPostClickMultiRoom(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	//로비 -> 내방으로 입장 시 통신
	void ReqPostClickMyRoom(FString url, FString json);
	void OnResPostClickMyRoom(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	//마이월드 -> 멀티월드 버튼 클릭 시 통신
	void StartHttpMultyWorld();
	void ReqPostClickMultiWorld(FString url, FString json);
	void OnResPostClickMultiWorld(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	//마이월드 인입 시 셋팅
	class AJS_SoundActor* SoundActor;
	void SetBackgroundSound();

	class AKGW_RoomlistActor* MyWorldPlayer;

	void ApplyMyWorldPointLightColors();
	void ApplyMyWorldNiagaraAssets();




	//다른 클래스에서 파싱된 RoomData를 사용하기 위한 Getter함수
	FRoomData GetRoomData() const;
	FRoomData RoomData;

	//RoomUI Timer
	FTimerHandle RoomUIWaitTimerHandle;

	//image
	UPROPERTY()
	class UWBP_Image* ImageUI;
	void ReqGetWebImage(FString url);

	void OnResGetWebImage(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void ReqPostTest(FString url, FString json);

	void OnResPostTest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void ReqPostRoomList(FString url, FString json);

	void OnResPostRoomList(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);


};
