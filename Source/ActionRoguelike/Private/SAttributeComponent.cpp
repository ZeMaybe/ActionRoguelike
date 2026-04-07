#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100.0f;
	Health = MaxHealth;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	if (const auto Cmp = GetAttributes(Actor))
		return Cmp->IsAlive();
	return false;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	float DeltaHealth = Health - OldHealth;

	if (DeltaHealth != 0.0f)
		OnHealthChanged.Broadcast(InstigatorActor, this, Health, DeltaHealth);
	return DeltaHealth != 0.0f;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health >= MaxHealth;
}

float USAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}
