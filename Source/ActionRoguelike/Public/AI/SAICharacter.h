#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "SAICharacter.generated.h"

class USAttributeComponent;
class USWorldUserWidget;

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter,public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ASAICharacter();

	virtual void PostInitializeComponents() override;

    virtual void SetGenericTeamId(const FGenericTeamId& TeamID)override;
    virtual FGenericTeamId GetGenericTeamId() const override;

protected:

	UPROPERTY(EditAnywhere, Category="Team")
    FGenericTeamId TeamId{ FGenericTeamId::NoTeam };

	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UAIPerceptionComponent> PerceptionComp;

	UPROPERTY(VisibleAnywhere, Category = "Components|Perception config")
    TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(VisibleAnywhere, Category = "Components|Perception config")
    TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USAttributeComponent> AttributeCmp;
	
	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<USWorldUserWidget> HealthWidgetClass;
	
	UPROPERTY(Transient)
	TObjectPtr<USWorldUserWidget> ActiveHealthWidget;

	UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	void SetTargetActor(AActor* NewTarget) const;
};
