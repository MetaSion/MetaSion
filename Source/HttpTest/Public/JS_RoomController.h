// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "JS_RoomWidget.h"
#include "JS_RoomController.generated.h"

UCLASS()
class HTTPTEST_API AJS_RoomController : public APlayerController
{
    GENERATED_BODY()

    
protected:
    virtual void BeginPlay() override;
    
public:
    AJS_RoomController(); // ������ �߰�

	virtual void Tick(float DeltaTime) override;


    /* Input */
    virtual void SetupInputComponent() override;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* IMC_Controller;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* IA_LeftMouse;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* IA_SettingUI;


    /* UI */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UHttpWidget>  LoginUIFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UJS_CreateRoomWidget> CR_UIFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UJS_RoomWidget> R_UIFactory;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UJS_ExplainWidget> Ex_UIFactory;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UKGW_RoomList> RL_UIFactory;
	UPROPERTY()
	class UHttpWidget* LoginUI;
	UPROPERTY()
	class UJS_CreateRoomWidget* CR_UI;
	UPROPERTY()
	class UJS_RoomWidget* R_UI;
    UPROPERTY(EditAnywhere)
	class UJS_ExplainWidget* Ex_UI;
    UPROPERTY(EditAnywhere)
	class UKGW_RoomList* RL_UI;

    //ShowRoomList TimerHandler
    FTimerHandle ShowRoomListTimerHandle;
    FTimerHandle LevelCheckTimerHandle;  // 타이머 핸들러
    FTimerHandle OtherRoomCheckTimerHandle;
    FDateTime LastCheckDate; // 마지막으로 확인한 날짜 (00:00 기준)
    bool bShowLoginScreen = false; // 초기 값 설정
    bool bOnlyIndexSend = false;
    bool bShowUI = false;
    bool bSuccess = false;
    void CheckDate();
    
    
    // UI
    void InitializeUIWidgets();

    //LoginUI
    void ShowLoginUI();
    void HideLoginUI();

    //CreateRoomUI
    void ShowCreateRoomUI();
    void HideCreateRoomUI();

    //RoomUI
    void ShowRoomUI();
    void HideRoomUI();
    void PlayUIAnimation();

    FTimerHandle HeartUITimer;
    void ShowHeartUITimer();
    void SpawnAndSwitchToCamera();

    void SetChangeLevelData();

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SwitchToCamera();

    //ExplainUI
    UFUNCTION(BlueprintCallable)
    void ShowExplainUI();
    void HideExplainUI();

    //RoomListUI
    void ShowRoomListUI();
    void HideRoomListUI();
    //KGW==============================================


    /* Inner World Setting UI */
    UPROPERTY()
    class USessionGameInstance* SessionGI;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCJS_InnerWorldSettingWidget> SettingUIFactory;
    UPROPERTY(BlueprintReadWrite)
	class UCJS_InnerWorldSettingWidget* SettingUI;

    void ShowSettingUI();
    void HideSettingUI();

    UFUNCTION(BlueprintCallable)
    void InitInnerWorldSetting();
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
    FString timeOfDay;
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
    FString cloudCoverage;
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
    FString fog ;
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
    FString rain;
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
    FString snow;
     UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
    FString dust ;
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
    FString thunder ;
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
    FString mainObject ;
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
    FString subObject ;
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
    FString background ;

    /* Inner World UI */
    void ShowInnerWorldUIZero();
    void HideInnerWorldUI();

    /* Chat Widget */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCJS_ChatWidget> ChatUIFactory;
    UPROPERTY(BlueprintReadWrite)
	class UCJS_ChatWidget* ChatUI;

    /* Lobby Widget */
    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCJS_LobbyWidget> LobbyUIFactory;
    UPROPERTY(BlueprintReadWrite)
	class UCJS_LobbyWidget* LobbyUI;*/


    //KGW==============================================
    void OnClickButtonImage();

    //Mouse Interaction
    void OnMouseClick();

    //Mouse Hover
    void OnMouseHover(AActor* HoveredActor);
    void OnMouseHoverEnd(AActor* HoveredActor);


    //void OpenMultiWorld();
    void SetActorLocationAfterLevelLoad();
    AActor* CurrentHoveredActor = nullptr;



    /* Screen Capture + Wallpaper Python Auto Execute */
    void ScreenCapture();
    void ExecuteWallPaperPython();

    /* Http Actor */
    class AHttpActor* HttpActor;

    /* Login Actor */
    class ACJS_LoginActor* LoginActor;
};
