#include "progression_manager.h"

ProgressionManager::ProgressionManager() { }

ProgressionManager::ProgressionManager(Database* database, Database* content_db) {
	LoadData(database, content_db);
}

void ProgressionManager::LoadData(Database* database, Database* content_db) {
	ProgressionManager::m_database = database;
	ProgressionManager::m_content_database = (content_db) ? content_db : database;
}

void ProgressionManager::Reload() {
	m_flags = ProgressionFlagsRepository::All(*m_content_database);
	m_stages = ProgressionStagesRepository::All(*m_content_database);
	m_atlas = ProgressionAtlasRepository::All(*m_content_database);
}

std::vector<ProgressionFlagsRepository::ProgressionFlags> ProgressionManager::GetFlagsList() const {
	return m_flags;
}

std::vector<ProgressionStagesRepository::ProgressionStages> ProgressionManager::GetStagesList() const {
	return m_stages;
}

std::string ProgressionManager::GetFlagDescription(const std::string& flag_name) const {
	for (const auto& f : m_flags) {
		if (f.name == flag_name) {
			return f.description;
		}
	}
	return "";
}

std::vector<ProgressionStagesRepository::ProgressionStages> ProgressionManager::GetStagesForFlag(const std::string& flag_name) const {
	std::vector<ProgressionStagesRepository::ProgressionStages> stages;

	int flag_id = 0;
	for (const auto& f : m_flags) {
		if (f.name == flag_name) {
			flag_id = f.id;
			break;
		}
	}

	if (flag_id > 0) {
		for (const auto& s : m_stages) {
			if (s.flag_id == flag_id) {
				stages.push_back(s);
			}
		}
	}

	return stages;
}

std::string ProgressionManager::GetFlagForZone(const int zone_id) const {
	for (const auto& a : m_atlas) {
		if (a.zone_id == zone_id) {
			for (const auto& f : m_flags) {
				if (f.id == a.flag_id) {
					return f.name;
				}
			}
		}
	}
	return "";
}

std::string ProgressionManager::GetFlagForStage(const std::string& stage_name) const {
	for (const auto& s : m_stages) {
		if (s.name == stage_name) {
			for (const auto& f : m_flags) {
				if (f.id == s.flag_id) {
					return f.name;
				}
			}
		}
	}
	return "";
}

bool ProgressionManager::DoesStageExist(const std::string& stage_name) const {
	for (const auto& s : m_stages) {
		if (s.name == stage_name) {
			return true;
		}
	}
	return false;
}

bool ProgressionManager::DoesFlagExist(const std::string& flag_name) const {
	for (const auto& f : m_flags) {
		if (f.name == flag_name) {
			return true;
		}
	}
	return false;
}

bool ProgressionManager::IsStageUnlockedByCharacter(const int character_id, const std::string& stage_name) const {
	auto flag_name = GetFlagForStage(stage_name);

	if (flag_name.empty() || stage_name.empty() || !DoesStageExist(stage_name)) {
		return false;
	}

	DataBucketKey k = {};

	k.character_id = character_id;
	k.key = fmt::format("progression.{}.{}", flag_name, stage_name);

	return !DataBucket::GetData(k).value.empty();
}

bool ProgressionManager::IsStageUnlockedByAccount(const int account_id, const std::string& stage_name) const {
	auto flag_name = GetFlagForStage(stage_name);

	if (flag_name.empty() || stage_name.empty() || !DoesStageExist(stage_name)) {
		return false;
	}

	DataBucketKey k = {};

	k.account_id = account_id;
	k.key = fmt::format("progression.{}.{}", flag_name, stage_name);

	return !DataBucket::GetData(k).value.empty();
}

bool ProgressionManager::IsFlagUnlockedByCharacter(const int character_id, const std::string& flag_name) const {
	if (flag_name.empty() || !DoesFlagExist(flag_name)) {
		return false;
	}

	auto stages = GetStagesForFlag(flag_name);
	for (const auto& s : stages) {
		if (!IsStageUnlockedByCharacter(character_id, s.name)) {
			return false;
		}
	}

	return !stages.empty();
}

bool ProgressionManager::IsFlagUnlockedByAccount(const int account_id, const std::string& flag_name) const {
	if (flag_name.empty() || !DoesFlagExist(flag_name)) {
		return false;
	}

	auto stages = GetStagesForFlag(flag_name);
	for (const auto& s : stages) {
		if (!IsStageUnlockedByAccount(account_id, s.name)) {
			return false;
		}
	}

	return !stages.empty();
}

bool ProgressionManager::IsZoneAvailableByCharacter(const int character_id, const int zone_id) const {
	auto flag_name = GetFlagForZone(zone_id);
	if (flag_name.empty()) {
		return true;
	}
	return IsFlagUnlockedByCharacter(character_id, flag_name);
}

bool ProgressionManager::IsZoneAvailableByAccount(const int account_id, const int zone_id) const {
	auto flag_name = GetFlagForZone(zone_id);
	if (flag_name.empty()) {
		return true;
	}
	return IsFlagUnlockedByAccount(account_id, flag_name);
}

bool ProgressionManager::UnlockStageByCharacter(const int character_id, const std::string& stage_name) {
	auto flag_name = GetFlagForStage(stage_name);

	if (flag_name.empty() || stage_name.empty() || !DoesStageExist(stage_name)) {
		return false;
	}

	DataBucketKey k = {};
	k.character_id = character_id;
	k.key = fmt::format("progression.{}.{}", flag_name, stage_name);
	k.value = "true";


	DataBucket::SetData(k);
	return true;
}

bool ProgressionManager::UnlockStageByAccount(const int account_id, const std::string& stage_name) {
	auto flag_name = GetFlagForStage(stage_name);

	if (flag_name.empty() || stage_name.empty() || !DoesStageExist(stage_name)) {
		return false;
	}

	DataBucketKey k = {};
	k.account_id = account_id;
	k.key = fmt::format("progression.{}.{}", flag_name, stage_name);
	k.value = "true";

	DataBucket::SetData(k);
	return true;
}