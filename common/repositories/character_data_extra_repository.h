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

};

#endif //EQEMU_CHARACTER_DATA_EXTRA_REPOSITORY_H