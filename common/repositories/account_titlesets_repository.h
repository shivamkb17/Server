#ifndef EQEMU_ACCOUNT_TITLESETS_REPOSITORY_H
#define EQEMU_ACCOUNT_TITLESETS_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_account_titlesets_repository.h"

class AccountTitlesetsRepository: public BaseAccountTitlesetsRepository {
public:

	static std::vector<uint32> GetTitlesetsByAccount(Database& d, uint32 account_id)
	{
		std::vector<uint32> titlesets;

		auto r = GetWhere(d, fmt::format("`account_id` = {}", account_id));

		for (const auto& e : r) {
			titlesets.push_back(e.title_set_id);
		}

		return titlesets;
	}

	static bool HasTitleset(Database& d, uint32 account_id, uint32 titleset_id)
	{
		auto r = GetWhere(
			d,
			fmt::format("`account_id` = {} AND `titleset_id` = {}", account_id, titleset_id)
		);

		return !r.empty();
	}

	static bool AddTitlesetToAccount(Database& d, uint32 account_id, uint32 titleset_id)
	{
		if (HasTitleset(d, account_id, titleset_id)) {
			return true;
		}

		auto e = NewEntity();
		e.account_id = account_id;
		e.title_set_id = titleset_id;

		auto r = InsertOne(d, e);
		return (r.account_id != 0);
	}

	static bool RemoveTitlesetFromAccount(Database& d, uint32 account_id, uint32 titleset_id)
	{
		int a = DeleteWhere(
			d,
			fmt::format("`account_id` = {} AND `titleset_id` = {}", account_id, titleset_id)
		);

		return (a > 0);
	}

	static int RemoveAllTitlesetsFromAccount(Database& d, uint32 account_id)
	{
		return DeleteWhere(d, fmt::format("`account_id` = {}", account_id));
	}

};

#endif //EQEMU_ACCOUNT_TITLESETS_REPOSITORY_H