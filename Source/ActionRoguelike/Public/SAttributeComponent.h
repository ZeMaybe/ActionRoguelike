
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged,AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USAttributeComponent();
		
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static bool IsActorAlive(AActor* Actor);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive()const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth()const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetHealth()const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth()const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool Kill(AActor* InstigatorActor);
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
protected:
	// EditAnywhere - edit in BP editor and per-instance in level.
	// EditDefaultsOnly - hide variable per-instance, edit in BP editor only
	// EditInstanceOnly - allow only editing of instance (eg. when placed in level)
	// VisibleAnywhere - 'read-only' in editor and level. (Use for Components)
	// VisibleDefaultsOnly - 'read-only' access for variable, only in BP editor (uncommon)
	// BlueprintReadOnly - read-only in the Blueprint scripting (does not affect 'details'-panel)
	// BlueprintReadWrite - read-write access in Blueprints
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth;
};
