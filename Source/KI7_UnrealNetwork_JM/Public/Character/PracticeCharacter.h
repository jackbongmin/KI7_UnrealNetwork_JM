// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PracticeCharacter.generated.h"

class UWidgetComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class KI7_UNREALNETWORK_JM_API APracticeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APracticeCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthWidgetComp;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass; 

	UPROPERTY()
	UUserWidget* HUDWidgetInstance; 

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_SetHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_SetLevel;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_SetExp;


	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float Health;

	UPROPERTY(ReplicatedUsing = OnRep_Level, VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 Level;

	UPROPERTY(ReplicatedUsing = OnRep_Exp, VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float Exp;

	UFUNCTION()
	void OnRep_Health();

	UFUNCTION()
	void OnRep_Level();

	UFUNCTION()
	void OnRep_Exp();

	void Input_IncreaseHealth();
	void Input_IncreaseLevel();
	void Input_IncreaseExp();

	UFUNCTION(Server, Reliable)
	void ServerSetHealth(float NewHealth);

	UFUNCTION(Server, Reliable)
	void ServerSetLevel(int32 NewLevel);

	UFUNCTION(Server, Reliable)
	void ServerSetExp(float NewExp);
};
