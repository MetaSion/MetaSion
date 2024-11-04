﻿// Fill out your copyright notice in the Description page of Project Settings.

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
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;
	FVector Direction; 

	// 머터리얼 ===============================================================================================
	UPROPERTY()
    UMaterialInstanceDynamic* DynamicMaterialInstance;
	// Material Rotation Quaternion
	FQuat MaterialRotationQuat;
	// Rotation angles for each key input
	float TargetYaw;
	// Control rotation
	FVector2D ControlInput;
	// Method to update material rotation
	void UpdateMaterialRotation(float DeltaTime);

	// 움직임을 위한 힘의 크기
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveForce;

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
	
	void OnMyActionMove(const FInputActionValue& Value);
	void OnMyActionLook(const FInputActionValue& Value);
	void OnMyActionJump(const FInputActionValue& Value);
	void OnMyActionThrow(const FInputActionValue& Value);
	void OnMyActionClick(const FInputActionValue& Value);
	void OnMyActionToggleAimPointUI(const FInputActionValue& Value);

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
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class UParticleSystem* HitVFXFactory;
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class UParticleSystem* SelfHitVFXFactory;
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
	//UPROPERTY()
	//class ACJS_MultiRoomActor* RefMultiRoom;
	UPROPERTY(EditDefaultsOnly, Category = "Lobby")
	TSubclassOf<class ACJS_MultiRoomActor> RefMultiRoom;
	TArray<ACJS_MultiRoomActor*> MultiRoomActors;
	TArray<TSharedPtr<FJsonValue>> AllUsersArray;
	void SetInitMultiRoomInfo(ACJS_MultiRoomActor* MultiRoomActor, int32 CurNumPlayer, int32 MaxNumPlayer, const FString& RoomName, const FString& Percent);


	
	// 캐릭터생성 -> 로비 통신 정보 설정
	//FString JsonData = TEXT("{\"UserId\":\"1\",\"R\":1.0,\"G\":0.9225690792809692,\"B\":0.4,\"SimilarUsers\":[{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sunny World\"},{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sol World\"},{\"UserId\":\"abc11\",\"EmotionScore\":81.0,\"RoomName\":\"KW World\"}],\"OppositeUsers\":[{\"UserId\":\"user_1\",\"EmotionScore\":283.5,\"RoomName\":\"JW World\"},{\"UserId\":\"user_3\",\"EmotionScore\":321.0,\"RoomName\":\"DL World\"}]}");

	FString Json = TEXT("{\"UserId\":\"testuser\",\"R\":1.0,\"G\":0.9225690792809692,\"B\":0.4,\"SimilarUsers\":[{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sunny World\"},{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sol World\"},{\"UserId\":\"abc11\",\"EmotionScore\":81.0,\"RoomName\":\"KW World\"}],\"OppositeUsers\":[{\"UserId\":\"user_1\",\"EmotionScore\":283.5,\"RoomName\":\"JW World\"},{\"UserId\":\"user_3\",\"EmotionScore\":321.0,\"RoomName\":\"DL World\"}]}");

	UPROPERTY()
	class USessionGameInstance* SessionGI;
	FString JsonData;
	void InitJsonData(FString LocalJsonData);


	// 로비 -> 체험방 입장 시 통신 ======================================================================
	UPROPERTY()
	//class ACJS_HttpActor* HttpActor;
	class AHttpActor* HttpActor;
	//FString CallBackURL = "https://jsonplaceholder.typicode.com/posts";
	FString MultiRoomURL = "125.132.216.190:3326/api/auth/getRoomData";
	//FString MyRoomURL = "http://125.132.216.190:3326/api/auth/userRooms";


	// 월페이퍼 파이썬 자동 실행 ========================================================================
	UFUNCTION(BlueprintCallable, Category = "Python")
	void ExecuteWallPaperPython();

};
