// Fill out your copyright notice in the Description page of Project Settings.


#include "GameKBEMain.h"

#include "KBDebug.h"
#include "KBEMain.h"
#include "KBEMainConfig.h"
#include "KBEngine.h"
#include "KBEngineArgs.h"
#include "KBEvent.h"
#include "KBEventTypes.h"
#include "Kismet/GameplayStatics.h"


void UGameKBEMain::Deinitialize()
{

	KBENGINE_DEREGISTER_ALL_EVENT();

	UE_LOG(LogTemp, Log, TEXT("KBEMain Deinitialize"));
	FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
	KBEngine::KBEngineApp::getSingleton().destroy();

	Super::Deinitialize();


}

void UGameKBEMain::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 创建一个全局ticker，去调用process，这样能保证SDK的逻辑运行一直在游戏线程中，不需要频繁 AsyncTask(ENamedThreads::GameThread)
	// 注意！：如果是自己调用process，请设置：kbemain->disableMainLoop = true;，process是线程不安全的
	// 如果自己不进行触发，可以设置kbemain->disableMainLoop = false;，这样SDK内部会启用一个线程调度，后续需要自己切换线程
	TickerHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(
			this, &UGameKBEMain::Tick),
		0.0f // 每帧
	);


	// 切换地图
	KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::addSpaceGeometryMapping, addSpaceGeometryMapping);

	KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::onKicked, onKicked);
	KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::onDisconnected, onDisconnected);
	KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::onConnectionState, onConnectionState);

	KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::onVersionNotMatch, onVersionNotMatch);
	KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::onScriptVersionNotMatch, onScriptVersionNotMatch);


	KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::onLoginBaseapp, onLoginBaseapp);
	KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::onLoginFailed, onLoginFailed);
	KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::onLoginBaseappFailed, onLoginBaseappFailed);
	KBENGINE_REGISTER_EVENT(KBEngine::KBEventTypes::onReloginBaseappSuccessfully, onReloginBaseappSuccessfully);
	KBENGINE_REGISTER_EVENT("onLoginSuccessfully", onLoginSuccessfully);
}

void UGameKBEMain::InitWithConfig()
{
	UE_LOG(LogTemp, Log, TEXT("KBEMain InitWithConfig"));
	if (!Config)
	{
		UE_LOG(LogTemp, Warning, TEXT("KBEMain InitWithConfig: Config is null"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("KBEMain Config IP: %s"), *Config->ip);


	auto kbemain = std::make_shared<KBEMain>();
	kbemain->ip = *Config->ip;
	kbemain->port = Config->port;
	kbemain->forceDisableUDP = Config->forceDisableUDP;
	kbemain->clientType = static_cast<EKCLIENT_TYPE>(Config->clientType);
	kbemain->networkEncryptType = static_cast<NETWORK_ENCRYPT_TYPE>(Config->networkEncryptType);
	kbemain->disableMainLoop = true;

	kbemain->init();

}

void UGameKBEMain::KBEngineEventPause()
{
	KBENGINE_EVENT_PAUSE();
}

void UGameKBEMain::KBEngineEventResume()
{
	KBENGINE_EVENT_RESUME();
}

void UGameKBEMain::addSpaceGeometryMapping(const std::shared_ptr<UKBEventData> pEventData)
{
	auto& pData = static_cast<const UKBEventData_addSpaceGeometryMapping&>(*pEventData);
	DEBUG_MSG("UGameKBEMain::addSpaceGeometryMapping: %s %s", *pData.eventName,*pData.spaceResPath);
	// 这里可以获取spaceResPath路径，加载进入对应地图，切换地图时请管理好event的resume和pause
	// AsyncTask(ENamedThreads::GameThread, [this]()
	// {
	// 这里先暂停事件触发，等待切换场景完成后，恢复事件并在对应的entity里创建相关组件
	KBEngineEventPause();
	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/Game/Scene/Lvl_World"));
	// });
}

void UGameKBEMain::onKicked(const std::shared_ptr<UKBEventData>& Shared)
{
	auto& pData = static_cast<const UKBEventData_onKicked&>(*Shared);
	FString Msg = FString::Printf(TEXT("kick, disconnect!, reason=%s"), UTF8_TO_TCHAR(KBEngine::KBEngineApp::getSingleton().serverErr(pData.failedcode).c_str()));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, Msg);
	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/Game/Scene/Lvl_Start"));
}

void UGameKBEMain::onDisconnected(const std::shared_ptr<UKBEventData>& Shared)
{
	FString Msg = FString::Printf(TEXT("disconnect! will try to reconnect...(你已掉线，可以在这里尝试重连!)"));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, Msg);
	// KBEngine::KBEngineApp::getSingleton().reloginBaseapp();
}

void UGameKBEMain::onConnectionState(const std::shared_ptr<UKBEventData>& Shared)
{
	auto& pData = static_cast<const UKBEventData_onConnectionState&>(*Shared);

	if (pData.success)
	{
		FString Msg = FString::Printf(TEXT("connect successfully, please wait...(连接成功，请等候...)"));
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Msg);
	}else
	{
		FString Msg = FString::Printf(TEXT("connect(%s:%d) is error! (连接错误)"),UTF8_TO_TCHAR(KBEngine::KBEngineApp::getSingleton().getInitArgs()->ip.c_str()),KBEngine::KBEngineApp::getSingleton().getInitArgs()->port);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, Msg);
	}
}

void UGameKBEMain::onVersionNotMatch(const std::shared_ptr<UKBEventData>& Shared)
{
	FString Msg = FString::Printf(TEXT("SDK 版本不匹配，请更新SDK！"));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, Msg);
}

void UGameKBEMain::onScriptVersionNotMatch(const std::shared_ptr<UKBEventData>& Shared)
{
	FString Msg = FString::Printf(TEXT("SDK 版本不匹配，请更新SDK！"));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, Msg);
}

void UGameKBEMain::onLoginBaseapp(const std::shared_ptr<UKBEventData>& Shared)
{
	FString Msg = FString::Printf(TEXT("connect to loginBaseapp, please wait...(连接到网关， 请稍后...)"));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Msg);
}

void UGameKBEMain::onLoginFailed(const std::shared_ptr<UKBEventData>& Shared)
{
	auto& pData = static_cast<const UKBEventData_onLoginFailed&>(*Shared);
	FString Msg = FString::Printf(TEXT("login is failed(登陆失败), err=%s"), UTF8_TO_TCHAR(KBEngine::KBEngineApp::getSingleton().serverErr(pData.failedcode).c_str()));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, Msg);
}

void UGameKBEMain::onLoginBaseappFailed(const std::shared_ptr<UKBEventData>& Shared)
{
	auto& pData = static_cast<const UKBEventData_onLoginBaseappFailed&>(*Shared);
	FString Msg = FString::Printf(TEXT("loginBaseapp is failed(登陆网关失败), err=%s"), UTF8_TO_TCHAR(KBEngine::KBEngineApp::getSingleton().serverErr(pData.failedcode).c_str()));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, Msg);
}

void UGameKBEMain::onReloginBaseappSuccessfully(const std::shared_ptr<UKBEventData>& Shared)
{
	FString Msg = FString::Printf(TEXT("relogin is successfully!(重连成功!)"));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Msg);
}

void UGameKBEMain::onLoginSuccessfully(const std::shared_ptr<UKBEventData>& Shared)
{
	FString Msg = FString::Printf(TEXT("login is successfully!(登陆成功!)"));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Msg);
}

bool UGameKBEMain::Tick(float DeltaTime)
{
	KBEngine::KBEngineApp::getSingleton().process();
	return true;
}
