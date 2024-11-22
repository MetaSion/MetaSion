// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CJS_MultiRoomActor.generated.h"


//class AMultiRoom : public AActor
//{
//public:
//	FString RoomNum;  
//	FString RoomName;  
//	FString Message;
//	FString PlayerNum; 
//
//	// 방 정보를 업데이트하는 함수
//	void UpdatePlayerNum(FString NewPlayerNum)
//	{
//		PlayerNum = NewPlayerNum;
//	}
//};

//USTRUCT(BlueprintType) // 블루프린트에서 사용할 수 있도록 구조체를 정의
//struct FMultiRoom // 방 정보를 저장하는 구조체
//{
//    GENERATED_BODY()
//
//    // 방 ID
//    UPROPERTY(BlueprintReadOnly)
//    FString RoomNum;
//    // 방 이름
//    UPROPERTY(BlueprintReadOnly)
//    FString RoomName;
//    // 현재 플레이어 수
//    UPROPERTY(BlueprintReadOnly)
//    FString PlayerNum;
//
//    // 체험자 수 업데이트 메서드
//    void UpdatePlayerCount(FString NewPlayerCount)
//    {
//		PlayerNum = NewPlayerCount;
//    }
//};


UCLASS()
class HTTPTEST_API ACJS_MultiRoomActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACJS_MultiRoomActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	 // Components
    UPROPERTY(VisibleAnywhere)
    class USphereComponent* SphereCollision;
    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* SphereMesh;
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* RefRoomInfoWidgetComp;


	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_CurNumPlayer;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_MaxNumPlayer;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_RefRoomName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_RefPercent;



	// Function to update the widget text values
	void InitRefRoomInfoWidget(const FString& CurNumPlayer, const FString& MaxNumPlayer, const FString& RoomName, const FString& Percent);

	// Function to change ref room Scale
	void InitRefRoomScale(const FString& percent);
	// Function to change ref room color
	void InitRefRoomColor();



	// Update MultiRoom PlayerNum value
	void UpdateClickedRefRoomPlayerNum(const FString& CurNumPlayer, const FString& MaxNumPlayer);

};
