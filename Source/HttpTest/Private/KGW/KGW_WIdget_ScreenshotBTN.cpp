// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_WIdget_ScreenshotBTN.h"
#include "Components/Button.h"
#include "JS_RoomController.h"
#include "Kismet/GameplayStatics.h"

void UKGW_WIdget_ScreenshotBTN::NativeConstruct()
{
    Super::NativeConstruct();

    Btn_CaptureImage->OnClicked.AddDynamic(this, &UKGW_WIdget_ScreenshotBTN::OnClickCaptureImage);
    Btn_MyPage->OnClicked.AddDynamic(this, &UKGW_WIdget_ScreenshotBTN::OnClikMypage);

    Controller = Cast<AJS_RoomController>(GetOwningPlayer());
}

void UKGW_WIdget_ScreenshotBTN::OnClickCaptureImage()
{  
  Controller->OnClickButtonImage();
}

void UKGW_WIdget_ScreenshotBTN::OnClikMypage()
{
    UGameplayStatics::OpenLevel(this, FName("Main_Sky"));
//     Controller-> SetActorLocationAfterLevelLoad();

    Controller->SetChangeLevelData();

   
}
