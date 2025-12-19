// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"


namespace KBEngine
{
	class AVATAR_INFOS;
	class AVATAR_INFOS_LIST;
}

class UCanvasPanel;
class UVerticalBox;


/**
 * Login UI
 */
UCLASS()
class DEMO_UE5_CXX_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	static ULoginWidget* Instance;


	//==============================Login相关=====================================
	UPROPERTY( BlueprintReadWrite ,meta = (BindWidget))
	UCanvasPanel* LoginPlane;



	//==============================SelectAvatar相关=====================================
	UPROPERTY(BlueprintReadWrite ,meta = (BindWidget))
	UCanvasPanel* SelectAvatarPlane;

	UPROPERTY( BlueprintReadWrite ,meta = (BindWidget))
	UVerticalBox* AvatarListBox;

	/**
	 * 显示更新角色列表
	 */
	void UIShowSelectAvatarPlane(const KBEngine::AVATAR_INFOS_LIST& AvatarInfoList);


	//==============================Create相关=====================================
	UPROPERTY( BlueprintReadWrite ,meta = (BindWidget))
	UCanvasPanel* CreatePlane;

	/**
	 * 创建角色
	*/
	UFUNCTION(BlueprintCallable)
	void UICreateAvatarBtnClicked(FString Username);

	/**
	 * 删除角色
	*/
	UFUNCTION(BlueprintCallable)
	void UIRemoveAvatarBtnClicked();

	/**
	 * 进入游戏
	*/
	UFUNCTION(BlueprintCallable)
	void UIEnterGameBtnClicked();


	/**
	 * 进入游戏
	*/
	UFUNCTION(BlueprintCallable)
	void UILoginBtnClicked(FString username, FString password, TArray<uint8> datas);


	uint64 SelectAvatarDBID;


protected:
	virtual void NativeConstruct() override;

};
