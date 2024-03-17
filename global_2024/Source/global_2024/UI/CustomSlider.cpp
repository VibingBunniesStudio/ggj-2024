// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CustomSlider.h"

void UCustomSlider::SetValueWithCallback(float InValue)
{
	SetValue(InValue);
	HandleOnValueChanged(InValue);
}
