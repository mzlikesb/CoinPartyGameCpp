// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

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

void AMyGameMode::UpdateAllPlayerData() {

	if (this == nullptr) return;
	AllPlayerData.Empty();
	
	for (int i = 0; i < Players.Num(); i++) {
		AllPlayerData.Add(Players[i]->PlayerData);
		FString playerName = Players[i]->PlayerData.Name;
		int playernum = AllPlayerData.Num();
		APawn* pawn = Players[i]->GetPawn();
		if (pawn) {
			AMyCharacter* character = Cast<AMyCharacter>(pawn);
			character->SetPlayerHat(AllPlayerData[i].HatType);
			character->SetPlayerColor(AllPlayerData[i].Color);
			character->SetPlayerID(i);
		}
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


	UE_LOG(LogTemp, Error, TEXT("Respawn1 %d %d"), Players.Num(), AllPlayerData.Num());

	if (playerID >= Players.Num() || playerID >= AllPlayerData.Num()) return;

	UE_LOG(LogTemp, Error, TEXT("Respawn2"));

	AActor* playerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AMyCharacter* pawn = GetWorld()->SpawnActor<AMyCharacter>(PawnClass, playerStart->GetActorTransform(), SpawnParams);
	Players[playerID]->Possess(pawn);
	FPlayerData data = AllPlayerData[playerID];
	pawn->SetPlayerHat(data.HatType);
	pawn->SetPlayerColor(data.Color);
}