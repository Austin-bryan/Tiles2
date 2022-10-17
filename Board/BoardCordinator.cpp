#include "Board/BoardCordinator.h"
#include <Kismet/GameplayStatics.h>
#include "Logger.h"

void UBoardCordinator::BeginPlay()
{
	//Super::BeginPlay();
	//TArray<AActor*> actors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoard::StaticClass(), actors);
	////instance = this;
	//board = dynamic_cast<ABoard*>(actors[0]);
}
void UBoardCordinator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

