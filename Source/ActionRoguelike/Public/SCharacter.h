#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void PrimaryAttack();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmCmp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraCmp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;
};
