// Fill out your copyright notice in the Description page of Project Settings.


#include "Fireball.h"
#include "Components/SphereComponent.h"

// Sets default values
AFireball::AFireball()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Player Hurtbox"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetRelativeScale3D(FVector(1, 1, 1));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(SphereComponent);
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
	Lifespan = -1;
	GetSphereComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFireball::BeginOverlap);
}

// Called every frame
void AFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Lifespan > 0) Lifespan--;
	if (Lifespan == 0) {
		if (ActiveHitbox) ActiveHitbox->Destroy();
		Destroy();
	}
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("Hippity hoppy")));

	// hard coded to see if it would move omo
	if (ProjectileData) {
		SetActorLocation(GetActorLocation() + FVector(ProjectileData->Speed * Direction, 0, 0));
	}
}

void AFireball::PerformLight() {
	// load data to use
	if (!Owner) { return; }
	FProjectileStruct* ProjData = ProjectileDataTable->FindRow<FProjectileStruct>("LightFireball", "LightFireball");
	SpawnFireball(ProjData);
}

void AFireball::PerformHeavy() {
	if (!Owner) { return; }
	FProjectileStruct* ProjData = ProjectileDataTable->FindRow<FProjectileStruct>("HeavyFireball", "HeavyFireball");
	SpawnFireball(ProjData);
}

void AFireball::SpawnFireball(FProjectileStruct* ProjData) {
	Direction = Owner->IsLeftSide ? 1 : -1;
	ProjectileData = ProjData;
	SetActorLocation(Owner->GetActorLocation() + FVector(ProjData->HitboxLoc.X * Direction, ProjData->HitboxLoc.Y, ProjData->HitboxLoc.Z));
	SetActorScale3D(ProjData->SizeScale);
	ActiveHitbox = GetWorld()->SpawnActor<AHitbox>(AHitbox::StaticClass(), Owner->GetActorLocation() + FVector(ProjData->HitboxLoc.X * Direction, ProjData->HitboxLoc.Y, ProjData->HitboxLoc.Z), FRotator::ZeroRotator);
	ActiveHitbox->OnTriggerDelegate.BindUFunction(this, "DestroySelf");
	ActiveHitbox->Initialize(ProjData, ProjData->HitboxScale, FVector::Zero(), this);
	Lifespan = ProjData->Lifespan;
	MeshComponent->SetVisibility(true);
}

void AFireball::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (Cast<AProjectileBase>(OtherActor)) {
		if (ActiveHitbox) {
			ActiveHitbox->Destroy();
		}
		Destroy();
	}
}


