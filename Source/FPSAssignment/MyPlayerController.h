#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

UCLASS()
class FPSASSIGNMENT_API AMyPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AMyPlayerController();

    virtual void BeginPlay() override;

    // Score tracking
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
    int32 PlayerScore;

    // Function to add score
    void AddScore(int32 Amount);

protected:
    // Reference to widget class set in BP
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UScoreWidget> ScoreWidgetClass;

    // Instance of the widget
    UPROPERTY()
    class UScoreWidget* ScoreWidget;
};
