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
};

#endif //EQEMU_THJ_WAYPOINTS_REPOSITORY_H