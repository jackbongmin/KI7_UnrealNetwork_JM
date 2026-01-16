

#include "Framework/TestPlayerState.h"
#include "Framework/MainHUD.h"
#include "UI/ScoreHudWidget.h"
#include "Net/UnrealNetwork.h"
#include "Character/PlayerStateCharacter.h"

void ATestPlayerState::AddMyScore(int32 Point)
{
	if (HasAuthority())
	{
		MyScore += Point;
		OnRep_MyScore();	// 서버는 리플리케이션이 없으니 수동으로 호출
	}
}

void ATestPlayerState::SetMyName(const FString& NewName)
{
	if (HasAuthority())
	{
		if (NewName.IsEmpty())
		{
			MyName = TEXT("MyPlayer");
		}
		else
		{
			MyName = NewName;
		}
		OnRep_MyName();
	}
}

void ATestPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATestPlayerState, MyScore);	// 모든 클라이언트에 리플리케이션
	DOREPLIFETIME(ATestPlayerState, MyName);
}

void ATestPlayerState::OnRep_MyScore()
{
	// UI 갱신
	UE_LOG(LogTemp, Log, TEXT("[%d]Score : %d"), GetPlayerId(), MyScore);

	if (!ScoreHud.IsValid())
	{
		if (APlayerController* PC = GetPlayerController())
		{
			AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD());
			if (HUD)
			{
				ScoreHud = Cast<UScoreHUDWidget>(HUD->GetMainHudWidget().Get());
			}
		}
	}

	if (ScoreHud.IsValid())
	{
		ScoreHud->UpdateScore(MyScore);
	}


}

void ATestPlayerState::OnRep_MyName()
{
	UE_LOG(LogTemp, Log, TEXT("[%d]Name : %s"), GetPlayerId(), *MyName);

	if (APlayerStateCharacter* Character = GetPawn<APlayerStateCharacter>())
	{
		//UE_LOG(LogTemp, Log, TEXT("APlayerStateCharacter 있음"));
		Character->UpdateNamePlate(MyName);
	}
}
