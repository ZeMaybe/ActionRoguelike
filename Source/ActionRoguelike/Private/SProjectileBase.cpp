
#include "SProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASProjectileBase::ASProjectileBase()
{
	SphereCmp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCmp"));
	SphereCmp->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = SphereCmp;

	EffectCmp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectCmp"));
	EffectCmp->SetupAttachment(RootComponent);

	MovementCmp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementCmp"));
	MovementCmp->bRotationFollowsVelocity = true;
	MovementCmp->bInitialVelocityInLocalSpace = true;
	MovementCmp->InitialSpeed = 1000.0f;
	MovementCmp->ProjectileGravityScale = 0.0f;
}

void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (const auto Tmp = GetInstigator(); IsValid(Tmp))
	{
		SphereCmp->IgnoreActorWhenMoving(Tmp, true);
	}
}

void ASProjectileBase::Explode_Implementation()
{
	if (ensure(IsValid(this)))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect, GetActorLocation(), GetActorRotation());

		MovementCmp->StopMovementImmediately();
		EffectCmp->DeactivateSystem();
		SetActorEnableCollision(false);

		Destroy();
	}
}
