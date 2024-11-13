// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CJS_HeartActor.generated.h"

UCLASS()
class HTTPTEST_API ACJS_HeartActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACJS_HeartActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 충돌체, 외관을 만들고싶다.
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* SphereComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* SphereMeshComp;

	// Projectile Movement Component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    class UProjectileMovementComponent* ProjectileMovementComp;

	// 충돌 시 효과
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class UNiagaraSystem* ImpactParticle;
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class USoundBase* ImpactSound;
	void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);
};
