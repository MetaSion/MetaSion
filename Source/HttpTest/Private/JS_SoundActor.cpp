// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_SoundActor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "HttpActor.h"
#include "JsonParseLib.h"
#include "CJS/SessionGameInstance.h"

// Sets default values
AJS_SoundActor::AJS_SoundActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Audio Component ���� �� ����
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false;  // �ڵ� ��� ����
}

// Called when the game starts or when spawned
void AJS_SoundActor::BeginPlay()
{
	Super::BeginPlay();

	/*HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
	if (HttpActor) {
		FRoomData RoomData = HttpActor->GetAIRecommendation();
		FString FileName = RoomData.userMusic;
		SetBackgroundSoundByFileName(FileName);
	}*/
	SessionGI = Cast<USessionGameInstance>(GetGameInstance());
	HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
	if (HttpActor)
	{
		//OnAIRecommendationInitialized();
	}
}

// Called every frame
void AJS_SoundActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJS_SoundActor::SetBackgroundSoundByFileName(const FString& FileName)
{
	// ���� ������ ������ ������ Ư�� ��ο� �ִ� ���
	// ���� ���, "/Game/Audio/" ���丮�� ���� ������ �ִٰ� �����մϴ�.
	FString AssetPath = FString::Printf(TEXT("/Game/Main/Sound/%s.%s"), *FileName, *FileName);

	// ���� ������ �������� �ε�
	USoundBase* NewSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, *AssetPath));

	if (NewSound)
	{
		// ����� ������Ʈ�� �� ���带 �����ϰ� ���
		AudioComponent->SetSound(NewSound);
		AudioComponent->Play();
		UE_LOG(LogTemp, Warning, TEXT("Background sound changed and started playing: %s"), *FileName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load sound: %s"), *AssetPath);
	}
}

void AJS_SoundActor::OnAIRecommendationInitialized()
{
	FString FileName = SessionGI->RoomMusicData;
	SetBackgroundSoundByFileName(FileName);
}

