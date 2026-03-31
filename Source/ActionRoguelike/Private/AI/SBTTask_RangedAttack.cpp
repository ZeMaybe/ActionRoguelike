// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "SProjectileBase.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		auto MyCharacter = Cast<ACharacter>(MyController->GetPawn());
		if (!MyCharacter)
			return EBTNodeResult::Failed;
		
		auto TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
		if (!TargetActor)
			return EBTNodeResult::Failed;
		
		FVector MuzzleLocation = MyCharacter->GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();
		
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = MyCharacter;
		
		auto Proj = GetWorld()->SpawnActor<AActor>(ProjectileClass,MuzzleLocation,MuzzleRotation,Params);
		return Proj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
