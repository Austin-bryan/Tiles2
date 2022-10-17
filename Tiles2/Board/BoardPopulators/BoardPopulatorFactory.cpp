#include "BoardPopulatorFactory.h"

// Sets default values
ABoardPopulatorFactory::ABoardPopulatorFactory()
{
	// Set this empty to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoardPopulatorFactory::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoardPopulatorFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABoardPopulatorFactory::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
