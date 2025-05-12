#ifndef EQEMU_THJ_WAYPOINTS_DEFAULT_REPOSITORY_H
#define EQEMU_THJ_WAYPOINTS_DEFAULT_REPOSITORY_H

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
#include "thj_waypoints_character_repository.h"
#include "thj_waypoints_account_repository.h"

class ThjWaypointsDefaultRepository: public BaseThjWaypointsDefaultRepository {
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

    // Get all waypoints for a specific race
    static std::vector<ThjWaypointsDefault> GetByRaceId(Database& db, int32 race_id) {
        return GetWhere(
            db,
            fmt::format("race_id = {}", race_id)
        );
    }

    // Get all waypoint IDs for a character based on race, class, and level
    static std::vector<int32> GetWaypointIdsForCharacter(Database& db, int32 race_id, int32 class_id, int32 level) {
        std::vector<int32> waypoint_ids;

        // Class bitmask for the character's class (1 << (class_id - 1))
        uint32 class_bit = 1 << (class_id - 1);

        // Get waypoints where:
        // 1. Race matches OR race is 0 (any race) AND
        // 2. Class bitmask has the character's class bit set OR class_mask is 65535 (all classes) AND
        // 3. Level is between min_level and max_level
        auto results = db.QueryDatabase(
            fmt::format(
                "SELECT DISTINCT waypoint_id FROM {} "
                "WHERE (race_id = {} OR race_id = 0) "
                "AND ((class_mask & {}) > 0 OR class_mask = 65535) "
                "AND {} BETWEEN min_level AND max_level "
                "ORDER BY waypoint_id",
                TableName(),
                race_id,
                class_bit,
                level
            )
        );

        if (results.Success()) {
            for (auto row = results.begin(); row != results.end(); ++row) {
                waypoint_ids.push_back(Strings::ToInt(row[0]));
            }
        }

        return waypoint_ids;
    }

    // Check if a specific race, class, level combination has a specific waypoint
    static bool HasWaypoint(Database& db, int32 race_id, int32 class_id, int32 level, int32 waypoint_id) {
        // Class bitmask for the character's class
        uint32 class_bit = 1 << (class_id - 1);

        auto count = Count(
            db,
            fmt::format(
                "waypoint_id = {} "
                "AND (race_id = {} OR race_id = 0) "
                "AND ((class_mask & {}) > 0 OR class_mask = 65535) "
                "AND {} BETWEEN min_level AND max_level",
                waypoint_id,
                race_id,
                class_bit,
                level
            )
        );

        return count > 0;
    }

    // Add a default waypoint
    static bool AddWaypoint(
        Database& db,
        int32 waypoint_id,
        int32 race_id = 0,
        uint32 class_mask = 65535,
        int32 min_level = 1,
        int32 max_level = 255
    ) {
        ThjWaypointsDefault entry = NewEntity();
        entry.waypoint_id = waypoint_id;
        entry.race_id = race_id;
        entry.class_mask = class_mask;
        entry.min_level = min_level;
        entry.max_level = max_level;

        auto result = InsertOne(db, entry);
        return result.id > 0;
    }

    // Remove a specific default waypoint
    static bool RemoveWaypoint(
        Database& db,
        int32 waypoint_id,
        int32 race_id = 0,
        uint32 class_mask = 65535,
        int32 min_level = 1
    ) {
        return DeleteWhere(
            db,
            fmt::format(
                "waypoint_id = {} AND race_id = {} AND class_mask = {} AND min_level = {}",
                waypoint_id,
                race_id,
                class_mask,
                min_level
            )
        ) > 0;
    }

    // Get all races that have a specific waypoint
    static std::vector<int32> GetRacesWithWaypoint(Database& db, int32 waypoint_id) {
        std::vector<int32> race_ids;

        auto results = db.QueryDatabase(
            fmt::format(
                "SELECT DISTINCT race_id FROM {} WHERE waypoint_id = {} AND race_id > 0",
                TableName(),
                waypoint_id
            )
        );

        if (results.Success()) {
            for (auto row = results.begin(); row != results.end(); ++row) {
                race_ids.push_back(Strings::ToInt(row[0]));
            }
        }

        return race_ids;
    }

    // Get all classes that have a specific waypoint (returns class IDs, not bitmask)
    static std::vector<int32> GetClassesWithWaypoint(Database& db, int32 waypoint_id) {
        std::vector<int32> class_ids;

        // We need to query all possible class bits (1-16)
        for (int i = 0; i < 16; i++) {
            uint32 class_bit = 1 << i;

            auto count = Count(
                db,
                fmt::format(
                    "waypoint_id = {} AND (class_mask & {}) > 0 AND class_mask != 65535",
                    waypoint_id,
                    class_bit
                )
            );

            if (count > 0) {
                // Class IDs are 1-based, so add 1 to the bit position
                class_ids.push_back(i + 1);
            }
        }

        return class_ids;
    }
};

#endif //EQEMU_THJ_WAYPOINTS_DEFAULT_REPOSITORY_H