// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerState.h"
#include "MyGameMode.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Hat = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hat"));
	Hat->SetupAttachment(GetMesh(), TEXT("head"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>((TEXT("Camera")));
	Camera->SetupAttachment(SpringArm);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	APlayerController* PC = GetController<APlayerController>();
	if (!GI || !PC) return;
	if(PC->IsLocalPlayerController()) {
		SetPlayerHat(GI->GetPlayerHatType());
		SetPlayerColor(GI->GetPlayerColor());
	}

	if (InputMappingContext) {
		UEnhancedInputLocalPlayerSubsystem* inputSubsystem = PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (inputSubsystem) {
			inputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyCharacter::SetPlayerHat_Implementation(EHatType type) {
	if (!Hat) return;

	FString path = "";

	switch (type)
	{
	case EHatType::NoHat:
		path = "";
		break;
	case EHatType::Apple:
		path = TEXT("/Game/Assets/Models/apple.apple");
		break;
	case EHatType::Banana:
		path = TEXT("/Game/Assets/Models/banana.banana");
		break;
	case EHatType::Carrot:
		path = TEXT("/Game/Assets/Models/carrot.carrot"); 
		break;
	default:
		path = "";
		break;
	}

	if (path.IsEmpty()) {
		Hat->SetStaticMesh(nullptr);
	}
	else {
		Hat->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *path));
	}
}		

void AMyCharacter::SetPlayerColor_Implementation(FLinearColor color) {
	USkeletalMeshComponent* skeletalMeshComp = GetMesh();
	if (!skeletalMeshComp)return;

	int materialNum = skeletalMeshComp->GetMaterials().Num();
	for (int i = 0; i < materialNum; i++)
	{
		UMaterialInstanceDynamic* dynamicMaterial = skeletalMeshComp->CreateAndSetMaterialInstanceDynamic(i);
		if (dynamicMaterial) {
			dynamicMaterial->SetVectorParameterValue(FName("Tint"), color);
		}
	}
}

void AMyCharacter::Move(const FInputActionValue& Value) {

	if (!Controller) return;

	FVector2D MovementVector = Value.Get<FVector2D>();
	FRotator Rot = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rot.Yaw, 0.0f);

	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(RightDirection, MovementVector.X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

}

void AMyCharacter::Look(const FInputActionValue& Value) {
	AddControllerYawInput(Value.Get<FVector2D>().X);
	AddControllerPitchInput(Value.Get<FVector2D>().Y);
}

void AMyCharacter::JumpStarted(const FInputActionValue& Value) {
	Jump();
}

void AMyCharacter::JumpCompleted(const FInputActionValue& Value) {
	StopJumping();
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyCharacter::JumpStarted);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyCharacter::JumpCompleted);
	}
}

void AMyCharacter::AddCoins(uint8 Value) {
	AMyPlayerState* PS = Cast<AMyPlayerState>(GetPlayerState());
	if (PS) {
		PS->AddCoins(Value);
	}
}

void AMyCharacter::SetPlayerID(uint8 id) {
	PlayerID = id;
}

void AMyCharacter::Destroyed() {
	Super::Destroyed();

	UE_LOG(LogTemp, Error, TEXT("Destroyed1"));
	if (!HasAuthority()) return;
	
	//AController* controller = GetController();
	//if (!controller) return;

	UE_LOG(LogTemp, Error, TEXT("Destroyed2"));
	AMyGameMode* gm = GetWorld()->GetAuthGameMode<AMyGameMode>();
	if (!gm) return;

	UE_LOG(LogTemp, Error, TEXT("Destroyed3"));
	gm->RespawnPlayer(PlayerID);
}