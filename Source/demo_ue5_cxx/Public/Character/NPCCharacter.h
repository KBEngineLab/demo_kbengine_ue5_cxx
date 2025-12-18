// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KBECharacterBase.h"
#include "GameFramework/Character.h"
#include "NPCCharacter.generated.h"

namespace KBEngine
{
	class NPC;
}

UCLASS()
class DEMO_UE5_CXX_API ANPCCharacter : public AKBECharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCCharacter();
	void UpdateHeadInfo();

	KBEngine::NPC* NPCEntity;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
