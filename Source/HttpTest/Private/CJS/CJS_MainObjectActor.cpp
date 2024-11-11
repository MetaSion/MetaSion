// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_MainObjectActor.h"

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

