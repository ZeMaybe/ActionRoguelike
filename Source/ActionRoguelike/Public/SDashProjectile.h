#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	ASDashProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Teleport")
	float DetonateDelay;

	UPROPERTY(EditDefaultsOnly, Category="Teleport")
	float TeleportDelay;

	FTimerHandle TimerHandle_DelayedDetonate;
	FTimerHandle TimerHandle_DelayedTeleport;

	virtual void BeginPlay() override;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();
};
