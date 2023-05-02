// Заполните уведомление об авторских правах на странице «Описание» в настройках проекта.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Cpp_Cam.generated.h"

class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;


UCLASS()
class HEROES_API ACpp_Cam : public APawn
{
	GENERATED_BODY()

public:
	// Устанавливает значения по умолчанию для свойств этой пешки
	ACpp_Cam();

protected:
	// Вызывается при запуске игры или при появлении
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Input)
		UInputMappingContext* BattleMappingContext;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Input)
		UInputAction* MoveAction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Input)
		UInputAction* RotateAction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Input)
		UInputAction* ZoomAction;

	//Управление движением по верикали
	void CameraMove(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);

public:	
	// Вызывается каждый кадр
	virtual void Tick(float DeltaTime) override;

	// Вызывается для привязки функциональности к вводу
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera Default")
		UCameraComponent* Camera;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera Default")
		USpringArmComponent* SpringArm;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera Default")
		USceneComponent* DefaultSceneRoot;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Zoom")
		float ZoomDesired = 700.0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Location")
		FVector LocationDesired;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Rotation")
		FRotator RotationDesired;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Rotation")
		float RotationSpeed = 45.0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Location")
		float LocationSpeed = 20.0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Zoom")
		float ZoomSpeed = 50.0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Zoom")
		float ZoomMin = 100.0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Zoom")
		float ZoomMax = 5000.0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Zoom")
		float ZoomInterp = 2.0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Location")
		float LocationInterp = 0.0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Rotation")
		float RotationInterp = 5.0;

private:
	float FollowTime; // Как долго он был нажат
};
