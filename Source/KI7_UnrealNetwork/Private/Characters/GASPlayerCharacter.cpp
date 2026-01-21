// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GASPlayerCharacter.h"
#include "Framework/GASPlayerState.h"
#include "Framework/TestPlayerController.h"
#include "GAS/TestAttributeSet.h"
#include "GAS/GameAbilitySystemEnums.h"
#include "UI/DataLineWidget.h"
#include "Components/BilboardWidgetComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AGASPlayerCharacter::AGASPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Widget = CreateDefaultSubobject<UBilboardWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(GetRootComponent());
}

void AGASPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearInputBind();
	Super::EndPlay(EndPlayReason);
}

void AGASPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitializeInputBind(NewController);
	InitializeAbilitySystem();	// 서버에서 실행하기 위한 용도
}

void AGASPlayerCharacter::UnPossessed()
{
	ClearInputBind();
	Super::UnPossessed();
}

void AGASPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitializeInputBind(GetController());
	InitializeAbilitySystem();	// 클라이언트에서 실행
}

void AGASPlayerCharacter::OnAbility1Press()
{
	if (ASC/* && IsLocallyControlled()*/)
	{
		//UE_LOG(LogTemp, Log, TEXT("입력은 들어옴"));
		Server_ExecuteAbility1();
	}
}

void AGASPlayerCharacter::OnAbility2Press()
{
	if (ASC)
	{
		//UE_LOG(LogTemp, Log, TEXT("입력은 들어옴 2 Press"));
		Server_ExecuteAbility2();
	}
}

void AGASPlayerCharacter::OnAbility2Release()
{
	if (ASC)
	{
		//UE_LOG(LogTemp, Log, TEXT("입력은 들어옴 2 Release"));
		Server_EndAbility2();
	}
}

void AGASPlayerCharacter::Server_ExecuteAbility1_Implementation()
{
	//UE_LOG(LogTemp, Log, TEXT("Server RPC 호출"));
	ASC->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::Shoot));
}

void AGASPlayerCharacter::Server_ExecuteAbility2_Implementation()
{
	ASC->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::Beam));
}

void AGASPlayerCharacter::Server_EndAbility2_Implementation()
{
	ASC->AbilityLocalInputReleased(static_cast<int32>(EAbilityInputID::Beam));
}

void AGASPlayerCharacter::InitializeInputBind(AController* ControllerToBind)
{
	//UE_LOG(LogTemp, Log, TEXT("[%s] PossessedBy : %d"),
	//	HasAuthority() ? TEXT("Server") : TEXT("Client"), GetPlayerState()->GetPlayerId());
	if (ATestPlayerController* PC = Cast<ATestPlayerController>(GetController()))
	{
		PC->OnAbility1Press.Unbind();
		PC->OnAbility1Press.BindUObject(this, &AGASPlayerCharacter::OnAbility1Press);	// 플레이어 컨트롤러의 입력 신호 받기	

		PC->OnAbility2Press.Unbind();
		PC->OnAbility2Press.BindUObject(this, &AGASPlayerCharacter::OnAbility2Press);

		PC->OnAbility2Release.Unbind();
		PC->OnAbility2Release.BindUObject(this, &AGASPlayerCharacter::OnAbility2Release);
	}
}

void AGASPlayerCharacter::ClearInputBind()
{
	if (ATestPlayerController* PC = Cast<ATestPlayerController>(GetController()))
	{
		PC->OnAbility1Press.Unbind();
		PC->OnAbility2Press.Unbind();
		PC->OnAbility2Release.Unbind();
	}
}

