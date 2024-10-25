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
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;
	FVector Direction; 

	// 인풋 동작 ===============================================================================================
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
	void RequestMapTravel(const FString& MapPath);
	UFUNCTION(Server, Reliable)
	//void ServerRPC_RequestMapTravel(const FString& MapPath);
	void ServerRPC_RequestMapTravel(const FString& MapPath, APlayerController* RequestingPC);


	// 로비 입장 시 초기 설정 ============================================================================
	void InitializeFromJson(const FString& LocalJsonData);
	/* 재질 색상 */
	FLinearColor InitColorValue; // RGB 값을 저장하는 변수 (생성 시 초기화에 사용)
	void SetInitColorValue(float r, float g, float b);
	/* 추천방 정보 */
	UPROPERTY()
	class ACJS_MultiRoomActor* RefMultiRoom;
	//void SetInitMultiRoomInfo(int32 CurNumPlayer, int32 MaxNumPlayer, const FString& RoomName, float Percent);
	TArray<ACJS_MultiRoomActor*> MultiRoomActors;
	void SetInitMultiRoomInfo(ACJS_MultiRoomActor* MultiRoomActor, int32 CurNumPlayer, int32 MaxNumPlayer, const FString& RoomName, float Percent);


	// <----- 경원아 아래 JsonData 변수에 값이 할당될 수 있도록 InitJsonData()를 호촐하면 돼!!!
	//FString JsonData = TEXT("{\"UserId\":\"1\",\"R\":1.0,\"G\":0.9225690792809692,\"B\":0.4,\"SimilarUsers\":[{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sunny World\"},{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sol World\"},{\"UserId\":\"abc11\",\"EmotionScore\":81.0,\"RoomName\":\"KW World\"}],\"OppositeUsers\":[{\"UserId\":\"user_1\",\"EmotionScore\":283.5,\"RoomName\":\"JW World\"},{\"UserId\":\"user_3\",\"EmotionScore\":321.0,\"RoomName\":\"DL World\"}]}");
	//FString Json = TEXT("{\"UserId\":\"1\",\"R\":1.0,\"G\":0.9225690792809692,\"B\":0.4,\"SimilarUsers\":[{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sunny World\"},{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sol World\"},{\"UserId\":\"abc11\",\"EmotionScore\":81.0,\"RoomName\":\"KW World\"}],\"OppositeUsers\":[{\"UserId\":\"user_1\",\"EmotionScore\":283.5,\"RoomName\":\"JW World\"},{\"UserId\":\"user_3\",\"EmotionScore\":321.0,\"RoomName\":\"DL World\"}]}");
	FString JsonData;
	void InitJsonData(FString LocalJsonData);
};
