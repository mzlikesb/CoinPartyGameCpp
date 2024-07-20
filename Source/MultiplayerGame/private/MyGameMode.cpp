// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "MyPlayerState.h"
#include "MyGameState.h"

AMyGameMode::AMyGameMode()
{
	
}

void AMyGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	
	AMyPlayerController* pc = Cast<AMyPlayerController>(NewPlayer);
	if (pc) {
		Players.Add(pc);
		pc->RequestClientToSendPlayerData();
	}
}


void AMyGameMode::BeginPlay() {
	Super::BeginPlay();

	AMyGameState* GS = GetGameState<AMyGameState>();
	if (GS) {
		GS->StartGame();
	}
}

void AMyGameMode::UpdateAllPlayerData() {

	if (this == nullptr) return;
	AllPlayerData.Empty();
	
	for (int i = 0; i < Players.Num(); i++) {
		AllPlayerData.Add(Players[i]->PlayerData);
		FString playerName = Players[i]->PlayerData.Name;

		int playernum = AllPlayerData.Num();
		APawn* pawn = Players[i]->GetPawn();
		if (!pawn) continue;

		// server pawn은 아직 생성되지 않음
		AMyCharacter* character = Cast<AMyCharacter>(pawn);
		if (!character) continue;
		InitPawn(character, i);
	}
	for (AMyPlayerController* player : Players) {
		player->UpdateAllPlayerData(AllPlayerData);
	}
}

void AMyGameMode::Logout(AController* Exiting) {
	Super::Logout(Exiting);

	AMyPlayerController* player = Cast<AMyPlayerController>(Exiting);
	if (player) {
		int32 index = Players.Find(player);
		if (index >= Players.Num() || index >= AllPlayerData.Num()) return;
		Players.RemoveAt(index);
		AllPlayerData.RemoveAt(index);
		UpdateAllPlayerData();
	}
}

void AMyGameMode::RespawnPlayer(uint8 playerID) {

	if (playerID >= Players.Num() || playerID >= AllPlayerData.Num()) return;


	AActor* playerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AMyCharacter* pawn = GetWorld()->SpawnActor<AMyCharacter>(PawnClass, playerStart->GetActorTransform(), SpawnParams);
	Players[playerID]->Possess(pawn);
	FPlayerData data = AllPlayerData[playerID];
	InitPawn(pawn, playerID);
}

void AMyGameMode::InitPawn(AMyCharacter* pawn, uint8 id){
	if (this == nullptr) return;
	pawn->SetPlayerHat(AllPlayerData[id].HatType);
	pawn->SetPlayerColor(AllPlayerData[id].Color);
	pawn->SetPlayerID(id);

	AMyPlayerState* ps = Cast<AMyPlayerState>(pawn->GetPlayerState());
	if (ps) {
		ps->InitScore(id, AllPlayerData[id].Name);
	}
}

void AMyGameMode::GameOver(TArray<FPlayerScore>& AllPlayerScore) {

	AMyGameState* GS = GetGameState<AMyGameState>();
	if (GS) {
		for (AMyPlayerController* Player : Players) {
			Player->ShowGameOverWidget(AllPlayerScore);
		}
	}
}