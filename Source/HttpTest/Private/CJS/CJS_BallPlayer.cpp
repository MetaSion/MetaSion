﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_BallPlayer.h"
#include "CJS/CJS_HeartActor.h"
#include "CJS/CJS_BallPlayerAnimInstance.h"
#include "CJS/CJS_AimPointWidget.h"
#include "CJS/CJS_MultiRoomActor.h"
#include "CJS/CJS_HttpActor.h"
#include "CJS/SessionGameInstance.h"
#include "CJS/CJS_UltraDynamicSkyActor.h"
#include "CJS/CJS_LobbyWidget.h"
#include "CJS/CJS_RefRoomInfoWidget.h"

#include "HttpActor.h"
#include "JsonParseLib.h"
#include "JS_CreateRoomWidget.h"
#include "JS_RoomController.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"
#include "Components/ArrowComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#include "JsonUtilities.h" // JSON 관련 유틸리티
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
//#include "../../../../Plugins/Experimental/PythonScriptPlugin/Source/PythonScriptPlugin/Public/IPythonScriptPlugin.h"  // 파이썬 자동 실행

#include "Math/Color.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"





// Sets default values
ACJS_BallPlayer::ACJS_BallPlayer() : Super()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	//SpringArmComp->SetupAttachment(RootComponent);
	//SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->SetupAttachment(RootComponent);
	//SpringArmComp->SetRelativeLocation(FVector(0, 0, 0));
//  	SpringArmComp->TargetArmLength = 3000.f;
//  	SpringArmComp->SocketOffset = FVector(0.f, 0.f, 100.0f);
//  	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	//CameraComp->bUsePawnControlRotation = true;
//  	CameraComp->bUsePawnControlRotation = false;

	NiagraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	NiagraComp->SetupAttachment(RootComponent);
	// 컨트롤러 회전 사용 설정
//  	bUseControllerRotationYaw = true;
//  	// 캐릭터가 이동 방향을 따르지 않도록 설정
//  	GetCharacterMovement()->bOrientRotationToMovement = false;

	// 설정할 하트의 초기 위치를 위한 위치 값 (직접 값을 조정 가능)
	HeartSpawnPosition = FVector(300.f, 0.f, 50.f); // 적당히 초기 위치 오프셋 지정 

	// 멀티 플레이 적용
	bReplicates = true; // 네트워크 복제를 설정
	SetReplicateMovement(true); // 이동 복제를 설정


	// 초기 설정 ================================================================================
	/* 재질 색상 설정 */
	//SetInitColorValue(1.0, 0.9225690792809692, 0.4);
	//InitColorValue = FLinearColor(0.1, 1.0, 0.7);
	/* 추천방 정보 설정 */
	//SetInitMultiRoomInfo(1, 5, "빛나는 호수", 87);
	// 월드에서 MultiRoomActor 클래스의 인스턴스를 찾습니다.

	//InitJsonData(Json);  //<-- 테스트 시 (통신 x)
}



// Called when the game starts or when spawned
void ACJS_BallPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 오로라 색상 변경
	//ModifyAuroraColors();

	// SessionGameInstance 할당  // <--- 통신 적용 시 (생성자의 InitJsonData() 주석 처리하기)
	SessionGI = Cast<USessionGameInstance>(GetGameInstance());
	if (SessionGI)
	{
		UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance is set"));
		//InitJsonData(SessionGI->GetNetInfoCharacterTOLobby());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("USessionGameInstance is not set"));
	}

	// Initialize from JSON data
	//InitializeFromJson(JsonData);
	InitializeFromJson();

	// Material 설정 부분 추가 (SkeletalMesh 사용)
	if (GetMesh()) // SkeletalMeshComponent 접근
	{
		UMaterialInterface* MaterialInterface = GetMesh()->GetMaterial(0);
		if (MaterialInterface)
		{
			// Dynamic Material Instance 생성
			//UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this);
			DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInterface, this);
			if (DynamicMaterialInstance)
			{
				GetMesh()->SetMaterial(0, DynamicMaterialInstance);

				// InitColorValue를 사용하여 TransmitB 파라미터 설정
				//DynamicMaterialInstance->SetVectorParameterValue(FName("TransmitB"), InitColorValue);
				DynamicMaterialInstance->SetVectorParameterValue(FName("InnerColor"), InitColorValue);
				UE_LOG(LogTemp, Warning, TEXT("SkeletalMesh Material color set using 'InnerColor' parameter to R: %f, G: %f, B: %f"), InitColorValue.R, InitColorValue.G, InitColorValue.B);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create Dynamic Material Instance."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MaterialInterface is null."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SkeletalMeshComponent (GetMesh()) is null."));
	}
	// 초기 회전값을 Identity로 설정
	//MaterialRotationQuat = FQuat::Identity;
	//TargetYaw = 0.0f; // 초기 목표 Yaw 값


	// 추천방 정보 초기화
	//SetInitMultiRoomInfo(1, 5, "SunnyWorld", 87);
	//AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACJS_MultiRoomActor::StaticClass());
	//if (FoundActor)
	//{
	//	// ACJS_MultiRoomActor로 캐스팅
	//	RefMultiRoom = Cast<ACJS_MultiRoomActor>(FoundActor);
	//	if (RefMultiRoom)
	//	{
	//		// 초기 설정 함수 호출
	//		RefMultiRoom->InitRefRoomInfoWidget(1, 5, "SunnyWorld", 87);
	//		UE_LOG(LogTemp, Warning, TEXT("MultiRoom information initialized successfully."));
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("Failed to cast FoundActor to ACJS_MultiRoomActor."));
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("No ACJS_MultiRoomActor found in the world."));
	//}


	// 컨트롤러를 가져와서 캐스팅
	PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay()::PlayerController (pc) is assigned in BeginPlay"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay()::PlayerController (pc) is null in BeginPlay"));
		return;
	}


	// 애니메이션 시퀀스가 제대로 로드되었는지 확인 (1개씩 적용)
	/*if (TestAnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay() - TestAnimSequence loaded successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay() - TestAnimSequence is not set"));
	}*/

	// 애니메이션 시퀀스 배열이 제대로 설정되었는지 확인
	if (AnimSequences.Num() == 8)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay() - AnimSequences initialized with 8 elements"));
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay() - AnimSequences does not have 8 elements"));
	}

	// WBP_AimPoint 위젯 생성
	if (WBP_AimPoint)  // WBP_aimpoint가 올바르게 할당되어 있는지 확인
	{
		AimPointUI = CreateWidget<UCJS_AimPointWidget>(GetWorld(), WBP_AimPoint);
		if (AimPointUI)
		{
			AimPointUI->AddToViewport();
			AimPointUI->SetVisibility(ESlateVisibility::Hidden);
			//UE_LOG(LogTemp, Warning, TEXT("AimPointUI successfully created and added to viewport & Hidden right now"));
		}
		else
		{
			//UE_LOG(LogTemp, Error, TEXT("Failed to create AimPointUI Widget"));
		}
		//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay()::WBP_AimPoint is assigned!"));
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay()::WBP_AimPoint is not assigned! Please assign it in the Blueprint."));
	}

	bAimPointUIShowing = false;

	// WBP_LobbyWidget 생성
	if (LobbyUIFactory)  // 
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay() LobbyUIFactory exsited"));
		LobbyUI = CreateWidget<UCJS_LobbyWidget>(GetWorld(), LobbyUIFactory);
		if (LobbyUI)
		{
			LobbyUI->AddToViewport();
			LobbyUI->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay() LobbyUI successfully created and added to viewport & Hidden right now"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create LobbyUI Widget"));
		}
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay()::LobbyUI is assigned!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay()::LobbyUI is not assigned! Please assign it in the Blueprint."));
	}
	// WBP_LobbyWidget 생성
	if (RefRoomUIFactory)  
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay() RefRoomUIFactory exsited"));
		RefRoomUI = CreateWidget<UCJS_RefRoomInfoWidget>(GetWorld(), RefRoomUIFactory);
		if (RefRoomUI)
		{
			RefRoomUI->AddToViewport();
			RefRoomUI->SetVisibility(ESlateVisibility::Hidden);
			//RefRoomUI->SetVisibility(ESlateVisibility::Visible);
			UE_LOG(LogTemp, Log, TEXT("RefRoomUI Visibility: %d"), (int32)RefRoomUI->GetVisibility());
			RefRoomUI->AddToViewport(999);
			//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay() RefRoomUI successfully created and added to viewport & Hidden right now"));
			
			// FWidgetTransform에서 Translation 값 추출
			FVector2D Translation = RefRoomUI->RenderTransform.Translation;
			FVector2D Scale = RefRoomUI->RenderTransform.Scale;

			// 디버깅 로그 추가
			UE_LOG(LogTemp, Log, TEXT("Translation: %s"), *Translation.ToString());
			UE_LOG(LogTemp, Log, TEXT("Scale: %s"), *Scale.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create RefRoomUI Widget"));
		}
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay()::RefRoomUI is assigned!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay()::RefRoomUIFactory is not assigned! Please assign it in the Blueprint."));
	}

	// HttpActor 초기화 시도
	HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
	if (HttpActor == nullptr)
	{
		// HttpActor를 찾지 못한 경우, 새로 생성
		FActorSpawnParameters SpawnParams;
		HttpActor = GetWorld()->SpawnActor<AHttpActor>(AHttpActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (HttpActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("HttpActor spawned successfully."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn HttpActor."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HttpActor found and cast successfully."));
	}

	/*USessionGameInstance* sgi = Cast<USessionGameInstance>(GetGameInstance());
	sgi->AssignSessionNameFromPlayerState();*/

	// 물리 시뮬레이션 활성화
// 	if (GetMesh())
// 	{
// 		// 물리 시뮬레이션 및 중력 활성화
// 		GetMesh()->SetSimulatePhysics(true);
// 		GetMesh()->SetEnableGravity(true);
// 		
// 		// 축 고정 해제
// 		/*GetMesh()->BodyInstance.bLockXRotation = false;
// 		GetMesh()->BodyInstance.bLockYRotation = false;
// 		GetMesh()->BodyInstance.bLockZRotation = false;
// 		GetMesh()->BodyInstance.bLockXTranslation = false;
// 		GetMesh()->BodyInstance.bLockYTranslation = false;
// 		GetMesh()->BodyInstance.bLockZTranslation = false;*/
// 
// 		// 움직임 멈춤을 위해 물리 속도 감쇠 적용
// 		GetMesh()->SetLinearDamping(3.0f); // 값이 높을수록 빠르게 감속
// 		GetMesh()->SetAngularDamping(3.0f); // 값이 높을수록 회전 감속이 빠름
// 	
// 	}
// 
// 	// 기본 이동 힘 설정
// 	MoveForce = 150.0f;
	FString UserMusic = TEXT("Music_12");
	FString AssetPath = FString::Printf(TEXT("/Game/Main/Sound/%s.%s"), *UserMusic, *UserMusic);

	// ���� ������ �������� �ε�
	USoundBase* NewSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, *AssetPath));

	if (NewSound)
	{
		SessionGI->FadeOutAndPlayNewMusic(NewSound);
		UE_LOG(LogTemp, Warning, TEXT("Background sound changed and started playing: %s"), *UserMusic);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load sound: %s"), *AssetPath);
	}



}

