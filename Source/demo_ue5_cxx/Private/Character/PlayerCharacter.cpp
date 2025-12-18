// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "KBEngine.h"
#include "KBEScripts/Avatar.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


void APlayerCharacter::UpdateHeadInfo()
{
	if (!AvatarEntity)
		return;

	EntityName = AvatarEntity->name;

	BP_UpdateHeadInfo();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 上传Player位置
	FVector PlayerLocation = GetActorLocation();
	KBEngine::Entity* kbe_player = KBEngine::KBEngineApp::getSingleton().player();
	kbe_player->position.x = PlayerLocation.Y / 100.f;
	kbe_player->position.y = PlayerLocation.Z / 100.f;
	kbe_player->position.z = PlayerLocation.X / 100.f;

	// 上传Player朝向
	FRotator PlayerRotation = GetActorRotation();
	kbe_player->direction.x =  FMath::DegreesToRadians<float>(PlayerRotation.Roll);
	kbe_player->direction.y =  FMath::DegreesToRadians<float>(PlayerRotation.Pitch);
	kbe_player->direction.z =  FMath::DegreesToRadians<float>(PlayerRotation.Yaw);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

