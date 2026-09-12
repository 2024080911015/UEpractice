#include "EnemyAIController.h"

#include "EnemyCharacter.h"

#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	// AI 没必要每一帧思考。
	// 每 0.1 秒判断一次已经非常流畅。
	PrimaryActorTick.TickInterval = 0.1f;
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	RefreshPlayerTarget();
}

void AEnemyAIController::OnPossess(
	APawn* InPawn
)
{
	Super::OnPossess(InPawn);

	RefreshPlayerTarget();
}

void AEnemyAIController::RefreshPlayerTarget()
{
	if (!IsValid(PlayerTarget))
	{
		PlayerTarget =
			UGameplayStatics::GetPlayerPawn(
				this,
				0
			);
	}
}

void AEnemyAIController::Tick(
	float DeltaSeconds
)
{
	Super::Tick(DeltaSeconds);

	AEnemyCharacter* Enemy =
		Cast<AEnemyCharacter>(
			GetPawn()
		);

	if (!Enemy)
	{
		return;
	}

	RefreshPlayerTarget();

	if (!IsValid(PlayerTarget))
	{
		SetState(EEnemyAIState::Idle);
		return;
	}

	const FVector EnemyLocation =
		Enemy->GetActorLocation();

	const FVector PlayerLocation =
		PlayerTarget->GetActorLocation();

	const float Distance =
		FVector::Dist2D(
			EnemyLocation,
			PlayerLocation
		);

	/*
	 * LineOfSightTo 会做视线检测。
	 *
	 * 有墙挡着时，敌人不会在 Idle 状态
	 * 直接隔墙发现玩家。
	 */
	const bool bCanSeePlayer =
		LineOfSightTo(PlayerTarget);

	const float CurrentTime =
		GetWorld()->GetTimeSeconds();

	if (bCanSeePlayer)
	{
		LastSeenTime = CurrentTime;
	}

	switch (CurrentState)
	{
	case EEnemyAIState::Idle:

		if (
			Distance <= Enemy->GetDetectionRadius()
			&& bCanSeePlayer
			)
		{
			SetState(
				EEnemyAIState::Chase
			);
		}

		break;


	case EEnemyAIState::Chase:
	{
		const bool bForgotPlayer =
			!bCanSeePlayer &&
			CurrentTime - LastSeenTime >
			ForgetTargetDelay;

		if (
			Distance >
			Enemy->GetLoseTargetRadius()
			||
			bForgotPlayer
			)
		{
			SetState(
				EEnemyAIState::Idle
			);

			break;
		}

		if (
			Distance <=
			Enemy->GetAttackRange()
			&& bCanSeePlayer
			)
		{
			SetState(
				EEnemyAIState::Attack
			);

			break;
		}

		/*
		 * NavMesh 自动寻路追玩家。
		 *
		 * 玩家球一直在移动也没关系，
		 * MoveToActor 的目标是 Actor，
		 * 而不是固定 FVector。
		 */
		MoveToActor(
			PlayerTarget,
			Enemy->GetAttackRange() * 0.8f,
			true,
			true,
			false,
			nullptr,
			true
		);

		break;
	}


	case EEnemyAIState::Attack:

		StopMovement();

		SetFocus(PlayerTarget);

		if (
			Distance >
			Enemy->GetAttackRange() * 1.25f
			||
			!bCanSeePlayer
			)
		{
			SetState(
				EEnemyAIState::Chase
			);

			break;
		}

		Enemy->TryAttack(
			PlayerTarget
		);

		break;
	}
}

void AEnemyAIController::SetState(
	EEnemyAIState NewState
)
{
	if (CurrentState == NewState)
	{
		return;
	}

	CurrentState = NewState;

	switch (CurrentState)
	{
	case EEnemyAIState::Idle:

		StopMovement();

		ClearFocus(
			EAIFocusPriority::Gameplay
		);

		break;


	case EEnemyAIState::Chase:

		ClearFocus(
			EAIFocusPriority::Gameplay
		);

		break;


	case EEnemyAIState::Attack:

		StopMovement();

		if (PlayerTarget)
		{
			SetFocus(
				PlayerTarget,
				EAIFocusPriority::Gameplay
			);
		}

		break;
	}
}