// Called every frame
void ACJS_BallPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 	// 카메라 Yaw를 따라 캐릭터 Yaw 회전을 업데이트
	// 	if (Controller)
	// 	{
	// 		FRotator NewRotation = GetActorRotation();
	// 		NewRotation.Yaw = Controller->GetControlRotation().Yaw;
	// 		SetActorRotation(NewRotation);
	// 	}

		// 디버그 로그로 Direction 확인
		//UE_LOG(LogTemp, Warning, TEXT("Direction: X=%f, Y=%f, Z=%f"), Direction.X, Direction.Y, Direction.Z);

	// 	// 방향에 힘을 적용하여 이동
	// 	if (!Direction.IsNearlyZero())
	// 	{
	// 		FVector Force = Direction * MoveForce;
	// 		//GetMesh()->AddForce(Force, NAME_None, true);
	// 		GetMesh()->AddImpulse(Force, NAME_None, true);
	// 
	// 		// 방향을 리셋
	// 		Direction = FVector::ZeroVector;
		//}

	if (!bShowRefRoomInfoUI)
	{
		// MultiRoomActors 배열을 사용해 가장 가까운 방을 찾음
		ACJS_MultiRoomActor* NearestRoom = nullptr;
		float NearestDistance = FLT_MAX;
		for (ACJS_MultiRoomActor* RoomActor : MultiRoomActors)
		{
			if (RoomActor)
			{
				float Distance = FVector::Dist(this->GetActorLocation(), RoomActor->GetActorLocation());
				//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::Tick() Distance to %s: %f"), *RoomActor->GetName(), Distance);

				if (Distance < NearestDistance)
				{
					NearestDistance = Distance;
					NearestRoom = RoomActor;
				}
			}
		}
		// 가까운 방이 ActivationDistance 내에 있으면 UI를 보여줌
		if (NearestRoom && NearestDistance <= ActivationDistance)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Nearest room is within activation distance (%f <= %f)."), NearestDistance, ActivationDistance);

			if (ClosestRoom != NearestRoom) // 가장 가까운 방이 바뀌었을 때만 처리
			{
				//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::Tick() ClosestRoom changed to: %s"), *NearestRoom->GetName());
				ClosestRoom = NearestRoom;
				SetRefRoomInfo(ClosestRoom); // 방 정보를 UI에 반영
			}
		}
		else
		{
			if (ClosestRoom) // 멀어졌다면 UI를 비활성화
			{
				//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::Tick() Player is out of activation distance. Clearing ClosestRoom."));
				//ClosestRoom = nullptr;
				// HideRoomUI();
			}
		}
	}
	
}


// Called to bind functionality to input  =========================================================================================================================================
void ACJS_BallPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetupPlayerInputComponent"));
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(IMC_Player, 0);
		}
	}

	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (input)
	{
		// 		// 이동
		// 		input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ACJS_BallPlayer::OnMyActionMove);
		// 		// 방향
		// 		input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ACJS_BallPlayer::OnMyActionLook);
		// 		// 점프
		// 		input->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionJump);
				// 던지기
		input->BindAction(IA_Throw, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionThrow);
		// 클릭
		input->BindAction(IA_Click, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionClick);
		// 토글 조준점
		input->BindAction(IA_AimPoint, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionToggleAimPointUI);
		// 게임 종료
		input->BindAction(IA_QuitGame, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionQuitGame);
		// 체험 UI
		input->BindAction(IA_InnerWorldUI, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionShowInnerWorldUI);
		// 로비 UI
		input->BindAction(IA_LobbyUI, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionLobbyUI);
		// 체험방 이동
		input->BindAction(IA_MoveRefWorld, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionMoveRefWorld);

		
		// 숫자키 애니메이션 (인덱스 사용해 바인딩)
		for (int32 i = 0; i < 8; i++)
		{
			if (IA_NumKeys[i])
			{
				// 로그 출력 추가
				//UE_LOG(LogTemp, Warning, TEXT("Binding action for Key %d with Index %d"), i, i);
				input->BindAction(IA_NumKeys[i], ETriggerEvent::Started, this, &ACJS_BallPlayer::OnNumberKeyPressed, i);
			}
			else
			{
				// 로그 출력: 인풋 액션이 null일 경우
				//UE_LOG(LogTemp, Error, TEXT("IA_NumKeys[%d] is null"), i);
			}
		}
		// Log to check if input actions are bound
		//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetupPlayerInputComponent() - Input actions are bound"));
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::SetupPlayerInputComponent():: EnhancedInputComponent is null"));
	}
}

