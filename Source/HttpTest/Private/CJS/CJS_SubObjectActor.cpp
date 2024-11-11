// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_SubObjectActor.h"

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
	
}

// Called every frame
void ACJS_SubObjectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACJS_SubObjectActor::InitSubObject(FString objName)
{

}

