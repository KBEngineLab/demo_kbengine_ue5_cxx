// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KBECharacterBase.generated.h"

UCLASS()
class DEMO_UE5_CXX_API AKBECharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKBECharacterBase();

	// 外部（服务器 / KBE）设置目标点
	void SetMoveTarget(const FVector& InTarget)
	{
		MoveTarget = InTarget;
		bHasMoveTarget = true;
		CurrentVelocity = FVector::ZeroVector;
	}

	void SetMoveSpeed(float InMoveSpeed);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BP_UpdateHeadInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="KBEngine")
	FString EntityName;

protected:


	// 平滑移动到目标点
	void SmoothMoveToTarget(float DeltaTime);

	// 直接移动到目标点（瞬时移动）
	void TeleportToTarget();

	// 计算移动方向
	FVector CalculateMovementDirection() const;

	// 检查是否到达目标
	bool HasReachedTarget() const;

	// 停止移动
	void StopMovement();

	FVector MoveTarget;
	bool bHasMoveTarget = false; // 移动状态
	FVector CurrentVelocity; // 用于平滑移动的当前速度

	UPROPERTY(EditAnywhere, Category="Move")
	float MoveSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category="Move")
	float StopDistance = 5.f;


	UPROPERTY(EditAnywhere, Category="Move", meta=(ClampMin="0.0", ClampMax="1.0"))
	float MovementSmoothFactor = 8.0f; // 移动平滑系数

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
