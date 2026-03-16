
#include "SMagicProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

ASMagicProjectile::ASMagicProjectile()
{
	SphereCmp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCmp"));
	SphereCmp->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = SphereCmp;
	
	EffectCmp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectCmp"));
	EffectCmp->SetupAttachment(RootComponent);
	
	MovementCmp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementCmp"));
	MovementCmp->InitialSpeed = 1000.0f;
	MovementCmp->bRotationFollowsVelocity = true;
	MovementCmp->bInitialVelocityInLocalSpace = true;
}

void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}
