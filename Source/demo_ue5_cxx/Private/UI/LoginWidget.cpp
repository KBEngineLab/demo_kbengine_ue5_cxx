// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoginWidget.h"

#include "KBEngine.h"
#include "KBETypes.h"
#include "KBEvent.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "KBEScripts/Account.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SelectAvatarItem.h"

ULoginWidget* ULoginWidget::Instance = nullptr;


void ULoginWidget::UIShowSelectAvatarPlane(const KBEngine::AVATAR_INFOS_LIST& AvatarInfoList)
{
	// AsyncTask(ENamedThreads::GameThread, [this, AvatarInfoList]()
	// {
		if (!AvatarListBox) return;
		const UWorld* World = GetWorld();
		if ( !World) return;

		LoginPlane->SetVisibility(ESlateVisibility::Hidden);
		SelectAvatarPlane->SetVisibility(ESlateVisibility::Visible);
		CreatePlane->SetVisibility(ESlateVisibility::Hidden);

		for (UWidget* Child : AvatarListBox->GetAllChildren())
		{
			if (UButton* Btn = Cast<UButton>(Child))
			{
				// 移除所有 OnClicked 动态绑定事件
				Btn->OnClicked.Clear();
			}
		}

		AvatarListBox->ClearChildren();

		for (auto& characterInfoItem : AvatarInfoList.values)
		{
			USelectAvatarItem* NewButton = NewObject<USelectAvatarItem>();
			NewButton->Init(characterInfoItem);

			UVerticalBoxSlot* VerticalSlot = AvatarListBox->AddChildToVerticalBox(NewButton);
			if (VerticalSlot)
			{
				// 只改 Bottom
				VerticalSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 30.f));
			}

		}
	// });
}


void ULoginWidget::UICreateAvatarBtnClicked(FString Username)
{
	UE_LOG(LogTemp, Log, TEXT("CreateAvatarBtn clicked!: Username:%s"),*Username);

	FString Msg = FString::Printf(TEXT("CreateAvatarBtn clicked!: Username:%s"),*Username);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Msg);

	KBEngine::Account::Instance->pBaseEntityCall->reqCreateAvatar(1,Username);
}

void ULoginWidget::UIRemoveAvatarBtnClicked()
{
	UE_LOG(LogTemp, Log, TEXT("RemoveAvatarBtn clicked! dbid:%llu"),SelectAvatarDBID);

	if (SelectAvatarDBID == 0)
	{
		FString Msg = FString::Printf(TEXT("请选择角色"));
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Msg);
		return;
	}

	KBEngine::Account::Instance->pBaseEntityCall->reqRemoveAvatarDBID(SelectAvatarDBID);

}

void ULoginWidget::UIEnterGameBtnClicked()
{
	DEBUG_MSG("Account::selectAvatarGame(): name=%lld", SelectAvatarDBID);
	if (SelectAvatarDBID == 0)
	{
		FString Msg = FString::Printf(TEXT("请选择角色"));
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Msg);
		return;
	}

	// 这里偷懒直接进入新场景，实际上应该在addSpaceGeometryMapping的事件中去创建跳转场景
	// UGameplayStatics::OpenLevel(GetWorld(), "GameWorld");
	KBEngine::Account::Instance->pBaseEntityCall->selectAvatarGame(SelectAvatarDBID);
}

void ULoginWidget::UILoginBtnClicked(FString username, FString password, TArray<uint8> datas)
{
	if (!KBEngine::KBEngineApp::getSingleton().isInitialized())
	{
		return ;
	}
	DEBUG_MSG("UGameKBEMain::login");
	KBEngine::KBEngineApp::getSingleton().reset();

	auto pEventData = std::make_shared<UKBEventData_login>();
	pEventData->username = username;
	pEventData->password = password;
	pEventData->datas = datas;
	KBENGINE_EVENT_FIRE(KBEngine::KBEventTypes::login, pEventData);
}

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Instance = this;
}
