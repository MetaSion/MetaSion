// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_MainObjectActor.h"
#include "CJS/SessionGameInstance.h"

// Sets default values
ACJS_MainObjectActor::ACJS_MainObjectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACJS_MainObjectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACJS_MainObjectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACJS_MainObjectActor::SetMainObject()
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_MainObjectActor::SetMainObject()"));
	USessionGameInstance* SessionGI = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
	if (SessionGI)
	{
		TreeObject = SessionGI->WorldSetting.MainObject;
		//TreeObject = "Tree_summer";  // <--- 잘 바뀜!!
		UE_LOG(LogTemp, Warning, TEXT("ACJS_MainObjectActor::SetMainObject() TreeObject : %s"), *TreeObject);
	}
}

FString ACJS_MainObjectActor::GetMainObject()
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_MainObjectActor::GetMainObject()"));
	return TreeObject;
}

