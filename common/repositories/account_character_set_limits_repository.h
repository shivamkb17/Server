#ifndef EQEMU_ACCOUNT_CHARACTER_SET_LIMITS_REPOSITORY_H
#define EQEMU_ACCOUNT_CHARACTER_SET_LIMITS_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_account_character_set_limits_repository.h"

class AccountCharacterSetLimitsRepository : public BaseAccountCharacterSetLimitsRepository {
public:

	static AccountCharacterSetLimits GetAccountSetMeta(Database &db, uint32 account_id) {
		return FindOne(db, account_id);
	}

	static void UpdateAccountSetMeta(Database &db, AccountCharacterSetLimits data) {
		ReplaceOne(db, data);
	}

};

#endif // EQEMU_ACCOUNT_CHARACTER_SET_LIMITS_REPOSITORY_H