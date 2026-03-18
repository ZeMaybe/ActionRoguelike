#include "SItemChest.h"

#include "Particles/ParticleSystemComponent.h"

ASItemChest::ASItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(RootComponent);
	
	PileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PileMesh"));
	PileMesh->SetupAttachment(RootComponent);
	
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->bAutoActivate = false;
	ParticleSystem->SetupAttachment(PileMesh);

	TargetPitch = 110.0f;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	if (LidMesh->GetRelativeRotation().Pitch > 0)
	{
		LidMesh->SetRelativeRotation(FRotator(0, 0.0f, 0.0f));
	}
	else
	{
		LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0.0f, 0.0f));
	}
}

void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
}
