// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BilboardWidgetComponent.h"

void UBilboardWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//UE_LOG(LogTemp, Log, TEXT("Tick : %.4f"), DeltaTime);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && PC->PlayerCameraManager)
	{
		// 회전 -> 벡터 만들기 가능(해당 회전으로 인한 Forward 백터를 만듬)
		// 벡터 -> 회전 만들기 가능

		FVector CameraForward = PC->PlayerCameraManager->GetCameraRotation().Vector();	// 카메라의 Forward 백터
		FVector WidgetForward = CameraForward * -1.0f;
		SetWorldRotation(WidgetForward.Rotation());
	}
}
