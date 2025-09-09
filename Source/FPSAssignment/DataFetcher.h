#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpModule.h"
#include "DataFetcher.generated.h"

UCLASS()
class FPSASSIGNMENT_API ADataFetcher : public AActor
{
    GENERATED_BODY()

public:
    ADataFetcher();

    virtual void BeginPlay() override;

    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

protected:
    void FetchBoxData();

    FString JsonURL;
};