// void ACJS_BallPlayer::OnMyActionMove(const FInputActionValue& Value)
// {
// 	//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionMove()"));
// 
// 	FVector2D v = Value.Get<FVector2D>();
// 
// 	// 카메라의 Yaw를 기준으로 방향을 변환
// 	FRotator CameraYawRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
// 	FVector ForwardDirection = FRotationMatrix(CameraYawRotation).GetUnitAxis(EAxis::X) * v.X;
// 	FVector RightDirection = FRotationMatrix(CameraYawRotation).GetUnitAxis(EAxis::Y) * v.Y;
// 
// 	// 이동 방향을 카메라 기준으로 설정
// 	Direction = (ForwardDirection + RightDirection).GetSafeNormal();
// 
// 	//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionMove():: Move Direction: X=%f, Y=%f"), Direction.X, Direction.Y);
// }
// void ACJS_BallPlayer::OnMyActionLook(const FInputActionValue& Value)
// {
// 	//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionLook()"));
// 
// 	FVector2D v = Value.Get<FVector2D>();
// 
// 	// Log to check if the look input is being received
// 	//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionLook():: Look Direction: X=%f, Y=%f"), v.X, v.Y);
// 
// 	AddControllerPitchInput(-v.Y);
// 	AddControllerYawInput(v.X);
// }
// void ACJS_BallPlayer::OnMyActionJump(const FInputActionValue& Value)
// {
// 	Jump();
// 	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionJump() - Jump Triggered"));
// }
void ACJS_BallPlayer::OnMyActionThrow(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionThrow()"));
	if (HasAuthority())
	{
		// 서버에서 직접 하트를 던지기
		ServerRPC_ThrowHeart();
	}
	else
	{
		// 클라이언트에서 서버로 RPC 호출
		ServerRPC_ThrowHeart();
	}
}

void ACJS_BallPlayer::OnMyActionClick(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionClick()"));
	if (!bAimPointUIShowing)
	{
		// AimPointUI가 표시되지 않았을 때는 클릭을 무시
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionClick()::bAimPointUIShowing==false return"));
		return;
	}

	FVector Start = CameraComp->GetComponentLocation();
	FVector End = Start + CameraComp->GetForwardVector() * 100000.0f;
	FHitResult Outhit;
	ECollisionChannel TraceChannel = ECC_Visibility;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Outhit, Start, End, TraceChannel, CollisionParams);

	if (bHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit something!"));

		if (Outhit.Component.IsValid())
		{
			FString HitComponentName = Outhit.Component->GetName();
			//UE_LOG(LogTemp, Warning, TEXT("Hit Component: %s"), *HitComponentName);
		}

		AActor* HitActor = Outhit.GetActor();
		if (HitActor)
		{
			FString HitActorName = HitActor->GetName();
			//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActorName);

			if (HitActorName.Contains("MultiRoom"))
			{
				//UE_LOG(LogTemp, Warning, TEXT("BP_CJS_MultiRoom Clicked"));
				if (PC)
				{
					//RequestMoveMultiRoom(PC);

					// GameInstance에서 MySessionName 값을 가져옴
					FString UserId;
					int32 ActorIndex;
					FString RoomOwner;
					USessionGameInstance* GameInstance = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
					if (GameInstance)
					{
						UserId = GameInstance->MySessionName;
						//UE_LOG(LogTemp, Warning, TEXT("Assigned UserId from MySessionName: %s"), *UserId);
					}

					// MultiRoomActors의 인덱스를 찾기
					ActorIndex = MultiRoomActors.IndexOfByKey(HitActor);
					if (ActorIndex != INDEX_NONE && AllUsersArray.IsValidIndex(ActorIndex))
					{
						// AllUsersArray에서 UserObject를 가져와 정보 추출
						TSharedPtr<FJsonObject> UserObject = AllUsersArray[ActorIndex]->AsObject();
						if (UserObject.IsValid())
						{
							RoomOwner = UserObject->GetStringField(TEXT("UserId"));
							ClickedRoomNum = UserObject->GetStringField(TEXT("roomNum"));
							//ClickedRoomNum = "3";  // <-- 미리 방 정보 추가로 다 가지고 있어야 함.
							//UE_LOG(LogTemp, Warning, TEXT("MultiRoomActor Owner UserId: %s, RoomNum: %s"), *RoomOwner, *ClickedRoomNum);
							//UE_LOG(LogTemp, Warning, TEXT("MultiRoomActor RoomOwner: %s, RoomNum: %s"), *RoomOwner, *ClickedRoomNum);
						}
						else
						{
							//UE_LOG(LogTemp, Error, TEXT("Failed to retrieve UserObject from AllUsersArray"));
						}
					}
					else
					{
						//UE_LOG(LogTemp, Error, TEXT("Could not find the MultiRoomActor in MultiRoomActors or invalid index in AllUsersArray"));
					}

					// 사용자 데이터를 맵에 추가
					TMap<FString, FString> MultiRoomData;

					//MyRoomData.Add("userId", UserId);
					//MultiRoomData.Add("userId", UserId);

					MultiRoomData.Add("room_num", ClickedRoomNum);

					// JSON 형식으로 변환
					FString json = UJsonParseLib::MakeJson(MultiRoomData);

					// 로그 출력 (디버깅용)
					//UE_LOG(LogTemp, Warning, TEXT("MakeJson() Ok!!!!"));
					//UE_LOG(LogTemp, Warning, TEXT("userId: %s, room_num: %s"), *UserId, *ClickedRoomNum);
					//UE_LOG(LogTemp, Warning, TEXT("json: %s"), *json);

					HttpActor->ReqPostClickMultiRoom(MultiRoomURL, json);
					//UE_LOG(LogTemp, Warning, TEXT("----------  post reqeust done"));
				}
				else
				{
					//UE_LOG(LogTemp, Error, TEXT("PlayerController is nullptr. Cannt Move to the MultiRoomMap"));
				}
			}
			else if (HitActorName.Contains("MyRoom"))
			{
				//UE_LOG(LogTemp, Warning, TEXT("BP_CJS_MyRoom Clicked"));
				if (PC)
				{
					//JS ReWrite 이쪽에 방 데이터 송수신 하는 부분 넣고 수신 하는 부분에서 방 이동
					FString UserId;
					USessionGameInstance* GameInstance = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
					if (GameInstance)
					{
						//UserId = GameInstance->MySessionName;
						UserId = "testuser";
						//UE_LOG(LogTemp, Warning, TEXT("Assigned UserId from MySessionName: %s"), *UserId);
					}

					// 사용자 데이터를 맵에 추가
					TMap<FString, FString> MyRoomData;
					MyRoomData.Add("userId", UserId);

					// JSON 형식으로 변환
					FString JsonRequest = UJsonParseLib::MakeJson(MyRoomData);

					// 로그 출력 (디버깅용)
					//UE_LOG(LogTemp, Warning, TEXT("userId: %s"), *UserId);
					//UE_LOG(LogTemp, Warning, TEXT("Json Request: %s"), *JsonRequest);

					// 서버로 요청 전송
					HttpActor->ReqPostClickMyRoom(HttpActor->MyRoomURL, JsonRequest);
				}
				else
				{
					//UE_LOG(LogTemp, Error, TEXT("PlayerController is nullptr. Cannt Move to the MyRoomMap"));
				}
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionClick()::Hit Actor is NULL"));
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionClick()::No Hit Detected"));
	}

}

void ACJS_BallPlayer::OnMyActionToggleAimPointUI(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionToggleAimPointUI()"));
	if (AimPointUI)
	{
		if (!bAimPointUIShowing)
		{
			bAimPointUIShowing = true;
			AimPointUI->SetVisibility(ESlateVisibility::Visible);
			//UE_LOG(LogTemp, Warning, TEXT("AimPointUI is now Visible, Visibility State: %d"), (int32)AimPointUI->GetVisibility());
		}
		else
		{
			bAimPointUIShowing = false;
			AimPointUI->SetVisibility(ESlateVisibility::Hidden);
			//UE_LOG(LogTemp, Warning, TEXT("AimPointUI is now Hidden, Visibility State: %d"), (int32)AimPointUI->GetVisibility());
		}
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("AimPointUI is null! UI cannot be toggled."));
	}
}

void ACJS_BallPlayer::OnMyActionQuitGame(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionQuitGame()"));
	if (GEngine) // Check if GEngine is valid
	{
		// Display a log message (optional)
		//UE_LOG(LogTemp, Warning, TEXT("ESC key pressed. Exiting game..."));

		// Quit the game
		FGenericPlatformMisc::RequestExit(false);
	}
}

