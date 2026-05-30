// Alexandr Sharizanov. All rights reserved.


#include "Framework/ArkanoidGameInstance.h"
#include "GameFramework/GameUserSettings.h"
#include "SaveFiles/RecordsSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "AssetRegistry/AssetRegistryModule.h"

void UArkanoidGameInstance::Init()
{
	Super::Init();

	SetGameSettings();
	LoadRecords();
	CollectGameLevels();
}

void UArkanoidGameInstance::CollectGameLevels()
{
	LevelNames.Empty();

	const FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	FARFilter Filter;
	Filter.PackagePaths.Add(*LevelsPath);
	Filter.bRecursivePaths = false;

	Filter.ClassPaths.Add(UWorld::StaticClass()->GetClassPathName());

	TArray<FAssetData> AssetDataList;
	AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

	for (const FAssetData& AssetData : AssetDataList)
	{
		LevelNames.Add(AssetData.AssetName.ToString());
	}

	LevelNames.Sort();
}

void UArkanoidGameInstance::SetGameSettings() const
{
	if (!GEngine)
		return;

	GEngine->GameUserSettings->SetVSyncEnabled(true);
	GEngine->GameUserSettings->ApplySettings(true);
	GEngine->GameUserSettings->SaveSettings();
	GEngine->Exec(GetWorld(), TEXT("t.MaxFPS 60"));
}

void UArkanoidGameInstance::SaveRecords() const
{
	auto SaveGameFile = Cast<URecordsSaveGame>(UGameplayStatics::LoadGameFromSlot(NameSaveFile, 0));

	if (!SaveGameFile)
		SaveGameFile = Cast<URecordsSaveGame>(UGameplayStatics::CreateSaveGameObject(URecordsSaveGame::StaticClass()));
	
	if (SaveGameFile)
	{		
		SaveGameFile->SavedRecords = LevelRecords;
	    UGameplayStatics::SaveGameToSlot(SaveGameFile, NameSaveFile, 0);
	}
}

void UArkanoidGameInstance::LoadRecords()
{
	const auto LoadedSaveGame = UGameplayStatics::LoadGameFromSlot(NameSaveFile, 0);

	if (const auto SaveGameFile = Cast<URecordsSaveGame>(LoadedSaveGame))
	    LevelRecords = SaveGameFile->SavedRecords;
}

void UArkanoidGameInstance::DeleteRecords() const
{
	if (UGameplayStatics::DoesSaveGameExist(NameSaveFile, 0))
	{ 
		UGameplayStatics::DeleteGameInSlot(NameSaveFile, 0);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, ("Deleted Records %s", *NameSaveFile));
	}
}

void UArkanoidGameInstance::OpenNextLevel(const FString& CurrentLevelName)
{
	const int32 CurrentLevelIndex = LevelNames.Find(CurrentLevelName);

	// Попытка открыть следующий уровень
	if (CurrentLevelIndex != INDEX_NONE)
	{
		const int32 NextIndex = CurrentLevelIndex + 1;
		if (LevelNames.IsValidIndex(NextIndex))
		{
			const FString NextLevelName = LevelNames[NextIndex];
			if (FPackageName::DoesPackageExist(FString::Printf(TEXT("%s/%s"), *LevelsPath, *NextLevelName)))
			{
				UGameplayStatics::OpenLevel(this, FName(*NextLevelName));
				return;
			}
		}
	}

	// Если следующего уровня нет (последний) — открыть первый уровень (wrap-around), если он существует
	if (LevelNames.IsValidIndex(0))
	{
		const FString FirstLevelName = LevelNames[0];
		if (FPackageName::DoesPackageExist(FString::Printf(TEXT("%s/%s"), *LevelsPath, *FirstLevelName)))
		{
			UGameplayStatics::OpenLevel(this, FName(*FirstLevelName));
			return;
		}
	}

	// Фолбэк — меню, если ничего не найдено
	UGameplayStatics::OpenLevel(this, FName("Menu"));
}

void UArkanoidGameInstance::SetLevelRecord(const FString& LevelName, const int32 NewRecord)
{
	if (NewRecord > GetLevelRecord(LevelName))
	{
		LevelRecords.Add(LevelName, NewRecord);
		SaveRecords();
	}
}

int32 UArkanoidGameInstance::GetLevelRecord(const FString& LevelName) const
{
	return LevelRecords.FindRef(LevelName);
}
