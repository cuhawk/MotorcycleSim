// Fill out your copyright notice in the Description page of Project Settings.


#include "MotorcycleGameMode.h"

// Sets default values
AMotorcycleGameMode::AMotorcycleGameMode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMotorcycleGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMotorcycleGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

