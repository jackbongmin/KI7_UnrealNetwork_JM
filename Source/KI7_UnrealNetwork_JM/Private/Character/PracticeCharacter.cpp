

#include "Character/PracticeCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"

APracticeCharacter::APracticeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComp"));
	HealthWidgetComp->SetupAttachment(GetRootComponent());
	HealthWidgetComp->SetDrawSize(FVector2D(150.0f, 30.0f));
	HealthWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f)); 

	Health = 100.0f;
	Level = 1;
	Exp = 0.0f;
}

void APracticeCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled() && HUDWidgetClass)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}

			HUDWidgetInstance = CreateWidget<UUserWidget>(PC, HUDWidgetClass);
			if (HUDWidgetInstance)
			{
				HUDWidgetInstance->AddToViewport();
			}
		}
	}
}

void APracticeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APracticeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APracticeCharacter, Health);

	DOREPLIFETIME_CONDITION(APracticeCharacter, Level, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(APracticeCharacter, Exp, COND_OwnerOnly);
}

void APracticeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_SetHealth, ETriggerEvent::Started, this, &APracticeCharacter::Input_IncreaseHealth);
		EnhancedInputComponent->BindAction(IA_SetLevel, ETriggerEvent::Started, this, &APracticeCharacter::Input_IncreaseLevel);
		EnhancedInputComponent->BindAction(IA_SetExp, ETriggerEvent::Started, this, &APracticeCharacter::Input_IncreaseExp);
	}
}

void APracticeCharacter::Input_IncreaseHealth()
{
	ServerSetHealth(Health - 10.0f);
}

void APracticeCharacter::Input_IncreaseLevel()
{
	ServerSetLevel(Level + 1);
}

void APracticeCharacter::Input_IncreaseExp()
{
	ServerSetExp(Exp + 10.0f);
}

void APracticeCharacter::ServerSetHealth_Implementation(float NewHealth)
{
	Health = NewHealth;
	OnRep_Health();
}

void APracticeCharacter::ServerSetLevel_Implementation(int32 NewLevel)
{
	Level = NewLevel;
	OnRep_Level();
}

void APracticeCharacter::ServerSetExp_Implementation(float NewExp)
{
	Exp = NewExp;
	OnRep_Exp();
}

void APracticeCharacter::OnRep_Health()
{
	UE_LOG(LogTemp, Warning, TEXT("Health : %f"), Health);
}

void APracticeCharacter::OnRep_Level()
{
	UE_LOG(LogTemp, Warning, TEXT("Level : %d"), Level);
}

void APracticeCharacter::OnRep_Exp()
{
	UE_LOG(LogTemp, Warning, TEXT("Exp : %f"), Exp);
}