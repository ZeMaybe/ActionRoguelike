
#include "SExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"

ASExplosiveBarrel::ASExplosiveBarrel()
{
	MeshCmp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshCmp"));
	MeshCmp->SetSimulatePhysics(true);
	RootComponent = MeshCmp;
	
	RadialForceCmp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceCmp"));
	RadialForceCmp->SetupAttachment(RootComponent);
	RadialForceCmp->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	RadialForceCmp->Radius = 700.0f;
	RadialForceCmp->ImpulseStrength = 2000.0f;
	RadialForceCmp->bImpulseVelChange = true;
	RadialForceCmp->bAutoActivate = false;
	RadialForceCmp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}


