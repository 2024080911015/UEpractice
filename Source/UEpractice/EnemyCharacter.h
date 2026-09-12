#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class UEPRACTICE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	float GetDetectionRadius() const;
	float GetLoseTargetRadius() const;
	float GetAttackRange() const;

	bool TryAttack(AActor* Target);

protected:

	// 以后可以在蓝图里接攻击动画、音效、特效
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy|Combat")
	void OnAttack(AActor* Target);

private:

	// 多远能发现玩家
	UPROPERTY(EditAnywhere, Category = "Enemy|AI")
	float DetectionRadius = 1600.0f;

	// 玩家跑多远后放弃追踪
	UPROPERTY(EditAnywhere, Category = "Enemy|AI")
	float LoseTargetRadius = 2200.0f;

	// 多近开始攻击
	UPROPERTY(EditAnywhere, Category = "Enemy|Combat")
	float AttackRange = 220.0f;

	// 攻击间隔
	UPROPERTY(EditAnywhere, Category = "Enemy|Combat")
	float AttackCooldown = 1.2f;

	// 把球撞飞的力度
	UPROPERTY(EditAnywhere, Category = "Enemy|Combat")
	float KnockbackStrength = 1200.0f;

	float LastAttackTime = -100000.0f;
};