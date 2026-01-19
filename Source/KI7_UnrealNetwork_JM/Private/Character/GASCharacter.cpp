// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GASCharacter.h"
#include "GAS/TestAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


AGASCharacter::AGASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	// Minimal : 어트리뷰트와 큐만 오너와 다른 클라이언트에게 리플리케이션.(ex : NPC, 적)
	// Mixed : 미니멀 + (오너에게 모두 리플리케이션) (ex : 자기 플레이어)
	// Full : 모두에게 모두 리플리케이션(디버깅, 관전모드용)

	ResourceAttributeSet = CreateDefaultSubobject<UTestAttributeSet>(TEXT("ResourceAS"));

}

void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);

		FOnGameplayAttributeValueChange& onHealthChange =
			ASC->GetGameplayAttributeValueChangeDelegate(UTestAttributeSet::GetHealthAttribute());
		onHealthChange.AddUObject(this, &AGASCharacter::OnHealthChancged);
	
	}

	
}

void AGASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGASCharacter::OnHealthChancged(const FOnAttributeChangeData& Data)
{
	const float NewHealth = Data.NewValue;
	UE_LOG(LogTemp, Log, TEXT("현재 체력 : %.1f"), NewHealth);
}

