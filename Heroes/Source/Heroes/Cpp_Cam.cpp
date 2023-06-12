// Заполните уведомление об авторских правах на странице «Описание» в настройках проекта.


#include "Cpp_Cam.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
ACpp_Cam::ACpp_Cam()
{
 	// Установите эту пешку для вызова Tick() в каждом кадре. Вы можете отключить это, чтобы улучшить производительность, если вам это не нужно.
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->TargetArmLength = 3500.0f;
	SpringArm->SetWorldRotation(FRotator(-60.0f, 0.0f, 0.0f)); // Y Z X
	SpringArm->SetupAttachment(DefaultSceneRoot);
	SpringArm->bDoCollisionTest = false; // Отключаем столкновение

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

}

// Вызывается при запуске игры или при появлении
void ACpp_Cam::BeginPlay()
{
	Super::BeginPlay();

	LocationDesired = GetActorLocation();
	ZoomDesired = SpringArm->TargetArmLength;
	RotationDesired = GetActorRotation();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) //Получаем контроллер игрока
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(BattleMappingContext, 0); //Добавить контекст сопоставления ввода
		}
	}
	
}

void ACpp_Cam::CameraMove(const FInputActionValue& Value)
{
	//Есть баг: при перемещении камеры над объектом она смещается к нему. В реализации BluePrint не встречается. Возможно из-за 2д вектора
	// Мы отмечаем, что ввод нажат
	FollowTime += GetWorld()->GetDeltaSeconds();
	const FVector2D CValue = Value.Get<FVector2D>().GetSafeNormal();
	const FVector ForwardVector = CValue.Component(0) * LocationSpeed * GetActorForwardVector();
	const FVector RightVector = CValue.Component(1) * LocationSpeed * GetActorRightVector();
	LocationDesired = ForwardVector + LocationDesired + RightVector;
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), LocationDesired, FollowTime, LocationInterp));
	//UE_LOG(LogTemp, Warning, TEXT("Value: %s"), *CValue.ToString());
}

void ACpp_Cam::Rotate(const FInputActionValue& Value)
{
	const float CValue = Value.Get<float>();
	const float z = RotationSpeed * CValue;
	RotationDesired = RotationDesired.Add(0,z,0);
	//UE_LOG(LogTemp, Warning, TEXT("Value: %f"), CValue);
}

void ACpp_Cam::Zoom(const FInputActionValue& Value)
{
	const float CValue = Value.Get<float>();
	const float fZoom = CValue * ZoomSpeed + ZoomDesired;
	ZoomDesired = FMath::Clamp(fZoom, ZoomMin, ZoomMax);
}

// Вызывается каждый кадр
void ACpp_Cam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// В каждом кадре интерполируйте данные о движениях камеры (чтобы сделать их более плавными) и применяйте их к фактической камере. Оптимизировать.
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ZoomDesired, GetWorld()->GetDeltaSeconds(), ZoomInterp);
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), RotationDesired, GetWorld()->GetDeltaSeconds(), RotationInterp));
}

// Вызывается для привязки функциональности к вводу
void ACpp_Cam::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Настраиваем привязки действий
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) // Получите UEnhancedInputComponent
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACpp_Cam::CameraMove);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ACpp_Cam::Rotate);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ACpp_Cam::Zoom);
	}

}

