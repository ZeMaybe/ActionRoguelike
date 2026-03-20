#include "SExplosiveBarrel.h"

#include "SAttributeComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASExplosiveBarrel::ASExplosiveBarrel()
{
	MeshCmp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshCmp"));
	MeshCmp->SetSimulatePhysics(true);
	RootComponent = MeshCmp;

	RadialForceCmp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceCmp"));
	RadialForceCmp->SetupAttachment(RootComponent);

	RadialForceCmp->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	RadialForceCmp->Radius = 750.0f;
	RadialForceCmp->ImpulseStrength = 2500.0f;
	RadialForceCmp->bImpulseVelChange = true;
	RadialForceCmp->bAutoActivate = false;
	RadialForceCmp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MeshCmp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));
	UE_LOG(LogTemp, Warning, TEXT("OtherActor : %s,OtherComp : %s, hit at location : %s,at game time: %f"), 
		*GetNameSafe(OtherActor), 
		*GetNameSafe(OtherComp), 
		*Hit.ImpactPoint.ToString(),
		GetWorld()->TimeSeconds);
	// DrawDebugString(GetWorld(), Hit.ImpactPoint, FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString()), nullptr, FColor::Green, 2.0f, true);

	if (OtherActor)
	{
		if (auto Cmp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass())))
		{
			Cmp->ApplyHealthChange(-10.10f);
		}
	}

	RadialForceCmp->FireImpulse();
	// Detailed info on logging in ue4
	// https://nerivec.github.io/old-ue4-wiki/pages/logs-printing-messages-to-yourself-during-runtime.html
}
