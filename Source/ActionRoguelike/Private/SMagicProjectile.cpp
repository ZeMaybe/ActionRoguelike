#include "SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ASMagicProjectile::ASMagicProjectile()
{
	SphereCmp->OnComponentHit.AddDynamic(this, &ASMagicProjectile::OnActorHit);
	DamageAmount = -50.0f;
}

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (IsValid(this) && OtherActor != GetInstigator())
	{
		Explode();

		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 32, 32, FColor::Red, 0.1, 1.0);
		if (OtherActor)
		{
			if (auto Cmp = USAttributeComponent::GetAttributes(OtherActor))
			{
				Cmp->ApplyHealthChange(GetInstigator(), DamageAmount);
			}
		}
		Destroy();
	}
}
