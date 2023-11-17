// Fill out your copyright notice in the Description page of Project Settings.


#include "PivotPoint.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPivotPoint::UPivotPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPivotPoint::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), UPivotPoint::StaticClass(), FoundActors);
	if (!FoundActors.IsEmpty())
	{
		OtherPivot = Cast<UPivotPoint>(FoundActors[0]);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("OtherPivot Exists"));
	} 
	
}


// Called every frame
void UPivotPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Rotate player towards the other player
void UPivotPoint::Face()
{

}