void AGASPlayerCharacter::InitializeAbilitySystem()
{
	if (bAbilitySystemInitialized)
	{
		return;	// 이전에 초기화한 적이 있으면 종료
	}

	if (AGASPlayerState* PS = GetPlayerState<AGASPlayerState>())
	{
		ASC = PS->GetAbilitySystemComponent();
		ResourceAS = PS->GetResourceAttributeSet();

		if (ASC && ResourceAS)
		{
			if (HasAuthority())
			{
				UE_LOG(LogTemp, Log, TEXT("[%d] InitializeAbilitySystem"), PS->GetPlayerId());
				//UE_LOG(LogTemp, Log, TEXT("[%d] Widget : %p"), PS->GetPlayerId(), Widget.Get());
				//UE_LOG(LogTemp, Log, TEXT("[%d] Widget->GetWidget() : %p"), PS->GetPlayerId(), Widget ? Widget->GetWidget() : nullptr);
			}
			ASC->InitAbilityActorInfo(PS, this);
			FOnGameplayAttributeValueChange& onHealthChange =
				ASC->GetGameplayAttributeValueChangeDelegate(UTestAttributeSet::GetHealthAttribute());
			onHealthChange.AddUObject(this, &AGASPlayerCharacter::OnHealthChanged);

			if (Widget && Widget->GetWidget())	// Health UI 첫 초기화(문제 : 서버는 Widget->GetWidget()을 해도 nullptr이다)
			{
				//if (HasAuthority())
				//{
				//	UE_LOG(LogTemp, Log, TEXT("[%d] InitializeAbilitySystem : Widget 확인"), PS->GetPlayerId());
				//}

				UDataLineWidget* HealthWidget = Cast<UDataLineWidget>(Widget->GetWidget());
				float Health = ResourceAS->GetHealth();
				float MaxHealth = ResourceAS->GetMaxHealth();
				HealthWidget->UpdateName(FText::AsNumber(Health));
				HealthWidget->UpdateIntValue(FMath::FloorToInt32(MaxHealth));				

				//FTimerHandle Handle;
				//GetWorld()->GetTimerManager().SetTimer(
				//	Handle,
				//	[HealthWidget, Health, MaxHealth]()
				//	{
				//		HealthWidget->UpdateName(FText::AsNumber(Health));
				//		HealthWidget->UpdateIntValue(FMath::FloorToInt32(MaxHealth));
				//	},
				//	1.0f, false
				//);
			}

			bAbilitySystemInitialized = true;

			PS->SetDefaultAbilitySystemData();

			//UE_LOG(LogTemp, Log, TEXT("[%s] ASC 초기화 완료."), HasAuthority() ? TEXT("Server") : TEXT("Client"));
		}
	}
}

void AGASPlayerCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	const float NewHealth = Data.NewValue;
	//UE_LOG(LogTemp, Log, TEXT("현재 체력 : %.1f"), NewHealth);

	if (Widget && Widget->GetWidget())
	{
		UDataLineWidget* HealthWidget = Cast<UDataLineWidget>(Widget->GetWidget());
		HealthWidget->UpdateName(FText::AsNumber(NewHealth));
		HealthWidget->UpdateIntValue(FMath::FloorToInt32(ResourceAS->GetMaxHealth()));
	}
}

void AGASPlayerCharacter::Multicast_StartBeam_Implementation(UNiagaraSystem* BeamSystem, FName BeamEndParam)
{
	if (BeamSystem )
	{
		if (BeamNiagaraComponent)
		{
			BeamNiagaraComponent->DestroyComponent();
			BeamNiagaraComponent = nullptr;
		}
		BeamNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			BeamSystem,
			GetRootComponent(),
			NAME_None,
			FVector::ZeroVector, FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);

		if (BeamNiagaraComponent)
		{
			// 끝점을 아바타 케릭터 위치로 설정(=안보임)
			BeamNiagaraComponent->SetVariablePosition(BeamEndParam, GetActorLocation());
		}
	}
}

void AGASPlayerCharacter::Multicast_StopBeam_Implementation()
{
	if (BeamNiagaraComponent)
	{
		BeamNiagaraComponent->DestroyComponent();
		BeamNiagaraComponent = nullptr;
	}
}

void AGASPlayerCharacter::Multicast_UpdateBeamEndPoint_Implementation(FName BeamEndParam, const FVector& EndPoint)
{
	if (BeamNiagaraComponent)
	{
		BeamNiagaraComponent->SetVariablePosition(BeamEndParam, EndPoint);
	}
}

void AGASPlayerCharacter::Server_RequestIgnoreMoveInput_Implementation(bool bIgnore)
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->ClientIgnoreMoveInput(bIgnore);
	}
}

float AGASPlayerCharacter::GetHealth() const
{
	return ResourceAS ? ResourceAS->GetHealth() : 0.0f;
}

float AGASPlayerCharacter::GetMaxHealth() const
{
	return ResourceAS ? ResourceAS->GetMaxHealth() : 0.0f;
}
