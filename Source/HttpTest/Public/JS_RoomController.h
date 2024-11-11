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

    virtual void SetupInputComponent() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    class AHttpActor* HttpActor;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* IMC_Controller;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* IA_LeftMouse;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* IA_SettingUI;


    // �������̽��� ���� UI
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UHttpWidget>  LoginUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UJS_CreateRoomWidget> CR_UIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UJS_RoomWidget> R_UIFactory;

	UPROPERTY(EditAnywhere)
	class UHttpWidget* LoginUI;

	UPROPERTY(EditAnywhere)
	class UJS_CreateRoomWidget* CR_UI;

	UPROPERTY(EditAnywhere)
	class UJS_RoomWidget* R_UI;


    FTimerHandle LevelCheckTimerHandle;  // 타이머 핸들러

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

    //Mouse Interaction
    void OnMouseClick();

    //Mouse Hover
    void OnMouseHover(AActor* HoveredActor);
    void OnMouseHoverEnd(AActor* HoveredActor);

    //myWorld -> MultiWorld:: Make Session
 /*   UPROPERTY()
	class AHttpActor* HttpActor;*/

    //void OpenMultiWorld();
    void SetActorLocationAfterLevelLoad();
    AActor* CurrentHoveredActor = nullptr;


    /* Screen Capture + Wallpaper Python Auto Execute */
    void ScreenCapture();
    void ExecuteWallPaperPython();

    /* Chat Widget */
    UPROPERTY(EditDefaultsOnly, Category = "Heart")
	class ACJS_JS_WidgetFunction* ChatActorFactory;

    /* Inner World Setting UI */
    UPROPERTY()
    class USessionGameInstance* SessionGI;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCJS_InnerWorldSettingWidget> SettingUIFactory;
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
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
};
