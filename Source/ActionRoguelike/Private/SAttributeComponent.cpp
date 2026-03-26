#include "SAttributeComponent.h"

#include "Kismet/KismetSystemLibrary.h"

USAttributeComponent::USAttributeComponent()
{
	Health = 100.0f;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return true;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}
