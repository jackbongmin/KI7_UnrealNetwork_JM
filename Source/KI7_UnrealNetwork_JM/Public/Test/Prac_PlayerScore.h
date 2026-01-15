// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Prac_PlayerScore.generated.h"


class UTextBlock;
/**
 * 
 */
UCLASS()
class KI7_UNREALNETWORK_JM_API UPrac_PlayerScore : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void UpdateIntValue(int32 InValue);

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Score = nullptr;



};
