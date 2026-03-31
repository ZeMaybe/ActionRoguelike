// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("BehaviorTree is nullptr in %s, please assign a valid BehaviorTree asset."), *GetName()))
	{
		RunBehaviorTree(BehaviorTree);
	}

	// if (APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	// {
	// 	GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), MyPawn);
	// 	GetBlackboardComponent()->SetValueAsVector(TEXT("MoveToLocation"), MyPawn->GetActorLocation());
	// }
}
