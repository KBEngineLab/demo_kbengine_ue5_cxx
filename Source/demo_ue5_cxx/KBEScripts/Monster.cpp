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
	int32 RandomNum = FMath::RandRange(100, 999);
	actorId = FString::FromInt(RandomNum);
}

Monster::~Monster()
{
}

void Monster::__init__()
{
	std::string EventName =
		std::string("onMonsterEnterWorldCallback_") + std::to_string(id()) + "_" + TCHAR_TO_UTF8(*actorId);

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
	std::string("onMonsterEnterWorldCallback_") + std::to_string(id()) + "_" + TCHAR_TO_UTF8(*actorId);
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
	std::string("onMonsterEnterWorldCallback_") + std::to_string(id()) + "_" + TCHAR_TO_UTF8(*actorId);
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

	FVector SpawnLocation(position.x * 100.f, position.z * 100.f, position.y * 100.f + 500.f); // 可以根据 NPC 位置设置
	FRotator SpawnRotation(0.f, 0.f, 0.f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// SpawnParams.Name = FName(*FString::Printf(TEXT("Monster__%d__%hs"), id(), actorId.c_str()));
	// 生成蓝图 NPC
	MonsterActor = World->SpawnActor<AMonsterCharacter>(AWorldGameMode::Instance->MonsterBlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);
	MonsterActor->MonsterEntity = this;
	MonsterActor->UpdateHeadInfo();
	MonsterActor->SetActorLabel(
		FString::Printf(TEXT("Monster__%d__"), id()) + TCHAR_TO_UTF8(*actorId)
	);

	FVector TargetPos(
		position.x * 100.f,
		position.z * 100.f,
		position.y * 100.f + 500.f   // 从上方开始
	);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MonsterActor);

	bool bHit = MonsterActor->GetWorld()->LineTraceSingleByChannel(
		Hit,
		TargetPos,
		TargetPos - FVector(0, 0, 2000.f),   // 向下
		ECC_Visibility,
		Params
	);

	if (bHit)
	{
		TargetPos.Z = Hit.ImpactPoint.Z + 87.f; // 胶囊底到脚
	}
	MonsterActor->SetActorLocation(TargetPos, false);
}

void Monster::onPositionChanged(const KBVector3f& oldValue)
{
	MonsterBase::onPositionChanged(oldValue);



	// if (MonsterActor) MonsterActor->SetActorLocation(FVector(position.x * 100.f, position.z * 100.f, position.y * 100.f + 87.f - 30.f));
	if (MonsterActor)
	{
		FVector TargetPos(
			position.x * 100.f,
			position.z * 100.f,
			position.y * 100.f + 500.f   // 从上方开始
		);

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(MonsterActor);

		bool bHit = MonsterActor->GetWorld()->LineTraceSingleByChannel(
			Hit,
			TargetPos,
			TargetPos - FVector(0, 0, 2000.f),   // 向下
			ECC_Visibility,
			Params
		);
		if (bHit)
		{
			TargetPos.Z = Hit.ImpactPoint.Z + 87.f; // 胶囊底到脚
		}

		MonsterActor->SetActorLocation(TargetPos, false);
	}
}

void Monster::onSmoothPositionChanged(const KBVector3f& oldValue)
{
	MonsterBase::onSmoothPositionChanged(oldValue);
	// if (MonsterActor) MonsterActor->SetActorLocation(FVector(position.x * 100.f, position.z * 100.f, position.y * 100.f + 87.f - 30.f));
	if (MonsterActor)
	{
		FVector TargetPos(
			position.x * 100.f,
			position.z * 100.f,
			position.y * 100.f + 500.f   // 从上方开始
		);

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(MonsterActor);

		bool bHit = MonsterActor->GetWorld()->LineTraceSingleByChannel(
			Hit,
			TargetPos,
			TargetPos - FVector(0, 0, 2000.f),   // 向下
			ECC_Visibility,
			Params
		);
		if (bHit)
		{
			TargetPos.Z = Hit.ImpactPoint.Z + 87.f; // 胶囊底到脚
		}

		MonsterActor->SetActorLocation(TargetPos, false);
	}
}

void Monster::onDirectionChanged(const KBVector3f& oldValue)
{
	MonsterBase::onDirectionChanged(oldValue);
	// FRotator rotator = FRotator(FMath::RadiansToDegrees<float>(direction.y),FMath::RadiansToDegrees<float>(-direction.z),FMath::RadiansToDegrees<float>(direction.x));
	// if (MonsterActor) MonsterActor->SetActorRotation(rotator);

	if (MonsterActor)
	{
		float RollDeg = -FMath::RadiansToDegrees(direction.x);             // Roll 取反
		float PitchDeg = -FMath::RadiansToDegrees(direction.y);             // Pitch 取反
		float YawDeg = -FMath::RadiansToDegrees(direction.z) + 90.f;      // Yaw 取反 + 坐标轴偏移

		FRotator rotator(PitchDeg, YawDeg, RollDeg);

		if (MonsterActor)
		{
			MonsterActor->SetActorRotation(rotator);
		}
	}
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