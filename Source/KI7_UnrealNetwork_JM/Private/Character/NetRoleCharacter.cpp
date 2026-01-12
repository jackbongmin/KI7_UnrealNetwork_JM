// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NetRoleCharacter.h"

// Sets default values
ANetRoleCharacter::ANetRoleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANetRoleCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANetRoleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FString LocalRoleString = UEnum::GetValueAsString(GetLocalRole());
	const FString RemoteRoleString = UEnum::GetValueAsString(GetRemoteRole());
	
	const FString OwnerString = GetOwner() ? GetOwner()->GetName() : TEXT("오너 없음");
	const FString ConnectionString = GetNetConnection() ? TEXT("커넥션 있음") : TEXT("커넥션 없음");
	
	const FString NetworkInfo = FString::Printf(
		TEXT("LocalRole : %s\nRemoteRole : %s\nOwner : %s\nConnection : %s"),
		*LocalRoleString, *RemoteRoleString, *OwnerString, *ConnectionString);
	DrawDebugString(GetWorld(), GetActorLocation(), NetworkInfo, nullptr, FColor::White, 0.0f, true);

}

// Called to bind functionality to input
void ANetRoleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

