// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGameMode.h"

#include "Character/PlayerCharacter.h"
#include "GameFramework/Character.h"

AWorldGameMode* AWorldGameMode::Instance = nullptr;



void AWorldGameMode::BeginPlay()
{
	Super::BeginPlay();
	Instance = this;

}

void AWorldGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("PostLogin: PlayerController ready"));
	CachedPlayerController = NewPlayer;
}

APlayerController* AWorldGameMode::GetMainPlayerController() const
{
	return CachedPlayerController;
}
