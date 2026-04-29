#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "SCharacter.generated.h"

class UAnimMontage;
class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter, public IGenericTeamAgentInterface
{
    GENERATED_BODY()

public:
    ASCharacter();
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PostInitializeComponents() override;

    UFUNCTION(Exec)
    void HealSelf(float Amount = 100);

    virtual void SetGenericTeamId(const FGenericTeamId& TeamID)override;
    virtual FGenericTeamId GetGenericTeamId() const override;

protected:
    UFUNCTION()
    void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

    void MoveForward(float AxisValue);
    void MoveRight(float AxisValue);

    FTimerHandle TimerHandle_PrimaryAttack;
    void PrimaryAttack();
    void PrimaryAttack_TimeElapsed();

    FTimerHandle TimerHandle_BlackHole;
    void BlackHoleAttack();
    void BlackHoleAttack_TimeElapsed();

    FTimerHandle TimerHandle_Dash;
    void Dash();
    void Dash_TimeElapsed();

    void PrimaryInteract();

    void StartAttackEffects();
    void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

    UPROPERTY(EditAnywhere, Category = "Team")
    FGenericTeamId TeamId{ FGenericTeamId::NoTeam };

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USpringArmComponent> SpringArmCmp;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> CameraCmp;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USInteractionComponent> InteractionCmp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USAttributeComponent> AttributeCmp;

    UPROPERTY(EditAnywhere, Category = "Attack")
    TSubclassOf<AActor> ProjectileClass;

    UPROPERTY(EditAnywhere, Category = "Attack")
    TSubclassOf<AActor> DashProjectileClass;

    UPROPERTY(EditAnywhere, Category = "Attack")
    TSubclassOf<AActor> BlackHoleProjectileClass;

    UPROPERTY(EditAnywhere, Category = "Attack")
    TObjectPtr<UAnimMontage> AttackAnim;

    UPROPERTY(EditAnywhere, Category = "Attack")
    TObjectPtr<UParticleSystem> CastingEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Attack")
    float AttackAnimDelay;

    UPROPERTY(VisibleAnywhere, Category = "Effects")
    FName HandSocketName;

    UPROPERTY(VisibleAnywhere, Category = "Effects")
    FName TimeToHitParamName;
};
