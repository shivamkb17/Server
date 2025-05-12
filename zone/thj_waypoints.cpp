#include "../common/repositories/thj_waypoints_repository.h"
#include "../common/repositories/thj_waypoints_categories_repository.h"
#include "../common/repositories/thj_waypoints_default_repository.h"
#include "../common/repositories/thj_waypoints_account_repository.h"
#include "../common/repositories/thj_waypoints_character_repository.h"

#include "client.h"
#include "zone.h"

#include "thj_waypoints.h"

std::vector<ThjWaypointsRepository::ThjWaypoints>& Zone::GetAllWaypoints(bool force_reload) {
    if (force_reload || m_all_waypoints.empty()) {
        m_all_waypoints = ThjWaypointsRepository::All(content_db);
    }

    return m_all_waypoints;
}

bool Zone::WaypointShouldSpawn(std::string zone_shortname) {

}

std::vector<ThjWaypointsRepository::ThjWaypoints>& Client::GetUnlockedWaypoints(bool force_reload) {
    if (force_reload || m_unlocked_waypoints.empty()) {
        LogDebug("Querying Database for waypoints");
        m_unlocked_waypoints.clear();

        auto all_waypoints = ThjWaypointsRepository::All(content_db);
        std::set<int32> unlocked_ids;

        if (AllowAccountWaypoints()) {
            auto account_waypoints = ThjWaypointsAccountRepository::GetWaypointIds(database, AccountID());
            unlocked_ids.insert(account_waypoints.begin(), account_waypoints.end());
        } else {
            auto character_waypoints = ThjWaypointsCharacterRepository::GetWaypointIds(database, CharacterID());
            unlocked_ids.insert(character_waypoints.begin(), character_waypoints.end());
        }

        auto race_waypoints = ThjWaypointsDefaultRepository::GetWaypointIdsForCharacter(content_db, GetBaseRace(), GetClassesBits(), GetLevel());
        unlocked_ids.insert(race_waypoints.begin(), race_waypoints.end());

        for (const auto& wp : all_waypoints) {
            if (unlocked_ids.find(wp.id) != unlocked_ids.end()) {
                m_unlocked_waypoints.push_back(wp);
            }
        }
    }

    return m_unlocked_waypoints;
}

bool Client::WaypointCheck(int32 waypoint_id) {
    auto& waypoints = GetUnlockedWaypoints();

    for (const auto& wp : waypoints) {
        if (wp.id == waypoint_id) {
            return true;
        }
    }

    return false;
}

bool Client::WaypointCheck(std::string waypoint_shortname) {
    auto& waypoints = GetUnlockedWaypoints();

    for (const auto& wp : waypoints) {
        if (wp.shortname == waypoint_shortname) {
            return true;
        }
    }

    return false;
}

bool Client::WaypointUnlock(std::string waypoint_shortname)
{
    auto all_waypoints = zone->GetAllWaypoints();

    for (const auto& waypoint : all_waypoints) {
        if (waypoint.shortname == waypoint_shortname) {
            return WaypointUnlock(waypoint.id);
        }
    }

    return false;
}

bool Client::WaypointUnlock(int32 waypoint_id) {
    bool added = false;

    if (AllowAccountWaypoints()) {
        added |= ThjWaypointsAccountRepository::AddWaypoint(database, AccountID(), waypoint_id);
    } else {
        added |= ThjWaypointsCharacterRepository::AddWaypoint(database, CharacterID(), waypoint_id);
    }

    if (added) {
        GetUnlockedWaypoints(true);
    }

    return added;
}

void Client::WaypointListSend() {
    auto& all_waypoints = zone->GetAllWaypoints();
    auto& unlocked_waypoints = GetUnlockedWaypoints();

    std::set<int32> unlocked_ids;
    for (const auto& wp : unlocked_waypoints) {
        unlocked_ids.insert(wp.id);
    }

    size_t entry_count = all_waypoints.size();
    size_t packet_size = sizeof(bool) +
                         sizeof(bool) +
                         sizeof(uint32) +
                         (entry_count * sizeof(WaypointListEntry_Struct));

    auto outapp = new EQApplicationPacket(OP_WaypointList, packet_size);

    WaypointList_Struct* wp_list = reinterpret_cast<WaypointList_Struct*>(outapp->pBuffer);

    wp_list->group_enabled = WaypointCheckGroupFeature();
    wp_list->expedition_enabled = (WaypointCheckGroupFeature() && GetExpedition());
    wp_list->group_selected = false;
    wp_list->entry_count = entry_count;

    // Add counters for enabled/disabled waypoints
    size_t enabled_count = 0;
    size_t disabled_count = 0;

    // Interleave all waypoints with unlocked status
    for (size_t i = 0; i < entry_count; i++) {
        const auto& wp = all_waypoints[i];
        WaypointListEntry_Struct& entry = wp_list->entries[i];

        entry.category_id = wp.category;
        entry.waypoint_id = wp.id;
        bool is_enabled = (unlocked_ids.find(wp.id) != unlocked_ids.end());
        entry.enabled = is_enabled ? 1 : 0;

        // Update counters
        if (is_enabled) {
            enabled_count++;
        } else {
            disabled_count++;
        }

        strncpy(entry.name, wp.long_name.c_str(), sizeof(entry.name) - 1);
        entry.name[sizeof(entry.name) - 1] = '\0';
    }

    QueuePacket(outapp);
    safe_delete(outapp);

    LogDebug("Sent {} waypoints to client {} (Enabled: {}, Disabled: {}, Group: {}, Expedition: {})",
             entry_count,
             GetName(),
             enabled_count,
             disabled_count,
             wp_list->group_enabled ? "Enabled" : "Disabled",
             wp_list->expedition_enabled ? "Enabled" : "Disabled");
}

bool Client::AllowAccountWaypoints() {
    // Can adjust this for Self-Found, Hardcore, etc.
    return true;
}

bool Client::WaypointCheckGroupFeature() {
    if (GetGM()) {
        return true;
    }

    if (m_expanded_waypoints == -1) {
        std::string bucket_val = GetAccountBucket("expanded_waypoints");

        if (!bucket_val.empty()) {
            m_expanded_waypoints = 1;
        } else {
            m_expanded_waypoints = 0;
        }
    }

    return (1 == m_expanded_waypoints);
}

void Client::WaypointEnableGroupFeature() {
    m_expanded_waypoints = 1;
    SetAccountBucket("expanded_waypoints", "true");

	WaypointListSend();
}