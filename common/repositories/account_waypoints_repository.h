// account_waypoints_repository.h
#ifndef EQEMU_ACCOUNT_WAYPOINTS_REPOSITORY_H
#define EQEMU_ACCOUNT_WAYPOINTS_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_waypoints_repository.h"
#include "base/base_race_waypoints_repository.h"
#include "base/base_waypoint_categories_repository.h"
#include "base/base_character_waypoints_repository.h"
#include "base/base_account_waypoints_repository.h"

class AccountWaypointsRepository: public BaseAccountWaypointsRepository {
public:
    // Custom extended repository methods here

    // Get all waypoints for a specific account
    static std::vector<AccountWaypoints> GetByAccountId(Database& db, uint64_t account_id) {
        return GetWhere(
            db,
            fmt::format("account_id = {}", account_id)
        );
    }

    // Check if an account has a specific waypoint
    static bool HasWaypoint(Database& db, uint64_t account_id, int32_t waypoint_id) {
        auto count = Count(
            db,
            fmt::format(
                "account_id = {} AND waypoint_id = {}",
                account_id,
                waypoint_id
            )
        );

        return count > 0;
    }

    // Add a waypoint for an account (with duplicate check)
    static bool AddWaypoint(Database& db, uint64_t account_id, int32_t waypoint_id) {
        if (HasWaypoint(db, account_id, waypoint_id)) {
            return true; // Already has it
        }

        AccountWaypoints entry = NewEntity();
        entry.account_id = account_id;
        entry.waypoint_id = waypoint_id;
        entry.unlock_time = std::time(nullptr);

        auto result = InsertOne(db, entry);
        return result.id > 0;
    }

    // Get all waypoint IDs for an account
    static std::vector<int32_t> GetWaypointIds(Database& db, uint64_t account_id) {
        std::vector<int32_t> waypoint_ids;

        auto results = db.QueryDatabase(
            fmt::format(
                "SELECT waypoint_id FROM {} WHERE account_id = {}",
                TableName(),
                account_id
            )
        );

        if (results.Success()) {
            for (auto row = results.begin(); row != results.end(); ++row) {
                waypoint_ids.push_back(Strings::ToInt(row[0]));
            }
        }

        return waypoint_ids;
    }

    // Remove a specific waypoint from an account
    static bool RemoveWaypoint(Database& db, uint64_t account_id, int32_t waypoint_id) {
        return DeleteWhere(
            db,
            fmt::format(
                "account_id = {} AND waypoint_id = {}",
                account_id,
                waypoint_id
            )
        ) > 0;
    }

    // Get all accounts that have a specific waypoint
    static std::vector<uint64_t> GetAccountsWithWaypoint(Database& db, int32_t waypoint_id) {
        std::vector<uint64_t> account_ids;

        auto results = db.QueryDatabase(
            fmt::format(
                "SELECT DISTINCT account_id FROM {} WHERE waypoint_id = {}",
                TableName(),
                waypoint_id
            )
        );

        if (results.Success()) {
            for (auto row = results.begin(); row != results.end(); ++row) {
                account_ids.push_back(Strings::ToUnsignedBigInt(row[0]));
            }
        }

        return account_ids;
    }

    // Get waypoints unlocked after a certain time
    static std::vector<AccountWaypoints> GetUnlockedAfter(Database& db, uint64_t account_id, time_t timestamp) {
        return GetWhere(
            db,
            fmt::format(
                "account_id = {} AND unlock_time > FROM_UNIXTIME({})",
                account_id,
                timestamp
            )
        );
    }

    // Count waypoints for an account
    static int64 CountWaypointsForAccount(Database& db, uint64_t account_id) {
        return Count(
            db,
            fmt::format("account_id = {}", account_id)
        );
    }
};

#endif //EQEMU_ACCOUNT_WAYPOINTS_REPOSITORY_H