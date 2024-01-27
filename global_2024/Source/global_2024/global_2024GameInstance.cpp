// Fill out your copyright notice in the Description page of Project Settings.


#include "global_2024GameInstance.h"

Uglobal_2024GameInstance::Uglobal_2024GameInstance()
{
	_soundManager = NewObject<USoundManager>(this, USoundManager::StaticClass(), TEXT("SOUND MANAGER"));
}
