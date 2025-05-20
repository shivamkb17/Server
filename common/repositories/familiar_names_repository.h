#ifndef EQEMU_FAMILIAR_NAMES_REPOSITORY_H
#define EQEMU_FAMILIAR_NAMES_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_familiar_names_repository.h"

class FamiliarNamesRepository : public BaseFamiliarNamesRepository
{
public:
	/**
	 * Gets all possible familiar names for a specific spell ID
	 * @param db Database reference
	 * @param spell_id The spell ID to look up names for
	 * @return A vector of possible familiar names
	 */
	static std::vector<std::string> GetFamiliarNamesBySpellID(Database &db, int spell_id)
	{
		auto familiar_names = FindOne(db, spell_id);
		if (familiar_names.spell_id == 0)
		{
			// No names found for this spell ID
			return std::vector<std::string>();
		}

		return Strings::Split(familiar_names.name_list, ',');
	}

	/**
	 * Gets a random name from the name list for a specific spell ID
	 * @param db Database reference
	 * @param spell_id The spell ID to look up a name for
	 * @param default_name A fallback name to use if no names are found
	 * @return A randomly selected name from the list or the default name
	 */
	static std::string GetRandomFamiliarName(Database &db, int spell_id, std::string default_name)
	{
		auto names = GetFamiliarNamesBySpellID(db, spell_id);
		if (names.empty())
		{
			return default_name;
		}

		// Get a random index within the available names
		size_t index = static_cast<size_t>(rand()) % names.size();
		return names[index];
	}

	/**
	 * Gets the size modifier for a familiar by spell ID
	 *
	 * @param db
	 * @param spell_id
	 * @return int The size modifier, or -1 if not found
	 */
	static int GetFamiliarSizeMod(
		Database &db,
		int spell_id)
	{
		auto entity = FindOne(db, spell_id);
		if (entity.spell_id == 0)
		{
			return 0;
		}

		return entity.size_mod;
	}
};

#endif // EQEMU_FAMILIAR_NAMES_REPOSITORY_H