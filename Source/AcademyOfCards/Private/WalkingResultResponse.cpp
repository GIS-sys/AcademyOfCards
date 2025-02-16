// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultResponse.h"

WalkingResultResponse::WalkingResultResponse(TSharedPtr<FJsonObject> data)
{
	if (data->HasField("text")) {
		Response = data->GetStringField("text");
	}
	else
	{
		Response = "";
	}
}

WalkingResultResponse::~WalkingResultResponse()
{
}

void WalkingResultResponse::Execute()
{
	// TODO
	UE_LOG(LogTemp, Error, TEXT("EXECUTE: Response(%s)"), *Response);
}