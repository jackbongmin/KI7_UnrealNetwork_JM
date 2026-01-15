// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreHUDWidget.generated.h"


class UDataLineWidget;
class UEditableTextBox;
/**
 *
 */
UCLASS()
class KI7_UNREALNETWORK_JM_API UScoreHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateScore(int32 NewScore);

private:
	UFUNCTION()
	void NameCommitted(const FText& Text, ETextCommit::Type ComitMethod);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UDataLineWidget> Score;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UEditableTextBox> NameInput;



};
