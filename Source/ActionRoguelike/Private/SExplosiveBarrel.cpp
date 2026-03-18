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
	RadialForceCmp->Radius = 750.0f;
	RadialForceCmp->ImpulseStrength = 2500.0f;
	RadialForceCmp->bImpulseVelChange = true;
	RadialForceCmp->bAutoActivate = false;
	RadialForceCmp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshCmp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceCmp->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));
	UE_LOG(LogTemp, Warning, TEXT("OtherActor : %s,at game time: %f"), *GetNameSafe(OtherActor),
	       GetWorld()->TimeSeconds);
	DrawDebugString(GetWorld(), Hit.ImpactPoint,
	                FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString()), nullptr, FColor::Green,
	                2.0f, true);
	
	// Detailed info on logging in ue4
	// https://nerivec.github.io/old-ue4-wiki/pages/logs-printing-messages-to-yourself-during-runtime.html
}
