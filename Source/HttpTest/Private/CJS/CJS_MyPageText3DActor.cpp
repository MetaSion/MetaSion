// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_MyPageText3DActor.h"
//#include "KGW/KGW_RoomlistActor.h"
#include "Kismet/GameplayStatics.h"
#include "HttpActor.h"

void ACJS_MyPageText3DActor::CallMyPageActor()
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_MyPageText3DActor::CallMyPageActor()"));
    AHttpActor* httpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
    if (!httpActor)
    {
        UE_LOG(LogTemp, Error, TEXT("AHttpActor::BeginPlay() MyWorldPlayer not found in the level."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AHttpActor::BeginPlay() MyWorldPlayer in the level."));
        httpActor->SetMyWorldUIOn();
        //httpActor->SetMyWorldUIOff();

    }
}
