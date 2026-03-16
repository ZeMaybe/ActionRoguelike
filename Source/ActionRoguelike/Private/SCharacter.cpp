#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmCmp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmCmp"));
	SpringArmCmp->SetupAttachment(RootComponent);

	CameraCmp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraCmp"));
	CameraCmp->SetupAttachment(SpringArmCmp);
	
	bUseControllerRotationYaw = false;
	SpringArmCmp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector LineStart = GetActorLocation();
	LineStart += GetActorRightVector() * 100.0f;
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	FVector ControlDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, 100.0f, FColor::Yellow, false, 0.0f, 0, 5.0f);
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControlDirection_LineEnd, 100.0, FColor::Green, false, 0.0f, 0, 5.0f);
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacter::MoveForward(float AxisValue)
{
	auto ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;

	AddMovementInput(ControlRot.Vector(), AxisValue);
	// AddMovementInput(UKismetMathLibrary::GetForwardVector(ControlRot), AxisValue);
	// AddMovementInput(FRotationMatrix(ControlRot).GetScaledAxis(EAxis::X), AxisValue);
}

void ASCharacter::MoveRight(float AxisValue)
{
	auto ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll = 0.f;

	// x : forward  (red)
	// y : right    (green)
	// z : up       (blue)

	// AddMovementInput(UKismetMathLibrary::GetRightVector(ControlRot), AxisValue);
	AddMovementInput(FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y), AxisValue);
}

void ASCharacter::PrimaryAttack()
{
	FTransform SpawnTransform = FTransform(GetControlRotation(), GetMesh()->GetSocketLocation(TEXT("Muzzle_01")));
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASCharacter::MoveForward);
		PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASCharacter::MoveRight);
		PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

		PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
		PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);

		PlayerInputComponent->BindAction(TEXT("PrimaryAttack"), EInputEvent::IE_Pressed, this,
		                                 &ASCharacter::PrimaryAttack);
	}
}
