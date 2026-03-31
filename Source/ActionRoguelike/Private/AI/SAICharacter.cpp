#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	auto MyController = Cast<AAIController>(GetController());
	if (ensureMsgf(MyController, TEXT("SAICharacter %s has no AIController, cannot set TargetActor in Blackboard."), *GetName()))
	{
		MyController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Pawn);
		DrawDebugString(GetWorld(),Pawn->GetActorLocation(),FString::Printf(TEXT("The player %s is seen!"),*Pawn->GetName()),nullptr,FColor::Red,2.f,true);
	}
}
