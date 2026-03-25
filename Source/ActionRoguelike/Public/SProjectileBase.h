
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class UParticleSystem;
class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASProjectileBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Explode();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereCmp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> MovementCmp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystemComponent> EffectCmp;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystem> HitEffect;
};
