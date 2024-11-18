// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_UserCharacter.h"
#include "CJS/CJS_AimPointWidget.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"


// Sets default values
ACJS_UserCharacter::ACJS_UserCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	/*bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;*/

	// Configure character movement
	//GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	//GetCharacterMovement()->bUseControllerDesiredRotation = true; // Character moves in the direction of input...	
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	/*GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;*/

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 8.5f));
	CameraBoom->TargetOffset = FVector(0.f, 0.f, 100.0f);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->FieldOfView = 90.0f;
}

// Called when the game starts or when spawned
void ACJS_UserCharacter::BeginPlay()
{
	Super::BeginPlay();


	// Add Input Mapping Context
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (subsys && IMC_Player)
		{
			subsys->AddMappingContext(IMC_Player, 0);
			//UE_LOG(LogTemp, Warning, TEXT("ACJS_LobbyPlayer::BeginPlay()::IMC_Player Set OK"));
		}
		else
		{
			//UE_LOG(LogTemp, Error, TEXT("ACJS_UserCharacter::BeginPlay()::IMC_Player is null or subsystem is null"));
		}
		
	}
	else
	{		
		//UE_LOG(LogTemp, Error, TEXT("ACJS_LobbyPlayer::BeginPlay()::PlayerController (pc) is null in BeginPlay"));

	}


	// WBP_AimPoint 위젯 생성
	//if (WBP_AimPoint)  // WBP_aimpoint가 올바르게 할당되어 있는지 확인
	//{
	//	AimPointUI = CreateWidget<UCJS_AimPointWidget>(GetWorld(), WBP_AimPoint);
	//	if (AimPointUI)
	//	{
	//		AimPointUI->AddToViewport(999);
	//		AimPointUI->SetVisibility(ESlateVisibility::Visible);
	//		UE_LOG(LogTemp, Warning, TEXT("AimPointUI successfully created and added to viewport"));
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("Failed to create AimPointUI Widget"));
	//	}
	//	UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::BeginPlay()::WBP_AimPoint is assigned!"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("ACJS_UserCharacter::BeginPlay()::WBP_AimPoint is not assigned! Please assign it in the Blueprint."));
	//}

}


// Called every frame
void ACJS_UserCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*FTransform tran = FTransform(GetControlRotation());
	Direction = tran.TransformVector(Direction);
	Direction.Z = 0.f;
	Direction.Normalize();
	AddMovementInput(Direction);
	Direction = FVector::ZeroVector;*/
}


// Called to bind functionality to input
void ACJS_UserCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (input)
	{

		// Jumping
		//input->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACJS_UserCharacter::Jump);
		//input->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACJS_UserCharacter::StopJumping);

		// Moving
		//input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ACJS_UserCharacter::OnMyActionMove);

		// Looking
		//input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ACJS_UserCharacter::OnMyActionLook);

		// Clicking
		input->BindAction(IA_Click, ETriggerEvent::Started, this, &ACJS_UserCharacter::OnMyActionClick);

		// Toggling
		//input->BindAction(IA_AimPoint, ETriggerEvent::Started, this, &ACJS_UserCharacter::OnMyActionToggleAimPointUI);

	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ACJS_UserCharacter::OnMyActionMove(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionMove()"));

	FVector2D v = Value.Get<FVector2D>();
	FVector MovementDirection = FVector(v.X, v.Y, 0.f);
	MovementDirection = GetActorRotation().RotateVector(MovementDirection);
	MovementDirection.Normalize();

	AddMovementInput(MovementDirection);
}

void ACJS_UserCharacter::OnMyActionLook(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionLook()"));

	FVector2D v = Value.Get<FVector2D>();
	AddControllerPitchInput(-v.Y);
	AddControllerYawInput(v.X);
}

void ACJS_UserCharacter::OnMyActionClick(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionClick()"));

	if (!bAimPointUIShowing)
	{
		// AimPointUI가 표시되지 않았을 때는 클릭을 무시
		//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionClick()::bAimPointUIShowing==false return"));
		return;
	}

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + FollowCamera->GetForwardVector() * 100000.0f;
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

			if (HitActorName.Contains("BP_MultiRoom"))
			{
				//UE_LOG(LogTemp, Warning, TEXT("BP_MultiRoom Clicked"));	
				APlayerController* PC = Cast<APlayerController>(GetController());
				if (PC)
				{
					//PC->ClientTravel("/Game/CJS/Maps/CJS_MultiRoomMap", ETravelType::TRAVEL_Absolute);  <--- 각각 이동하면 다른 곳에 존재해서, 같이 이동하도록 수정하려고 함.
					//GetWorld()->ServerTravel("/Game/CJS/Maps/CJS_MultiRoomMap?listen");  <--- 이렇게 하면 서버가 이동하니 Lobby가 없어져 버림.

					RequestMapTravel("/Game/CJS/Maps/CJS_MultiRoomMap");
				}
				else
				{
					//UE_LOG(LogTemp, Error, TEXT("PlayerController is nullptr. Cannt Move to the MultiRoomMap"));
				}
			}
			else if (HitActorName.Contains("BP_MyRoom"))
			{
				//UE_LOG(LogTemp, Warning, TEXT("BP_MyRoom Clicked"));
				APlayerController* PC = Cast<APlayerController>(GetController());
				if (PC)
				{
					//PC->ClientTravel("/Game/CJS/Maps/CJS_MyRoomMap", ETravelType::TRAVEL_Absolute);
					RequestMapTravel("/Game/CJS/Maps/CJS_MyRoomMap");
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

void ACJS_UserCharacter::OnAimPointUIStateChanged(bool bIsVisible)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnAimPointUIStateChanged()"));

	bAimPointUIShowing = bIsVisible;
	if (bAimPointUIShowing)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnAimPointUIStateChanged()::bAimPointUIShowing = true"));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnAimPointUIStateChanged()::bAimPointUIShowing = false"));
	}
}

