#ifndef EQEMU_THJ_WAYPOINTS_CATEGORIES_REPOSITORY_H
#define EQEMU_THJ_WAYPOINTS_CATEGORIES_REPOSITORY_H

#pragma once

#include "../database.h"
#include "../strings.h"
#include "base/base_thj_waypoints_categories_repository.h"
#include "base/base_thj_waypoints_repository.h"
#include "base/base_thj_waypoints_default_repository.h"
#include "base/base_thj_waypoints_account_repository.h"
#include "base/base_thj_waypoints_character_repository.h"

#include "thj_waypoints_repository.h"
#include "thj_waypoints_default_repository.h"
#include "thj_waypoints_character_repository.h"
#include "thj_waypoints_account_repository.h"

class ThjWaypointsCategoriesRepository: public BaseThjWaypointsCategoriesRepository {
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
     *
     * Example custom methods in a repository
     *
     * ThjWaypointsCategoriesRepository::GetByZoneAndVersion(int zone_id, int zone_version)
     * ThjWaypointsCategoriesRepository::GetWhereNeverExpires()
     * ThjWaypointsCategoriesRepository::GetWhereXAndY()
     * ThjWaypointsCategoriesRepository::DeleteWhereXAndY()
     *
     * Most of the above could be covered by base methods, but if you as a developer
     * find yourself re-using logic for other parts of the code, its best to just make a
     * method that can be re-used easily elsewhere especially if it can use a base repository
     * method and encapsulate filters there
     */

    // Find a category by name
    static ThjWaypointsCategories FindByName(Database& db, const std::string& name) {
        auto results = db.QueryDatabase(
            fmt::format(
                "{} WHERE name = '{}' LIMIT 1",
                BaseSelect(),
                Strings::Escape(name)
            )
        );

        auto row = results.begin();
        if (results.RowCount() == 1) {
            ThjWaypointsCategories e{};
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
    static std::map<int32, std::string> GetCategoriesMap(Database& db) {
        std::map<int32, std::string> categories;

        auto results = All(db);
        for (const auto& category : results) {
            categories[category.id] = category.name;
        }

        return categories;
    }
};

#endif //EQEMU_THJ_WAYPOINTS_CATEGORIES_REPOSITORY_H