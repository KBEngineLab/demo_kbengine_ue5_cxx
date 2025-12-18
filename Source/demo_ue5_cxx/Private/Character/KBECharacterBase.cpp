// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KBECharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AKBECharacterBase::AKBECharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AKBECharacterBase::SetMoveSpeed(float InMoveSpeed)
{
	MoveSpeed = InMoveSpeed;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}


void AKBECharacterBase::SmoothMoveToTarget(float DeltaTime)
{
	// 检查是否已到达目标
	if (HasReachedTarget())
	{
		StopMovement();
		return;
	}

	// 计算移动方向
	FVector Direction = CalculateMovementDirection();
	if (Direction.IsNearlyZero())
	{
		StopMovement();
		return;
	}

	// 方案1：使用CharacterMovementComponent进行平滑移动（推荐）
	if (GetCharacterMovement())
	{
		// 计算移动到目标所需的速度
		FVector TargetVelocity = Direction * MoveSpeed;

		// 平滑插值当前速度到目标速度
		CurrentVelocity = FMath::VInterpTo(
			CurrentVelocity,
			TargetVelocity,
			DeltaTime,
			MovementSmoothFactor
		);

		// 应用移动
		AddMovementInput(Direction, CurrentVelocity.Size() / MoveSpeed);
	}

	// 方案2：直接设置位置（更直接的控制）
	// FVector NewLocation = FMath::VInterpTo(
	//     GetActorLocation(),
	//     MoveTarget,
	//     DeltaTime,
	//     MovementSmoothFactor / 10.0f // 需要调整系数
	// );
	// SetActorLocation(NewLocation);
}

void AKBECharacterBase::TeleportToTarget()
{
	if (!HasReachedTarget())
	{
		FVector NewLocation = MoveTarget;
		NewLocation.Z = GetActorLocation().Z; // 保持当前高度
		SetActorLocation(NewLocation);
	}
	bHasMoveTarget = false;
}

FVector AKBECharacterBase::CalculateMovementDirection() const
{
	if (!bHasMoveTarget) return FVector::ZeroVector;

	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = MoveTarget;

	// 保持在同一水平面上移动（忽略Z轴差异）
	CurrentLocation.Z = 0;
	TargetLocation.Z = 0;

	FVector Direction = TargetLocation - CurrentLocation;

	// 如果距离已经很小，返回零向量
	if (Direction.Size() < KINDA_SMALL_NUMBER)
		return FVector::ZeroVector;

	return Direction.GetSafeNormal();
}

bool AKBECharacterBase::HasReachedTarget() const
{
	if (!bHasMoveTarget) return true;

	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = MoveTarget;

	// 只计算水平距离
	CurrentLocation.Z = 0;
	TargetLocation.Z = 0;

	float Distance = FVector::Dist(CurrentLocation, TargetLocation);

	return Distance <= StopDistance;
}

void AKBECharacterBase::StopMovement()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
	}

	bHasMoveTarget = false;
	CurrentVelocity = FVector::ZeroVector;
}

void AKBECharacterBase::BP_UpdateHeadInfo_Implementation()
{
}

// Called when the game starts or when spawned
void AKBECharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AKBECharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 如果设置了移动目标，执行平滑移动
	if (bHasMoveTarget)
	{
		SmoothMoveToTarget(DeltaTime);
	}
}

// Called to bind functionality to input
void AKBECharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

