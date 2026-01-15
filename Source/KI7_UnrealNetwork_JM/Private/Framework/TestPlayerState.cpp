// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/TestPlayerState.h"
#include "Net/UnrealNetwork.h"

void ATestPlayerState::AddMyScore(int32 Point)
{
	if (HasAuthority())
	{
		MyScore += Point;
		OnRep_MyScore();	// 서버는 리플리케이션이 없으니 수동으로 호출
	}
}

void ATestPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATestPlayerState, MyScore);	// 모든 클라이언트에 리플리케이션

}

void ATestPlayerState::OnRep_MyScore()
{
	if (OnScoreChanged.IsBound())
	{
		OnScoreChanged.Broadcast(MyScore);
	}

	UE_LOG(LogTemp, Log, TEXT("Score : %d"), MyScore);
}
