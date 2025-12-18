#include "Avatar.h"
#include "../kbengine_cxx_plugins/Entity.h"
#include "LogicEvents.h"
#include "WorldGameMode.h"

#include "../kbengine_cxx_plugins/EntityFactory.h"
#include "Character/AvatarCharacter.h"
#include "Character/PlayerCharacter.h"


namespace KBEngine
{

Avatar::Avatar():
	AvatarBase(),
	PlayerCharacter(nullptr),
	AvatarCharacter(nullptr)
{
}

Avatar::~Avatar()
{
}

void Avatar::__init__()
{
	std::string EventName =
		std::string("onAvatarEnterSpaceCallback_") + std::to_string(id());

	KBENGINE_REGISTER_EVENT(
		EventName,
		onAvatarEnterSpaceCallback
	);

	EventName =
		std::string("onAvatarEnterWorldCallback_") + std::to_string(id());

	KBENGINE_REGISTER_EVENT(
		EventName,
		onAvatarEnterSpaceCallback
	);

	// 注册事件
	// if (isPlayer())
	// {
	// 	KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("relive", "relive", [this](std::shared_ptr<UKBEventData> pEventData)
	// 	{
	// 		const UKBEventData_reqRelive& data = static_cast<const UKBEventData_reqRelive&>(*pEventData);
	// 		reqRelive(data.reliveType);
	// 	});
	// }
}

void Avatar::onAvatarEnterSpaceCallback(std::shared_ptr<UKBEventData> pEventData)
{
	// 延迟到下一帧或 GameThread
	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		if (PlayerCharacter || AvatarCharacter) return;
		DEBUG_MSG("Avatar::onAvatarEnterSpaceCallback(): onAvatarEnterSpaceCallback");
		if (isPlayer())
		{
			UWorld* World = AWorldGameMode::Instance->GetWorld();
			AWorldGameMode* GM = World->GetAuthGameMode<AWorldGameMode>();
			if (!GM)
				return;

			APlayerController* PC = GM->GetMainPlayerController();
			if (!PC)
			{
				UE_LOG(LogTemp, Error, TEXT("PlayerController not ready"));
				return;
			}

			FVector SpawnLocation = FVector(position.z * 100.f, position.x * 100.f, 100.f);
			// FVector SpawnLocation = FVector(0, 0, 300);
			FRotator SpawnRotation = FRotator::ZeroRotator;

			FActorSpawnParameters Params;
			Params.Owner = PC;
			Params.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			APlayerCharacter* Player =
				World->SpawnActor<APlayerCharacter>(
					AWorldGameMode::Instance->PlayerBlueprintClass,
					SpawnLocation,
					SpawnRotation,
					Params
				);

			if (Player)
			{
				PC->Possess(Player);
				PlayerCharacter = Player;
				PlayerCharacter->AvatarEntity = this;
				PlayerCharacter->UpdateHeadInfo();
			}
		}else
		{
			UWorld* World = AWorldGameMode::Instance->GetWorld();

			FVector SpawnLocation(position.z * 100.f, position.x * 100.f, 100.f); // 可以根据 NPC 位置设置
			FRotator SpawnRotation(0.f, 0.f, 0.f);

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			SpawnParams.Name = FName(*FString::Printf(TEXT("Monster_%d"), id()));
			// 生成蓝图 NPC
			AvatarCharacter = World->SpawnActor<AAvatarCharacter>(AWorldGameMode::Instance->AvatarBlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);
			AvatarCharacter->AvatarEntity = this;
			AvatarCharacter->UpdateHeadInfo();
		}
	});

}


void Avatar::reqRelive(uint8 reliveType)
{
	DEBUG_MSG("Avatar::reqRelive(): reqRelive");
	pCellEntityCall->relive(reliveType);
}

void Avatar::onDestroy()
{
	AvatarBase::onDestroy();

	// 注销注册的所有事件
	// KBENGINE_DEREGISTER_ALL_EVENT();

	std::string EventName =
	std::string("onAvatarEnterSpaceCallback_") + std::to_string(id());
	KBENGINE_DEREGISTER_EVENT(EventName);


	EventName =
	std::string("onAvatarEnterWorldCallback_") + std::to_string(id());
	KBENGINE_DEREGISTER_EVENT(EventName);
}

void Avatar::onEnterWorld()
{
	UE_LOG(LogTemp, Log, TEXT("Avatar::onEnterWorld(): onEnterWorld %d"),this->id_);
	AvatarBase::onEnterWorld();

	if (!isPlayer())
	{
		std::string EventName = std::string("onAvatarEnterWorldCallback_") + std::to_string(id());
		KBENGINE_EVENT_FIRE(EventName,nullptr);
	}
}

// 玩家自己：onEnterWorld → addSpaceGeometryMapping → onEnterSpace
// 其他实体：addSpaceGeometryMapping → onEnterWorld
void Avatar::onLeaveWorld()
{
	AvatarBase::onLeaveWorld();


}

