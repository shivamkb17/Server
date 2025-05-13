#include "../common/repositories/thj_waypoints_repository.h"
#include "../common/repositories/thj_waypoints_categories_repository.h"
#include "../common/repositories/thj_waypoints_default_repository.h"
#include "../common/repositories/thj_waypoints_account_repository.h"
#include "../common/repositories/thj_waypoints_character_repository.h"

#include "client.h"
#include "zone.h"
#include "dynamic_zone.h"
#include "groups.h"

#include "thj_waypoints.h"

std::vector<ThjWaypointsRepository::ThjWaypoints>& Zone::GetAllWaypoints(bool force_reload) {
    if (force_reload || m_all_waypoints.empty()) {
        m_all_waypoints = ThjWaypointsRepository::All(content_db);
    }

    return m_all_waypoints;
}

const ThjWaypointsRepository::ThjWaypoints* Zone::WaypointGetSpawn(std::string zone_shortname) {
    auto waypoints = GetAllWaypoints();

    for (const auto& wp : waypoints) {
        if (wp.shortname == zone_shortname) {
            return &wp;
        }
    }

    return nullptr;
}

bool Zone::SpawnWaypointNPC()
{
	#define WAYPOINT_NPC_ID 26999

	auto waypoint = WaypointGetSpawn(short_name);

	if (!waypoint) {
		return false;
	}

	glm::vec4 position = glm::vec4(waypoint->x, waypoint->y, waypoint->z, waypoint->heading);
	if(auto waypoint_npctype = content_db.LoadNPCTypesData(WAYPOINT_NPC_ID))
	{
		auto waypoint_npc = new NPC(waypoint_npctype, nullptr, position, GravityBehavior::Water);
		entity_list.AddNPC(waypoint_npc, true, true);
	}

	return true;
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

const ThjWaypointsRepository::ThjWaypoints* Client::GetWaypoint(int waypoint_id) {
	auto& waypoints = GetUnlockedWaypoints();

    for (const auto& wp : waypoints) {
        if (wp.id == waypoint_id) {
            return &wp;
        }
    }

    return nullptr;
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

void Client::WaypointListSend(bool force) {
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
    wp_list->group_selected = GetWaypointGroupFeatureState();
    wp_list->entry_count = entry_count;
	wp_list->force_show = force;

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

void Client::TransportToWaypoint(uint32 waypoint_id)
{
    uint32 zone_id = Zones::BAZAAR;  // Default to bazaar
    uint32 instance_id = 0;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float h = 0.0f;
    uint8 zone_mode = ZoneToSafeCoords;

    if (waypoint_id == 0) {
        if (GetExpedition()) {
            zone_id = GetExpedition()->GetZoneID();
            instance_id = GetExpedition()->GetInstanceID();
            zone_mode = ZoneSolicited;
            LogDebug("Expedition teleport -> Zone: [{}], Instance: [{}]", zone_id, instance_id);
        }
    }
    else if (auto waypoint = GetWaypoint(waypoint_id)) {
        x = waypoint->x;
        y = waypoint->y;
        z = waypoint->z;
        h = waypoint->heading;

        zone_mode = ZoneSolicited;
        zone_id = zone_store.GetZoneID(waypoint->shortname);
        LogDebug("Waypoint teleport -> Zone: [{}], Waypoint ID: [{}]", zone_id, waypoint_id);
    }
    else {
        LogError("Waypoint not found");
		return;
    }

    //MovePC(zone_id, instance_id, x, y, z, h, zone_mode);

	auto group = GetGroup();
	if (GetWaypointGroupFeatureState() && group) {
		for (const auto& gm : group->members) {
			if (!gm->IsClient()) {
				continue;
			}

			gm->CastToClient()->WayportGroupTransport(this, zone_id, "", x, y, z, h);
		}
	} else {
		WayportGroupTransport(this, zone_id, "", x, y, z, h);
	}
}

void Client::WayportGroupTransport(Mob *Caster, uint32 zoneID, const char* zoneName, float x, float y, float z, float heading) {
    if (!Caster || PendingTranslocate)
        return;

    auto outapp = new EQApplicationPacket(OP_Translocate, sizeof(Translocate_Struct));
    Translocate_Struct *ts = (Translocate_Struct*)outapp->pBuffer;

    strcpy(ts->Caster, Caster->GetName());
    ts->SpellID = 0;

    PendingTranslocateData.spell_id = 0;
    PendingTranslocateData.zone_id = ts->ZoneID = zoneID;
    PendingTranslocateData.instance_id = 0;
    PendingTranslocateData.x = ts->x = x;
    PendingTranslocateData.y = ts->y = y;
    PendingTranslocateData.z = ts->z = z;
    PendingTranslocateData.heading = heading;

    ts->unknown008 = 0;
    ts->Complete = 0;

    PendingTranslocate = true;
    TranslocateTime = time(nullptr);

    QueuePacket(outapp);
    safe_delete(outapp);

    return;
}

bool Client::GetWaypointGroupFeatureState()
{
	if (!WaypointCheckGroupFeature())
	{
		return false;
	}

    if (m_group_feature_state == -1) {
        std::string bucket_val = GetAccountBucket("group_feature_state");
        m_group_feature_state = !bucket_val.empty() ? 1 : 0;
    }

    return (m_group_feature_state == 1);
}

void Client::SetWaypointGroupFeatureState(bool val)
{
    m_group_feature_state = val ? 1 : 0;

    if (val) {
        SetBucket("group_feature_state", "enabled");
    } else {
        DeleteBucket("group_feature_state");
    }
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

	WaypointListSend(false);
}