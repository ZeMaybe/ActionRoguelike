
#include "SProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

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


