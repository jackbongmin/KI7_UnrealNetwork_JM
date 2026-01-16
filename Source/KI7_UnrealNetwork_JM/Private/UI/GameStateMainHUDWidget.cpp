// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameStateMainHUDWidget.h"
#include "Framework/TestGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UGameStateMainHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//GameOverText->SetVisibility(ESlateVisibility::Hidden);
}

void UGameStateMainHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!CachedGameState.IsValid())
	{
		CachedGameState = Cast<ATestGameState>(UGameplayStatics::GetGameState(this));
	}


	UpdateTimeDisplay();
	UpdateGameOverDisplay();
	//if (CachedGameState->IsGameOver())
	//{
	//	GameOverText->SetVisibility(ESlateVisibility::HitTestInvisible);
	//}

}

void UGameStateMainHUDWidget::UpdateTimeDisplay()
{

	if (CachedGameState.IsValid())
	{
		int32 Total = FMath::FloorToInt(CachedGameState->GetGameElapsedTime());
		int32 Minutes = Total / 60;
		int32 Seconds = Total % 60;

		ElapsedTimeText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds)));

		int32 Total1 = FMath::FloorToInt(CachedGameState->GetGameRemainingTime());
		int32 Minutes1 = Total1 / 60;
		int32 Seconds1 = Total1 % 60;
		RemainingTimeText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), Minutes1, Seconds1)));



	}
}

void UGameStateMainHUDWidget::UpdateGameOverDisplay()
{
	if (CachedGameState.IsValid() && CachedGameState->IsGameOver())
	{
		GameOverText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		GameOverText->SetVisibility(ESlateVisibility::Hidden);
	}

}
