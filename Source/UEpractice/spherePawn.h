// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "spherePawn.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class USceneComponent;



UCLASS()
class UEPRACTICE_API AspherePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AspherePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SphereMesh;


	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* CameraRoot;
};
