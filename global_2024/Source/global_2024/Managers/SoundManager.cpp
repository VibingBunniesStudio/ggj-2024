// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/SoundManager.h"
#include "FMODBus.h"
#include "FMODBlueprintStatics.h"

USoundManager::USoundManager()
{
	_bgmVolume = 0.5f;
	_sfxVolume = 0.5f;

	UFMODBus* bgmBus = Cast<UFMODBus>(UFMODBlueprintStatics::FindAssetByName("bus:/bgm"));

	UFMODBlueprintStatics::BusSetVolume(bgmBus, _bgmVolume);

	UFMODBus* sfxBus = Cast<UFMODBus>(UFMODBlueprintStatics::FindAssetByName("bus:/sfx"));

	UFMODBlueprintStatics::BusSetVolume(sfxBus, _sfxVolume);
}

float USoundManager::GetBGMusicVolume()
{
	return _bgmVolume;
}

bool USoundManager::SetBGMusicVolume(float newVolume)
{
	if (newVolume == _bgmVolume) return true;
	if (newVolume < 0.0) return false;

	_bgmVolume = newVolume;

	UFMODBus* bgmBus = Cast<UFMODBus>(UFMODBlueprintStatics::FindAssetByName("bus:/bgm"));

	if (!bgmBus)
	{
		return false;
	}

	UFMODBlueprintStatics::BusSetVolume(bgmBus, _bgmVolume);

	return true;
}

float USoundManager::GetSFXVolume()
{
	return _sfxVolume;
}

bool USoundManager::SetSFXVolume(float newVolume)
{
	if (newVolume == _sfxVolume) return true;
	if (newVolume < 0.0) return false;

	_sfxVolume = newVolume;

	UFMODBus* sfxBus = Cast<UFMODBus>(UFMODBlueprintStatics::FindAssetByName("bus:/sfx"));

	if (!sfxBus)
	{
		return false;
	}

	UFMODBlueprintStatics::BusSetVolume(sfxBus, _sfxVolume);


	return true;
}