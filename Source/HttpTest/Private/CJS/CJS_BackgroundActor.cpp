// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_BackgroundActor.h"
#include "CJS/SessionGameInstance.h"

// Sets default values
ACJS_BackgroundActor::ACJS_BackgroundActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACJS_BackgroundActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACJS_BackgroundActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACJS_BackgroundActor::SetBGObject()
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BackgroundActor::SetBGObject()"));
	USessionGameInstance* SessionGI = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
	if (SessionGI)
	{
		//BGObject = SessionGI->WorldSetting.Background;
		BGObject = "mountain_summer";  // <--- 잘 바뀜!!
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BackgroundActor::SetBGObject() BGObject : %s"), *BGObject);
	}
}

FString ACJS_BackgroundActor::GetBGObject()
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BackgroundActor::GetBGObject()"));
	return BGObject;
}

