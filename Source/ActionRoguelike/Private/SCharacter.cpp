#include "SCharacter.h"

#include "SAttributeComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SInteractionComponent.h"

ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmCmp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmCmp"));
	SpringArmCmp->SetupAttachment(RootComponent);

	CameraCmp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraCmp"));
	CameraCmp->SetupAttachment(SpringArmCmp);

	InteractionCmp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionCmp"));
	AttributeCmp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeCmp"));

	bUseControllerRotationYaw = false;
	SpringArmCmp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	AttackAnimDelay = 0.2f;
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector LineStart = GetActorLocation();
	LineStart += GetActorRightVector() * 100.0f;
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	FVector ControlDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);

	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, 100.0f, FColor::Yellow, false, 0.0f, 0,
	                          5.0f);
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControlDirection_LineEnd, 100.0, FColor::Green, false, 0.0f, 0,
	                          5.0f);
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
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed,
	                                AttackAnimDelay);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionCmp)
	{
		InteractionCmp->PrimaryInteract();
	}
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensure(ClassToSpawn))
	{
		auto HandLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));

		FHitResult Hit;
		auto TraceStart = CameraCmp->GetComponentLocation();
		auto TraceEnd = TraceStart + (GetControlRotation().Vector() * 5000.0f);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		FRotator ProjRotation;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd,
		                                        FQuat::Identity, ObjParams, Shape, QueryParams))
		{
			TraceEnd = Hit.ImpactPoint;
		}
		ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTransform = FTransform(ProjRotation, HandLocation);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
	}
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
		PlayerInputComponent->BindAction(TEXT("PrimaryInteract"), EInputEvent::IE_Pressed, this,
		                                 &ASCharacter::PrimaryInteract);
	}
}