void ACJS_BallPlayer::OnMyActionShowInnerWorldUI(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionShowInnerWorldUI()"));

	if (!bMultiInnerWorldUIOn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionShowInnerWorldUI() !bMultiInnerWorldUIOn : %d"), bMultiInnerWorldUIOn);
		if (!SessionGI)
		{
			UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::OnMyActionShowInnerWorldUI() SessionGI is null"));
			return;
		}

		/*if (SessionGI->GetbRefRoomUIMultiOn())
		{
			UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionShowInnerWorldUI() SessionGI->GetbRefRoomUIMultiOn() : %d"), SessionGI->GetbRefRoomUIMultiOn());
			return;
		}
		else
		{*/
		if (SessionGI)
		{
			SessionGI->SetbRefRoomUIMultiOn(true);
			if (SessionGI->GetbRefRoomUIMultiOn())
			{
				UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionShowInnerWorldUI() After SessionGI->SetbRefRoomUIMultiOn() : %d"), SessionGI->GetbRefRoomUIMultiOn());
				auto* myPC = Cast<AJS_RoomController>(Controller);
				if (myPC && myPC->CR_UIFactory)
				{
					UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionShowInnerWorldUI() CR_UIFactory exsited"));
					if (!myPC->CR_UI)
					{
						UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionShowInnerWorldUI() not exsited InnerWorldUI"));

						//auto* myPC = Cast<AJS_RoomController>(Controller);
						myPC->CR_UI = CreateWidget<UJS_CreateRoomWidget>(myPC, myPC->CR_UIFactory);
						if (myPC && myPC->CR_UI)
						{
							UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionShowInnerWorldUI() AJS_RoomController Set"));
							CR_UI = myPC->CR_UI;
							CR_UI->SetVisibility(ESlateVisibility::Visible);
							CR_UI->DelayedSwitchToWidget();
							UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionShowInnerWorldUI() CR_UI assigned from RoomController"));
						}
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT(" ACJS_BallPlayer::OnMyActionShowInnerWorldUI() already exsited InnerWorldUI"));
						CR_UI = myPC->CR_UI;
						CR_UI->SetVisibility(ESlateVisibility::Visible);
						CR_UI->DelayedSwitchToWidget();
						UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionShowInnerWorldUI() InnerWorldUI set Visible"));
					}

					bMultiInnerWorldUIOn = true;
					UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionShowInnerWorldUI() Set bMultiInnerWorldUIOn : %d"), bMultiInnerWorldUIOn);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::OnMyActionShowInnerWorldUI() MyPC No CR_UIFactory"));
					return;
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::OnMyActionShowInnerWorldUI()SessionGI->GetbRefRoomUIMultiOn() true!! "));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::OnMyActionShowInnerWorldUI()SessionGI Null!! "));
		}
		//}
	}
	else
	{
		CR_UI->SetVisibility(ESlateVisibility::Hidden);
		bMultiInnerWorldUIOn = false;
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionShowInnerWorldUI() Set bMultiInnerWorldUIOn : %d"), bMultiInnerWorldUIOn);
	}
}

void ACJS_BallPlayer::OnMyActionLobbyUI(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionLobbyUI()"));
	if (!bShowLobbyUI)
	{
		//LobbyUI->SetVisibility(ESlateVisibility::Visible);
		LobbyUI->ShowLobbyUIFirstOrder();
		if(PC)
		{
			UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionLobbyUI() PC exsited"));
			PC->SetIgnoreLookInput(true); // 카메라 회전에 대한 마우스 입력 비활성화
			PC->bShowMouseCursor = true;
			PC->bEnableClickEvents = true;
			PC->bEnableMouseOverEvents = true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::OnMyActionLobbyUI() NO PC"));
		}
		bShowLobbyUI = true;
	}
	else
	{
		//LobbyUI->SetVisibility(ESlateVisibility::Hidden);
		LobbyUI->HideLobbyUIFirstOrder();
		if (PC)
		{
			UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionLobbyUI() PC exsited"));
			PC->SetIgnoreLookInput(false); // 카메라 회전에 대한 마우스 입력 비활성화
			PC->bShowMouseCursor = false;
			PC->bEnableClickEvents = false;
			PC->bEnableMouseOverEvents = false;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::OnMyActionLobbyUI() NO PC"));
		}
		bShowLobbyUI = false;
	}
}

// 체험방 이동
void ACJS_BallPlayer::OnMyActionMoveRefWorld(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionMoveRefWorld()"));
	FTimerHandle MoveRefWorldHandler;
	if (GetWorld()) 
	{
		if (PC)
		{
			FTimerDelegate TimerDel;
			TimerDel.BindUObject(this, &ACJS_BallPlayer::RequestMoveMultiRoom, PC); // PC를 인수로 바인딩

			GetWorld()->GetTimerManager().SetTimer(
				MoveRefWorldHandler,
				TimerDel,                         // 바인딩된 델리게이트
				1.0f,                             // 대기 시간
				false                             // 반복 여부
			);

			
				UE_LOG(LogTemp, Warning, TEXT("AHttpActor::SetBackgroundSound() SessionGI  OK"));
				// Get UserMusic from WorldSetting and play it
				FString UserMusic = TEXT("Music_02");
				FString AssetPath = FString::Printf(TEXT("/Game/Main/Sound/%s.%s"), *UserMusic, *UserMusic);

				// ���� ������ �������� �ε�
				USoundBase* NewSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, *AssetPath));

				if (NewSound)
				{
					SessionGI->FadeOutAndPlayNewMusic(NewSound);
					UE_LOG(LogTemp, Warning, TEXT("Background sound changed and started playing: %s"), *UserMusic);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to load sound: %s"), *AssetPath);
				}

				//         SoundActor->SetBackgroundSoundByFileName(UserMusic);
			

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PC is null!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() returned NULL!"));
	}
}

// 체험방 UI 멀티플레이 적용 시
void ACJS_BallPlayer::ServerRPC_Chat_Implementation(const FString& msg)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::ServerRPC_Chat_Implementation()"));
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::ServerRPC_Chat_Implementation() I am the server!"));
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::ServerRPC_Chat() - Server received message: %s"), *FString(msg));
		// 서버에서 멀티캐스트 호출
		MulticastRPC_Chat(msg);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::ServerRPC_Chat_Implementation() I am a client!"));
	}
}
void ACJS_BallPlayer::MulticastRPC_Chat_Implementation(const FString& msg)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::MulticastRPC_Chat_Implementation()"));

	//if (!CR_UI && CR_UIFactory)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::MulticastRPC_Chat_Implementation() CR_UI is null, cannot add chat message"));
	//	// 클라이언트에서 CR_UI가 생성되지 않았으면 새로 생성
	//	CR_UI = CreateWidget<UJS_CreateRoomWidget>(GetWorld(), CR_UIFactory);
	//	if (CR_UI)
	//	{
	//		CR_UI->AddToViewport();
	//		CR_UI->SetVisibility(ESlateVisibility::Visible);
	//		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::MulticastRPC_Chat_Implementation() CR_UI created and visible"));
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::MulticastRPC_Chat_Implementation() Failed to create CR_UI"));
	//	}
	//}

	if (!CR_UI) // 타인
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::MulticastRPC_Chat_Implementation() CR_UI is null, cannot add chat message"));
		
		APlayerController* player = GetWorld()->GetFirstPlayerController();
		auto* MyPC = Cast<AJS_RoomController>(player);
		if (MyPC && MyPC->CR_UI)
		{
			UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::MulticastRPC_Chat_Implementation() AJS_RoomController Set"));
			CR_UI = MyPC->CR_UI;
			UE_LOG(LogTemp, Warning, TEXT("CR_UI assigned from RoomController in MulticastRPC_Chat_Implementation"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to assign CR_UI from RoomController in MulticastRPC_Chat_Implementation"));
		}
	}

	if (CR_UI) // 본인
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::MulticastRPC_Chat_Implementation() CR_UI exists, adding chat message"));
		CR_UI->AddChatMessage(msg);
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::MulticastRPC_Chat_Implementation() AddChatMessage executed"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::MulticastRPC_Chat_Implementation() CR_UI is null, cannot add chat message"));
	}
}


