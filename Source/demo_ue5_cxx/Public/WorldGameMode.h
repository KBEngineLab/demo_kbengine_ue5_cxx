// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WorldGameMode.generated.h"

class AAvatarCharacter;
class APlayerCharacter;
class AMonsterCharacter;
class ANPCCharacter;
/**
 * 
 */
UCLASS()
class DEMO_UE5_CXX_API AWorldGameMode : public AGameModeBase
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

public:
	static AWorldGameMode* Instance;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	APlayerController* GetMainPlayerController() const;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character蓝图")
	TSubclassOf<ANPCCharacter> NPCBlueprintClass;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character蓝图")
	TSubclassOf<AMonsterCharacter> MonsterBlueprintClass;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character蓝图")
	TSubclassOf<APlayerCharacter> PlayerBlueprintClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character蓝图")
	TSubclassOf<AAvatarCharacter> AvatarBlueprintClass;

private:
	UPROPERTY()
	APlayerController* CachedPlayerController = nullptr;
};
