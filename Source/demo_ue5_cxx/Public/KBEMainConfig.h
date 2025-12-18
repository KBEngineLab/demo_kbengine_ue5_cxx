// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameKBEMain.h"
#include "Engine/DataAsset.h"
#include "KBEMainConfig.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_UE5_CXX_API UKBEMainConfig : public UDataAsset
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString ip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	int port;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	UE_CLIENT_TYPE clientType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	UE_NETWORK_ENCRYPT_TYPE networkEncryptType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	bool forceDisableUDP;

};
