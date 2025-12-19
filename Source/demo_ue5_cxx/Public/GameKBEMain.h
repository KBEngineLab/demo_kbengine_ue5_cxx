// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameKBEMain.generated.h"


class UKBEventData;
class UKBEMainConfig;
class KBEMain;

UENUM(BlueprintType)
enum class UE_CLIENT_TYPE : uint8
{
	CLIENT_TYPE_UNKNOWN		UMETA(DisplayName = "unknown"),
	CLIENT_TYPE_MOBILE		UMETA(DisplayName = "Mobile"),
	CLIENT_TYPE_WIN			UMETA(DisplayName = "Windows"),
	CLIENT_TYPE_LINUX		UMETA(DisplayName = "Linux"),
	CLIENT_TYPE_MAC			UMETA(DisplayName = "Mac"),
	CLIENT_TYPE_BROWSER		UMETA(DisplayName = "Browser"),
	CLIENT_TYPE_BOTS		UMETA(DisplayName = "Bots"),
	CLIENT_TYPE_MINI		UMETA(DisplayName = "Mini"),
};

UENUM(BlueprintType)
enum class UE_NETWORK_ENCRYPT_TYPE : uint8
{
	ENCRYPT_TYPE_NONE			UMETA(DisplayName = "None"),
	ENCRYPT_TYPE_BLOWFISH		UMETA(DisplayName = "Blowfish"),
};





/**
 * 全局单例类
 */
UCLASS(Blueprintable)
class DEMO_UE5_CXX_API UGameKBEMain : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;



	UFUNCTION(BlueprintCallable)
	void InitWithConfig();



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="KBEngine")
	UKBEMainConfig* Config;

	UFUNCTION(BlueprintCallable, Category = "KBEngine")
	void KBEngineEventPause();
	UFUNCTION(BlueprintCallable, Category = "KBEngine")
	void KBEngineEventResume();


	/**
	 * 场景切换
	 * @param pEventData
	 */
	void addSpaceGeometryMapping(const std::shared_ptr<UKBEventData> pEventData);

	void onKicked(const std::shared_ptr<UKBEventData>& Shared);
	void onDisconnected(const std::shared_ptr<UKBEventData>& Shared);
	void onConnectionState(const std::shared_ptr<UKBEventData>& Shared);
	void onVersionNotMatch(const std::shared_ptr<UKBEventData>& Shared);
	void onScriptVersionNotMatch(const std::shared_ptr<UKBEventData>& Shared);
	void onLoginBaseapp(const std::shared_ptr<UKBEventData>& Shared);
	void onLoginFailed(const std::shared_ptr<UKBEventData>& Shared);
	void onLoginBaseappFailed(const std::shared_ptr<UKBEventData>& Shared);
	void onReloginBaseappSuccessfully(const std::shared_ptr<UKBEventData>& Shared);
	void onLoginSuccessfully(const std::shared_ptr<UKBEventData>& Shared);


private:
	std::shared_ptr<KBEMain> kbeMain;
	FTSTicker::FDelegateHandle TickerHandle;
	bool Tick(float DeltaTime);
};
