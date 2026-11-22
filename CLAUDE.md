# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project overview

Unreal Engine 5.8 motorcycle simulation project. Currently in early prototyping — the pawn uses simple linear `AddActorLocalOffset` movement (not physics-based); engine rendering features are cranked (Lumen, virtual shadow maps, Substrate, ray tracing).

- **Engine:** UE 5.8, BuildSettings V7
- **Module:** `MotorcycleSim` (Runtime)
- **Default map:** `/Game/Motorcycle/Maps/MainMap`
- **Default game mode:** `BP_MotorcycleGameMode` (Blueprint subclass of `AMotorcycleGameMode`)
- **Default pawn:** `BP_MotorcyclePawn` (Blueprint subclass of `AMotorcyclePawn`)

## Build

```powershell
# Editor build (via UBT, from project root)
& "C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" MotorcycleSimEditor Win64 Development -Project="<REPO_ROOT>\MotorcycleSim.uproject" -WaitMutex

# Game build
& "C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" MotorcycleSim Win64 Development -Project="<REPO_ROOT>\MotorcycleSim.uproject" -WaitMutex
```

Or open `MotorcycleSim.sln` / `MotorcycleSim.slnx` in Visual Studio / Rider and build from the IDE. The `.slnx` is the newer format; the legacy `.sln` in `Backup\` is an old copy.

## Architecture

```
Content/Motorcycle/
├── Blueprints/
│   ├── BP_MotorcycleGameMode.uasset   → extends AMotorcycleGameMode
│   └── BP_MotorcyclePawn.uasset       → extends AMotorcyclePawn; the player pawn
├── Maps/
│   └── MainMap.umap                    → default map
├── IA_Move.uasset                      → UInputAction for 2D movement
├── IA_Test.uasset                      → UInputAction for test/debug button
└── IMC_Default.uasset                  → UInputMappingContext binding IA_Move + IA_Test

Source/MotorcycleSim/
├── MotorcycleSim.cpp / .h              → IMPLEMENT_PRIMARY_GAME_MODULE
├── MotorcycleGameMode.cpp / .h         → AGameModeBase subclass, almost all logic in Blueprint
├── MotorcyclePawn.cpp / .h             → Core pawn: camera rig + Enhanced Input + movement
├── TestPawn.cpp / .h                   → Minimal stub pawn for testing
├── MotorcycleSim.Build.cs              → Module deps: Core, CoreUObject, Engine, InputCore, EnhancedInput
├── MotorcycleSim.Target.cs             → Game target
└── MotorcycleSimEditor.Target.cs       → Editor target
```

### Input flow

`AMotorcyclePawn` uses **Enhanced Input** (not legacy axis bindings):

1. In `BeginPlay()`, the pawn retrieves the `UEnhancedInputLocalPlayerSubsystem` and adds `DefaultMappingContext` (assigned in the Blueprint — set to `IMC_Default`).
2. In `SetupPlayerInputComponent()`, it casts to `UEnhancedInputComponent` and binds `MoveAction` (`Triggered` + `Completed`) and `TestAction` (`Triggered`).
3. `Move()` reads an `FVector2D` from the input action value and calls `AddActorLocalOffset(X*10, Y*10, 0)` — simple linear movement, no physics.
4. There's also a legacy `SpaceBar` binding via `PlayerInputComponent->BindKey` that coexists with Enhanced Input — this is diagnostic debug code.

### Pawn composition (C++)

```
ARootComponent (SceneComponent)
└── SpringArm (USpringArmComponent, 400 units)
    └── Camera (UCameraComponent)
```

`bUsePawnControlRotation` is disabled; the camera is fixed relative to the pawn.

### Blueprint vs C++ split

Nearly all gameplay configuration lives in Blueprints — the C++ classes provide the skeleton (component hierarchy, input plumbing) but concrete asset references (`DefaultMappingContext`, `MoveAction`, `TestAction`) are assigned in the Blueprint editor. The game mode class is also overridden via Blueprint in `DefaultEngine.ini`.

### Rendering config

DX12 default with SM6 enabled. Lumen GI (method 1), virtual shadow maps, ray tracing, Substrate materials, and mesh distance fields are all on. Static lighting is disabled — this is a fully dynamic lighting setup.

### MCP plugins

This project has `ModelContextProtocol` and `MCPClientToolset` plugins enabled (not Editor-only). These are AI/MCP integration plugins — relevant if you're using external tools to interact with the editor at runtime.
