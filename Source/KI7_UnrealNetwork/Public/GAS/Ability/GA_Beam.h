// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Beam.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class KI7_UNREALNETWORK_API UGA_Beam : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Beam();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
		
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	void BeamTick();
	void PerformBeamTrace(FHitResult& OutHit, FVector& OutEndPoint);
	void ApplyDamageToTarget(UAbilitySystemComponent* TargetASC);
	//void RemoveDamageFormCurrentTarget();
	void CleanupBeam();


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Beam")
	float BeamRange = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Beam")
	float DamagePerSecond = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Beam")
	float TickRate = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Beam")
	TSubclassOf<UGameplayEffect> DamageEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Beam")
	TWeakObjectPtr<UNiagaraSystem> BeamNiagaraSystem = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Beam")
	FName BeamEndParamName = FName("BeamEnd");

private:
	UPROPERTY()
	TWeakObjectPtr<UNiagaraComponent> BeamNiagaraComponent;

	//FActiveGameplayEffectHandle CurrentEffectHandle;
	//AActor* CurrentTarget = nullptr;
	class AGASPlayerCharacter* AvatarCharacter = nullptr;

	FTimerHandle BeamTickTimer;


};
