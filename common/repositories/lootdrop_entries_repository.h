#ifndef EQEMU_LOOTDROP_ENTRIES_REPOSITORY_H
#define EQEMU_LOOTDROP_ENTRIES_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_lootdrop_entries_repository.h"

class LootdropEntriesRepository: public BaseLootdropEntriesRepository {
public:

	static LootdropEntries NewNpcEntity()
	{
		LootdropEntries e{};

		e.lootdrop_id       = 0;
		e.item_id           = 0;
		e.item_charges      = 1;
		e.equip_item        = 1;
		e.chance            = 0;
		e.disabled_chance   = 0;
		e.trivial_min_level = 0;
		e.trivial_max_level = 0;
		e.multiplier        = 0;
		e.npc_min_level     = 0;
		e.npc_max_level     = 0;

		return e;
	}

	static uint32_t GetSharedLootdropId(Database& db, const std::vector<uint32_t>& item_ids)
	{
		if (item_ids.empty()) {
			return 0;
		}

		std::string item_list = Strings::Join(item_ids, ",");
		std::set<uint32_t> unique_items(item_ids.begin(), item_ids.end());

		auto r = db.QueryDatabase(
			fmt::format(
				"SELECT lootdrop_id, COUNT(*) as item_count "
				"FROM {} "
				"WHERE item_id IN ({}) "
				"GROUP BY lootdrop_id "
				"HAVING COUNT(*) = {} "
				"ORDER BY lootdrop_id ASC "
				"LIMIT 1",
				TableName(),
				item_list,
				unique_items.size()
			)
		);

		if (r.Success() && r.RowCount() > 0) {
			auto row = r.begin();
			return Strings::ToUnsignedInt(row[0]);
		}

		return 0;
	}

};

#endif //EQEMU_LOOTDROP_ENTRIES_REPOSITORY_H
