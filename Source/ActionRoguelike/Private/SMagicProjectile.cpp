#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ASMagicProjectile::ASMagicProjectile()
{
	SphereCmp->OnComponentHit.AddDynamic(this, &ASMagicProjectile::OnActorHit);
}

void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	auto Tmp = GetInstigator();
	if (IsValid(Tmp))
	{
		SphereCmp->IgnoreActorWhenMoving(Tmp, true);
	}
}

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (IsValid(this) && OtherActor != GetInstigator())
	{
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 32, 32, FColor::Red, 0.1, 1.0);
		if (ensure(HitEffect))
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.ImpactPoint, FRotator::ZeroRotator, true);
		Destroy();
	}
}
