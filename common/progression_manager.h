
#ifndef PROGRESSION_H
#define PROGRESSION_H

#include <vector>

#include "database.h"
#include "data_bucket.h"
#include "repositories/progression_atlas_repository.h"
#include "repositories/progression_flags_repository.h"
#include "repositories/progression_stages_repository.h"

/*
Flags are overall objectives composed of one or more Stages.
Flags have names and descriptions
Stages have names and an associated flag

Atlas is provided as an example of how these can be used.
*/

class ProgressionManager {
public:
	ProgressionManager();
	ProgressionManager(Database* database, Database* content_db = nullptr);

	void Reload();
	void LoadData(Database* database, Database* content_db);

	std::vector<ProgressionFlagsRepository::ProgressionFlags> GetFlagsList() const;
	std::vector<ProgressionStagesRepository::ProgressionStages> GetStagesList() const;

	std::string GetFlagDescription(const std::string& flag_name) const;
	std::vector<ProgressionStagesRepository::ProgressionStages> GetStagesForFlag(const std::string& flag_name) const;
	std::string GetFlagForStage(const std::string& stage_name) const;
	bool DoesStageExist(const std::string& stage_name) const;
	bool DoesFlagExist(const std::string& flag_name) const;

	std::string GetFlagForZone(const int zone_id) const;

	bool IsFlagUnlockedByCharacter(const int character_id, const std::string& flag_name) const;
	bool IsFlagUnlockedByAccount(const int account_id, const std::string& flag_name) const;

	bool IsStageUnlockedByCharacter(const int character_id, const std::string& stage_name) const;
	bool IsStageUnlockedByAccount(const int account_id, const std::string& stage_name) const;

	bool UnlockStageByCharacter(const int character_id, const std::string& stage_name);
	bool UnlockStageByAccount(const int account_id, const std::string& stage_name);

	bool IsZoneAvailableByCharacter(const int character_id, const int zone_id) const;
	bool IsZoneAvailableByAccount(const int account_id, const int zone_id) const;

private:
	Database* m_database;
	Database* m_content_database;

	std::vector<ProgressionFlagsRepository::ProgressionFlags>	m_flags;
	std::vector<ProgressionStagesRepository::ProgressionStages>	m_stages;
	std::vector<ProgressionAtlasRepository::ProgressionAtlas>	m_atlas;
};

/*
// Client Methods

Mob::IsProgressionFlagUnlocked(const std::string& flag_name);
Mob::IsProgressionStageUnlocked(const std::string& stage_name);
Mob::UnlockProgressionStage(const std::string& stage_name);
*/

#endif