//void ACJS_BallPlayer::SetMultiRoomInfo(int32 roomIndex)
void ACJS_BallPlayer::SetRefRoomInfo(ACJS_MultiRoomActor* roomActor)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetMultiRoomInfo()"));
	if (!bShowRefRoomInfoUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetMultiRoomInfo() !bShowRefRoomInfoUI %d"), bShowRefRoomInfoUI);
		bShowRefRoomInfoUI = true;
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetMultiRoomInfo() set bShowRefRoomInfoUI %d"), bShowRefRoomInfoUI);
		//if (MultiRoomActors.IsValidIndex(roomIndex))
		if (roomActor)
		{
			//ACJS_MultiRoomActor* RoomActor = MultiRoomActors[roomIndex];
			FString name = roomActor->RoomInfo.room_name;
			FString owner = roomActor->RoomInfo.room_id;
			FString description = roomActor->RoomInfo.roomdescription;
			FString percent = roomActor->RoomInfo.percent_message;
			FString reason = roomActor->RoomInfo.reason_message;
			UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetMultiRoomInfo() RoomActor name : %s, owner : %s, description : %s, percent : %s, reason : %s"), *name, *owner, *description, *percent, *reason);
			
			/*FString name = "hhhhh";  // 테스트용
			FString owner = "oooooo";
			FString description = "ppppppp";
			FString percent = "87.00 %";
			FString reason = "oooooooo";*/

			if (RefRoomUI)
			{
				//LobbyUI->SetVisibility(ESlateVisibility::Visible);
				RefRoomUI->SetRefWorldInfo(name, owner, description, percent, reason);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("RefRoomUI is null. Cannot update world info."));
			}
		}
		else
		{
			//UE_LOG(LogTemp, Error, TEXT("Invalid roomIndex: %d. MultiRoomActors array does not contain this index."), roomIndex);
			UE_LOG(LogTemp, Error, TEXT("No roomActor"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::SetMultiRoomInfo() bShowRefRoomInfoUI = true"));
	}
}

void ACJS_BallPlayer::OnNumberKeyPressed(const FInputActionValue& Value, int32 KeyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnNumberKeyPressed() - Key %d pressed"), KeyIndex + 1);
	//PlayAnimationByIndex(KeyIndex);

	// 서버에 애니메이션 재생을 요청합니다.
	if (HasAuthority())
	{
		MulticastRPC_PlayAnimation(KeyIndex); // 서버는 직접 멀티캐스트 실행
	}
	else
	{
		ServerRPC_PlayAnimation(KeyIndex); // 클라이언트는 서버에 요청
	}
}
void ACJS_BallPlayer::PlayAnimationByIndex(int32 Index)
{
	// 유효한 애니메이션 인덱스와 Mesh 여부 확인
	if (AnimSequences.IsValidIndex(Index) && AnimSequences[Index] && GetMesh())
	{
		// 물리적 속도 및 회전 초기화 (움직임을 멈추기 위함)
// 		GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);   // 선형 속도 초기화
// 		GetMesh()->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector); // 각속도 초기화

		// Actor와 메시의 전체 회전을 (0, 0, 0)으로 초기화
		SetActorRotation(FRotator::ZeroRotator);

		// 메시의 로컬 회전을 (0, 0, 0)으로 설정하여 Actor와 메시 회전을 동기화 (Teleport 플래그 사용)
		/*skeletalMeshComponent가 물리 시뮬레이션을 활성화한 상태에서 회전이나 위치를 강제로 설정하려면, Teleport 플래그를 사용해야 함.
		  ETeleportType::TeleportPhysics 플래그는 물리 시뮬레이션이 활성화된 메시에도 강제로 회전을 적용하도록 합*/
		if (GetMesh())
		{
			GetMesh()->SetRelativeRotation(FRotator::ZeroRotator, false, nullptr, ETeleportType::TeleportPhysics);
		}

		// 메시 컴포넌트의 실제 회전 값 가져오기
		//FRotator MeshRotation = GetMesh()->GetComponentRotation();
		// 메시의 Pitch, Yaw, Roll 각도 출력
		//UE_LOG(LogTemp, Warning, TEXT("Mesh Rotation - Pitch: %f, Yaw: %f, Roll: %f"), MeshRotation.Pitch, MeshRotation.Yaw, MeshRotation.Roll);

		// 볼 플레이어의 현재 회전 각도 가져오기
		//FRotator PlayerRotation = GetActorRotation();
		// 로그로 볼 플레이어의 Pitch, Yaw, Roll 각도 출력
		//UE_LOG(LogTemp, Warning, TEXT("Player Rotation - Pitch: %f, Yaw: %f, Roll: %f"), PlayerRotation.Pitch, PlayerRotation.Yaw, PlayerRotation.Roll);


		// 애니메이션 재생
		GetMesh()->PlayAnimation(AnimSequences[Index], false);
		//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::PlayAnimationByIndex() - Animation %d played"), Index + 1);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::PlayAnimationByIndex() - Invalid index or animation sequence"));
	}
}


// 공끼리 충돌 시  =========================================================================================================================================
void ACJS_BallPlayer::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Check if the hit actor is another BallPlayer
	if (Other && Other != this && Other->IsA(ACJS_BallPlayer::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::NotifyHit() - Collided with another BallPlayer"));

		// Activate particle effect at the hit location
		if (HitVFXFactory)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFXFactory, HitLocation);
		}
		if (HitSFX)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), HitSFX);
		}
		TriggerSelfHitEffects(HitLocation);
	}
}
void ACJS_BallPlayer::TriggerSelfHitEffects(FVector HitLocation)
{
	// Log to indicate collision effects triggered
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::TriggerCollisionEffects() - Triggered on collision"));

	// Activate particle effect at the hit location
	if (SelfHitVFXFactory)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SelfHitVFXFactory, HitLocation);
	}
	//// Play sound effect
	//if (HitSFX)
	//{
	//	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSFX, HitLocation);
	//}
}

// 숫자키 애니메이션 작동 (멀티)
void ACJS_BallPlayer::ServerRPC_PlayAnimation_Implementation(int32 AnimationIndex)
{
	// 서버에서 멀티캐스트 호출
	MulticastRPC_PlayAnimation(AnimationIndex);
}

bool ACJS_BallPlayer::ServerRPC_PlayAnimation_Validate(int32 AnimationIndex)
{
	return true;
}

void ACJS_BallPlayer::MulticastRPC_PlayAnimation_Implementation(int32 AnimationIndex)
{
	PlayAnimationByIndex(AnimationIndex);
}


// 하트 던지기 (멀티)
void ACJS_BallPlayer::ServerRPC_ThrowHeart_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::ServerRPC_ThrowHeart()"));
	// 서버에서만 하트 생성
	MulticastRPC_ThrowHeart(); // 모든 클라이언트에게 하트를 던지라고 브로드캐스트
}
bool ACJS_BallPlayer::ServerRPC_ThrowHeart_Validate()
{
	return true;
}
void ACJS_BallPlayer::MulticastRPC_ThrowHeart_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::MulticastRPC_ThrowHeart()"));
	if (HeartItemFactory)
	{
		// 일정한 방향으로 던지기
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * HeartSpawnPosition.X + GetActorUpVector() * HeartSpawnPosition.Z;
		FRotator SpawnRotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		ACJS_HeartActor* SpawnedHeart = GetWorld()->SpawnActor<ACJS_HeartActor>(HeartItemFactory, SpawnLocation, SpawnRotation, SpawnParams);

		if (SpawnedHeart && SpawnedHeart->ProjectileMovementComp)
		{
			FVector LaunchDirection = SpawnRotation.Vector();
			SpawnedHeart->ProjectileMovementComp->Velocity = LaunchDirection * SpawnedHeart->ProjectileMovementComp->InitialSpeed;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::Multicast_ThrowHeart() - ACJS_HeartActor is null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::Multicast_ThrowHeart() - HeartItemFactory is null"));
	}
}


