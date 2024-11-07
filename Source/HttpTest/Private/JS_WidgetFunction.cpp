// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_WidgetFunction.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "KGW/WBP_Image.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/BillboardComponent.h"

// Sets default values
AJS_WidgetFunction::AJS_WidgetFunction()
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
	

    BillboardComp = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComp"));
    BillboardComp->SetupAttachment(RootComponent);


    // 위젯 컴포넌트 생성 및 초기화
    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
    WidgetComponent->SetupAttachment(RootComponent);


    // 위젯 클래스 설정 (Blueprint에서 WBP_Image로 설정해주어야 함)
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/Path/To/WBP_Image"));
    if (WidgetClass.Succeeded())
    {
        WidgetComponent->SetWidgetClass(WidgetClass.Class);
    }
}

// Called when the game starts or when spawned
void AJS_WidgetFunction::BeginPlay()
{
	Super::BeginPlay();
    SetActorVisibilityHidden();
}

// Called every frame
void AJS_WidgetFunction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJS_WidgetFunction::PlayShowImageAnimation()
{
    if (WidgetComponent)
    {
        // 위젯을 가져오기
        UUserWidget* Widget = WidgetComponent->GetWidget();
        if (Widget)
        {
            // WBP_Image로 캐스팅
            if (UWBP_Image* WBPImage = Cast<UWBP_Image>(Widget))
            {
                // 애니메이션 재생
                if (WBPImage->ShowImage)
                {
                    WBPImage->PlayAnimation(WBPImage->ShowImage, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
                }
            }
        }
    }

}

void AJS_WidgetFunction::SetActorVisibilityHidden()
{
    if (boxComp) {
        boxComp->SetVisibility(false);
    }
}

void AJS_WidgetFunction::SetActorVisibilityVisible()
{
    if (boxComp) {
        boxComp->SetVisibility(true);
    }
}

