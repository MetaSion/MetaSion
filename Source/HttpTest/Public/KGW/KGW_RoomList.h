// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CJS/SessionGameInstance.h"
#include "Components/UniformGridPanel.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "CJS/CJS_InnerWorldParticleActor.h"
#include "KGW_RoomList.generated.h"

/**
 *
 */
UCLASS()
class HTTPTEST_API UKGW_RoomList : public UUserWidget
{
	GENERATED_BODY()

public:
	//  		UPROPERTY(meta=(BindWidget))
	// 	class UTextBlock* Text_Finding;
	virtual void  NativeConstruct() override;

	UPROPERTY()
	class USessionGameInstance* SGI;
	UPROPERTY()
	class AHttpActor* HttpActor;

	UPROPERTY(EditAnywhere)
	class USessionGameInstance* GameInstance;

	UPROPERTY(EditAnywhere)
    class AJS_RoomController* pc;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_RecomendRoom;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TxtBox_Report;

	UPROPERTY(meta = (BindWidget))
	class UButton* btn_ShowParticle;

	UPROPERTY(meta = (BindWidget))
	class UButton* btn_MyRoom;

	UPROPERTY(meta = (BindWidget))
	class UButton* btn_MyRoom_List;

	UPROPERTY(meta = (BindWidget))
	class UButton* btn_List_of_all_rooms;

	UPROPERTY(meta = (BindWidget))
	class UButton* btn_MultiWorld;

	//Reasons for recommending particles
	UPROPERTY(meta = (BindWidget))
	class UMultiLineEditableTextBox* txt_SG_ParticleReason;

	//SwitchWidget
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WS_RoomList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TSubclassOf<ACJS_InnerWorldParticleActor> ParticleActorFactory;
	// MyPage ºÎºÐ ------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory")
	TSubclassOf<class UJS_OnClickRoomUI> OnClickRoomUIFactory;
	UPROPERTY(EditAnywhere)
    class UJS_OnClickRoomUI* OnClickRoomUI;

	// SpawnBall ºÎºÐ ----------------------------------------------

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TSubclassOf<class AJS_ShowColorActor> SpawnShowColorActorFactory;
    UPROPERTY()
    AJS_ShowColorActor* CurrentBallActor;// ÇöÀç ½ºÆùµÈ Ball ÃßÀû

	FTimerHandle SpawnBallTimerHandle;

	void SpawnBall();
	// SpawnBall ºÎºÐ ----------------------------------------------
	UPROPERTY()
    AActor* CurrentParticleActor;
	
	//°æ·Î ¼¼ÆÃ
	TArray<FString> ImagePath;
	void SettingPath();
	FString GetRandomPath();

	void InitializeOnClickRoomUI();
	void ShowOnClickRoomUI();
	void HideOnClickRoomUI();

	void CleanParticle();
	void AddSessionSlotWidget(const TArray<FMyWorldRoomInfo>& RoomInfos);

	// switcher indexï¿½ï¿½ï¿½ï¿½ ï¿½Ù¸ï¿½ canvasï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½.
	void ChangeCanvas(int32 index);
	// Side Menu
	UFUNCTION()
	void ShowMenuUI();
	UFUNCTION()
	void ShowParticleUI();
	/*UFUNCTION()
	void ShowAIAnalysisUI();*/
	UFUNCTION()
	void ShowMyRoomListUI();

	UFUNCTION()
	void ShowListOfAllRooms();

	//±×¸®µå ÆÐ³Î ºÎºÐ
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* UGP_RoomList;
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* UGP_Multi_RoomList;

	//ÀÌ¹ÌÁö Ãß°¡ µÆ´ÂÁö È®ÀÎ
	bool bIsListMyRooms = false;
	bool bIsListAllRooms = false;
	bool bRoomList = false;
	bool bMultiRoomList = false;

    UFUNCTION(BlueprintCallable)
    void AddImageToGrid(FString TexturePath);

    UFUNCTION()
	void OnImageHovered();

	UFUNCTION()
	void OnImageUnhovered();

	UFUNCTION()
	void OnImageClicked();
    void ShowCommentUI(UImage* Image);
    void HideCommentUI();
	
	void SpawnParticle();

	// MyPage ºÎºÐ End ------------------------------------
	
	// 	void SetRecomendRoomName(const )

	// 	void SetFindActive(bool value);


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UKGW_UserRoomName> UserRoomNameFactory;


	void SetTextLog(FString explain);

	UFUNCTION()
	void OnClickInnerWorld();

	UFUNCTION()
	void OnClickMultiWorld();

	FString WheatherNum;

	void SetWheaterNumb(FString TempNUmb);

	UPROPERTY(VisibleAnywhere)
	class AHttpActor* OpenActor;

		UPROPERTY(VisibleAnywhere)
	class UCJS_InnerWorldSettingWidget* InnerWorldWidget;

	void StartHttpMultyWorld();
};
