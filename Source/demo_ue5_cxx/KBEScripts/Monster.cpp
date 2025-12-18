#include "Monster.h"
#include "../kbengine_cxx_plugins/Entity.h"
#include "LogicEvents.h"
#include "WorldGameMode.h"

#include "../kbengine_cxx_plugins/EntityFactory.h"
#include "Character/MonsterCharacter.h"


namespace KBEngine
{

Monster::Monster():
	MonsterBase(),
	MonsterActor(nullptr)
{
}

Monster::~Monster()
{
}

void Monster::__init__()
{
	std::string EventName =
		std::string("onMonsterEnterWorldCallback_") + std::to_string(id());

	KBENGINE_REGISTER_EVENT(
		EventName,
		onEnterWorldCallback
	);
}

void Monster::recvDamage(int32 arg1, int32 arg2, int32 arg3, int32 arg4) {
}

void Monster::onDestroy()
{
	MonsterBase::onDestroy();

	std::string EventName =
	std::string("onMonsterEnterWorldCallback_") + std::to_string(id());
	KBENGINE_DEREGISTER_EVENT(EventName);
}

void Monster::onLeaveWorld()
{
	MonsterBase::onLeaveWorld();

	if(MonsterActor)
	{
		MonsterActor->Destroy();
		MonsterActor = nullptr;
	}
}

void Monster::onLeaveSpace()
{
	MonsterBase::onLeaveSpace();
	if(MonsterActor)
	{
		MonsterActor->Destroy();
		MonsterActor = nullptr;
	}
}

void Monster::onEnterWorld()
{
	MonsterBase::onEnterWorld();

	std::string EventName =
	std::string("onMonsterEnterWorldCallback_") + std::to_string(id());
	// 此时关卡可能并未跳转成功，放在事件里处理
	KBENGINE_EVENT_FIRE(EventName,nullptr);
}

void Monster::onEnterSpace()
{
	MonsterBase::onEnterSpace();
}


void Monster::onEnterWorldCallback(const std::shared_ptr<UKBEventData> pEventData)
{
	UWorld* World = AWorldGameMode::Instance->GetWorld();

	FVector SpawnLocation(position.z * 100.f, position.x * 100.f, 100.f); // 可以根据 NPC 位置设置
	FRotator SpawnRotation(0.f, 0.f, 0.f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Name = FName(*FString::Printf(TEXT("Monster_%d"), id()));
	// 生成蓝图 NPC
	MonsterActor = World->SpawnActor<AMonsterCharacter>(AWorldGameMode::Instance->MonsterBlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);
	MonsterActor->MonsterEntity = this;
	MonsterActor->UpdateHeadInfo();

}

void Monster::onPositionChanged(const KBVector3f& oldValue)
{
	MonsterBase::onPositionChanged(oldValue);

	if (MonsterActor) MonsterActor->SetActorLocation(FVector(position.z * 100.f, position.x * 100.f, 100.f));
}

void Monster::onSmoothPositionChanged(const KBVector3f& oldValue)
{
	MonsterBase::onSmoothPositionChanged(oldValue);
	// if (MonsterActor) MonsterActor->SetActorLocation(FVector(position.z * 100.f, position.x * 100.f, 100.f));
	if (MonsterActor)
	{
		MonsterActor->SetMoveSpeed(moveSpeed*100);
		MonsterActor->SetMoveTarget(FVector(position.z * 100.f, position.x * 100.f, 100.f));
	}
}

void Monster::onDirectionChanged(const KBVector3f& oldValue)
{
	MonsterBase::onDirectionChanged(oldValue);
	FRotator rotator = FRotator(FMath::RadiansToDegrees<float>(direction.y),FMath::RadiansToDegrees<float>(direction.z),FMath::RadiansToDegrees<float>(direction.x));
	if (MonsterActor) MonsterActor->SetActorRotation(rotator);
}

void Monster::onMoveSpeedChanged(uint8 oldValue)
{
	MonsterBase::onMoveSpeedChanged(oldValue);
	if (MonsterActor)
	{
		MonsterActor->SetMoveSpeed(moveSpeed*100);
	}
}
}


// 静态注册
namespace {
	const bool registeredMonster = []() {
		EntityFactory::instance().registerType("Monster", []() {
			return new KBEngine::Monster();
		});
		return true;
	}();
}