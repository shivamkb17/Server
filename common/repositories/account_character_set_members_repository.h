#ifndef EQEMU_ACCOUNT_CHARACTER_SET_MEMBERS_REPOSITORY_H
#define EQEMU_ACCOUNT_CHARACTER_SET_MEMBERS_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_account_character_set_members_repository.h"

class AccountCharacterSetMembersRepository: public BaseAccountCharacterSetMembersRepository {
public:

    /**
     * Get all character IDs in a specific character set
     * @param db Database reference
     * @param set_id Set ID to get characters for
     * @return Vector of character IDs
     */
    static std::vector<int32_t> GetCharacterIdsInSet(
        Database& db,
        int32_t set_id
    )
    {
        auto members = GetWhere(db, fmt::format("set_id = {}", set_id));
        std::vector<int32_t> character_ids;
        character_ids.reserve(members.size());

        for (const auto& member : members) {
            character_ids.push_back(member.character_id);
        }

        return character_ids;
    }

    /**
     * Add a character to a character set
     * @param db Database reference
     * @param set_id Set ID to add character to
     * @param character_id Character ID to add
     * @return true on success, false if already exists or failed
     */
    static bool AddCharacterToSet(
        Database& db,
        int32_t set_id,
        int32_t character_id
    )
    {
        // Check if already exists
        auto existing = GetWhere(
            db,
            fmt::format("set_id = {} AND character_id = {}", set_id, character_id)
        );

        if (!existing.empty()) {
            return false; // Already exists
        }

        auto entity = NewEntity();
        entity.set_id = set_id;
        entity.character_id = character_id;

        auto result = InsertOne(db, entity);
        return result.set_id != 0;
    }

    /**
     * Remove a character from a character set
     * @param db Database reference
     * @param set_id Set ID to remove character from
     * @param character_id Character ID to remove
     * @return true on success, false on failure
     */
    static bool RemoveCharacterFromSet(
        Database& db,
        int32_t set_id,
        int32_t character_id
    )
    {
        int rows_affected = DeleteWhere(
            db,
            fmt::format("set_id = {} AND character_id = {}", set_id, character_id)
        );

        return rows_affected > 0;
    }

    /**
     * Move a character from one set to another
     * @param db Database reference
     * @param character_id Character ID to move
     * @param from_set_id Source set ID
     * @param to_set_id Destination set ID
     * @return true on success, false on failure
     */
    static bool MoveCharacterBetweenSets(
        Database& db,
        int32_t character_id,
        int32_t from_set_id,
        int32_t to_set_id
    )
    {
        // Remove from old set
        if (!RemoveCharacterFromSet(db, from_set_id, character_id)) {
            return false;
        }

        // Add to new set
        if (!AddCharacterToSet(db, to_set_id, character_id)) {
            // Try to add back to original set if new set addition failed
            AddCharacterToSet(db, from_set_id, character_id);
            return false;
        }

        return true;
    }

    /**
     * Get the set ID that a character belongs to
     * @param db Database reference
     * @param character_id Character ID to find set for
     * @return Set ID, or 0 if character is not in any set
     */
    static int32_t GetCharacterSetId(
        Database& db,
        int32_t character_id
    )
    {
        auto members = GetWhere(
            db,
            fmt::format("character_id = {} LIMIT 1", character_id)
        );

        if (members.empty()) {
            return 0;
        }

        return members[0].set_id;
    }

    /**
     * Remove all characters from a specific set
     * @param db Database reference
     * @param set_id Set ID to clear
     * @return Number of characters removed
     */
    static int ClearCharacterSet(
        Database& db,
        int32_t set_id
    )
    {
        return DeleteWhere(db, fmt::format("set_id = {}", set_id));
    }

    /**
     * Check if a character is in a specific set
     * @param db Database reference
     * @param set_id Set ID to check
     * @param character_id Character ID to check
     * @return true if character is in the set, false otherwise
     */
    static bool IsCharacterInSet(
        Database& db,
        int32_t set_id,
        int32_t character_id
    )
    {
        auto count = Count(
            db,
            fmt::format("set_id = {} AND character_id = {}", set_id, character_id)
        );

        return count > 0;
    }

};

#endif //EQEMU_ACCOUNT_CHARACTER_SET_MEMBERS_REPOSITORY_H