#ifndef EQEMU_PLAYER_TITLESETS_REPOSITORY_H
#define EQEMU_PLAYER_TITLESETS_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_player_titlesets_repository.h"

class PlayerTitlesetsRepository: public BasePlayerTitlesetsRepository {
public:

	static std::vector<uint32> GetTitlesetsByCharacter(Database& d, uint32 char_id)
	{
		std::vector<uint32> titlesets;

		auto r = GetWhere(d, fmt::format("`char_id` = {}", char_id));

		for (const auto& e : r) {
			titlesets.push_back(e.title_set);
		}

		return titlesets;
	}

	static bool HasTitleset(Database& d, uint32 char_id, uint32 title_set)
	{
		auto r = GetWhere(
			d,
			fmt::format("`char_id` = {} AND `title_set` = {}", char_id, title_set)
		);

		return !r.empty();
	}

	static bool AddTitleset(Database& d, uint32 char_id, uint32 title_set)
	{
		if (HasTitleset(d, char_id, title_set)) {
			return true;
		}

		auto e = NewEntity();
		e.char_id = char_id;
		e.title_set = title_set;

		auto r = InsertOne(d, e);
		return (r.id > 0);
	}

	static bool RemoveTitleset(Database& d, uint32 char_id, uint32 title_set)
	{
		int a = DeleteWhere(
			d,
			fmt::format("`char_id` = {} AND `title_set` = {}", char_id, title_set)
		);

		return (a > 0);
	}

	static int RemoveAllTitlesets(Database& d, uint32 char_id)
	{
		return DeleteWhere(d, fmt::format("`char_id` = {}", char_id));
	}

};

#endif //EQEMU_PLAYER_TITLESETS_REPOSITORY_H