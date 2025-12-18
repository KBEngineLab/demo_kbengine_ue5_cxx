#include "NPC.h"

#include "../kbengine_cxx_plugins/Entity.h"
#include "LogicEvents.h"
#include "WorldGameMode.h"

#include "../kbengine_cxx_plugins/EntityFactory.h"
#include "Character/NPCCharacter.h"

class ANPCCharacter;

namespace KBEngine
{

NPC::NPC():
	NPCBase()
{

}

NPC::~NPC()
{

}



void NPC::__init__()
{
	std::string EventName =
	std::string("onNPCEnterWorldCallback_") + std::to_string(id());

	KBENGINE_REGISTER_EVENT(
		EventName,
		onEnterWorldCallback
	);
}

void NPC::onDestroy()
{
	NPCBase::onDestroy();
	std::string EventName =
	std::string("onNPCEnterWorldCallback_") + std::to_string(id());
	KBENGINE_DEREGISTER_EVENT(EventName);
}

void NPC::onEnterWorld()
{
	NPCBase::onEnterWorld();
	std::string EventName =
	std::string("onNPCEnterWorldCallback_") + std::to_string(id());
	// 此时关卡可能并未跳转成功，放在事件里处理
	KBENGINE_EVENT_FIRE(EventName,nullptr);
}

void NPC::onLeaveWorld()
{
	NPCBase::onLeaveWorld();
	if(NPCActor)
	{
		NPCActor->Destroy();
		NPCActor = nullptr;
	}
}

void NPC::onEnterSpace()
{
	NPCBase::onEnterSpace();
	std::string EventName =
		std::string("onNPCEnterWorldCallback_") + std::to_string(id());

	KBENGINE_REGISTER_EVENT(
		EventName,
		onEnterWorldCallback
	);
}

void NPC::onLeaveSpace()
{
	NPCBase::onLeaveSpace();
	if(NPCActor)
	{
		NPCActor->Destroy();
		NPCActor = nullptr;
	}
}

void NPC::onEnterWorldCallback(const std::shared_ptr<UKBEventData> pEventData)
{
	auto& pData = static_cast<const UKBENPCEnterWorldEventData&>(*pEventData);
	DEBUG_MSG("NPC::onEnterWorldCallback ");


	UWorld* World = AWorldGameMode::Instance->GetWorld();



	FVector SpawnLocation(position.z * 100.f, position.x * 100.f, 100.f); // 可以根据 NPC 位置设置
	FRotator SpawnRotation(0.f, 0.f, 0.f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Name = FName(*FString::Printf(TEXT("NPC_%d"), id()));
	// 生成蓝图 NPC
	NPCActor = World->SpawnActor<ANPCCharacter>(AWorldGameMode::Instance->NPCBlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);
	NPCActor->NPCEntity = this;
	NPCActor->UpdateHeadInfo();
}
}


// 静态注册
namespace {
	const bool registeredNPC = []() {
		EntityFactory::instance().registerType("NPC", []() {
			return new KBEngine::NPC();
		});
		return true;
	}();
}