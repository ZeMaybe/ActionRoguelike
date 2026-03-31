#include "SAttributeComponent.h"

#include "Kismet/KismetSystemLibrary.h"

USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100.0f;
	Health = MaxHealth;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	float DeltaHealth = Health - OldHealth;

	if (DeltaHealth != 0.0f)
		OnHealthChanged.Broadcast(nullptr, this, Health, DeltaHealth);
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
