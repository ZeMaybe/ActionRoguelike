#include "SProjectileBase.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"

ASProjectileBase::ASProjectileBase()
{
	InitialLifeSpan = 5.0f;
	
	SphereCmp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCmp"));
	SphereCmp->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = SphereCmp;

	EffectCmp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectCmp"));
	EffectCmp->SetupAttachment(RootComponent);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);

	MovementCmp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementCmp"));
	MovementCmp->bRotationFollowsVelocity = true;
	MovementCmp->bInitialVelocityInLocalSpace = true;
	MovementCmp->InitialSpeed = 8000.0f;
	MovementCmp->ProjectileGravityScale = 0.0f;

	ImpactShakeInnerRadius = 250.0f;
	ImpactShakeOuterRadius = 2500.0f;
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
		if (ensure(ImpactVfx))
			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVfx, GetActorLocation(), GetActorRotation(), true);
		if (ensure(ImpactSound))
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
		if (ensure(ImpactShake))
			UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);

		MovementCmp->StopMovementImmediately();
		EffectCmp->DeactivateSystem();
		SetActorEnableCollision(false);
		// Destroy();
	}
}
