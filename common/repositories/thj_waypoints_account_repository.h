#ifndef EQEMU_THJ_WAYPOINTS_ACCOUNT_REPOSITORY_H
#define EQEMU_THJ_WAYPOINTS_ACCOUNT_REPOSITORY_H

#pragma once

#include "../database.h"
#include "../strings.h"
#include "base/base_thj_waypoints_categories_repository.h"
#include "base/base_thj_waypoints_repository.h"
#include "base/base_thj_waypoints_default_repository.h"
#include "base/base_thj_waypoints_account_repository.h"
#include "base/base_thj_waypoints_character_repository.h"

#include "thj_waypoints_repository.h"
#include "thj_waypoints_categories_repository.h"
#include "thj_waypoints_default_repository.h"
#include "thj_waypoints_character_repository.h"

class ThjWaypointsAccountRepository: public BaseThjWaypointsAccountRepository {
public:
    /**
     * This file was auto generated and can be modified and extended upon
     *
     * Base repository methods are automatically
     * generated in the "base" version of this repository. The base repository
     * is immutable and to be left untouched, while methods in this class
     * are used as extension methods for more specific persistence-layer
     * accessors or mutators.
     *
     * Base Methods (Subject to be expanded upon in time)
     *
     * Note: Not all tables are designed appropriately to fit functionality with all base methods
     *
     * InsertOne
     * UpdateOne
     * DeleteOne
     * FindOne
     * GetWhere(std::string where_filter)
     * DeleteWhere(std::string where_filter)
     * InsertMany
     * All
     */

    // Get all waypoints for a specific account
    static std::vector<ThjWaypointsAccount> GetByAccountId(Database& db, uint64 account_id) {
        return GetWhere(
            db,
            fmt::format("account_id = {}", account_id)
        );
    }

    // Check if an account has a specific waypoint
    static bool HasWaypoint(Database& db, uint64 account_id, int32 waypoint_id) {
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
    static bool AddWaypoint(Database& db, uint64 account_id, int32 waypoint_id) {
        if (HasWaypoint(db, account_id, waypoint_id)) {
            return true; // Already has it
        }

        ThjWaypointsAccount entry = NewEntity();
        entry.account_id = account_id;
        entry.waypoint_id = waypoint_id;
        entry.unlock_time = std::time(nullptr);

        auto result = InsertOne(db, entry);
        return result.id > 0;
    }

    // Get all waypoint IDs for an account
    static std::vector<int32> GetWaypointIds(Database& db, uint64 account_id) {
        std::vector<int32> waypoint_ids;

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
    static bool RemoveWaypoint(Database& db, uint64 account_id, int32 waypoint_id) {
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
    static std::vector<uint64> GetAccountsWithWaypoint(Database& db, int32 waypoint_id) {
        std::vector<uint64> account_ids;

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
    static std::vector<ThjWaypointsAccount> GetUnlockedAfter(Database& db, uint64 account_id, time_t timestamp) {
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
    static int64 CountWaypointsForAccount(Database& db, uint64 account_id) {
        return Count(
            db,
            fmt::format("account_id = {}", account_id)
        );
    }
};

#endif //EQEMU_THJ_WAYPOINTS_ACCOUNT_REPOSITORY_H