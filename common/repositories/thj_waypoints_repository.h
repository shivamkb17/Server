#ifndef EQEMU_THJ_WAYPOINTS_REPOSITORY_H
#define EQEMU_THJ_WAYPOINTS_REPOSITORY_H

#pragma once

#include "../database.h"
#include "../strings.h"
#include "base/base_thj_waypoints_repository.h"

#include "thj_waypoints_categories_repository.h"
#include "thj_waypoints_default_repository.h"
#include "thj_waypoints_character_repository.h"
#include "thj_waypoints_account_repository.h"

class ThjWaypointsRepository: public BaseThjWaypointsRepository {
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

    // Find a waypoint by shortname
    static ThjWaypoints FindByShortname(Database& db, const std::string& shortname) {
        auto results = db.QueryDatabase(
            fmt::format(
                "{} WHERE shortname = '{}' LIMIT 1",
                BaseSelect(),
                Strings::Escape(shortname)
            )
        );

        auto row = results.begin();
        if (results.RowCount() == 1) {
            ThjWaypoints e{};
            e.id        = row[0] ? Strings::ToInt(row[0]) : 0;
            e.shortname = row[1] ? row[1] : "";
            e.long_name = row[2] ? row[2] : "";
            e.category  = row[3] ? Strings::ToInt(row[3]) : 0;
            e.x         = row[4] ? Strings::ToFloat(row[4]) : 0;
            e.y         = row[5] ? Strings::ToFloat(row[5]) : 0;
            e.z         = row[6] ? Strings::ToFloat(row[6]) : 0;
            e.heading   = row[7] ? Strings::ToFloat(row[7]) : 0;
            return e;
        }

        return NewEntity();
    }

    // Get all waypoints by category
    static std::vector<ThjWaypoints> GetByCategory(Database& db, int32 category) {
        return GetWhere(
            db,
            fmt::format("category = {}", category)
        );
    }

    // Get waypoints in a specific zone (by shortname pattern)
    static std::vector<ThjWaypoints> GetByZone(Database& db, const std::string& zone_shortname) {
        return GetWhere(
            db,
            fmt::format("shortname LIKE '{}%'", Strings::Escape(zone_shortname))
        );
    }

    // Get waypoints within a radius from a position
    static std::vector<ThjWaypoints> GetNearPosition(Database& db, float x, float y, float z, float radius) {
        auto results = db.QueryDatabase(
            fmt::format(
                "SELECT {} FROM {} WHERE SQRT(POW(x - {}, 2) + POW(y - {}, 2) + POW(z - {}, 2)) <= {}",
                SelectColumnsRaw(),
                TableName(),
                x, y, z, radius
            )
        );

        std::vector<ThjWaypoints> all_entries;
        all_entries.reserve(results.RowCount());

        for (auto row = results.begin(); row != results.end(); ++row) {
            ThjWaypoints e{};
            e.id        = row[0] ? Strings::ToInt(row[0]) : 0;
            e.shortname = row[1] ? row[1] : "";
            e.long_name = row[2] ? row[2] : "";
            e.category  = row[3] ? Strings::ToInt(row[3]) : 0;
            e.x         = row[4] ? Strings::ToFloat(row[4]) : 0;
            e.y         = row[5] ? Strings::ToFloat(row[5]) : 0;
            e.z         = row[6] ? Strings::ToFloat(row[6]) : 0;
            e.heading   = row[7] ? Strings::ToFloat(row[7]) : 0;
            all_entries.push_back(e);
        }

        return all_entries;
    }

    // Get all unique shortnames
    static std::vector<std::string> GetAllShortnames(Database& db) {
        std::vector<std::string> shortnames;

        auto results = db.QueryDatabase(
            fmt::format(
                "SELECT DISTINCT shortname FROM {} ORDER BY shortname",
                TableName()
            )
        );

        if (results.Success()) {
            for (auto row = results.begin(); row != results.end(); ++row) {
                shortnames.push_back(row[0] ? row[0] : "");
            }
        }

        return shortnames;
    }

    // Check if a waypoint exists by shortname
    static bool ExistsByShortname(Database& db, const std::string& shortname) {
        auto count = Count(
            db,
            fmt::format("shortname = '{}'", Strings::Escape(shortname))
        );

        return count > 0;
    }
};

#endif //EQEMU_THJ_WAYPOINTS_REPOSITORY_H