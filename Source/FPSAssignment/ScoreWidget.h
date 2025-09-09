#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

UCLASS()
class FPSASSIGNMENT_API UScoreWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Bindable TextBlock from UMG
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ScoreText;

    // Function to update UI score
    UFUNCTION(BlueprintCallable)
    void UpdateScore(int32 NewScore);
};
