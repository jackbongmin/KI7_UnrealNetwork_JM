// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerStateCharacter.generated.h"

//DECLARE_DELEGATE_OneParam(FOnScoreUpdate, int32);

UCLASS()
class KI7_UNREALNETWORK_JM_API APlayerStateCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerStateCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//virtual void OnPlayerStateChanged(APlayerState* NewPlayerState, APlayerState* OldPlayerState) override;

	//void InitScoreWidget();
public:

	UFUNCTION(Server, Reliable)
	void Server_AddScore(int32 Point);


public:
	UFUNCTION(BlueprintCallable, Category = "Test")
	void TestAddScore();


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UWidgetComponent> ScoreWidgetComponent = nullptr;

	//FOnScoreUpdate OnScoreUpdated;

private:
	UPROPERTY()
	TWeakObjectPtr<class UDataLineWidget> ScoreWidget = nullptr;

};