// 玩家自己：onEnterWorld → addSpaceGeometryMapping → onEnterSpace
// 其他实体：addSpaceGeometryMapping → onEnterWorld
void Avatar::onEnterSpace()
{
	UE_LOG(LogTemp, Log, TEXT("Avatar::onEnterSpace(): onEnterSpace %d"),this->id_);
	AvatarBase::onEnterSpace();


	if (isPlayer())
	{
		std::string EventName = std::string("onAvatarEnterSpaceCallback_") + std::to_string(id());
		// 此时关卡可能并未跳转成功，放在事件里处理
		KBENGINE_EVENT_FIRE(EventName,nullptr);
	}
}

void Avatar::onLeaveSpace()
{
	AvatarBase::onLeaveSpace();
}

void Avatar::onPositionChanged(const KBVector3f& oldValue)
{
	AvatarBase::onPositionChanged(oldValue);
	if (isPlayer())
	{
		if (PlayerCharacter)
		{
			PlayerCharacter->SetActorLocation(FVector(position.z * 100.f, position.x * 100.f, 100.f));
		}
	}else
	{
		if (AvatarCharacter)
		{
			AvatarCharacter->SetActorLocation(FVector(position.z * 100.f, position.x * 100.f, 100.f));
		}
	}
}

void Avatar::onSmoothPositionChanged(const KBVector3f& oldValue)
{
	AvatarBase::onSmoothPositionChanged(oldValue);

	if (isPlayer())
	{

	}else
	{
		if (AvatarCharacter)
		{
			AvatarCharacter->SetActorLocation(FVector(position.z * 100.f, position.x * 100.f, 100.f));
		}
	}
}

void Avatar::onDirectionChanged(const KBVector3f& oldValue)
{
	AvatarBase::onDirectionChanged(oldValue);

	if (isPlayer())
	{

	}else
	{
		FRotator rotator = FRotator(FMath::RadiansToDegrees<float>(direction.y),FMath::RadiansToDegrees<float>(direction.z),FMath::RadiansToDegrees<float>(direction.x));
		if (AvatarCharacter) AvatarCharacter->SetActorRotation(rotator);
	}

}

void Avatar::onAddSkill(int32 arg1)
{

}

void Avatar::onJump()
{

}

void Avatar::onRemoveSkill(int32 arg1)
{

}

void Avatar::recvDamage(int32 arg1, int32 arg2, int32 arg3, int32 arg4)
{

	auto pEventData = std::make_shared<UKBEventData_recvDamage>();
	pEventData->attackerID = arg1;
	pEventData->skillID = arg2;
	pEventData->damageType = arg3;
	pEventData->damage = arg4;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("recvDamage", pEventData);
}

void Avatar::onHPChanged(int32 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_HP>();
	pEventData->HP = HP;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_HP", pEventData);
}

void Avatar::onHP_MaxChanged(int32 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_HP_Max>();
	pEventData->HP_Max = HP_Max;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_HP_Max", pEventData);
}

void Avatar::onMPChanged(int32 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_MP>();
	pEventData->MP = MP;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_MP", pEventData);
}

void Avatar::onMP_MaxChanged(int32 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_MP_Max>();
	pEventData->MP_Max = MP_Max;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_MP_Max", pEventData);
}

void Avatar::onForbidsChanged(int32 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_forbids>();
	pEventData->forbids = forbids;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_forbids", pEventData);
}

void Avatar::onLevelChanged(uint16 oldValue)
{

}

void Avatar::onModelIDChanged(uint32 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_modelID>();
	pEventData->modelID = modelID;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_modelID", pEventData);
}

void Avatar::onModelScaleChanged(uint8 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_modelScale>();
	pEventData->modelScale = modelScale;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_modelScale", pEventData);
}

void Avatar::onMoveSpeedChanged(uint8 oldValue)
{
	velocity(moveSpeed);

	auto pEventData = std::make_shared<UKBEventData_set_moveSpeed>();
	pEventData->moveSpeed = moveSpeed;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_moveSpeed", pEventData);
}

void Avatar::onNameChanged(const KBString& oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_name>();
	pEventData->name = name;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_name", pEventData);
}

void Avatar::onOwn_valChanged(uint16 oldValue)
{

}

void Avatar::onSpaceUTypeChanged(uint32 oldValue)
{

}

void Avatar::onStateChanged(int8 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_state>();
	pEventData->state = state;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_state", pEventData);
}

void Avatar::onSubStateChanged(uint8 oldValue)
{
	auto pEventData = std::make_shared<UKBEventData_set_subState>();
	pEventData->subState = subState;
	pEventData->entityID = id();
	KBENGINE_EVENT_FIRE("set_subState", pEventData);
}

void Avatar::onUidChanged(uint32 oldValue)
{

}

void Avatar::onUtypeChanged(uint32 oldValue)
{

}

}


// 静态注册
namespace {
	const bool registered = []() {
		EntityFactory::instance().registerType("Avatar", []() {
			return new KBEngine::Avatar();
		});
		return true;
	}();
}