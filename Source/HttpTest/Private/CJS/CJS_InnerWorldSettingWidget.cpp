// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_InnerWorldSettingWidget.h"
#include "Components/Slider.h"
#include "JS_RoomController.h"
#include "Kismet/GameplayStatics.h"

void UCJS_InnerWorldSettingWidget::SetWinterSnowSlider()
{
	UDW_Snow_Slider-> SetValue(5.0f);	
}

void UCJS_InnerWorldSettingWidget::SetInnerWorldUI()
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_InnerWorldSettingWidget::SetInnerWorldUI"));
	AJS_RoomController* pc = Cast<AJS_RoomController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (pc)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCJS_InnerWorldSettingWidget::SetInnerWorldUI pc set"));
		pc->ShowInnerWorldUIZero();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UCJS_InnerWorldSettingWidget::SetInnerWorldUI pc NO"));
	}

}
