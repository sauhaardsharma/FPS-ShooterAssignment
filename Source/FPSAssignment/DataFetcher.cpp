#include "DataFetcher.h"
#include "Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "BoxActor.h"

ADataFetcher::ADataFetcher()
{
    PrimaryActorTick.bCanEverTick = false;
    JsonURL = "https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json";
}

void ADataFetcher::BeginPlay()
{
    Super::BeginPlay();
    FetchBoxData();
}

void ADataFetcher::FetchBoxData()
{
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &ADataFetcher::OnResponseReceived);
    Request->SetURL(JsonURL);
    Request->SetVerb("GET");
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->ProcessRequest();
}

void ADataFetcher::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("HTTP request failed!"));
        return;
    }

    FString JsonString = Response->GetContentAsString();
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("JSON parse failed."));
        return;
    }

    // Read "types" array
    const TArray<TSharedPtr<FJsonValue>>* TypesArray;
    if (!JsonObject->TryGetArrayField("types", TypesArray)) return;

    // Map typeName → typeData
    TMap<FString, TSharedPtr<FJsonObject>> TypesMap;
    for (const auto& TypeValue : *TypesArray)
    {
        TSharedPtr<FJsonObject> TypeObj = TypeValue->AsObject();
        if (TypeObj.IsValid())
        {
            FString Name = TypeObj->GetStringField("name");
            TypesMap.Add(Name, TypeObj);
        }
    }

    // Read "objects" array
    const TArray<TSharedPtr<FJsonValue>>* ObjectsArray;
    if (!JsonObject->TryGetArrayField("objects", ObjectsArray)) return;

    UWorld* World = GetWorld();
    if (!World) return;

    for (const auto& ObjVal : *ObjectsArray)
    {
        TSharedPtr<FJsonObject> Obj = ObjVal->AsObject();
        if (!Obj.IsValid()) continue;

        FString Type = Obj->GetStringField("type");
        if (!TypesMap.Contains(Type)) continue;

        TSharedPtr<FJsonObject> TypeData = TypesMap[Type];
        TSharedPtr<FJsonObject> Transform = Obj->GetObjectField("transform");

        // Extract arrays
        const TArray<TSharedPtr<FJsonValue>> LocationArr = Transform->GetArrayField("location");
        const TArray<TSharedPtr<FJsonValue>> RotationArr = Transform->GetArrayField("rotation");
        const TArray<TSharedPtr<FJsonValue>> ScaleArr = Transform->GetArrayField("scale");

        if (LocationArr.Num() < 3 || RotationArr.Num() < 3 || ScaleArr.Num() < 3) continue;

        FVector Location(
            LocationArr[0]->AsNumber(),
            LocationArr[1]->AsNumber(),
            LocationArr[2]->AsNumber()
        );

        FRotator Rotation(
            RotationArr[0]->AsNumber(),
            RotationArr[1]->AsNumber(),
            RotationArr[2]->AsNumber()
        );

        FVector Scale(
            ScaleArr[0]->AsNumber(),
            ScaleArr[1]->AsNumber(),
            ScaleArr[2]->AsNumber()
        );

        // Spawn actor
        FActorSpawnParameters SpawnParams;
        ABoxActor* SpawnedBox = World->SpawnActor<ABoxActor>(
            ABoxActor::StaticClass(),
            Location,
            Rotation,
            SpawnParams
        );

        if (SpawnedBox && TypeData.IsValid())
        {
            const TArray<TSharedPtr<FJsonValue>> ColorArr = TypeData->GetArrayField("color");
            if (ColorArr.Num() >= 3)
            {
                FLinearColor Color(
                    ColorArr[0]->AsNumber() / 255.0f,
                    ColorArr[1]->AsNumber() / 255.0f,
                    ColorArr[2]->AsNumber() / 255.0f
                );

                int32 Health = TypeData->GetIntegerField("health");
                int32 Score = TypeData->GetIntegerField("score");

                SpawnedBox->SetActorScale3D(Scale);
                SpawnedBox->InitBox(Color, 0, Health, Score); // "0" as type placeholder
            }
        }
    }
}
