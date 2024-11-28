// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_HeartActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"


// Sets default values
ACJS_HeartActor::ACJS_HeartActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 외형설정
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	SphereComp->SetSphereRadius(0.5f);

	SphereMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMeshComp"));
	SphereMeshComp->SetupAttachment(RootComponent);
	//SphereMeshComp->SetRelativeScale3D(FVector(0.25f));

	// 충돌설정
	SphereComp->SetCollisionProfileName(TEXT("BlockAll"));
	SphereMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Initialize the projectile movement component
    ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->SetUpdatedComponent(RootComponent);
    ProjectileMovementComp->InitialSpeed = 1000.f; // You can adjust the speed here
    ProjectileMovementComp->MaxSpeed = 1000.f;
    ProjectileMovementComp->bRotationFollowsVelocity = true;
    ProjectileMovementComp->bShouldBounce = true;
    ProjectileMovementComp->Bounciness = 0.3f; // 바운스 정도를 설정
    ProjectileMovementComp->Friction = 0.2f;   // 마찰 계수

    bReplicates = true; // 이 설정은 여전히 유효합니다.

    // UProjectileMovementComponent가 존재하는지 확인하고, 네트워크 복제를 설정합니다.
    if (ProjectileMovementComp)
    {
        ProjectileMovementComp->SetIsReplicated(true);
    }
}

// Called when the game starts or when spawned
void ACJS_HeartActor::BeginPlay()
{
	Super::BeginPlay();
	
//     GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Heart Actor Spawned!"));

    // 예: 랜덤 색상 설정
    if (SphereMeshComp)
    {
        UMaterialInstanceDynamic* MatInstance = SphereMeshComp->CreateAndSetMaterialInstanceDynamic(0);
        if (MatInstance)
        {
            FLinearColor RandomColor = FLinearColor::MakeRandomColor();
            MatInstance->SetVectorParameterValue(FName("Color"), RandomColor);
            UE_LOG(LogTemp, Warning, TEXT("color changed"));

        }
    }
}

// Called every frame
void ACJS_HeartActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACJS_HeartActor::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{   
    if (Other && Other != this)
    {
        UE_LOG(LogTemp, Warning, TEXT("ACJS_HeartActor::NotifyHit() - Collided with another things"));
        // 예: 충돌 시 파티클 이펙트 생성
        if (ImpactParticle)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactParticle, HitLocation);
        }

        // 예: 충돌 시 사운드 재생
        if (ImpactSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitLocation);
        }

        // 예: 충돌 시 이 오브젝트 파괴
        Destroy();
    }
}

