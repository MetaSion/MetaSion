// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_SubObjectActor.h"
#include "CJS/SessionGameInstance.h"

// Sets default values
ACJS_SubObjectActor::ACJS_SubObjectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACJS_SubObjectActor::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("ACJS_SubObjectActor::BeginPlay()"));
	//SetSubObject();
}

// Called every frame
void ACJS_SubObjectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ACJS_SubObjectActor::SetSubObject(FString value)
//{
//	UE_LOG(LogTemp, Warning, TEXT("ACJS_SubObjectActor::SetSubObject(FString value)"));
//	FlowerObject = value;
//	UE_LOG(LogTemp, Warning, TEXT("ACJS_SubObjectActor::SetSubObject() value : %s -> FlowerObject : %s"), *value, *FlowerObject);
//	//InitSubObject(FlowerObject);
//}

void ACJS_SubObjectActor::SetSubObject()
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_SubObjectActor::SetSubObject()"));
	USessionGameInstance* SessionGI = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
	if (SessionGI)
	{
		FlowerObject = SessionGI->WorldSetting.SubObject;
		UE_LOG(LogTemp, Warning, TEXT("ACJS_SubObjectActor::SetSubObject() FlowerObject : %s"), *FlowerObject);
	}
}

FString ACJS_SubObjectActor::GetSubObject()
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_SubObjectActor::GetSubObject()"));
	return FlowerObject;
}

FString ACJS_SubObjectActor::InitSubObject(FString value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_SubObjectActor::InitSubObject()"));
	return value;
}

