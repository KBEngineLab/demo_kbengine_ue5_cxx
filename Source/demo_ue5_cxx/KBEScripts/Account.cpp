#include "Account.h"

#include "AvatarBase.h"
#include "../kbengine_cxx_plugins/Entity.h"
#include "../kbengine_cxx_plugins/KBEngine.h"
#include "../kbengine_cxx_plugins/KBDebug.h"
#include "LogicEvents.h"
#include "../kbengine_cxx_plugins/EntityFactory.h"
#include "UI/LoginWidget.h"


namespace KBEngine
{
Account* Account::Instance = nullptr;
Account::Account():
	AccountBase(),
	characters()
{
	if (Instance == nullptr)
	{
		Instance = this;
	}
}

Account::~Account()
{
	if (Instance == this)
	{
		Instance = nullptr;
	}
}

void Account::__init__()
{
	// 注册事件
	KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("reqCreateAvatar", "reqCreateAvatar", [this](std::shared_ptr<UKBEventData> pEventData)
	{
		const UKBEventData_reqCreateAvatar& data = static_cast<const UKBEventData_reqCreateAvatar&>(*pEventData);
		reqCreateAvatar(data.roleType, data.name);
	});

	KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("reqRemoveAvatar", "reqRemoveAvatar", [this](std::shared_ptr<UKBEventData> pEventData)
	{
		const UKBEventData_reqRemoveAvatar& data = static_cast<const UKBEventData_reqRemoveAvatar&>(*pEventData);
		reqRemoveAvatar(data.avatarInfos.dbid);
	});

	KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC("selectAvatarGame", "selectAvatarGame", [this](std::shared_ptr<UKBEventData> pEventData)
	{
		const UKBEventData_selectAvatarGame& data = static_cast<const UKBEventData_selectAvatarGame&>(*pEventData);
		selectAvatarGame(data.avatarInfos.dbid);
	});

	// 触发登陆成功事件
	// UKBEventData_onLoginSuccessfully* pEventData = NewObject<UKBEventData_onLoginSuccessfully>();
	auto pEventData = std::make_shared<UKBEventData_onLoginSuccessfully>();
	pEventData->entity_uuid = KBEngineApp::getSingleton().entity_uuid();
	pEventData->entity_id = id();
	KBENGINE_EVENT_FIRE("onLoginSuccessfully", pEventData);

	// 向服务端请求获得角色列表
	pBaseEntityCall->reqAvatarList();

}

void Account::onDestroy()
{
	// 注销注册的所有事件
	KBENGINE_DEREGISTER_ALL_EVENT();

	AccountBase::onDestroy();
}

void Account::onLastSelCharacterChanged(uint64 oldValue)
{

}

void Account::reqCreateAvatar(uint8 roleType, const KBString& name)
{
	DEBUG_MSG("Account::reqCreateAvatar(): roleType=%d", roleType);
	pBaseEntityCall->reqCreateAvatar(roleType, name);
}

void Account::reqRemoveAvatar(uint64 dbid)
{
	DEBUG_MSG("Account::reqRemoveAvatar(): dbid=%lld", dbid);
	pBaseEntityCall->reqRemoveAvatarDBID(dbid);
}

void Account::selectAvatarGame(uint64 dbid)
{
	DEBUG_MSG("Account::selectAvatarGame(): dbid=%lld", dbid);
	pBaseEntityCall->selectAvatarGame(dbid);
}

void Account::onReqAvatarList(const AVATAR_INFOS_LIST& datas)
{
	DEBUG_MSG("Account::onReqAvatarList(): datas.size()=%d", datas.values.Num());
	ULoginWidget::Instance->SelectAvatarDBID = 0;
	ULoginWidget::Instance->UIShowSelectAvatarPlane(datas);
	// auto pEventData = std::make_shared<UKBEventData_onReqAvatarList>();
	// for (auto& characterInfoItem : datas.values)
	// {
	// 	FAVATAR_INFOS event_avatar;
	//
	// 	const AVATAR_INFOS& characterInfo_fixed_dict = characterInfoItem;
	//
	// 	DEBUG_MSG("Account::onReqAvatarList(): name=%s, dbid=%lld, level=%d, roleType=%d", characterInfo_fixed_dict.name.c_str(), characterInfo_fixed_dict.dbid, characterInfo_fixed_dict.level, characterInfo_fixed_dict.roleType);
	//
	// 	AVATAR_INFOS infos;
	//
	// 	infos.name = characterInfo_fixed_dict.name;
	// 	infos.dbid = characterInfo_fixed_dict.dbid;
	// 	infos.level = characterInfo_fixed_dict.level;
	// 	infos.roleType = characterInfo_fixed_dict.roleType;
	//
	// 	const AVATAR_DATA& data_fixed_dict = characterInfo_fixed_dict.data;
	//
	// 	infos.data.param1 = data_fixed_dict.param1;
	// 	infos.data.param2 = data_fixed_dict.param2;
	//
	// 	characters.values.Add(infos);
	//
	// 	// fill eventData
	// 	event_avatar.set(infos.dbid, infos.name, infos.roleType, infos.level, (lastSelCharacter == infos.dbid));
	// 	pEventData->avatars.Add(event_avatar);
	// }
	//
	// KBENGINE_EVENT_FIRE("onReqAvatarList", pEventData);
}

void Account::onCreateAvatarResult(uint8 retcode, const AVATAR_INFOS& info)
{
	DEBUG_MSG("Account::onCreateAvatarResult(): retcode=%d", retcode);
	if (retcode == 0)
	{
		pBaseEntityCall->reqAvatarList();
	}else
	{
		UE_LOG(LogTemp, Log, TEXT("Account::onCreateAvatarResult(): retcode=%d"),retcode);
		if (retcode == 3){
			FString Msg = FString::Printf(TEXT("无法创建更多的角色"));
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Msg);
		}

	}
	// auto pEventData = std::make_shared<UKBEventData_onCreateAvatarResult>();
	// AVATAR_INFOS infos;
	//
	// infos.name = info.name;
	// infos.dbid = info.dbid;
	// infos.level = info.level;
	// infos.roleType = info.roleType;
	//
	// const AVATAR_DATA& data_fixed_dict = info.data;
	//
	// infos.data.param1 = data_fixed_dict.param1;
	// infos.data.param2 = data_fixed_dict.param2;
	//
	// if(retcode == 0)
	// 	characters.values.Add(infos);
	//
	// // fill eventData
	// pEventData->avatarInfos.set(infos.dbid, infos.name, infos.roleType, infos.level, (lastSelCharacter == infos.dbid));
	// pEventData->errorCode = retcode;
	//
	// // Error codes given by Account::reqCreateAvatar on the server
	// if (retcode > 0)
	// {
	// 	if (retcode == 3)
	// 		pEventData->errorStr = KBTEXT("Limiting the number of characters!");
	// 	else
	// 		pEventData->errorStr = KBTEXT("Unknown error!");
	// }
	//
	// KBENGINE_EVENT_FIRE("onCreateAvatarResult", pEventData);
}

void Account::onRemoveAvatar(uint64 dbid)
{
	DEBUG_MSG("Account::onRemoveAvatar(): dbid=%lld", dbid);
	pBaseEntityCall->reqAvatarList();
	// int infosFind = -1;
	// for (int32 index = 0; index != characters.values.Num(); ++index)
	// {
	// 	if(characters.values[index].dbid == dbid)
	// 		infosFind = index;
	// }
	//
	// if (infosFind < 0)
	// 	return;
	//
	// AVATAR_INFOS infos = characters.values[infosFind];
	//
	// // ui event
	//
	// auto pEventData = std::make_shared<UKBEventData_onRemoveAvatar>();
	// pEventData->avatarInfos.set(infos.dbid, infos.name, infos.roleType, infos.level, (lastSelCharacter == infos.dbid));
	// KBENGINE_EVENT_FIRE("onRemoveAvatar", pEventData);
	//
	// characters.values.RemoveAt(infosFind);
}

}


// 静态注册
namespace {
	const bool registered = []() {
		EntityFactory::instance().registerType("Account", []() {
			return new KBEngine::Account();
		});
		return true;
	}();
}