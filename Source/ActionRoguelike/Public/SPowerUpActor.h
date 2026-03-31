#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPowerUpActor : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	ASPowerUpActor();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	FTimerHandle TimerHandle_Respawn;
	
	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USphereComponent> SphereComp;
	
	UFUNCTION()
	void ShowPowerUp();
	
	void HideAndCooldownPowerUp();
	
	void SetPowerUpState(bool bNewIsActive);
};
