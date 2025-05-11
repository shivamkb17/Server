// character_waypoints_repository.h
#ifndef EQEMU_CHARACTER_WAYPOINTS_REPOSITORY_H
#define EQEMU_CHARACTER_WAYPOINTS_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_waypoints_repository.h"
#include "base/base_race_waypoints_repository.h"
#include "base/base_waypoint_categories_repository.h"
#include "base/base_character_waypoints_repository.h"
#include "base/base_account_waypoints_repository.h"

class CharacterWaypointsRepository: public BaseCharacterWaypointsRepository {
public:
    // Custom extended repository methods here

    // Get all waypoints for a specific character
    static std::vector<CharacterWaypoints> GetByCharacterId(Database& db, uint64_t character_id) {
        return GetWhere(
            db,
            fmt::format("character_id = {}", character_id)
        );
    }

    // Check if a character has a specific waypoint
    static bool HasWaypoint(Database& db, uint64_t character_id, int32_t waypoint_id) {
        auto count = Count(
            db,
            fmt::format(
                "character_id = {} AND waypoint_id = {}",
                character_id,
                waypoint_id
            )
        );

        return count > 0;
    }

    // Add a waypoint for a character (with duplicate check)
    static bool AddWaypoint(Database& db, uint64_t character_id, int32_t waypoint_id) {
        if (HasWaypoint(db, character_id, waypoint_id)) {
            return true; // Already has it
        }

        CharacterWaypoints entry = NewEntity();
        entry.character_id = character_id;
        entry.waypoint_id = waypoint_id;
        entry.unlock_time = std::time(nullptr);

        auto result = InsertOne(db, entry);
        return result.id > 0;
    }

    // Get all waypoint IDs for a character
    static std::vector<int32_t> GetWaypointIds(Database& db, uint64_t character_id) {
        std::vector<int32_t> waypoint_ids;

        auto results = db.QueryDatabase(
            fmt::format(
                "SELECT waypoint_id FROM {} WHERE character_id = {}",
                TableName(),
                character_id
            )
        );

        if (results.Success()) {
            for (auto row = results.begin(); row != results.end(); ++row) {
                waypoint_ids.push_back(Strings::ToInt(row[0]));
            }
        }

        return waypoint_ids;
    }

    // Remove a specific waypoint from a character
    static bool RemoveWaypoint(Database& db, uint64_t character_id, int32_t waypoint_id) {
        return DeleteWhere(
            db,
            fmt::format(
                "character_id = {} AND waypoint_id = {}",
                character_id,
                waypoint_id
            )
        ) > 0;
    }

    // Copy waypoints from account to character
    static bool CopyFromAccount(Database& db, uint64_t character_id, uint64_t account_id) {
        auto results = db.QueryDatabase(
            fmt::format(
                "INSERT IGNORE INTO {} (character_id, waypoint_id, unlock_time) "
                "SELECT {}, waypoint_id, unlock_time FROM account_waypoints WHERE account_id = {}",
                TableName(),
                character_id,
                account_id
            )
        );

        return results.Success();
    }

    // Get waypoints that a character has but the account doesn't
    static std::vector<int32_t> GetCharacterExclusiveWaypoints(
        Database& db,
        uint64_t character_id,
        uint64_t account_id
    ) {
        std::vector<int32_t> exclusive_waypoints;

        auto results = db.QueryDatabase(
            fmt::format(
                "SELECT cw.waypoint_id FROM {} cw "
                "LEFT JOIN account_waypoints aw ON cw.waypoint_id = aw.waypoint_id "
                "AND aw.account_id = {} "
                "WHERE cw.character_id = {} AND aw.id IS NULL",
                TableName(),
                account_id,
                character_id
            )
        );

        if (results.Success()) {
            for (auto row = results.begin(); row != results.end(); ++row) {
                exclusive_waypoints.push_back(Strings::ToInt(row[0]));
            }
        }

        return exclusive_waypoints;
    }

    // Sync character waypoints with account waypoints
    static bool SyncWithAccount(Database& db, uint64_t character_id, uint64_t account_id) {
        // Add account waypoints to character
        return CopyFromAccount(db, character_id, account_id);
    }

    // Get waypoints unlocked after a certain time
    static std::vector<CharacterWaypoints> GetUnlockedAfter(Database& db, uint64_t character_id, time_t timestamp) {
        return GetWhere(
            db,
            fmt::format(
                "character_id = {} AND unlock_time > FROM_UNIXTIME({})",
                character_id,
                timestamp
            )
        );
    }

    // Count waypoints for a character
    static int64 CountWaypointsForCharacter(Database& db, uint64_t character_id) {
        return Count(
            db,
            fmt::format("character_id = {}", character_id)
        );
    }

    // Add multiple waypoints at once
    static int AddManyWaypoints(Database& db, uint64_t character_id, const std::vector<int32_t>& waypoint_ids) {
        std::vector<CharacterWaypoints> entries;

        for (auto waypoint_id : waypoint_ids) {
            if (!HasWaypoint(db, character_id, waypoint_id)) {
                CharacterWaypoints entry = NewEntity();
                entry.character_id = character_id;
                entry.waypoint_id = waypoint_id;
                entry.unlock_time = std::time(nullptr);
                entries.push_back(entry);
            }
        }

        return InsertMany(db, entries);
    }
};

#endif //EQEMU_CHARACTER_WAYPOINTS_REPOSITORY_H