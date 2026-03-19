
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class UParticleSystem;
class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS()
class ACTIONROGUELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASProjectileBase();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereCmp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> MovementCmp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystemComponent> EffectCmp;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystem> HitEffect;
};
