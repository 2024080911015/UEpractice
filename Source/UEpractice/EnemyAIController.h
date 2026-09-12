#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class AEnemyCharacter;

UENUM(BlueprintType)
enum class EEnemyAIState : uint8
{
	Idle,
	Chase,
	Attack
};

UCLASS()
class UEPRACTICE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(
		APawn* InPawn
	) override;

	virtual void Tick(
		float DeltaSeconds
	) override;

private:

	void RefreshPlayerTarget();

	void SetState(
		EEnemyAIState NewState
	);

	UPROPERTY(
		VisibleInstanceOnly,
		BlueprintReadOnly,
		Category = "Enemy|AI",
		meta = (AllowPrivateAccess = "true")
	)
	EEnemyAIState CurrentState =
		EEnemyAIState::Idle;

	UPROPERTY()
	APawn* PlayerTarget = nullptr;

	// 敌人失去视野后还能记住玩家多久
	UPROPERTY(EditAnywhere, Category = "Enemy|AI")
	float ForgetTargetDelay = 2.0f;

	float LastSeenTime = -100000.0f;
};