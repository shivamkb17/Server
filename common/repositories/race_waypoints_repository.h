// race_waypoints_repository.h
#ifndef EQEMU_RACE_WAYPOINTS_REPOSITORY_H
#define EQEMU_RACE_WAYPOINTS_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_waypoints_repository.h"
#include "base/base_race_waypoints_repository.h"
#include "base/base_waypoint_categories_repository.h"
#include "base/base_character_waypoints_repository.h"
#include "base/base_account_waypoints_repository.h"

class RaceWaypointsRepository: public BaseRaceWaypointsRepository {
public:
    // Custom extended repository methods here

    // Get all waypoints for a specific race
    static std::vector<RaceWaypoints> GetByRaceId(Database& db, int32_t race_id) {
        return GetWhere(
            db,
            fmt::format("race_id = {}", race_id)
        );
    }

    // Get all waypoint IDs for a race
    static std::vector<int32_t> GetWaypointIds(Database& db, int32_t race_id) {
        std::vector<int32_t> waypoint_ids;

        auto results = db.QueryDatabase(
            fmt::format(
                "SELECT waypoint_id FROM {} WHERE race_id = {}",
                TableName(),
                race_id
            )
        );

        if (results.Success()) {
            for (auto row = results.begin(); row != results.end(); ++row) {
                waypoint_ids.push_back(Strings::ToInt(row[0]));
            }
        }

        return waypoint_ids;
    }

    // Check if a race has a specific waypoint
    static bool HasWaypoint(Database& db, int32_t race_id, int32_t waypoint_id) {
        auto count = Count(
            db,
            fmt::format(
                "race_id = {} AND waypoint_id = {}",
                race_id,
                waypoint_id
            )
        );

        return count > 0;
    }

    // Add a waypoint for a race (with duplicate check)
    static bool AddWaypoint(Database& db, int32_t race_id, int32_t waypoint_id) {
        if (HasWaypoint(db, race_id, waypoint_id)) {
            return true; // Already has it
        }

        RaceWaypoints entry = NewEntity();
        entry.race_id = race_id;
        entry.waypoint_id = waypoint_id;

        auto result = InsertOne(db, entry);
        return result.id > 0;
    }

    // Remove a waypoint from a race
    static bool RemoveWaypoint(Database& db, int32_t race_id, int32_t waypoint_id) {
        return DeleteWhere(
            db,
            fmt::format(
                "race_id = {} AND waypoint_id = {}",
                race_id,
                waypoint_id
            )
        ) > 0;
    }

    // Get all races that have a specific waypoint
    static std::vector<int32_t> GetRacesWithWaypoint(Database& db, int32_t waypoint_id) {
        std::vector<int32_t> race_ids;

        auto results = db.QueryDatabase(
            fmt::format(
                "SELECT DISTINCT race_id FROM {} WHERE waypoint_id = {}",
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

    // Get waypoints with details for a race (joins with waypoints table)
    static std::vector<std::pair<RaceWaypoints, WaypointsRepository::Waypoints>> GetWithWaypointDetails(
        Database& db,
        int32_t race_id
    ) {
        std::vector<std::pair<RaceWaypoints, WaypointsRepository::Waypoints>> result;

        auto results = db.QueryDatabase(
            fmt::format(
                "SELECT rw.{}, w.{} FROM {} rw "
                "INNER JOIN waypoints w ON rw.waypoint_id = w.id "
                "WHERE rw.race_id = {}",
                SelectColumnsRaw(),
                WaypointsRepository::SelectColumnsRaw(),
                TableName(),
                race_id
            )
        );

        if (results.Success()) {
            for (auto row = results.begin(); row != results.end(); ++row) {
                RaceWaypoints rw{};
                rw.id          = row[0] ? Strings::ToInt(row[0]) : 0;
                rw.race_id     = row[1] ? Strings::ToInt(row[1]) : 0;
                rw.waypoint_id = row[2] ? Strings::ToInt(row[2]) : 0;

                WaypointsRepository::Waypoints w{};
                w.id        = row[3] ? Strings::ToInt(row[3]) : 0;
                w.shortname = row[4] ? row[4] : "";
                w.long_name = row[5] ? row[5] : "";
                w.category  = row[6] ? Strings::ToInt(row[6]) : 0;
                w.x         = row[7] ? Strings::ToFloat(row[7]) : 0;
                w.y         = row[8] ? Strings::ToFloat(row[8]) : 0;
                w.z         = row[9] ? Strings::ToFloat(row[9]) : 0;
                w.heading   = row[10] ? Strings::ToFloat(row[10]) : 0;

                result.push_back(std::make_pair(rw, w));
            }
        }

        return result;
    }
};

#endif //EQEMU_RACE_WAYPOINTS_REPOSITORY_H