void ACJS_UserCharacter::RequestMapTravel(const FString& MapPath)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && !HasAuthority())  // 클라이언트만 요청
	{
		Server_RequestMapTravel(MapPath);
	}
}

void ACJS_UserCharacter::Server_RequestMapTravel_Implementation(const FString& MapPath)
{
	// 문제) 호스트클라이언트 + 클라이언트 모두 이동함
	//APlayerController* PC = Cast<APlayerController>(GetController());
	//if (HasAuthority() && PC)
	//{
	//	// 서버가 다른 모든 클라이언트들에게 같은 맵으로 이동하라고 명령합니다.
	//	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	//	{
	//		APlayerController* OtherPC = Iterator->Get();
	//		if (OtherPC)
	//		{
	//			OtherPC->ClientTravel(MapPath, ETravelType::TRAVEL_Absolute);
	//		}
	//	}
	//}

	float StartYValue = 0.0f; // 시작 Y 값
	float YOffsetIncrement = 100.0f; // 각 클라이언트마다 Y 값 증가량
	int32 ClientIndex = 0; // 클라이언트 인덱스

	// 서버가 모든 플레이어 컨트롤러를 탐색합니다.
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* OtherPC = Iterator->Get();

		// 서버 겸 클라이언트가 아닌 일반 클라이언트만 이동합니다. 서버 겸 클라이언트(호스트)는 ROLE_Authority
		if (OtherPC && OtherPC->GetRemoteRole() == ROLE_AutonomousProxy)
		{
			// listen 파라미터 없이 이동하여 기존 서버 세션을 따르게 합니다.		
			//OtherPC->ClientTravel(MapPath, ETravelType::TRAVEL_Absolute);  // <-- 서버가 같은 공간에 있는 게 아니라서 계속 따로 이동함
			
			// 그래서 그냥 위치 이동하는 걸로 변경해 봄
			APawn* ControlledPawn = OtherPC->GetPawn();
			if (ControlledPawn)
			{
				// 캐릭터의 위치를 변경합니다.
				//ControlledPawn->SetActorLocation(FVector(9950.0f, 0.0f, 0.0f));

				// 각 클라이언트마다 다른 Y 값을 사용하여 위치를 변경합니다.
				FVector NewLocation(9950.0f, StartYValue + (YOffsetIncrement * ClientIndex), 0.0f);
				ControlledPawn->SetActorLocation(NewLocation);
				ClientIndex++; // 다음 클라이언트를 위해 인덱스 증가
			}
		}
	}
}

void ACJS_UserCharacter::OnMyActionToggleAimPointUI(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionToggleAimPointUI()"));

	if (AimPointUI)
	{
		if (!bAimPointUIShowing)
		{
			bAimPointUIShowing = true;
			AimPointUI->SetVisibility(ESlateVisibility::Visible);
			//UE_LOG(LogTemp, Warning, TEXT("AimPointUI is now Visible"));

			// 마우스 커서를 표시하고 입력 모드를 UI와 게임 모두로 설정
			/*if (PC)
			{
				PC->bShowMouseCursor = true;
				PC->SetInputMode(FInputModeGameAndUI());
			}*/
		}
		else
		{
			bAimPointUIShowing = false;
			AimPointUI->SetVisibility(ESlateVisibility::Hidden);
			//UE_LOG(LogTemp, Warning, TEXT("AimPointUI is now Hidden"));

			// 마우스 커서를 숨기고 입력 모드를 게임 전용으로 설정
			/*if (PC)
			{
				PC->bShowMouseCursor = false;
				PC->SetInputMode(FInputModeGameOnly());
			}*/
		}
	}

	//if (WBP_AimPoint) // 위젯 블루프린트가 할당되어 있는지 확인
	//{
	//	// AimPointUI가 아직 생성되지 않았다면 생성
	//	if (!AimPointUI)
	//	{
	//		AimPointUI = CreateWidget<UCJS_AimPointWidget>(GetWorld(), WBP_AimPoint);
	//		if (AimPointUI)
	//		{
	//			AimPointUI->AddToViewport();
	//		}
	//		else
	//		{
	//			UE_LOG(LogTemp, Error, TEXT("Failed to create AimPointUI Widget"));
	//			return;
	//		}
	//	}

	//	// AimPointUI의 표시 상태 토글
	//	if (!bAimPointUIShowing)
	//	{
	//		AimPointUI->SetVisibility(ESlateVisibility::Visible);
	//		bAimPointUIShowing = true;
	//		// AimPointUI 파괴

	//		UE_LOG(LogTemp, Warning, TEXT("AimPointUI is now Visible"));

	//	}
	//	else
	//	{
	//		AimPointUI->SetVisibility(ESlateVisibility::Hidden);
	//		bAimPointUIShowing = false;
	//		UE_LOG(LogTemp, Warning, TEXT("AimPointUI is now Hidden"));

	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("WBP_AimPoint is not assigned! Please assign it in the Blueprint."));
	//}
}
