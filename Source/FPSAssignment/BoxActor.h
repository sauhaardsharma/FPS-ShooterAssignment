#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxActor.generated.h"

UCLASS()
class FPSASSIGNMENT_API ABoxActor : public AActor
{
    GENERATED_BODY()

public:
    ABoxActor();

    virtual void Tick(float DeltaTime) override;

    // Called to setup box properties
    void InitBox(FLinearColor Color, int32 Type, int32 Health, int32 Score);

    // Override UE damage system
    virtual float TakeDamage(
        float DamageAmount,
        struct FDamageEvent const& DamageEvent,
        class AController* EventInstigator,
        AActor* DamageCauser
    ) override;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* BoxMesh;

    UPROPERTY()
    UMaterialInstanceDynamic* DynamicMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box")
    int32 BoxType = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box")
    int32 BoxHealth = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box")
    int32 BoxScore = 100;

protected:
    virtual void BeginPlay() override;
};
