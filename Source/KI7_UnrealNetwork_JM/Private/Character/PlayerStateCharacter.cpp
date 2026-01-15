// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerStateCharacter.h"
#include "Framework/TestPlayerState.h"
#include "Components/WidgetComponent.h"
#include "Test/Prac_PlayerScore.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APlayerStateCharacter::APlayerStateCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ScoreWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ScoreWidget"));
	ScoreWidgetComponent->SetupAttachment(RootComponent);

}

//void APlayerStateCharacter::OnPlayerStateChanged(APlayerState* NewPlayerState, APlayerState* OldPlayerState)
//{
//	Super::OnPlayerStateChanged(NewPlayerState, OldPlayerState);
//
//	InitScoreWidget();
//}
//void APlayerStateCharacter::InitScoreWidget()
//{
//
//	UPrac_PlayerScore* MyScoreWidget = Cast<UPrac_PlayerScore>(ScoreWidgetComponent->GetUserWidgetObject());
//
//	ATestPlayerState* PS = GetPlayerState<ATestPlayerState>();
//
//	if (MyScoreWidget && PS)
//	{
//		PS->OnScoreChanged.RemoveDynamic(MyScoreWidget, &UPrac_PlayerScore::UpdateIntValue);
//		PS->OnScoreChanged.AddDynamic(MyScoreWidget, &UPrac_PlayerScore::UpdateIntValue);
//
//		MyScoreWidget->UpdateIntValue(PS->GetScore());
//
//		UE_LOG(LogTemp, Log, TEXT("UI Linked Successfully!"));
//	}
//
//}
// Called when the game starts or when spawned
void APlayerStateCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UPrac_PlayerScore* myScoreWidget = Cast<UPrac_PlayerScore>(ScoreWidgetComponent->GetUserWidgetObject());

	ATestPlayerState* PS = GetPlayerState<ATestPlayerState>();

	if (myScoreWidget && PS)
	{
		PS->OnScoreChanged.AddDynamic(myScoreWidget, &UPrac_PlayerScore::UpdateIntValue);

		myScoreWidget->UpdateIntValue(PS->GetScore());
	}

}

// Called every frame
void APlayerStateCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerStateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void APlayerStateCharacter::Server_AddScore_Implementation(int32 Point)
{
	ATestPlayerState* PS = GetPlayerState<ATestPlayerState>();
	if (PS)
	{
		PS->AddMyScore(Point);
	}
}

void APlayerStateCharacter::TestAddScore()
{
	if (IsLocallyControlled())
	{
		Server_AddScore(10);
	}
}

