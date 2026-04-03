
#include "SGameModeBase.h"
#include "EngineUtils.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 3.0f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
    GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
   auto QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
    if (ensure(QueryInstance))
    {
        QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
    }
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
    if (QueryStatus != EEnvQueryStatus::Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
        return;
    }
    
    int AliveCount = 0;
    for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
    {
        auto Bot = *It;
        auto AttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
        if (ensure(AttributeComp) && AttributeComp->IsAlive())
        {
            AliveCount++;
        }
    }
    
    float MaxCount = 10.0f;
    if (DifficultyCurve)
        MaxCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
    
    if (AliveCount >= MaxCount)
        return;
    
    auto Locations = QueryInstance->GetResultsAsLocations();
    if (Locations.IsValidIndex(0))
    {
        GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
        UE_LOG(LogTemp, Log, TEXT("Spawning bot at location: %s"), *Locations[0].ToString());
    }
}
