// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_ShowColorActor.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AJS_ShowColorActor::AJS_ShowColorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
    SetRootComponent(boxComp);
    boxComp->SetBoxExtent(FVector(50));

    meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
    meshComp->SetupAttachment(boxComp);
    meshComp->SetRelativeScale3D(FVector(1));
    meshComp->SetRelativeLocation(FVector(0, 0, -50.f));

    // 나이아가라 컴포넌트 추가
    CircleNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CircleNiagara"));
    CircleNiagara->SetupAttachment(meshComp);
    CircleNiagara->bAutoActivate = false; // 처음에는 비활성 상태로 설정
    //CircleNiagara->SetRelativeScale3D(FVector(.5f)); // 필요에 따라 크기 조정
}

// Called when the game starts or when spawned
void AJS_ShowColorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_ShowColorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJS_ShowColorActor::SetMaterialColor(FLinearColor newColor)
{
    UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::SetMaterialColor()"));
	if (!meshComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeshComp is not valid."));
		return;
	}

     //Create a dynamic material instance and set the color
    UMaterialInstanceDynamic* DynamicMaterial = meshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, meshComp->GetMaterial(0));
    if (DynamicMaterial)
    {
        DynamicMaterial->SetVectorParameterValue(FName("InnerColor"), newColor);
        UE_LOG(LogTemp, Warning, TEXT("Material color set to R: %f, G: %f, B: %f"), newColor.R, newColor.G, newColor.B);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create dynamic material instance for MeshComp."));
    }
}

void AJS_ShowColorActor::DeactivateNiagaraEffect()
{
    UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::DeactivateNiagaraEffect()"));
    if (CircleNiagara && CircleNiagara->IsActive())
    {
        CircleNiagara->Deactivate();
        UE_LOG(LogTemp, Warning, TEXT("AKGW_RoomlistActor::DeactivateNiagaraEffect() CircleNiagara->Deactivate();"));
    }
}

