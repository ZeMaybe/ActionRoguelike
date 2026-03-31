#include "SPowerupActor.h"
#include "Components/SphereComponent.h"

ASPowerUpActor::ASPowerUpActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionProfileName(TEXT("PowerUp"));
	RootComponent = SphereComp;

	RespawnTime = 8.0f;
}

void ASPowerUpActor::Interact_Implementation(APawn* InstigatorPawn)
{
	//--
}

void ASPowerUpActor::ShowPowerUp()
{
	SetPowerUpState(true);
}

void ASPowerUpActor::HideAndCooldownPowerUp()
{
	SetPowerUpState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ASPowerUpActor::ShowPowerUp, RespawnTime);
}

void ASPowerUpActor::SetPowerUpState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);
	RootComponent->SetVisibility(bNewIsActive, true);
}
