#ifndef EQEMU_ACCOUNT_CHARACTER_SETS_REPOSITORY_H
#define EQEMU_ACCOUNT_CHARACTER_SETS_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_account_character_sets_repository.h"
#include "account_character_set_members_repository.h"
#include "account_character_set_limits_repository.h"

class AccountCharacterSetsRepository: public BaseAccountCharacterSetsRepository {
public:
    static AccountCharacterSets CreateCharacterSet(
        Database& db,
        int32_t account_id,
        const std::string& set_name
    )
    {
        auto entity = NewEntity();
        entity.account_id = account_id;
        entity.set_name = set_name;
        entity.created_at = std::time(nullptr);

        return InsertOne(db, entity);
    }

    static bool DeleteCharacterSetIfEmpty(Database& db, int32_t set_id) {
        auto c = AccountCharacterSetMembersRepository::Count(db, fmt::format("set_id = {}", set_id));

        if (c > 0) {
            return false;
        }

        int rows_affected = DeleteOne(db, set_id);
        return rows_affected > 0;
    }

    static bool RenameCharacterSet(
        Database& db,
        int32_t set_id,
        const std::string& new_name
    )
    {
        auto entity = FindOne(db, set_id);
        if (entity.set_id == 0) {
            return false;
        }

        entity.set_name = new_name;
        int rows_affected = UpdateOne(db, entity);
        return rows_affected > 0;
    }

    static std::vector<AccountCharacterSets> GetAccountCharacterSets(
        Database& db,
        int32_t account_id
    )
    {
        return GetWhere(
            db,
            fmt::format("account_id = {} ORDER BY set_name", account_id)
        );
    }

    static std::vector<std::pair<AccountCharacterSets, int64>> GetAccountCharacterSetsWithMemberCount(
        Database& db,
        int32_t account_id
    )
    {
        auto sets = GetAccountCharacterSets(db, account_id);
        std::vector<std::pair<AccountCharacterSets, int64>> result;

        for (const auto& set : sets) {
            int64 member_count = AccountCharacterSetMembersRepository::Count(
                db,
                fmt::format("set_id = {}", set.set_id)
            );
            result.emplace_back(set, member_count);
        }

        return result;
    }

};

#endif //EQEMU_ACCOUNT_CHARACTER_SETS_REPOSITORY_H