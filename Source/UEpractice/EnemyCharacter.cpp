// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "EnemyAIController.h"

#include "Components/PrimitiveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AEnemyAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 420.f;

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 520.0f, 0.0f);

	

}

float AEnemyCharacter::GetDetectionRadius() const
{
	return DetectionRadius;
}

float AEnemyCharacter::GetLoseTargetRadius() const
{
	return LoseTargetRadius;
}

float AEnemyCharacter::GetAttackRange() const
{
	return AttackRange;
}

bool AEnemyCharacter::TryAttack(AActor* Target) {
	if (!Target || !GetWorld()) {
		return false;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastAttackTime < AttackCooldown) {
		return false;
	}

	const float Distance =
		FVector::Dist(
			GetActorLocation(),
			Target->GetActorLocation()
		);
	if (Distance > AttackRange) {
		return false;
	}

	LastAttackTime = CurrentTime;

	UPrimitiveComponent* TargetPrimitive = Cast<UPrimitiveComponent>(Target->GetRootComponent());

	if (TargetPrimitive && TargetPrimitive->IsSimulatingPhysics()) {
		FVector KnockDirection = Target->GetActorLocation() - GetActorLocation();
		KnockDirection = KnockDirection.GetSafeNormal();

		KnockDirection =
			(
				KnockDirection +
				FVector::UpVector * 0.35f
				).GetSafeNormal();

		TargetPrimitive->AddImpulse(
			KnockDirection * KnockbackStrength,
			NAME_None,
			true
		);
	}
	OnAttack(Target);
	return true;
}


