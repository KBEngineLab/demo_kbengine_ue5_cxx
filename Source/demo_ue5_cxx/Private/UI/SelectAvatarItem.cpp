// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SelectAvatarItem.h"

#include "Components/TextBlock.h"
#include "UI/LoginWidget.h"

void USelectAvatarItem::Init(const KBEngine::AVATAR_INFOS& AvatarInfo)
{
	UTextBlock* ButtonText = NewObject<UTextBlock>(this);
	ButtonText->SetText(FText::FromString(AvatarInfo.name));
	ButtonText->SetJustification(ETextJustify::Center);

	FButtonStyle NewStyle = this->GetStyle();
	NewStyle.SetNormalPadding(FMargin(12.f, 5.f, 12.f, 5.f));
	NewStyle.SetPressedPadding(FMargin(10.f, 4.f, 10.f, 4.f));
	this->SetStyle(NewStyle);
	this->AddChild(ButtonText);

	this->AvatarInfo_ = AvatarInfo;

	this->OnClicked.AddDynamic(this, &USelectAvatarItem::OnBtnClicked);
}

USelectAvatarItem::~USelectAvatarItem()
{
	this->OnClicked.RemoveDynamic(this, &USelectAvatarItem::OnBtnClicked);
}

void USelectAvatarItem::OnBtnClicked()
{
	FString Name = UTF8_TO_TCHAR(AvatarInfo_.name.c_str());
	UE_LOG(LogTemp, Log, TEXT("AvatarItem clicked! %s %llu"),*Name,AvatarInfo_.dbid);

	FString Msg = FString::Printf(TEXT("AvatarItem clicked! %s %llu"),*Name,AvatarInfo_.dbid);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Msg);
	ULoginWidget::Instance->SelectAvatarDBID = AvatarInfo_.dbid;
}
