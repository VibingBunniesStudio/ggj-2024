// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Referee.h"

// Sets default values
AReferee::AReferee()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AReferee::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReferee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

