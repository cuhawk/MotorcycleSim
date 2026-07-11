#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TestPawn.generated.h"

class UInputMappingContext;

UCLASS()
class MOTORCYCLESIM_API ATestPawn : public APawn
{
    GENERATED_BODY()

public:
    ATestPawn();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    TObjectPtr<UInputMappingContext> TestMapping;
};