// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CJS_BallPlayer.generated.h"

UCLASS()
class HTTPTEST_API ACJS_BallPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACJS_BallPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// 플레이어 컨트롤러 ======================================================================================
	UPROPERTY()
	class APlayerController* PC;

	// 카메라 =================================================================================================
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UCameraComponent* CameraComp;
	FVector Direction; 

	// 머터리얼 ===============================================================================================
	UPROPERTY()
    UMaterialInstanceDynamic* DynamicMaterialInstance;
	
	//	나이아가라 ===============================================================================================
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	class UNiagaraComponent* NiagraComp;

	// 인풋 동작 ==============================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputMappingContext* IMC_Player;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_Look;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_Jump;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_Throw;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_Click;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_AimPoint;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_QuitGame;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_InnerWorldUI;
	
// 	void OnMyActionMove(const FInputActionValue& Value);
// 	void OnMyActionLook(const FInputActionValue& Value);
// 	void OnMyActionJump(const FInputActionValue& Value);
	void OnMyActionThrow(const FInputActionValue& Value);
	void OnMyActionClick(const FInputActionValue& Value);
	void OnMyActionToggleAimPointUI(const FInputActionValue& Value);
	void OnMyActionQuitGame(const FInputActionValue& Value);
	void OnMyActionShowInnerWorldUI(const FInputActionValue& Value);

	// 움직임을 위한 힘의 크기
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveForce;


	// 인풋 애니메이션 =========================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_NumKeys[8];
	void OnNumberKeyPressed(const FInputActionValue& Value, int32 KeyIndex);

	// 애니메이션 시퀀스 ========================================================================================
	// 1개씩 적용
	/*UPROPERTY(EditDefaultsOnly, Category = "Anim")
	class UAnimSequence* TestAnimSequence;
	void PlayTestAnimation();*/

	// 애니메이션 시퀀스 배열
	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	TArray<class UAnimSequence*> AnimSequences;
	void PlayAnimationByIndex(int32 Index);


	// 부딪혔을 때 ==============================================================================================
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
	class UNiagaraSystem* HitVFXFactory;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "FX")
	class UNiagaraSystem* SelfHitVFXFactory;
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class USoundBase* HitSFX;
	void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);
	void TriggerSelfHitEffects(FVector HitLocation);

	// 멀티 적용
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_PlayAnimation(int32 AnimationIndex);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_PlayAnimation(int32 AnimationIndex);


	// 하트 던질 때 =============================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Heart")
	TSubclassOf<class ACJS_HeartActor> HeartItemFactory;
	UPROPERTY(EditAnyWhere)
	FVector HeartSpawnPosition;

	// 멀티 적용
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_ThrowHeart();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ThrowHeart();


	// 조준점 위젯 ==============================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> WBP_AimPoint;
	UPROPERTY()
	class UCJS_AimPointWidget* AimPointUI;
	bool bAimPointUIShowing;


	// 방 클릭 시 (클라 -> 서버에 이동 요청 -> 클라 위치 이동) ====================================================
	/* 멀티 방 이동 */
	void RequestMoveMultiRoom(APlayerController* RequestingPC);
	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestMoveMultiRoom(APlayerController* RequestingPC);


	// 로비 입장 시 초기 설정 ============================================================================
	void InitializeFromJson(const FString& LocalJsonData);

	/* 재질 색상 */
	FLinearColor InitColorValue; // RGB 값을 저장하는 변수 (생성 시 초기화에 사용)
	void SetInitColorValue(float r, float g, float b);

	/* 추천방 정보 */
	UPROPERTY(EditDefaultsOnly, Category = "Factory")
	TSubclassOf<class ACJS_MultiRoomActor> RefMultiRoomClass;
	TArray<ACJS_MultiRoomActor*> MultiRoomActors;
	TArray<TSharedPtr<FJsonValue>> AllUsersArray;
	void SetInitMultiRoomInfo(ACJS_MultiRoomActor* MultiRoomActor, const FString& CurNumPlayer, const FString& MaxNumPlayer, const FString& RoomName, const FString& Percent);
	void UpdateMultiRoomPlayerNumInfo(ACJS_MultiRoomActor* MultiRoomActor, const FString& CurNumPlayer, const FString& MaxNumPlayer);
	
	/* 오로라 색상 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aurora")
    TSubclassOf<class ACJS_UltraDynamicSkyActor> DynamicSkyClass;
	UFUNCTION()
	void ModifyAuroraColors();

	
	// 캐릭터생성 -> 로비 통신 정보 설정  ======================================================================
	//FString JsonData = TEXT("{\"UserId\":\"1\",\"R\":1.0,\"G\":0.9225690792809692,\"B\":0.4,\"SimilarUsers\":[{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sunny World\"},{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sol World\"},{\"UserId\":\"abc11\",\"EmotionScore\":81.0,\"RoomName\":\"KW World\"}],\"OppositeUsers\":[{\"UserId\":\"user_1\",\"EmotionScore\":283.5,\"RoomName\":\"JW World\"},{\"UserId\":\"user_3\",\"EmotionScore\":321.0,\"RoomName\":\"DL World\"}]}");
	FString Json = TEXT("{\"UserId\":\"testuser\",\"R\":1.0,\"G\":0.9225690792809692,\"B\":0.4,\"SimilarUsers\":[{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sunny World\"},{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sol World\"},{\"UserId\":\"abc11\",\"EmotionScore\":81.0,\"RoomName\":\"KW World\"}],\"OppositeUsers\":[{\"UserId\":\"user_1\",\"EmotionScore\":283.5,\"RoomName\":\"JW World\"},{\"UserId\":\"user_3\",\"EmotionScore\":321.0,\"RoomName\":\"DL World\"}]}");
	//FString Json = TEXT("{\"UserId\":\"testuser\",\"R\":1.0,\"G\":0.9225690792809692,\"B\":0.4,\"SimilarUsers\":[{\"UserId\":\"user_19\",\"Message\":12.0,\"RoomName\":\"user_19\"},{\"UserId\":\"user_1\",\"Message\":82.0,\"RoomName\":\"user_1\"},{\"UserId\":\"user_2\",\"Message\":51.0,\"RoomName\":\"user_2\"},{\"UserId\":\"user_3\",\"Message\":73.0,\"RoomName\":\"user_3\"},{\"UserId\":\"user_4\",\"Message\":48.0,\"RoomName\":\"user_4\"},{\"UserId\":\"user_5\",\"Message\":56.0,\"RoomName\":\"user_5\"},{\"UserId\":\"user_6\",\"Message\":69.0,\"RoomName\":\"user_6\"},{\"UserId\":\"user_7\",\"Message\":73.0,\"RoomName\":\"user_7\"},{\"UserId\":\"user_8\",\"Message\":88.0,\"RoomName\":\"user_8\"},{\"UserId\":\"user_9\",\"Message\":93.0,\"RoomName\":\"user_9\"},{\"UserId\":\"user_10\",\"Message\":86.0,\"RoomName\":\"user_10\"},{\"UserId\":\"user_11\",\"Message\":99.0,\"RoomName\":\"user_11\"}],\"OppositeUsers\":[{\"UserId\":\"user_20\",\"Message\":283.5,\"RoomName\":\"user_20\"},{ \"UserId\":\"user_12\",\"Message\":11.0,\"RoomName\":\"user_12\"},{\"UserId\":\"user_13\",\"Message\":34.0,\"RoomName\":\"user_13\"},{\"UserId\":\"user_14\",\"Message\":27.0,\"RoomName\":\"user_14\"},{\"UserId\":\"user_15\",\"Message\":39.0,\"RoomName\":\"user_15\"},{\"UserId\":\"user_16\",\"Message\":07.0,\"RoomName\":\"user_16\"},{\"UserId\":\"user_17\",\"Message\":43.0,\"RoomName\":\"user_17\"},{\"UserId\":\"user_18\",\"Message\":31.0,\"RoomName\":\"user_18\"}]}");

	UPROPERTY()
	class USessionGameInstance* SessionGI;
	FString JsonData;
	void InitJsonData(FString LocalJsonData);


	// 로비 -> 체험방 입장 시 통신 ======================================================================
	UPROPERTY()
	//class ACJS_HttpActor* HttpActor;
	class AHttpActor* HttpActor;
	FString ClickedRoomNum;

	//FString CallBackURL = "https://jsonplaceholder.typicode.com/posts";
	FString MultiRoomURL = "125.132.216.190:3326/api/auth/getRoomData";
	//FString MyRoomURL = "http://125.132.216.190:3326/api/auth/userRooms";
	void FindMultiRoomList(FString roomNum, FString newPlayerNum);


	// 월페이퍼 파이썬 자동 실행 ========================================================================
	UFUNCTION(BlueprintCallable, Category = "Python")
	void ExecuteWallPaperPython();


	// 체험방 UI ======================================================================================
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	//TSubclassOf<class UJS_CreateRoomWidget> CR_UIFactory;
	UPROPERTY()
	class UJS_CreateRoomWidget* CR_UI;


	// 멀티 적용
	UPROPERTY()
	class AJS_RoomController* RoomController;
	UFUNCTION(Server, Reliable)
	void ServerRPC_Chat(const FString& msg);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Chat(const FString& msg);

};
