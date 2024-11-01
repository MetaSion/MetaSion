// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_SaveLevel.h"
#include "Kismet/GameplayStatics.h"


bool UKGW_SaveLevel::SaveGameData(AActor* SkyActor, AActor* WeatherActor)
{
    // ���̺� ���� ��ü ����
    UKGW_SaveLevel* SaveGameInstance = Cast<UKGW_SaveLevel>(UGameplayStatics::CreateSaveGameObject(UKGW_SaveLevel::StaticClass()));
    bool bSaveSuccessful = false;
    if (SaveGameInstance)
    {
        // Ultra Sky ���� ����
        if (SkyActor)
        {
            // �������Ʈ���� TimeOfDay��� �Ӽ��� ���� �� ����
            FProperty* TimeOfDayProperty = SkyActor->GetClass()->FindPropertyByName(FName("TimeOfDay"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(TimeOfDayProperty))
            {
                float LocalTimeOfDay = NumericProperty->GetFloatingPointPropertyValue(TimeOfDayProperty->ContainerPtrToValuePtr<void>(SkyActor));
                SaveGameInstance->TimeOfDay = LocalTimeOfDay;
            }
        }

        // Ultra Weather ���� ����
        if (WeatherActor)
        {
            // ������ �Ӽ��� ���� ������� ����
            FProperty* CloudCoverageProperty = WeatherActor->GetClass()->FindPropertyByName(FName("CloudCoverage"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(CloudCoverageProperty))
            {
                float LocalCloudCoverage = NumericProperty->GetFloatingPointPropertyValue(CloudCoverageProperty->ContainerPtrToValuePtr<void>(WeatherActor));
                SaveGameInstance->CloudCoverage = LocalCloudCoverage;
                UE_LOG(LogTemp, Warning, TEXT("Generated Server URL: %f"), LocalCloudCoverage);

            }

            // �ٸ� �Ӽ��鿡 ���ؼ��� ���� ������� ����
            FProperty* FogProperty = WeatherActor->GetClass()->FindPropertyByName(FName("Fog"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(FogProperty))
            {
                float LocalFog = NumericProperty->GetFloatingPointPropertyValue(FogProperty->ContainerPtrToValuePtr<void>(WeatherActor));
                SaveGameInstance->Fog = LocalFog;
            }

            FProperty* WindIntensityProperty = WeatherActor->GetClass()->FindPropertyByName(FName("WindIntensity"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(WindIntensityProperty))
            {
                float LocalWindIntensity = NumericProperty->GetFloatingPointPropertyValue(WindIntensityProperty->ContainerPtrToValuePtr<void>(WeatherActor));
                SaveGameInstance->WindIntensity = LocalWindIntensity;
            }

            // ���������� �ٸ� �Ӽ��鵵 ó�� (Rain, Snow, Dust ���)
        }

        // ���̺� ���� ���Ϸ� ����
        bSaveSuccessful =UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySaveSlot"), 0);
    }
    return bSaveSuccessful;
}

void UKGW_SaveLevel::LoadGameData(AActor* SkyActor, AActor* WeatherActor)
{
    // ���̺� ���� �����͸� �ҷ����� (MySaveSlot���� 0�� ����)
    UKGW_SaveLevel* LoadGameInstance = Cast<UKGW_SaveLevel>(UGameplayStatics::LoadGameFromSlot(TEXT("MySaveSlot"), 0));

    if (LoadGameInstance)
    {
        // ���̺� ���� �����͸� ���������� �ҷ��� ���

        // Ultra Sky ������ ����
        if (SkyActor)
        {
            FProperty* TimeOfDayProperty = SkyActor->GetClass()->FindPropertyByName(FName("TimeOfDay"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(TimeOfDayProperty))
            {
                // ���̺�� �����͸� SkyActor�� ����
                NumericProperty->SetFloatingPointPropertyValue(TimeOfDayProperty->ContainerPtrToValuePtr<void>(SkyActor), LoadGameInstance->TimeOfDay);
            }
        }

        // Ultra Weather ������ ����
        if (WeatherActor)
        {
            // CloudCoverage
            FProperty* CloudCoverageProperty = WeatherActor->GetClass()->FindPropertyByName(FName("CloudCoverage"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(CloudCoverageProperty))
            {
                NumericProperty->SetFloatingPointPropertyValue(CloudCoverageProperty->ContainerPtrToValuePtr<void>(WeatherActor), LoadGameInstance->CloudCoverage);
            }

            // Fog
            FProperty* FogProperty = WeatherActor->GetClass()->FindPropertyByName(FName("Fog"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(FogProperty))
            {
                NumericProperty->SetFloatingPointPropertyValue(FogProperty->ContainerPtrToValuePtr<void>(WeatherActor), LoadGameInstance->Fog);
            }

            // WindIntensity
            FProperty* WindIntensityProperty = WeatherActor->GetClass()->FindPropertyByName(FName("WindIntensity"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(WindIntensityProperty))
            {
                NumericProperty->SetFloatingPointPropertyValue(WindIntensityProperty->ContainerPtrToValuePtr<void>(WeatherActor), LoadGameInstance->WindIntensity);
            }
        }

        // �α� ���: ���̺� �����͸� ���������� �ҷ������� �˸�
        UE_LOG(LogTemp, Log, TEXT("Game data loaded successfully"));
    }
    else
    {
        // ���̺� �����͸� �ҷ����� �� ������ ���
        UE_LOG(LogTemp, Error, TEXT("Failed to load game data"));
    }
}
