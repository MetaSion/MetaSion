// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_MultiRoomActor.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextBlock.h"
#include "Engine/Font.h"

// Sets default values
ACJS_MultiRoomActor::ACJS_MultiRoomActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the sphere collision component
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	RootComponent = SphereMesh;

	// Create the sphere mesh component and attach it to the collision
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(SphereMesh);

	//RefRoomInfoWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("RefRoomInfoWidget"));
	//RefRoomInfoWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ACJS_MultiRoomActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the user widget instance
	//UUserWidget* WidgetInstance = Cast<UUserWidget>(RefRoomInfoWidgetComp->GetUserWidgetObject());
	//if (WidgetInstance)
	//{
	//	// Get references to the TextBlock components inside the widget
	//	Txt_CurNumPlayer = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("Txt_CurNumPlayer")));
	//	Txt_MaxNumPlayer = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("Txt_MaxNumPlayer")));
	//	Txt_RefRoomName = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("Txt_RefRoomName")));
	//	Txt_RefPercent = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("Txt_RefPercent")));
	//}
}

// Called every frame
void ACJS_MultiRoomActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//// 사용자의 카메라를 찾고
	//FVector target = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	//// 그 카메라를 바라보는 방향을 만들어서
	//FVector dir = target - RefRoomInfoWidgetComp->GetComponentLocation();
	//dir.Normalize();

	//// HPComp를 회전하고싶다.(Billboard 기법)
	//FRotator rot = dir.ToOrientationRotator();
	//RefRoomInfoWidgetComp->SetWorldRotation(rot);
}

void ACJS_MultiRoomActor::InitRefRoomInfoWidget(const FString& CurNumPlayer, const FString& MaxNumPlayer, const FString& RoomName, const FString& Percent)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_MultiRoomActor::InitRefRoomInfoWidget"));

	// MultiRoomActor 클래스 내에서 InitRefRoomInfoWidget 함수의 시작 부분에 추가
	UE_LOG(LogTemp, Warning, TEXT("Initializing Room Info Widget for Room: %s"), *RoomName);
	UE_LOG(LogTemp, Warning, TEXT("MultiRoomActor location: %s"), *GetActorLocation().ToString());

	//if (Txt_CurNumPlayer)
	//{
	//	Txt_CurNumPlayer->SetText(FText::FromString(CurNumPlayer));
	//}
	//if (Txt_MaxNumPlayer)
	//{
	//	Txt_MaxNumPlayer->SetText(FText::FromString(MaxNumPlayer));
	//}
	//if (Txt_RefRoomName)
	//{
	//	// 한글 텍스트 설정
	//	Txt_RefRoomName->SetText(FText::FromString(RoomName));
	//}
	//if (Txt_RefPercent)
	//{
	//	// Percent 값을 텍스트로 변환하여 표시 (예: 50%)
	//	//FString PercentText = FString::Printf(TEXT("%.2f%"), Percent);
	//	Txt_RefPercent->SetText(FText::FromString(Percent));
	//}
}

void ACJS_MultiRoomActor::UpdateClickedRefRoomPlayerNum(const FString& CurNumPlayer, const FString& MaxNumPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_MultiRoomActor::UpdateClickedRefRoomPlayerNum"));

	if (Txt_CurNumPlayer)
	{
		Txt_CurNumPlayer->SetText(FText::FromString(CurNumPlayer));
	}

	if (Txt_MaxNumPlayer)
	{
		Txt_MaxNumPlayer->SetText(FText::FromString(MaxNumPlayer));
	}
}


// 초기 설정
void ACJS_MultiRoomActor::InitRefRoomScale(const FString& percent)
{
	// 메시지 값으로 메쉬 크기 조정
	int32 MessageValue = FCString::Atoi(*percent); // percent에서 메시지 값 가져오기 (문자열을 정수로 변환)
	UE_LOG(LogTemp, Warning, TEXT("MessageValue: %d"), MessageValue); // MessageValue 로그 출력

	float ScaleFactor = FMath::GetMappedRangeValueClamped(FVector2D(0, 100), FVector2D(2.0f, 10.0f), MessageValue); // 예: 0~100의 메시지를 0.5~2.0의 범위로 변환
	UE_LOG(LogTemp, Warning, TEXT("ScaleFactor: %f"), ScaleFactor); // ScaleFactor 로그 출력

	// 스케일 적용
	SphereMesh->SetWorldScale3D(FVector(ScaleFactor)); // 메쉬 크기 조정
}
void ACJS_MultiRoomActor::InitRefRoomColor()
{
	// 머터리얼 색깔 변경 (파라미터 이름 "EdgeGlowColor")
	 // Dynamic Material Instance 생성 및 색상 설정
	if (SphereMesh)
	{
		// StaticMesh의 첫 번째 머터리얼 가져오기
		UMaterialInstanceDynamic* DynamicMaterial = SphereMesh->CreateAndSetMaterialInstanceDynamic(0);

		if (DynamicMaterial)
		{
			// RGB 값을 0~255 사이에서 랜덤 생성
			float R = FMath::RandRange(0, 255) / 255.0f; // 0~1 사이 값으로 변환
			float G = FMath::RandRange(0, 255) / 255.0f; // 0~1 사이 값으로 변환
			float B = FMath::RandRange(0, 255) / 255.0f; // 0~1 사이 값으로 변환

			// EdgeGlowColor 파라미터에 색상 설정
			DynamicMaterial->SetVectorParameterValue(FName(TEXT("EdgeGlowColor")), FLinearColor(R, G, B));

			// 로그 출력
			UE_LOG(LogTemp, Warning, TEXT("EdgeGlowColor set to R: %f, G: %f, B: %f"), R, G, B);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create Dynamic Material Instance."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SphereMesh is not valid."));
	}
}
