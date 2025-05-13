#ifndef EQEMU_THJ_WAYPOINTS_CHARACTER_REPOSITORY_H
#define EQEMU_THJ_WAYPOINTS_CHARACTER_REPOSITORY_H

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
#include "thj_waypoints_account_repository.h"

class ThjWaypointsCharacterRepository: public BaseThjWaypointsCharacterRepository {
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

    // Check if a character has a specific waypoint
    static bool HasWaypoint(Database& db, uint64 character_id, int32 waypoint_id) {
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
    static bool AddWaypoint(Database& db, uint64 character_id, int32 waypoint_id) {
        if (HasWaypoint(db, character_id, waypoint_id)) {
            return true; // Already has it
        }

        ThjWaypointsCharacter entry = NewEntity();
        entry.character_id = character_id;
        entry.waypoint_id = waypoint_id;
        entry.unlock_time = std::time(nullptr);

        auto result = InsertOne(db, entry);
        return result.id > 0;
    }

    // Get all waypoint IDs for a character
    static std::vector<int32> GetWaypointIds(Database& db, uint64 character_id) {
        std::vector<int32> waypoint_ids;

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
};

#endif //EQEMU_THJ_WAYPOINTS_CHARACTER_REPOSITORY_H