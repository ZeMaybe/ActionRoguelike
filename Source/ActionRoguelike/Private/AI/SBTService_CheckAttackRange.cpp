#include "AI/SBTService_CheckAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto BlackboardCmp = OwnerComp.GetBlackboardComponent();
	auto MyController = OwnerComp.GetAIOwner();

	if (ensure(BlackboardCmp) && ensure(MyController))
	{
		auto MyPawn = MyController->GetPawn();
		auto TargetActor = Cast<AActor>(BlackboardCmp->GetValueAsObject(TEXT("TargetActor")));

		if (ensure(MyPawn) && TargetActor)
		{
			float Distance = FVector::Distance(TargetActor->GetActorLocation(), MyPawn->GetActorLocation());
			auto bWithinRange = Distance < BlackboardCmp->GetValueAsFloat(RangeAmountKey.SelectedKeyName);

			bool bHasLOS = false;
			if (bWithinRange)
			{
				bHasLOS = MyController->LineOfSightTo(TargetActor);
			}

			BlackboardCmp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLOS);
		}
	}
}
