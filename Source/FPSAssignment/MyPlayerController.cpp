#include "MyPlayerController.h"
#include "ScoreWidget.h"
#include "Blueprint/UserWidget.h"

AMyPlayerController::AMyPlayerController()
{
    PlayerScore = 0;
}

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (ScoreWidgetClass)
    {
        ScoreWidget = CreateWidget<UScoreWidget>(this, ScoreWidgetClass);
        if (ScoreWidget)
        {
            ScoreWidget->AddToViewport();
            ScoreWidget->UpdateScore(PlayerScore);
        }
    }
}

void AMyPlayerController::AddScore(int32 Amount)
{
    PlayerScore += Amount;

    if (ScoreWidget)
    {
        ScoreWidget->UpdateScore(PlayerScore);
    }
}
