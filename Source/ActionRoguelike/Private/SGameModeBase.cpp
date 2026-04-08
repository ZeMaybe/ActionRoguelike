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

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		auto Cmp = USAttributeComponent::GetAttributes(*It);
		if (ensure(Cmp) && Cmp->IsAlive())
			Cmp->Kill(this);
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	int AliveCount = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		if (USAttributeComponent::IsActorAlive(*It))
		{
			AliveCount++;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), AliveCount);

	float MaxCount = 10.0f;
	if (DifficultyCurve)
		MaxCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);

	if (AliveCount >= MaxCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}

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

	auto Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 12, FColor::Blue, false, 60.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query returned no valid locations!"));
	}
}
