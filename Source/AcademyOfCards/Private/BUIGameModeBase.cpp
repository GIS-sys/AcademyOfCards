// Fill out your copyright notice in the Description page of Project Settings.


#include "BUIGameModeBase.h"
#include "Serialization/JsonSerializer.h"


ABUIGameModeBase::ABUIGameModeBase() {
    FString FilePath = FPaths::ProjectContentDir() / TEXT("WalkingStage/Configs/config_walking_events.json");
    FString JsonString;
    if (FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*JsonString);
        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            for (const auto& [x, y] : JsonObject->Values) {
                UE_LOG(LogTemp, Log, TEXT("Event: %s"), *x);
                UE_LOG(LogTemp, Log, TEXT("Text: %s"), *(y->AsObject()->TryGetField("text")->AsString()));
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file from path: %s"), *FilePath);
    }
}