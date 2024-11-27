// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CJS/SessionGameInstance.h"
#include "Components/UniformGridPanel.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "CJS/CJS_InnerWorldParticleActor.h"
#include "JS_ShowColorActor.h"
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
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_ShowCharacter;

	//Reasons for recommending particles
	UPROPERTY(meta = (BindWidget))
	class UMultiLineEditableTextBox* txt_SG_ParticleReason;
	UPROPERTY(meta = (BindWidget))
	class UMultiLineEditableTextBox* txt_SG_ColorReason;

	//SwitchWidget
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WS_RoomList;

	// Particle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TSubclassOf<ACJS_InnerWorldParticleActor> ParticleActorFactory;

	// MyPage �κ� ------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory")
	TSubclassOf<class UJS_OnClickRoomUI> OnClickRoomUIFactory;
	UPROPERTY(EditAnywhere)
    class UJS_OnClickRoomUI* OnClickRoomUI;

	//Wrap Box
		UPROPERTY(meta = (BindWidget))
	class UWrapBox* RoomWrapBox;


	// SpawnBall �κ� ----------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TSubclassOf<class AJS_ShowColorActor> SpawnShowColorActorFactory;
    UPROPERTY()
    AJS_ShowColorActor* CurrentBallActor;// ���� ������ Ball ����

	FTimerHandle SpawnBallTimerHandle;

	void SpawnBall();
	void DestroyBall();
	// SpawnBall �κ� ----------------------------------------------
	
	UPROPERTY()
    AActor* CurrentParticleActor;
	
	//��� ����
	TArray<FString> ImagePath;
	void SettingPath();
	FString GetRandomPath();

	void InitializeOnClickRoomUI();
	void ShowOnClickRoomUI();
	void HideOnClickRoomUI();

	void CleanParticle();
	void AddSessionSlotWidget(const TArray<FMyWorldRoomInfo>& RoomInfos);


	// switcher index���� �ٸ� canvas��������.
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
	UFUNCTION()
	void ShowCharacterUI();

	//�׸��� �г� �κ�
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* UGP_RoomList;
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* UGP_Multi_RoomList;

	//�̹��� �߰� �ƴ��� Ȯ��
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
	//void OnClickedImageRoomList();
    void ShowCommentUI(UImage* Image);
    void HideCommentUI();
	
	void SpawnParticle();

	// MyPage �κ� End ------------------------------------
	
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

	int32 RoomNumber = -1;
};
