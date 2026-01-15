// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/Prac_PlayerScore.h"
#include "Components/TextBlock.h"


void UPrac_PlayerScore::UpdateIntValue(int32 InValue)
{
	if (Score)
	{
		Score->SetText(FText::AsNumber(InValue));
	}
}
