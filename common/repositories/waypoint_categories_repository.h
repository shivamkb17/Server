// waypoint_categories_repository.h
#ifndef EQEMU_WAYPOINT_CATEGORIES_REPOSITORY_H
#define EQEMU_WAYPOINT_CATEGORIES_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_waypoints_repository.h"
#include "base/base_race_waypoints_repository.h"
#include "base/base_waypoint_categories_repository.h"
#include "base/base_character_waypoints_repository.h"
#include "base/base_account_waypoints_repository.h"

class WaypointCategoriesRepository: public BaseWaypointCategoriesRepository {
public:
    // Custom extended repository methods here

    // Find a category by name
    static WaypointCategories FindByName(Database& db, const std::string& name) {
        auto results = db.QueryDatabase(
            fmt::format(
                "{} WHERE name = '{}' LIMIT 1",
                BaseSelect(),
                Strings::Escape(name)
            )
        );

        auto row = results.begin();
        if (results.RowCount() == 1) {
            WaypointCategories e{};
            e.id   = row[0] ? Strings::ToInt(row[0]) : 0;
            e.name = row[1] ? row[1] : "";
            return e;
        }

        return NewEntity();
    }

    // Get all category names
    static std::vector<std::string> GetAllNames(Database& db) {
        std::vector<std::string> names;

        auto results = db.QueryDatabase(
            fmt::format(
                "SELECT name FROM {} ORDER BY id",
                TableName()
            )
        );

        if (results.Success()) {
            for (auto row = results.begin(); row != results.end(); ++row) {
                names.push_back(row[0] ? row[0] : "");
            }
        }

        return names;
    }

    // Get categories as a map (id => name)
    static std::map<int32_t, std::string> GetCategoriesMap(Database& db) {
        std::map<int32_t, std::string> categories;

        auto results = All(db);
        for (const auto& category : results) {
            categories[category.id] = category.name;
        }

        return categories;
    }
};

#endif //EQEMU_WAYPOINT_CATEGORIES_REPOSITORY_H