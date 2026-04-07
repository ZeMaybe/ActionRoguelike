#include "SPowerup_HealthPotion.h"
#include "SAttributeComponent.h"

ASPowerUp_HealthPotion::ASPowerUp_HealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
}

void ASPowerUp_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
		return;

	if (auto Cmp = USAttributeComponent::GetAttributes(InstigatorPawn))
	{
		if (Cmp->ApplyHealthChange(this,Cmp->GetMaxHealth()))
		{
			HideAndCooldownPowerUp();
		}
	}
}