// 로비 -> 체험방 이동 ======================================================================================================================
void ACJS_BallPlayer::RequestMoveMultiRoom(APlayerController* RequestingPC)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::RequestMoveMultiRoom()"));

	if (!RequestingPC)
	{
		UE_LOG(LogTemp, Error, TEXT("RequestMoveMultiRoom():: RequestingPC is null!"));
		return;
	}

	// 클라이언트에서 서버로 요청
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Requesting Server to move to MultiRoom from Client"));
		ServerRPC_RequestMoveMultiRoom(RequestingPC); // 서버로 요청
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Handling move to MultiRoom on Server"));
		MoveToMultiRoom(RequestingPC); // 서버에서 직접 처리
	}
}
void ACJS_BallPlayer::ServerRPC_RequestMoveMultiRoom_Implementation(APlayerController* RequestingPC)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::ServerRPC_RequestMoveMultiRoom_Implementation()"));

	if (!RequestingPC)
	{
		UE_LOG(LogTemp, Error, TEXT("ServerRPC_RequestMoveMultiRoom():: RequestingPC is null!"));
		return;
	}

	// 서버에서 클라이언트 요청 처리
	MoveToMultiRoom(RequestingPC);
}
void ACJS_BallPlayer::MoveToMultiRoom(APlayerController* RequestingPC)
{
	if (!RequestingPC)
	{
		UE_LOG(LogTemp, Error, TEXT("MoveToMultiRoom():: RequestingPC is null!"));
		return;
	}

	APawn* ControlledPawn = RequestingPC->GetPawn();
	if (ControlledPawn)
	{
		// 체험방 위치 정의 (Room3 및 Room4)
		FVector Room3Location(308907.221385f, 923351.405033f, -3730.988342f);
		//FVector Room4Location(1895545.290658f, 1285152.582352f, 520820.178077f);

		// 랜덤으로 위치 선택
		//FVector NewLocation = FMath::RandBool() ? Room3Location : Room4Location;

		// 선택된 위치로 이동
		//ControlledPawn->SetActorLocation(NewLocation);
		ControlledPawn->SetActorLocation(Room3Location);
		//UE_LOG(LogTemp, Warning, TEXT("MoveToMultiRoom():: ControlledPawn moved to location: %s"), *NewLocation.ToString());
		UE_LOG(LogTemp, Warning, TEXT("MoveToMultiRoom():: ControlledPawn moved to location: %s"), *Room3Location.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MoveToMultiRoom():: ControlledPawn is null!"));
	}

	// 마우스 커서 표시 및 이벤트 활성화
	RequestingPC->bShowMouseCursor = true;
	RequestingPC->bEnableClickEvents = true;
	RequestingPC->bEnableMouseOverEvents = true;
	RequestingPC->SetIgnoreLookInput(true); // 카메라 회전에 대한 마우스 입력 비활성화
	UE_LOG(LogTemp, Warning, TEXT("MoveToMultiRoom():: Mouse cursor and Look input ignore."));

	// BallPlayer를 Hidden으로 설정
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false); // 필요 시 충돌 비활성화
	UE_LOG(LogTemp, Warning, TEXT("MoveToMultiRoom():: ControlledPawn is now hidden."));
	bShowRefRoomInfoUI = true;

	// 새로운 카메라 액터 생성 (Room3 카메라 위치)
	FVector CameraLocation(308445.662792f, 922703.557885f, -3140.599543f); // 지정된 위치
	FRotator CameraRotation(0.0f, 120.0f, 0.0f); // 예제 회전값 (필요시 수정)
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; // BallPlayer를 소유자로 설정
	ACameraActor* NewCamera = GetWorld()->SpawnActor<ACameraActor>(CameraLocation, CameraRotation, SpawnParams);
	if (NewCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveToMultiRoom():: New camera created at location: %s"), *CameraLocation.ToString());

		// 새로운 카메라로 뷰 전환
		RequestingPC->SetViewTargetWithBlend(NewCamera, 0.5f); // 전환 시간: 0.5초 (필요시 조정 가능)
		UE_LOG(LogTemp, Warning, TEXT("MoveToMultiRoom():: View switched to new camera."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MoveToMultiRoom():: Failed to create new camera actor."));
	}
}

// 체험방 -> 로비 이동 =========================================================================================================
void ACJS_BallPlayer::RequestMoveLobby(APlayerController* RequestingPC)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::RequestMoveLobby()"));

	if (!RequestingPC)
	{
		UE_LOG(LogTemp, Error, TEXT("RequestMoveLobby():: RequestingPC is null!"));
		return;
	}

	// 클라이언트 요청 처리
	if (!HasAuthority()) // 클라이언트에서 호출
	{
		UE_LOG(LogTemp, Warning, TEXT("Requesting Server to move to Lobby from Client"));
		ServerRPC_RequestMoveLobby(RequestingPC); // 서버에 RPC 요청
	}
	else // 서버에서 호출
	{
		UE_LOG(LogTemp, Warning, TEXT("Handling move to Lobby on Server"));
		MoveToLobby(RequestingPC); // 직접 이동 처리
	}
}
void ACJS_BallPlayer::ServerRPC_RequestMoveLobby_Implementation(APlayerController* RequestingPC)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::ServerRPC_RequestMoveLobby_Implementation()"));

	if (!RequestingPC)
	{
		UE_LOG(LogTemp, Error, TEXT("ServerRPC_RequestMoveLobby():: RequestingPC is null!"));
		return;
	}

	// 서버에서 이동 처리
	MoveToLobby(RequestingPC);
}
void ACJS_BallPlayer::MoveToLobby(APlayerController* RequestingPC)
{
	if (!RequestingPC)
	{
		UE_LOG(LogTemp, Error, TEXT("MoveToLobby():: RequestingPC is null!"));
		return;
	}

	APawn* ControlledPawn = RequestingPC->GetPawn();
	if (ControlledPawn)
	{
		FVector NewLocation(19145.640926f, 4627.819710f, -871.678926f); // 이동 위치(로비 PlayerStart 지점)
		ControlledPawn->SetActorLocation(NewLocation);
		UE_LOG(LogTemp, Log, TEXT("ControlledPawn moved to Lobby at location: %s"), *NewLocation.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MoveToLobby():: ControlledPawn is null!"));
	}

	if (CR_UI)
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveToLobby():: CR_UI is exsited"));
		CR_UI->SetVisibility(ESlateVisibility::Hidden);
		bMultiInnerWorldUIOn = false;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MoveToLobby():: CR_UI is null!"));
	}

	if (SessionGI->GetbRefRoomUIMultiOn())
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::MoveToLobby() SessionGI->GetbRefRoomUIMultiOn() : %d"), SessionGI->GetbRefRoomUIMultiOn());
		SessionGI->SetbRefRoomUIMultiOn(false);
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::MoveToLobby() after SessionGI->GetbRefRoomUIMultiOn() : %d"), SessionGI->GetbRefRoomUIMultiOn());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::MoveToLobby() !SessionGI->bRefRoomUIMultiOn()"));
	}

	// 마우스 커서 표시 및 이벤트 비활성화
	RequestingPC->bShowMouseCursor = false;
	RequestingPC->bEnableClickEvents = false;
	RequestingPC->bEnableMouseOverEvents = false;
	RequestingPC->SetIgnoreLookInput(false); // 카메라 회전에 대한 마우스 입력 비활성화
	UE_LOG(LogTemp, Warning, TEXT("MoveToMultiRoom():: lookInput activate."));

	// BallPlayer를 Hidden으로 설정
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true); // 필요 시 충돌 비활성화
	UE_LOG(LogTemp, Warning, TEXT("MoveToMultiRoom():: ControlledPawn is now hidden."));
	bShowRefRoomInfoUI = false;

	// MultiRoom에서 생성된 카메라 액터 제거
	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		ACameraActor* CameraActor = *It;
		if (CameraActor && CameraActor->GetOwner() == this)
		{
			CameraActor->Destroy();
			UE_LOG(LogTemp, Warning, TEXT("MoveToLobby():: Removed camera actor owned by BallPlayer."));
		}
	}
	// 뷰를 다시 ControlledPawn으로 전환
	RequestingPC->SetViewTargetWithBlend(this, 0.5f);
	UE_LOG(LogTemp, Warning, TEXT("MoveToLobby():: View switched back to BallPlayer."));

	UE_LOG(LogTemp, Warning, TEXT("AHttpActor::SetBackgroundSound() SessionGI  OK"));
	// Get UserMusic from WorldSetting and play it
	FString UserMusic = TEXT("Music_12");
	FString AssetPath = FString::Printf(TEXT("/Game/Main/Sound/%s.%s"), *UserMusic, *UserMusic);

	// ���� ������ �������� �ε�
	USoundBase* NewSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, *AssetPath));

	if (NewSound)
	{
		SessionGI->FadeOutAndPlayNewMusic(NewSound);
		UE_LOG(LogTemp, Warning, TEXT("Background sound changed and started playing: %s"), *UserMusic);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load sound: %s"), *AssetPath);
	}

	//         SoundActor->SetBackgroundSoundByFileName(UserMusic);

}

