#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	AttributeCmp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeCmp"));
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	TimeToHitParamName = "TimeToHit";
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeCmp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), Pawn->GetActorLocation(), FString::Printf(TEXT("The player %s is seen!"), *Pawn->GetName()), nullptr, FColor::Red, 2.f, true);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
			SetTargetActor(InstigatorActor);
		
		if(!ActiveHealthWidget)
		{
			ActiveHealthWidget = CreateWidget<USWorldUserWidget>(GetWorld(), HealthWidgetClass);
			if (ActiveHealthWidget)
			{
				ActiveHealthWidget->AttachedActor = this;
				ActiveHealthWidget->AddToViewport();
			}
		}
		
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		
		if (NewHealth <= 0.0f)
		{
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			SetLifeSpan(10.0f);
		}
	}
}

void ASAICharacter::SetTargetActor(AActor* NewTarget) const
{
	if (const auto AiController = Cast<AAIController>(GetController()))
	{
		AiController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), NewTarget);
	}
}
