#ifndef EQEMU_CHARACTER_DATA_EXTRA_REPOSITORY_H
#define EQEMU_CHARACTER_DATA_EXTRA_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_character_data_extra_repository.h"

class CharacterDataExtraRepository: public BaseCharacterDataExtraRepository {
public:

	static bool GetPlayModeSolo(Database& db, int character_id)
	{
		auto entry = FindOne(db, character_id);
		return entry.play_mode_solo != 0;
	}

	static int SetPlayModeSolo(Database& db, int character_id, bool enabled)
	{
		auto entry = FindOne(db, character_id);
		if (entry.character_id == 0) {
			// Create new entry if doesn't exist
			entry = NewEntity();
			entry.character_id = character_id;
		}
		entry.play_mode_solo = enabled ? 1 : 0;

		if (entry.character_id == character_id && FindOne(db, character_id).character_id != 0) {
			return UpdateOne(db, entry);
		} else {
			InsertOne(db, entry);
			return 1;
		}
	}

	static bool GetPlayModeSelfFound(Database& db, int character_id)
	{
		auto entry = FindOne(db, character_id);
		return entry.play_mode_self_found != 0;
	}

	static int SetPlayModeSelfFound(Database& db, int character_id, bool enabled)
	{
		auto entry = FindOne(db, character_id);
		if (entry.character_id == 0) {
			// Create new entry if doesn't exist
			entry = NewEntity();
			entry.character_id = character_id;
		}
		entry.play_mode_self_found = enabled ? 1 : 0;

		if (entry.character_id == character_id && FindOne(db, character_id).character_id != 0) {
			return UpdateOne(db, entry);
		} else {
			InsertOne(db, entry);
			return 1;
		}
	}

	static bool GetPlayModeHardcore(Database& db, int character_id)
	{
		auto entry = FindOne(db, character_id);
		return entry.play_mode_hardcore != 0;
	}

	static int SetPlayModeHardcore(Database& db, int character_id, bool enabled)
	{
		auto entry = FindOne(db, character_id);
		if (entry.character_id == 0) {
			// Create new entry if doesn't exist
			entry = NewEntity();
			entry.character_id = character_id;
		}
		entry.play_mode_hardcore = enabled ? 1 : 0;

		if (entry.character_id == character_id && FindOne(db, character_id).character_id != 0) {
			return UpdateOne(db, entry);
		} else {
			InsertOne(db, entry);
			return 1;
		}
	}

	static std::tuple<bool, bool, bool> GetAllPlayModes(Database& db, int character_id)
	{
		auto entry = FindOne(db, character_id);
		return std::make_tuple(
			entry.play_mode_solo != 0,
			entry.play_mode_self_found != 0,
			entry.play_mode_hardcore != 0
		);
	}

		static uint32 GetPlayModesBitMask(Database& db, int character_id)
	{
		auto entry = FindOne(db, character_id);
		return (entry.play_mode_solo != 0 ? SOLO_MODE : 0) |
			(entry.play_mode_self_found != 0 ? SELF_FOUND_MODE : 0) |
			(entry.play_mode_hardcore != 0 ? HARDCORE_MODE : 0);
	}

	static std::unordered_map<uint, uint32> GetPlayModesBitMask(Database& db, const std::vector<uint32>& character_ids)
	{
		std::unordered_map<uint32, uint32> result;

		if (character_ids.empty()) {
			return result;
		}

		std::string id_list;
		for (size_t i = 0; i < character_ids.size(); ++i) {
			if (i > 0) {
				id_list += ",";
			}
			id_list += std::to_string(character_ids[i]);
		}

		std::string where_filter = fmt::format("character_id IN ({})", id_list);
		auto entries = GetWhere(db, where_filter);

		result.reserve(entries.size());

		for (const auto& entry : entries) {
			uint32 bitmask = (entry.play_mode_solo != 0 ? SOLO_MODE : 0) |
							(entry.play_mode_self_found != 0 ? SELF_FOUND_MODE : 0) |
							(entry.play_mode_hardcore != 0 ? HARDCORE_MODE : 0);
			result[entry.character_id] = bitmask;
		}

		return result;
	}

	static int SetAllPlayModes(Database& db, int character_id, bool solo, bool self_found, bool hardcore)
	{
		auto entry = FindOne(db, character_id);
		if (entry.character_id == 0) {
			// Create new entry if doesn't exist
			entry = NewEntity();
			entry.character_id = character_id;
		}

		entry.play_mode_solo = solo ? 1 : 0;
		entry.play_mode_self_found = self_found ? 1 : 0;
		entry.play_mode_hardcore = hardcore ? 1 : 0;

		if (entry.character_id == character_id && FindOne(db, character_id).character_id != 0) {
			return UpdateOne(db, entry);
		} else {
			InsertOne(db, entry);
			return 1;
		}
	}

	static std::vector<uint32> GetAllHardcoreInSet(Database& db, const std::vector<uint32>& character_ids)
	{
		std::vector<uint32> hardcore_chars;

		if (character_ids.empty()) {
			return hardcore_chars;
		}

		std::string id_list;
		for (size_t i = 0; i < character_ids.size(); ++i) {
			if (i > 0) {
				id_list += ",";
			}
			id_list += std::to_string(character_ids[i]);
		}

		std::string where_filter = fmt::format("character_id IN ({}) AND play_mode_hardcore != 0", id_list);
		auto e = GetWhere(db, where_filter);

		hardcore_chars.reserve(e.size());

		for (const auto& entry : e) {
			hardcore_chars.push_back(entry.character_id);
		}

		return hardcore_chars;
	}

};

#endif //EQEMU_CHARACTER_DATA_EXTRA_REPOSITORY_H