// 로비 진입 시, 캐릭터 초기 설정 ================================================================================================
//void ACJS_BallPlayer::InitializeFromJson(const FString& LocalJsonData)
void ACJS_BallPlayer::InitializeFromJson()
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::InitializeFromJson()"));

	//// JSON 문자열을 JSON 객체로 파싱  // <--- 통신 필요 시 해제
	//TSharedPtr<FJsonObject> JsonObject;
	//TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(LocalJsonData);

	//if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	//{
	//	// 1. RGB 값 추출 및 SetInitColorValue 호출
	//	float R = JsonObject->GetNumberField(TEXT("R"));
	//	float G = JsonObject->GetNumberField(TEXT("G"));
	//	float B = JsonObject->GetNumberField(TEXT("B"));
	//	SetInitColorValue(R, G, B);

	// 1. RGB 값 추출 및 SetInitColorValue 호출
	if (SessionGI)
	{
		// WorldSetting에서 RGB 값 가져오기
		const FMyRGBColor& RGB = SessionGI->WorldSetting.RGB;
		// SetInitColorValue 호출
		SetInitColorValue(RGB.R, RGB.G, RGB.B);
		UE_LOG(LogTemp, Log, TEXT("ACJS_BallPlayer::InitializeFromJson() SetInitColorValue called with R=%f, G=%f, B=%f"), RGB.R, RGB.G, RGB.B);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::InitializeFromJson() Failed to cast GameInstance to UMySessionGameInstance."));
	}

		// 2. 월드에 배치된 20개의 MultiRoomActor를 찾고 저장
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACJS_MultiRoomActor::StaticClass(), FoundActors);

		// GameInstance에서 suggest_list 가져오기
		if (SessionGI)
		{
			if (!SessionGI || SessionGI->WorldSetting.suggest_list.Num() == 0)
			{
				UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::InitializeFromJson() SessionGI or suggest_list is invalid!"));
				return;
			}
		}
		const TArray<FMySuggest_List>& SuggestList = SessionGI->WorldSetting.suggest_list;

		// 최대 20개만 저장
		//for (int32 i = 0; i < FoundActors.Num() && i < 20; i++)  // <--- 테스트용
		//{
		//	ACJS_MultiRoomActor* MultiRoomActor = Cast<ACJS_MultiRoomActor>(FoundActors[i]);
		//	if (MultiRoomActor)
		//	{
		//		MultiRoomActors.Add(MultiRoomActor);
		//		// 인덱스 설정
		//		MultiRoomActor->ActorIndex = i;
		//		UE_LOG(LogTemp, Warning, TEXT("Found MultiRoomActor %d at location: %s"), i, *MultiRoomActor->GetActorLocation().ToString());
		//	}
		//}
		for (int32 i = 0; i < FoundActors.Num() && i < SuggestList.Num(); ++i)
		{
			ACJS_MultiRoomActor* MultiRoomActor = Cast<ACJS_MultiRoomActor>(FoundActors[i]);
			if (MultiRoomActor)
			{
				MultiRoomActor->ActorIndex = i; // 인덱스 설정
				MultiRoomActor->RoomInfo = SuggestList[i]; // suggest_list 데이터 저장
				MultiRoomActors.Add(MultiRoomActor);

				// 로그 출력
				UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::InitializeFromJson() Assigned Room Info to MultiRoomActor %d"), i);
				UE_LOG(LogTemp, Warning, TEXT("  Room Name: %s"), *SuggestList[i].room_name);
				UE_LOG(LogTemp, Warning, TEXT("  Room ID: %s"), *SuggestList[i].room_id);
				UE_LOG(LogTemp, Warning, TEXT("  Room Num: %s"), *SuggestList[i].room_num);
				UE_LOG(LogTemp, Warning, TEXT("  Percent Message: %s"), *SuggestList[i].percent_message);
				UE_LOG(LogTemp, Warning, TEXT("  Reason Message: %s"), *SuggestList[i].reason_message);
				UE_LOG(LogTemp, Warning, TEXT("  Room Description: %s"), *SuggestList[i].roomdescription);
			}
		}
		// 저장된 MultiRoomActor의 개수 출력
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::InitializeFromJson() Found %d MultiRoomActors in the world."), MultiRoomActors.Num());


		// 3. SimilarUsers 및 OppositeUsers 배열 추출 및 저장   // <-- 통신 시 주석 해제                         <-------------- 수정 필요 (소유자의 UserId, RoomNum 같이 저장 필요)
		//TArray<TSharedPtr<FJsonValue>> SimilarUsersArray = JsonObject->GetArrayField(TEXT("SimilarUsers"));
		//TArray<TSharedPtr<FJsonValue>> OppositeUsersArray = JsonObject->GetArrayField(TEXT("OppositeUsers"));

		////TArray<TSharedPtr<FJsonValue>> AllUsersArray;
		//AllUsersArray.Append(SimilarUsersArray);
		//AllUsersArray.Append(OppositeUsersArray);

		// 최대 20개의 방 정보를 저장하고, MultiRoomActor에 설정
		//for (int32 i = 0; i < AllUsersArray.Num() && i < MultiRoomActors.Num(); i++)
		//{
		//	TSharedPtr<FJsonObject> UserObject = AllUsersArray[i]->AsObject();
		//	if (UserObject.IsValid())
		//	{
		//		// EmotionScore와 RoomName을 가져와 설정
		//		FString Message = UserObject->GetStringField(TEXT("EmotionScore"));
		//		FString RoomName = UserObject->GetStringField(TEXT("RoomName"));
		//		FString CurNumPlayer = UserObject->GetStringField(TEXT("playerNum"));  // <-- 자료형 모두 str

		//		// 현재 사용자 수와 최대 수 설정 (예시)
		//		//int32 CurNumPlayer = FMath::RandRange(0, 5); // 예시로 랜덤 설정
		//		FString MaxNumPlayer = "5";
		//		//float Percent = (Message / 500.0f) * 100.0f; // Percent 계산 (예시로 500.0을 기준으로)

		//		// 각 MultiRoomActor에 정보 설정
		//		SetInitMultiRoomInfo(MultiRoomActors[i], CurNumPlayer, MaxNumPlayer, RoomName, Message);
		//	}
		//} 
		// -------------------------------------------------------------------------------------------------------

		//for (int32 i = 0; i < MultiRoomActors.Num(); i++)  //<--- 테스트 용 (통신 x)
		//{
		//	// Message에 0부터 100까지의 랜덤 값 할당
		//	FString Message = FString::Printf(TEXT("%d"), FMath::RandRange(0, 100));
		//	// RoomName에 "user_"와 인덱스 결합하여 할당
		//	FString RoomName = FString::Printf(TEXT("user_%d"), i);

		//	// 현재 사용자 수와 최대 수 설정 (예시)  <-- API 확정시 추가 수정하기
		//	FString CurNumPlayer = FString::FromInt(FMath::RandRange(0, 5));
		//	FString MaxNumPlayer = "5";
		//	//float Percent = (Message / 500.0f) * 100.0f; // Percent 계산 (예시로 500.0을 기준으로)

		//	// 각 MultiRoomActor에 정보 설정
		//	SetInitMultiRoomInfo(MultiRoomActors[i], CurNumPlayer, MaxNumPlayer, RoomName, Message);
		//}


		// 각 MultiRoomActor에 정보 설정
		for (int32 i = 0; i < MultiRoomActors.Num(); i++)
		{
			FString Percent = MultiRoomActors[i]->RoomInfo.percent_message;
			FString RoomName = MultiRoomActors[i]->RoomInfo.room_name;

			// 현재 사용자 수와 최대 수 설정 (예시) 
			FString CurNumPlayer = FString::FromInt(FMath::RandRange(0, 5));
			FString MaxNumPlayer = "5";
			SetInitMultiRoomInfo(MultiRoomActors[i], CurNumPlayer, MaxNumPlayer, RoomName, Percent);
		}
	/*}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON data."));
	}*/
}

