
#include "MyGameState.h"
#include "MyGameMode.h"
#include "MyPlayerState.h"
#include "Net/UnrealNetwork.h"

void AMyGameState::StartGame() {
	CurrentTime = LimitTime;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyGameState::OnTimerTick, TimerInterval, true);
}

void AMyGameState::GameOver() {
	GetWorldTimerManager().ClearTimer(TimerHandle);
	TArray<FPlayerScore> AllPlayerScore;
	for (APlayerState* player : PlayerArray) {
		AMyPlayerState* ps = Cast< AMyPlayerState>(player);
		AllPlayerScore.Add(ps->GetPlayerScore());
	}
	AMyGameMode* GM = GetWorld()->GetAuthGameMode<AMyGameMode>();
	if (GM) {
		GM->GameOver(AllPlayerScore);
	}
}

void AMyGameState::OnTimerTick() {
	if (CurrentTime >= 0.0f) {
		CurrentTime -= TimerInterval;
	}
	else {
		GameOver();
	}
}


void AMyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyGameState, CurrentTime);
}
