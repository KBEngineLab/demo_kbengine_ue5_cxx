// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "../kbengine_cxx_plugins/KBECommon.h"

#include "../kbengine_cxx_plugins/NPCBase.h"

class ANPCCharacter;
class UKBEventData;

namespace KBEngine
{

class NPC : public NPCBase
{
public:
	NPC();
	virtual ~NPC();

	void __init__() override;

	virtual void onDestroy() override;

	virtual void onEnterWorld() override;
	virtual void onLeaveWorld() override;
	virtual void onEnterSpace() override;
	virtual void onLeaveSpace() override;
	void onEnterWorldCallback(std::shared_ptr<UKBEventData> pEventData);


	ANPCCharacter* NPCActor;
};

}