void ACJS_BallPlayer::SetInitColorValue(float r, float g, float b) // 색상
{
	InitColorValue = FLinearColor(r, g, b);
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetInitColorValue() - Initial Color set to R: %f, G: %f, B: %f"), r, g, b);
}

//void ACJS_BallPlayer::SetInitMultiRoomInfo(int32 CurNumPlayer, int32 MaxNumPlayer, const FString& RoomName, float Percent)
//{
//	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetInitMultiRoomInfo()"));
//
//	// 월드에서 MultiRoomActor 클래스의 인스턴스를 찾습니다.   ---> 1개
//	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACJS_MultiRoomActor::StaticClass());
//	if (FoundActor)
//	{
//		// ACJS_MultiRoomActor로 캐스팅
//		RefMultiRoom = Cast<ACJS_MultiRoomActor>(FoundActor);
//		if (RefMultiRoom)
//		{
//			// 초기 설정 함수 호출
//			RefMultiRoom->InitRefRoomInfoWidget(CurNumPlayer, MaxNumPlayer, RoomName, Percent);
//			UE_LOG(LogTemp, Warning, TEXT("MultiRoom information initialized successfully."));
//		}
//		else
//		{
//			UE_LOG(LogTemp, Error, TEXT("Failed to cast FoundActor to ACJS_MultiRoomActor."));
//		}
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("No ACJS_MultiRoomActor found in the world."));
//	}
//}

void ACJS_BallPlayer::SetInitMultiRoomInfo(ACJS_MultiRoomActor* MultiRoomActor, const FString& CurNumPlayer, const FString& MaxNumPlayer, const FString& RoomName, const FString& Percent)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetInitMultiRoomInfo()"));
	if (MultiRoomActor)
	{
		//MultiRoomActor->InitRefRoomInfoWidget(CurNumPlayer, MaxNumPlayer, RoomName, Percent);
		
		// 크기 변경
		MultiRoomActor->InitRefRoomScale(Percent);
		// 색깔 변경
		MultiRoomActor->InitRefRoomColor();
		UE_LOG(LogTemp, Warning, TEXT("MultiRoom information initialized for Room: %s, Percent: %s"), *RoomName, *Percent);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid MultiRoomActor provided."));
	}
}
void ACJS_BallPlayer::UpdateMultiRoomPlayerNumInfo(ACJS_MultiRoomActor* MultiRoomActor, const FString& CurNumPlayer, const FString& MaxNumPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::UpdateMultiRoomPlayerNumInfo()"));
	if (MultiRoomActor)
	{
		MultiRoomActor->UpdateClickedRefRoomPlayerNum(CurNumPlayer, MaxNumPlayer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid MultiRoomActor provided."));
	}
}

void ACJS_BallPlayer::ModifyAuroraColors()
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::ModifyAuroraColors()"));

	// 현재 월드에 있는 ACJS_UltraDynamicSkyActor 인스턴스 찾기
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACJS_UltraDynamicSkyActor::StaticClass(), FoundActors);

	// 첫 번째 액터가 존재하는지 확인
	if (FoundActors.Num() > 0)
	{
		ACJS_UltraDynamicSkyActor* DynamicSky = Cast<ACJS_UltraDynamicSkyActor>(FoundActors[0]);

		// 색상 설정 (예: Aurora Color 1)
		//FLinearColor AuroraColor1(1.0f, 0.0f, 0.0f, 1.0f); // 새로운 색상
		//FLinearColor AuroraColor2(0.0f, 1.0f, 0.0f, 1.0f);
		//FLinearColor AuroraColor3(0.0f, 0.0f, 1.0f, 1.0f);

		// 각 색상의 R, G, B, A 값 출력
		//UE_LOG(LogTemp, Warning, TEXT("AuroraColor1: R=%f, G=%f, B=%f, A=%f"), AuroraColor1.R, AuroraColor1.G, AuroraColor1.B, AuroraColor1.A);
		//UE_LOG(LogTemp, Warning, TEXT("AuroraColor2: R=%f, G=%f, B=%f, A=%f"), AuroraColor2.R, AuroraColor2.G, AuroraColor2.B, AuroraColor2.A);
		//UE_LOG(LogTemp, Warning, TEXT("AuroraColor3: R=%f, G=%f, B=%f, A=%f"), AuroraColor3.R, AuroraColor3.G, AuroraColor3.B, AuroraColor3.A);

		// 속성 변경 후 업데이트
		//DynamicSky->UpdateAuroraColors(AuroraColor1, AuroraColor2, AuroraColor3);
		DynamicSky->SetModifyAuroraColors();
		//UE_LOG(LogTemp, Warning, TEXT("After Call DynamicSky->UpdateAuroraColors()"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::ModifyAuroraColors()::Cannot Find DynamicSky!!!"));
	}
}

// 로비 월드 초기 설정 (JSON 전달 부분)
void ACJS_BallPlayer::InitJsonData(FString LocalJsonData)
{
	JsonData = LocalJsonData;
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::InitJsonData()::JsonData initialized with value: %s"), *JsonData);
}

// 특정 MultiRoom 찾고 playerNum 업데이트하여 UI에 반영하기
void ACJS_BallPlayer::FindMultiRoomList(FString roomNum, FString newPlayerNum)
{
	int32 multiRoomIndex;
	// AllUsersArray에서 roomNum의 방 번호 찾기
	for (int32 i = 0; i < AllUsersArray.Num(); i++)
	{
		TSharedPtr<FJsonObject> UserObject = AllUsersArray[i]->AsObject();
		if (UserObject.IsValid())
		{
			FString RoomNum = UserObject->GetStringField(TEXT("roomNum"));

			// 만일 roomNum이 AllUsersArray에 있다면 
			if (RoomNum == roomNum)
			{
				multiRoomIndex = i;
				break;
			}
		}
	}

	// multiRoomIndex가 유효한지 확인
	if (multiRoomIndex != -1)
	{
		// AllUsersArray에서 playerNum 값을 newPlayerNum으로 업데이트
		TSharedPtr<FJsonObject> UserObject = StaticCastSharedPtr<FJsonObject>(AllUsersArray[multiRoomIndex]->AsObject());
		if (UserObject.IsValid())
		{
			UserObject->SetStringField(TEXT("playerNum"), newPlayerNum); // playerNum 값을 새로 설정

			// 업데이트된 playerNum 값을 로그로 출력
			FString UpdatedPlayerNum = UserObject->GetStringField(TEXT("playerNum")); // JSON에서 새로 설정한 playerNum을 가져옵니다.
			UE_LOG(LogTemp, Warning, TEXT("Updated playerNum for room %s: %s"), *roomNum, *UpdatedPlayerNum);

			// UI에 업데이트 시키기
			UpdateMultiRoomPlayerNumInfo(MultiRoomActors[multiRoomIndex], newPlayerNum, "5");
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Room number %s not found in AllUsersArray"), *roomNum);
	}
}


// 월페이퍼 PC 적용
void ACJS_BallPlayer::ExecuteWallPaperPython()
{
	// 파이썬 파일 경로 설정
	FString ScriptPath = FPaths::ProjectContentDir() + TEXT("Python/Wallpaper.py");

	// 파이썬 스크립트 실행
	//IPythonScriptPlugin* PythonPlugin = IPythonScriptPlugin::Get();
	//if (PythonPlugin && PythonPlugin->IsPythonAvailable())
	//{
	//	PythonPlugin->ExecPythonCommand(*ScriptPath);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Python is not available in this build."));
	//}
}




