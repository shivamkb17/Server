// waypoints_repository.h
#ifndef EQEMU_WAYPOINTS_REPOSITORY_H
#define EQEMU_WAYPOINTS_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_waypoints_repository.h"
#include "base/base_race_waypoints_repository.h"
#include "base/base_waypoint_categories_repository.h"
#include "base/base_character_waypoints_repository.h"
#include "base/base_account_waypoints_repository.h"

class WaypointsRepository: public BaseWaypointsRepository {
public:
    // Custom extended repository methods here

    // Find a waypoint by shortname
    static Waypoints FindByShortname(Database& db, const std::string& shortname) {
        auto results = db.QueryDatabase(
            fmt::format(
                "{} WHERE shortname = '{}' LIMIT 1",
                BaseSelect(),
                Strings::Escape(shortname)
            )
        );

        auto row = results.begin();
        if (results.RowCount() == 1) {
            Waypoints e{};
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
    static std::vector<Waypoints> GetByCategory(Database& db, int32_t category) {
        return GetWhere(
            db,
            fmt::format("category = {}", category)
        );
    }

    // Get waypoints in a specific zone (by shortname pattern)
    static std::vector<Waypoints> GetByZone(Database& db, const std::string& zone_shortname) {
        return GetWhere(
            db,
            fmt::format("shortname LIKE '{}%'", Strings::Escape(zone_shortname))
        );
    }

    // Get waypoints within a radius from a position
    static std::vector<Waypoints> GetNearPosition(Database& db, float x, float y, float z, float radius) {
        auto results = db.QueryDatabase(
            fmt::format(
                "SELECT {} FROM {} WHERE SQRT(POW(x - {}, 2) + POW(y - {}, 2) + POW(z - {}, 2)) <= {}",
                SelectColumnsRaw(),
                TableName(),
                x, y, z, radius
            )
        );

        std::vector<Waypoints> all_entries;
        all_entries.reserve(results.RowCount());

        for (auto row = results.begin(); row != results.end(); ++row) {
            Waypoints e{};
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

#endif //EQEMU_WAYPOINTS_REPOSITORY_H