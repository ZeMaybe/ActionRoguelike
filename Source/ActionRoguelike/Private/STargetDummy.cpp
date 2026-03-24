
#include "STargetDummy.h"

#include "SAttributeComponent.h"

ASTargetDummy::ASTargetDummy()
{
	MeshCmp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshCmp"));
	RootComponent = MeshCmp;
	
	AttributeCmp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeCmp"));
	AttributeCmp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		MeshCmp->SetScalarParameterValueOnMaterials(TEXT("TimeToHit"), GetWorld()->TimeSeconds);
	}
}

