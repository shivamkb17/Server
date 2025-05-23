#ifndef EQEMU_ACCOUNT_CHARACTER_SETS_REPOSITORY_H
#define EQEMU_ACCOUNT_CHARACTER_SETS_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_account_character_sets_repository.h"
#include "account_character_set_members_repository.h"

class AccountCharacterSetsRepository: public BaseAccountCharacterSetsRepository {
public:

    /**
     * Create a new character set for an account
     * @param db Database reference
     * @param account_id Account ID to create set for
     * @param set_name Name of the character set
     * @return AccountCharacterSets entity with populated set_id on success, or empty entity on failure
     */
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

    /**
     * Delete a character set (only if it's empty of members)
     * @param db Database reference
     * @param set_id Set ID to delete
     * @return true on successful deletion, false if set has members or deletion failed
     */
    static bool DeleteCharacterSetIfEmpty(
        Database& db,
        int32_t set_id
    )
    {
        // Check if set has any members
        auto member_count = AccountCharacterSetMembersRepository::Count(
            db,
            fmt::format("set_id = {}", set_id)
        );

        if (member_count > 0) {
            return false; // Cannot delete set with members
        }

        // Attempt to delete the set
        int rows_affected = DeleteOne(db, set_id);
        return rows_affected > 0;
    }

    /**
     * Rename a character set
     * @param db Database reference
     * @param set_id Set ID to rename
     * @param new_name New name for the set
     * @return true on success, false on failure
     */
    static bool RenameCharacterSet(
        Database& db,
        int32_t set_id,
        const std::string& new_name
    )
    {
        auto entity = FindOne(db, set_id);
        if (entity.set_id == 0) {
            return false; // Set not found
        }

        entity.set_name = new_name;
        int rows_affected = UpdateOne(db, entity);
        return rows_affected > 0;
    }

    /**
     * List all character sets for an account
     * @param db Database reference
     * @param account_id Account ID to get sets for
     * @return Vector of AccountCharacterSets for the account
     */
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

    /**
     * Get character set with member count
     * @param db Database reference
     * @param account_id Account ID to get sets for
     * @return Vector of pairs containing set info and member count
     */
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

    /**
     * Get or create default character set for an account
     * @param db Database reference
     * @param account_id Account ID
     * @return AccountCharacterSets entity for default set
     */
    static AccountCharacterSets GetOrCreateDefaultSet(
        Database& db,
        int32_t account_id
    )
    {
        // Try to find existing default set
        auto existing_sets = GetWhere(
            db,
            fmt::format("account_id = {} AND set_name = 'Default' LIMIT 1", account_id)
        );

        if (!existing_sets.empty()) {
            return existing_sets[0];
        }

        // Create default set if it doesn't exist
        return CreateCharacterSet(db, account_id, "Default");
    }

};

#endif //EQEMU_ACCOUNT_CHARACTER_SETS_REPOSITORY_H