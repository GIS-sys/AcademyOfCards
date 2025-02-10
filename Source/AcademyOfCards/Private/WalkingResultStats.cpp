// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultStats.h"

WalkingResultStats::WalkingResultStats(TSharedPtr<FJsonObject> data)
{
	StatsChange = data;
}

WalkingResultStats::~WalkingResultStats()
{
}
