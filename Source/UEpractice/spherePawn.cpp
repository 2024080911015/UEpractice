// Fill out your copyright notice in the Description page of Project Settings.


#include "spherePawn.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "Components/SceneComponent.h"

// Sets default values
AspherePawn::AspherePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//创建球体网格组件
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	//把球的mesh设置为根组件
	RootComponent = SphereMesh;
	SphereMesh->SetSimulatePhysics(true);
	SphereMesh->SetEnableGravity(true);
	//增加阻尼以及抑制线速度
	SphereMesh->SetAngularDamping(2.0f);
	SphereMesh->SetLinearDamping(0.3f);
	//创建弹簧臂：相机的"杆子"，挂在球体网格下面
	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	CameraRoot->SetupAttachment(SphereMesh);
	CameraRoot->SetUsingAbsoluteRotation(true);
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(CameraRoot);
	//固定斜俯视角：向下俯视45度，杆长600（先固定，下一步做鼠标转视角时再改）
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	//创建跟随相机，挂在弹簧臂"末端的插槽"上
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


}

// Called when the game starts or when spawned
void AspherePawn::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem && MappingContext)
		{
			Subsystem->AddMappingContext(MappingContext, 1);
		}
		PlayerController->SetControlRotation(FRotator(-45.f, 0.f, 0.f));
	}
	
}



// Called to bind functionality to input
void AspherePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent && MoveAction)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AspherePawn::Move);
	}
	if (EnhancedInputComponent && LookAction)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AspherePawn::Look);
	}

}

void AspherePawn::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (!Controller || !SphereMesh)
	{
		return;
	}

	// 获取镜头水平方向
	const FRotator ControlRotation = Controller->GetControlRotation();

	const FRotator YawRotation(
		0.0f,
		ControlRotation.Yaw,
		0.0f
	);

	const FVector ForwardDirection =
		FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightDirection =
		FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 玩家希望球移动的方向
	FVector MoveDirection =
		ForwardDirection * MovementVector.Y +
		RightDirection * MovementVector.X;

	if (MoveDirection.IsNearlyZero())
	{
		return;
	}

	MoveDirection.Normalize();

	// 球要朝某方向滚动，旋转轴应该与移动方向垂直
	const FVector TorqueAxis =
		FVector::CrossProduct(
			FVector::UpVector,
			MoveDirection
		);

	const float TorqueStrength = 15.0f;

	SphereMesh->AddTorqueInRadians(
		TorqueAxis * TorqueStrength,
		NAME_None,
		true
	);
}

void AspherePawn::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();

	if (!Controller)
	{
		return;
	}

	const float LookSensitivity = 0.15f;

	FRotator Rotation = Controller->GetControlRotation();

	// 左右旋转
	Rotation.Yaw += LookValue.X * LookSensitivity;

	// 上下旋转
	Rotation.Pitch += LookValue.Y * LookSensitivity;

	// 限制上下观察角度，避免镜头翻过去
	Rotation.Pitch = FMath::Clamp(
		Rotation.Pitch,
		-75.0f,
		30.0f
	);

	// 第三人称镜头不要出现侧翻
	Rotation.Roll = 0.0f;

	Controller->SetControlRotation(Rotation);
}

