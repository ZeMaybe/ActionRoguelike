#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

ASAICharacter::ASAICharacter()
{
    SetGenericTeamId(1);
    PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 5000.0f;
    SightConfig->LoseSightRadius = 5500.0f;
    SightConfig->PeripheralVisionAngleDegrees = 90.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;

    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    HearingConfig->HearingRange = 3000.0f;
    HearingConfig->SetMaxAge(5.0f);
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;

    PerceptionComp->ConfigureSense(*SightConfig);
    PerceptionComp->ConfigureSense(*HearingConfig);
    PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

    AttributeCmp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeCmp"));
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    TimeToHitParamName = "TimeToHit";
}

void ASAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ASAICharacter::OnTargetPerceptionUpdated);
    AttributeCmp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
    TeamId = TeamID;
}

FGenericTeamId ASAICharacter::GetGenericTeamId() const
{
    return TeamId;
}

void ASAICharacter::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Stimulus.WasSuccessfullySensed())
    {
        SetTargetActor(Actor);
        DrawDebugString(GetWorld(), Actor->GetActorLocation(), FString::Printf(TEXT("The player %s is seen!"), *Actor->GetName()), nullptr, FColor::Red, 2.f, true);
    }
    else
    {
        if (const auto AiController = Cast<AAIController>(GetController()))
        {
            auto CurrentActor = Cast<AActor>(AiController->GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
            if (CurrentActor == Actor)
            {
                SetTargetActor(nullptr);
                DrawDebugString(GetWorld(), CurrentActor->GetActorLocation(), FString::Printf(TEXT("The player %s is lost!"), *CurrentActor->GetName()), nullptr, FColor::Red, 2.f, true);
            }
        }
    }
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    if (Delta < 0.0f)
    {
        if (InstigatorActor != this)
            SetTargetActor(InstigatorActor);

        if (!ActiveHealthWidget)
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
