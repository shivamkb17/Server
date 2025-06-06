#ifndef EQEMU_ACCOUNT_CHARACTER_SET_LIMITS_REPOSITORY_H
#define EQEMU_ACCOUNT_CHARACTER_SET_LIMITS_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_account_character_set_limits_repository.h"

class AccountCharacterSetLimitsRepository : public BaseAccountCharacterSetLimitsRepository
{
public:
	 static bool SetExtraSets(Database &db, int32_t account_id, int32_t extra_sets)
	 {
		auto results = GetWhere(
			db,
			fmt::format("account_id = {} LIMIT 1", account_id)
		);
		if (results.empty()) {
			LogError("No account character set limits found for account ID: {}", account_id);
			return false; // No entry found
		}
		results[0].extra_sets = extra_sets;
		int rows_affected = UpdateOne(db, results[0]);
		if (rows_affected == 0) {
			LogError("Failed to update max sets for account ID: {}", account_id);
			return false; // Update failed
		}
		LogInfo("Set max sets for account ID: {} to {}", account_id, results[0].extra_sets);
		return true;
	 }

	static int GetMaxSets(Database &db, int32_t account_id)
	{
		int baseMaxSets = RuleI(Custom, MaximumBaseCharacterSets);
		auto results = GetWhere(
			db,
			fmt::format("account_id = {} LIMIT 1", account_id)
		);
		if (results.empty()) {
			LogError("No account character set limits found for account ID: {}", account_id);
			return 0; // No entry found
		}
		return baseMaxSets + results[0].extra_sets;
	}


	static bool UpdateDefaultSetID(Database &db, int32_t account_id, int32_t default_set_id)
	{
		auto results = GetWhere(
			db,
			fmt::format("account_id = {} LIMIT 1", account_id)
		);
		if (results.empty()) {
			LogError("No account character set limits found for account ID: {}", account_id);
			return false; // No entry found
		}
		results[0].default_set = default_set_id;
		int rows_affected = UpdateOne(db, results[0]);
		if (rows_affected == 0) {
			LogError("Failed to update default set ID for account ID: {}", account_id);
			return false;
		}
		LogInfo("Updated default set ID for account ID: {}", account_id);
		return true;
	}

	static int GetDefaultSetId(
		Database &db,
		int32_t account_id
	)
	{
		auto results = GetWhere(
			db,
			fmt::format("account_id = {} LIMIT 1", account_id)
		);
		if (results.empty()) {
			LogError("No account character set limits found for account ID: {}", account_id);
			return 0; // No entry found
		}
		return results[0].default_set;


	}
	static bool UpdateOrCreateAccountCharacterSetLimits(
		Database &db,
		int32_t account_id,
		int32_t created_sets,
		int32_t extra_sets,
		int32_t default_set)
	{
		auto existingEntry = GetWhere(
            db,
            fmt::format("account_id = {} LIMIT 1", account_id)
        );

        if (!existingEntry.empty()) {
            LogInfo("Data entry for set limits already exists for account ID: {}", account_id);
			// Update the existing entry
			existingEntry[0].created_sets = created_sets;
			existingEntry[0].extra_sets = extra_sets;
			existingEntry[0].default_set = default_set;

			int rows_affected = UpdateOne(db, existingEntry[0]);

			if (rows_affected == 0) {
				LogError("Failed to update account character set limits for account ID: {}", account_id);
				return false;
			}

			LogInfo("Updated account character set limits for account ID: {}", account_id);
			return true;
        }
			AccountCharacterSetLimits new_limits{};
			new_limits.account_id = account_id;
			new_limits.created_sets = created_sets;
			new_limits.extra_sets = extra_sets;
			new_limits.default_set = default_set;
			AccountCharacterSetLimits newEntry =  InsertOne(db, new_limits);
			return true;
	}
};

#endif // EQEMU_ACCOUNT_CHARACTER_SET_LIMITS_REPOSITORY_H
