#include "MotorcyclePawn.h"

#include "Engine/Engine.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"

#include "InputCoreTypes.h"

AMotorcyclePawn::AMotorcyclePawn()
{
    PrimaryActorTick.bCanEverTick = true;

    AutoPossessPlayer = EAutoReceiveInput::Player0;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(Root);

    SpringArm->TargetArmLength = 400.0f;
    SpringArm->bUsePawnControlRotation = false;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void AMotorcyclePawn::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("=== BeginPlay ==="));

    if (GetController())
    {
        UE_LOG(LogTemp, Warning, TEXT("Controller: %s"), *GetController()->GetName());

        GEngine->AddOnScreenDebugMessage(
            -1,
            5.f,
            FColor::Green,
            TEXT("Pawn Possessed"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Controller is NULL"));

        GEngine->AddOnScreenDebugMessage(
            -1,
            5.f,
            FColor::Red,
            TEXT("No Controller"));
    }

    if (!DefaultMappingContext)
    {
        UE_LOG(LogTemp, Error, TEXT("DefaultMappingContext is NULL"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Mapping Context = %s"), *DefaultMappingContext->GetName());

    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                UE_LOG(LogTemp, Warning, TEXT("Adding Mapping Context"));

                Subsystem->AddMappingContext(DefaultMappingContext, 0);
                const FModifyContextOptions Options;
                Subsystem->RequestRebuildControlMappings(Options, EInputMappingRebuildType::Rebuild);

                GEngine->AddOnScreenDebugMessage(
                    -1,
                    5.f,
                    FColor::Yellow,
                    TEXT("Mapping Added"));
            }
        }
    }
}

void AMotorcyclePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMotorcyclePawn::Move(const FInputActionValue& Value)
{
    const FVector2D Movement = Value.Get<FVector2D>();

    UE_LOG(LogTemp, Warning,
        TEXT("Move() X=%f Y=%f"),
        Movement.X,
        Movement.Y);

    GEngine->AddOnScreenDebugMessage(
        100,
        0.f,
        FColor::Red,
        FString::Printf(TEXT("Move %.2f %.2f"),
            Movement.X,
            Movement.Y));

    if (!Movement.IsNearlyZero())
    {
        AddActorLocalOffset(
            FVector(
                Movement.X * 10.f,
                Movement.Y * 10.f,
                0.f),
            true);
    }
}

void AMotorcyclePawn::TestInput(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("SPACE WORKS"));

    GEngine->AddOnScreenDebugMessage(
        -1,
        2.f,
        FColor::Green,
        TEXT("SPACE WORKS"));
}

void AMotorcyclePawn::LegacySpacePressed()
{
    UE_LOG(LogTemp, Warning, TEXT("LEGACY SPACE"));

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            2.f,
            FColor::Cyan,
            TEXT("LEGACY SPACE"));
    }
}

void AMotorcyclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UE_LOG(LogTemp, Warning, TEXT("=== SetupPlayerInputComponent ==="));

    PlayerInputComponent->BindKey(
        EKeys::SpaceBar,
        IE_Pressed,
        this,
        &AMotorcyclePawn::LegacySpacePressed);

    if (!PlayerInputComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerInputComponent NULL"));
        return;
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Input Component Class: %s"),
        *PlayerInputComponent->GetClass()->GetName());

    UEnhancedInputComponent* EnhancedInput =
        Cast<UEnhancedInputComponent>(PlayerInputComponent);

    if (!EnhancedInput)
    {
        UE_LOG(LogTemp, Error, TEXT("Not EnhancedInputComponent"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent OK"));

    if (!MoveAction)
    {
        UE_LOG(LogTemp, Error, TEXT("MoveAction NULL"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveAction = %s"), *MoveAction->GetName());

        EnhancedInput->BindAction(
            MoveAction,
            ETriggerEvent::Triggered,
            this,
            &AMotorcyclePawn::Move);

        EnhancedInput->BindAction(
            MoveAction,
            ETriggerEvent::Completed,
            this,
            &AMotorcyclePawn::Move);

        UE_LOG(LogTemp, Warning, TEXT("MoveAction bound"));
    }

    if (!TestAction)
    {
        UE_LOG(LogTemp, Error, TEXT("TestAction NULL"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("TestAction = %s"), *TestAction->GetName());

        EnhancedInput->BindAction(
            TestAction,
            ETriggerEvent::Triggered,
            this,
            &AMotorcyclePawn::TestInput);

        UE_LOG(LogTemp, Warning, TEXT("TestAction bound"));
    }
}