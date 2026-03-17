#include "SInteractionComponent.h"

#include "SGameplayInterface.h"

USInteractionComponent::USInteractionComponent()
{
}

void USInteractionComponent::PrimaryInteract() const
{
	TArray<FHitResult> HitResults;

	const auto OwnerActor = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	OwnerActor->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionShape Shape;
	Shape.SetSphere(30.0f);

	auto bBlockingHit = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, End, FQuat::Identity, QueryParams,
	                                                       Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (auto& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				APawn* OwnerPawn = Cast<APawn>(OwnerActor);
				ISGameplayInterface::Execute_Interact(HitActor, OwnerPawn);
				break;
			}
		}
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 32.0f,32, LineColor, false, 2.0f);
	}
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 10.0f, 0, 2.0f);
}

void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}
