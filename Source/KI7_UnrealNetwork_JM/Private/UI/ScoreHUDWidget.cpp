// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ScoreHUDWidget.h"
#include "UI/DataLineWidget.h"
#include "Components/EditableTextBox.h"

void UScoreHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Score->UpdateName(FText::FromString(TEXT("점수")));
	Score->UpdateIntValue(0);

	NameInput->OnTextCommitted.AddDynamic(this, &UScoreHUDWidget::NameCommitted);
}

void UScoreHUDWidget::UpdateScore(int32 NewScore)
{
	Score->UpdateIntValue(NewScore);
}

void UScoreHUDWidget::NameCommitted(const FText& Text, ETextCommit::Type ComitMethod)
{

}
