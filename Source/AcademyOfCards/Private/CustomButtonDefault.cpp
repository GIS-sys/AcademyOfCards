// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomButtonDefault.h"

void UCustomButtonDefault::CallOnClickDelegate() {
	if (OnClickDelegate.IsBound()) {
		OnClickDelegate.Execute();
	}
}