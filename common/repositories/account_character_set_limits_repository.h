#ifndef EQEMU_ACCOUNT_CHARACTER_SET_LIMITS_REPOSITORY_H
#define EQEMU_ACCOUNT_CHARACTER_SET_LIMITS_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_account_character_set_limits_repository.h"

class AccountCharacterSetLimitsRepository : public BaseAccountCharacterSetLimitsRepository {
public:
	static int GetExtraSets(Database &db, int32_t account_id) {
		auto e = FindOne(db, account_id);
		return e.extra_sets;
	}

	static bool SetExtraSets(Database &db, int32_t account_id, int32_t extra_sets) {
		auto e = FindOne(db, account_id);
		e.account_id = account_id;
		e.extra_sets = extra_sets;
		int rows_affected = ReplaceOne(db, e);
		if (rows_affected == 0) {
			LogError("Failed to set extra sets for account ID: {}", account_id);
			return false;
		}
		LogInfo("Set extra sets for account ID: {} to {}", account_id, extra_sets);
		return true;
	}

	static int GetMaxSets(Database &db, int32_t account_id) {
		int baseMaxSets = RuleI(Custom, MaximumBaseCharacterSets);
		auto e = FindOne(db, account_id);
		return baseMaxSets + e.extra_sets;
	}

	static bool SetDefaultSetID(Database &db, int32_t account_id, int32_t default_set_id) {
		auto e = FindOne(db, account_id);
		e.account_id = account_id;
		e.default_set = default_set_id;
		int rows_affected = ReplaceOne(db, e);
		if (rows_affected == 0) {
			LogError("Failed to update default set ID for account ID: {}", account_id);
			return false;
		}
		LogInfo("Updated default set ID for account ID: {}", account_id);
		return true;
	}

    static int GetDefaultSetID(Database &db, int32_t account_id) {
        auto e = FindOne(db, account_id);
        return e.default_set;
    }
};

#endif // EQEMU_ACCOUNT_CHARACTER_SET_LIMITS_REPOSITORY_H