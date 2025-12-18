// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KBETypes.h"
#include "Components/Button.h"
#include "SelectAvatarItem.generated.h"


/**
 * 
 */
UCLASS()
class DEMO_UE5_CXX_API USelectAvatarItem : public UButton
{
	GENERATED_BODY()

public:

	void Init(const KBEngine::AVATAR_INFOS& AvatarInfo);

	virtual ~USelectAvatarItem() override;


	UFUNCTION()
	void OnBtnClicked();
	/**
	 * 当前选择的角色列表项
	 */
	KBEngine::AVATAR_INFOS AvatarInfo_;
};
