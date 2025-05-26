#ifndef EQEMU_CHARACTER_PET_COMMANDSTATES_REPOSITORY_H
#define EQEMU_CHARACTER_PET_COMMANDSTATES_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "../../zone/common.h"
#include "../classes.h"
#include "base/base_character_pet_commandstates_repository.h"

class CharacterPetCommandstatesRepository: public BaseCharacterPetCommandstatesRepository {
public:

	struct PetCommandStates {
		bool assist = false;
		bool hold = false;
		bool ghold = false;
		bool focus = false;
		bool spellhold = false;
		bool taunt = false;


		PetCommandStates(int8_t pet_class = Class::None) {
			assist = (pet_class == Class::None);
		}
	};

	static PetCommandStates GetAllCommandStates(Database& db, int32_t character_id, int8_t pet_class)
	{
		PetCommandStates states(pet_class);

		auto results = GetWhere(db, fmt::format(
			"character_id = {} AND pet_class = {}",
			character_id, pet_class
		));

		for (const auto& entry : results) {
			switch (entry.command_id) {
				case CUSTOM_PET_ASSIST: states.assist = (entry.command_state == 1); break;
				case PET_HOLD: states.hold = (entry.command_state == 1); break;
				case PET_GHOLD: states.ghold = (entry.command_state == 1); break;
				case PET_FOCUS: states.focus = (entry.command_state == 1); break;
				case PET_SPELLHOLD: states.spellhold = (entry.command_state == 1); break;
				case PET_TAUNT: states.taunt = (entry.command_state == 1); break;
			}
		}

		return states;
	}

	static bool GetCommandState(Database& db, int32_t character_id, int8_t pet_class, int8_t command_id)
	{
		auto results = GetWhere(db, fmt::format(
			"character_id = {} AND pet_class = {} AND command_id = {}",
			character_id, pet_class, command_id
		));

		if (!results.empty()) {
			return results[0].command_state;
		}

		// doing this here prevents another db access for the default state of swarm pets, even if it is a bit awful.
		return (command_id == CUSTOM_PET_ASSIST && pet_class == Class::None) ? true : false;
	}

	static void SetCommandState(Database& db, int32_t character_id, int8_t pet_class, int8_t command_id, int8_t state)
	{
		CharacterPetCommandstates entry;
		entry.character_id = character_id;
		entry.pet_class = pet_class;
		entry.command_id = command_id;
		entry.command_state = state;

		ReplaceOne(db, entry);
	}

	static bool IsCommandEnabled(Database& db, int32_t character_id, int8_t pet_class, int8_t command_id)
	{
		return GetCommandState(db, character_id, pet_class, command_id);
	}

	static std::vector<CharacterPetCommandstates> GetForCharacterAndPetClass(Database& db, int32_t character_id, int8_t pet_class)
	{
		return GetWhere(db, fmt::format(
			"character_id = {} AND pet_class = {}",
			character_id, pet_class
		));
	}

};

#endif //EQEMU_CHARACTER_PET_COMMANDSTATES_REPOSITORY_H