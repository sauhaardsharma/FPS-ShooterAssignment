#include "BoxActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

// Include your custom PlayerController to update score
#include "MyPlayerController.h"

ABoxActor::ABoxActor()
{
    PrimaryActorTick.bCanEverTick = false;

    BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
    RootComponent = BoxMesh;

    // Load default cube mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxAsset(
        TEXT("/Engine/BasicShapes/Cube.Cube")
    );
    if (BoxAsset.Succeeded())
    {
        BoxMesh->SetStaticMesh(BoxAsset.Object);
    }

    // Load default material
    static ConstructorHelpers::FObjectFinder<UMaterial> BoxMat(
        TEXT("Material'/Game/Variant_Shooter/M_BoxDynamic.M_BoxDynamic'")
    );
    if (BoxMat.Succeeded())
    {
        BoxMesh->SetMaterial(0, BoxMat.Object);
    }

    // Enable collisions so bullets/line traces hit it
    BoxMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    BoxMesh->SetNotifyRigidBodyCollision(true);
}

void ABoxActor::BeginPlay()
{
    Super::BeginPlay();
}

void ABoxActor::InitBox(FLinearColor Color, int32 Type, int32 Health, int32 Score)
{
    BoxType = Type;
    BoxHealth = Health;
    BoxScore = Score;

    // Assign dynamic material for color customization
    if (!DynamicMaterial)
    {
        UMaterialInterface* BaseMat = BoxMesh->GetMaterial(0);
        if (BaseMat)
        {
            DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMat, this);
            BoxMesh->SetMaterial(0, DynamicMaterial);
        }
    }

    if (DynamicMaterial)
    {
        DynamicMaterial->SetVectorParameterValue("Color", Color);
    }
}

float ABoxActor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    const int32 Damage = FMath::Max(1, FMath::RoundToInt(DamageAmount));
    BoxHealth -= Damage;

    UE_LOG(LogTemp, Log, TEXT("Box hit! Remaining Health: %d"), BoxHealth);

    if (BoxHealth <= 0)
    {
        // Award score to instigator controller if valid
        if (EventInstigator)
        {
            if (AMyPlayerController* PC = Cast<AMyPlayerController>(EventInstigator))
            {
                PC->AddScore(BoxScore);
            }
        }

        Destroy();
    }

    UE_LOG(LogTemp, Warning, TEXT("TakeDamage called on %s, Health now: %d"), *GetName(), BoxHealth);
    return DamageAmount;
}
