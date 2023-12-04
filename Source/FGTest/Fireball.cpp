// Fill out your copyright notice in the Description page of Project Settings.


#include "Fireball.h"

// Sets default values
AFireball::AFireball()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetVisibility(false);
	//
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("popcorning")));
}

// Called when the game starts or when spawned
void AFireball::BeginPlay()
{
	Super::BeginPlay();	
	ProjectileData = ProjectileDataTable->FindRow<FProjectileStruct>("Fireball", "Fireball");
	Lifespan = ProjectileData->Lifespan;
}

// Called every frame
void AFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Lifespan > 0) Lifespan--;
	if (Lifespan == 0) {
		ActiveHitbox->Destroy();
		Destroy();
	}
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("Hippity hoppy")));

	// hard coded to see if it would move omo
	SetActorLocation(GetActorLocation() + FVector(ProjectileData->Speed * Direction, 0, 0));
}

void AFireball::PerformLight() {
	// load data to use
	if (ACharacter* CastOwner = Cast<ACharacter>(Owner)) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("putting")));
		SetActorLocation(CastOwner->GetActorLocation());
	}
	if (Owner) {
		Direction = Owner->IsLeftSide ? 1 : -1;
		FString Debug2 = FString::Printf(TEXT("IsLeft: (%d)"), Direction);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, Debug2);
	}
 // GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("neigh")));
 	//Hitbox stuff, need info to set

	ActiveHitbox = GetWorld()->SpawnActor<AHitbox>(AHitbox::StaticClass(), GetActorLocation() + ProjectileData->HitboxLoc, FRotator::ZeroRotator);
	ActiveHitbox->InitializeProjectile(ProjectileData, ProjectileData->HitboxScale, ProjectileData->HitboxLoc, this);
	if (ActiveHitbox) {
		if (ActiveHitbox->Owner) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "booooooooom");
		}
	}
	MeshComponent->SetVisibility(true);
}

void AFireball::PerformHeavy() {
	MeshComponent->SetVisibility(true);
}
