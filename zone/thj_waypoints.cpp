/*

-- Idempotent Migration Script for Waypoints System

-- Create waypoint_categories table if it doesn't exist
CREATE TABLE IF NOT EXISTS waypoint_categories (
    id INT PRIMARY KEY,
    name VARCHAR(32) NOT NULL UNIQUE
) ENGINE=InnoDB;

-- Create waypoints table if it doesn't exist
CREATE TABLE IF NOT EXISTS waypoints (
    id INT AUTO_INCREMENT PRIMARY KEY,
    shortname VARCHAR(32) UNIQUE NOT NULL,
    long_name VARCHAR(64) NOT NULL,
    category INT NOT NULL,
    x FLOAT NOT NULL,
    y FLOAT NOT NULL,
    z FLOAT NOT NULL,
    heading FLOAT NOT NULL,
    INDEX idx_waypoints_shortname (shortname),
    INDEX idx_waypoints_category (category)
) ENGINE=InnoDB;

-- Create character_waypoints table if it doesn't exist
CREATE TABLE IF NOT EXISTS character_waypoints (
    id INT AUTO_INCREMENT PRIMARY KEY,
    character_id BIGINT UNSIGNED NOT NULL,
    waypoint_id INT NOT NULL,
    unlock_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE KEY unique_character_waypoint (character_id, waypoint_id),
    INDEX idx_character_waypoints_character (character_id)
) ENGINE=InnoDB;

-- Create account_waypoints table if it doesn't exist
CREATE TABLE IF NOT EXISTS account_waypoints (
    id INT AUTO_INCREMENT PRIMARY KEY,
    account_id BIGINT UNSIGNED NOT NULL,
    waypoint_id INT NOT NULL,
    unlock_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE KEY unique_account_waypoint (account_id, waypoint_id),
    INDEX idx_account_waypoints_account (account_id)
) ENGINE=InnoDB;

-- Create race_waypoints table if it doesn't exist
CREATE TABLE IF NOT EXISTS race_waypoints (
    id INT AUTO_INCREMENT PRIMARY KEY,
    race_id INT NOT NULL,
    waypoint_id INT NOT NULL,
    UNIQUE KEY unique_race_waypoint (race_id, waypoint_id),
    INDEX idx_race_waypoints_race (race_id)
) ENGINE=InnoDB;

-- Create account_settings table if it doesn't exist (for expanded waypoints)
CREATE TABLE IF NOT EXISTS account_settings (
    id INT AUTO_INCREMENT PRIMARY KEY,
    account_id BIGINT UNSIGNED NOT NULL,
    setting_name VARCHAR(64) NOT NULL,
    setting_value VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    UNIQUE KEY unique_account_setting (account_id, setting_name),
    INDEX idx_account_settings_account (account_id)
) ENGINE=InnoDB;

-- Insert waypoint categories (only if they don't exist)
INSERT IGNORE INTO waypoint_categories (id, name) VALUES
(0, 'Antonica'),
(1, 'Faydwer'),
(2, 'Odus'),
(3, 'Kunark'),
(4, 'Velious'),
(5, 'Luclin'),
(6, 'The Planes'),
(7, 'Taelosia'),
(8, 'Discord');

-- Insert all waypoints (only if they don't exist by shortname)
INSERT IGNORE INTO waypoints (shortname, long_name, category, x, y, z, heading) VALUES
-- Antonica (0)
('blackburrow', 'Blackburrow', 0, -7, 38, 3, 300),
('commons', 'West Commonlands (Roadside Inn)', 0, 503, -127, -51, 128),
('ecommons', 'East Commonlands', 0, -356, -1603, 3, 0),
('feerrott', 'The Feerrott', 0, -1830, 430, 18, 33),
('freportw', 'West Freeport', 0, -396, -283, -23, 500),
('grobb', 'Grobb', 0, -200, 223, 3.75, 414),
('everfrost', 'Everfrost', 0, -6972, 2133, -58, 163),
('halas', 'Halas', 0, 0, 26, 3.75, 256),
('highkeep', 'High Keep', 0, -1, -17, -4, 388),
('lavastorm', 'Lavastorm Mountains (Druid Ring)', 0, 1318, 918, 119, 270),
('neriakb', 'Neriak Commons', 0, -493, 3, -10, 128),
('northkarana', 'Northern Plains of Karana (Gypsy Camp)', 0, -175, -688, -7.5, 10),
('eastkarana', 'Eastern Plains of Karana (Druid Ring)', 0, 423, 1333, 1, 210),
('oasis', 'Oasis of Marr', 0, 110, 532, 6, 225),
('oggok', 'Oggok', 0, 513, 465, 3.75, 205),
('oot', 'The Ocean of Tears', 0, -9172, 394, 6, 188),
('qey2hh1', 'Western Plains of Karana (Combine Spires)', 0, -14816, -3570, 36, 400),
('qeynos2', 'North Qeynos', 0, 392, 165, 4, 310),
('qrg', 'Surefall Glade', 0, -66, 45, 4, 200),
('rivervale', 'Rivervale', 0, -140, -10, 4, 220),
('gukbottom', 'Lower Guk (Undead Side)', 0, -233, 1157, -80, 418),
('lakerathe', 'Lake Rathetear (Outside Arena)', 0, 2673, 2404, 95, 381),
('southkarana', 'Southern Plains of Karana (Aviak Village)', 0, 1027, -6689, 0, 260),

-- Faydwer (1)
('akanon', 'Ak\'anon', 1, -761, 1279, -24.25, 182.25),
('cauldron', 'Dagnor\'s Cauldron (Near Unrest)', 1, -700, -1790, 100, 11),
('felwithea', 'Northern Felwithe', 1, -626, 240, -10.25, 330),
('gfaydark', 'Greater Faydark (Druid Ring)', 1, -385, 458, 0, 0),
('kaladima', 'Southern Kaladim', 1, 197, 90, 3.75, 492),
('mistmoore', 'Castle of Mistmoore', 1, 122, -294, -179, 135),

-- Odus (2)
('erudnext', 'Erudin', 2, -240, -1216, 52, 510),
('hole', 'The Ruins of Old Paineel', 2, -543, 287, -140, 125),
('paineel', 'Paineel', 2, 210, 839, 4, 275),
('tox', 'The Toxxulia Forest', 2, -916, -1510, -33, 0),
('stonebrunt', 'Stonebrunt Mountains', 2, 673, -4531, 0, 0),
('dulak', 'Dulak Harbor', 2, -1190, -190, 4, 128),
('gunthak', 'The Gulf of Gunthak', 2, -410, 1402, 3, 0),

-- Kunark (3)
('burningwood', 'Burning Wood (Chardok)', 3, -3876, 7407, -233, 303),
('cabeast', 'Eastern Cabilis', 3, -136, 969, 4.68, 271),
('citymist', 'The City of Mist', 3, -572, 249, 4, 130),
('dreadlands', 'The Dreadlands', 3, 9633, 3005, 1049, 0),
('fieldofbone', 'The Field of Bone', 3, 1617, -1684, -55, 0),
('firiona', 'Firiona Vie', 3, 1825, -2397, -98, 423),
('frontiermtns', 'Frontier Mountains', 3, 392, 53, -102, 39),
('karnor', 'Karnor\'s Castle', 3, 160, 251, 3.75, 310),
('lakeofillomen', 'Lake of Ill Omen', 3, -1070, 985, 78, 145),
('overthere', 'The Overthere', 3, 1480, -2757, 11, 500),
('skyfire', 'The Skyfire Mountains', 3, 780, -3100, -158, 0),
('timorous', 'The Firepot Room (Secret)', 3, 4366.5, -12256.8, -278, 256),
('trakanon', 'Trakanon\'s Teeth', 3, -4720, -1620, -473, 320),
('chardokb', 'The Halls of Betrayal', 3, -210, 315, 1.5, 200),

-- Velious (4)
('cobaltscar', 'Cobalt Scar', 4, -1633, -1064, 296, 115),
('eastwastes', 'Eastern Wastes (Crystal Caverns)', 4, 464, -4037, 144, 178),
('greatdivide', 'Great Divide (Velketor\'s Labyrinth)', 4, 3287, -6646, -35, 251),
('iceclad', 'The Iceclad Ocean (Tower of Frozen Shadow)', 4, 3127, 1300, 111, 500),
('wakening', 'The Wakening Land', 4, 4552, 1455, -60, 130),
('westwastes', 'The Western Wastes (Temple of Veeshan)', 4, 808, 1323, -196, 373),
('sirens', 'Siren\'s Grotto', 4, 20, -590, -93, 0),

-- Luclin (5)
('dawnshroud', 'The Dawnshroud Peaks', 5, -1260, -280, 97, 500),
('fungusgrove', 'Fungus Grove', 5, 1359, 2398, -261, 256),
('sharvahl', 'The City of Shar Vahl', 5, 250, 55, -188, 400),
('ssratemple', 'Ssraeshza Temple', 5, -6.5, 0, 4, 0),
('tenebrous', 'Tenebrous Mountains', 5, -967, -1514, -56, 443),
('umbral', 'The Umbral Plains', 5, 1840, -640, 24, 0),
('twilight', 'The Twilight Sea', 5, -1028, 1338, 39, 0),
('scarlet', 'The Scarlet Desert', 5, -1777, -956, -99, 0),
('paludal', 'Paludal Caverns', 5, 220, -1175, -236, 0),
('bazaar', 'The Bazaar', 5, 105, -175, -15, 65),

-- The Planes (6)
('airplane', 'The Plane of Sky', 6, 700, 1560, -680, 300),
('fearplane', 'The Plane of Fear', 6, 1065, -1305, 3, 500),
('hateplaneb', 'The Plane of Hate', 6, -400, 680, 4, 330),
('poknowledge', 'The Plane of Knowledge', 6, -215, 50, -160, 128),
('potranquility', 'The Plane of Tranquility', 6, -8, -192, -628, 115),
('potimea', 'The Plane of Time', 6, 0, 110, 8, 0),

-- Taelosia (7)
('barindu', 'Barindu, Hanging Gardens', 7, 210, -515, -117, 510),
('kodtaz', 'Kod\'Taz, Broken Trial Grounds', 7, 1536, -2422, -348, 4),
('natimbi', 'Natimbi, the Broken Shores', 7, -310, 125, 520, 70),
('qvic', 'Qvic, Prayer Grounds of Calling', 7, -1018, -1403, -490, 3),
('txevu', 'Txevu, Lair of the Elite', 7, -316, -20, -420, 430),

-- Discord (8)
('wallofslaughter', 'Wall of Slaughter', 8, -943, 13, 130, 0);

-- Insert race waypoints (only if they don't exist)
-- Create a temporary table for the values we want to insert
DROP TEMPORARY TABLE IF EXISTS temp_race_waypoints;
CREATE TEMPORARY TABLE temp_race_waypoints AS
SELECT 1 AS race_id, id AS waypoint_id FROM waypoints WHERE shortname = 'freportw'
UNION SELECT 1, id FROM waypoints WHERE shortname = 'qeynos2'
UNION SELECT 2, id FROM waypoints WHERE shortname = 'halas'
UNION SELECT 3, id FROM waypoints WHERE shortname = 'erudnext'
UNION SELECT 4, id FROM waypoints WHERE shortname = 'gfaydark'
UNION SELECT 5, id FROM waypoints WHERE shortname = 'felwithea'
UNION SELECT 6, id FROM waypoints WHERE shortname = 'neriakb'
UNION SELECT 7, id FROM waypoints WHERE shortname = 'freportw'
UNION SELECT 7, id FROM waypoints WHERE shortname = 'qeynos2'
UNION SELECT 7, id FROM waypoints WHERE shortname = 'gfaydark'
UNION SELECT 8, id FROM waypoints WHERE shortname = 'kaladima'
UNION SELECT 9, id FROM waypoints WHERE shortname = 'grobb'
UNION SELECT 9, id FROM waypoints WHERE shortname = 'neriakb'
UNION SELECT 10, id FROM waypoints WHERE shortname = 'oggok'
UNION SELECT 10, id FROM waypoints WHERE shortname = 'neriakb'
UNION SELECT 11, id FROM waypoints WHERE shortname = 'rivervale'
UNION SELECT 12, id FROM waypoints WHERE shortname = 'akanon'
UNION SELECT 128, id FROM waypoints WHERE shortname = 'cabeast'
UNION SELECT 130, id FROM waypoints WHERE shortname = 'sharvahl'
UNION SELECT 330, id FROM waypoints WHERE shortname = 'qeynos2';

-- Insert using the temporary table
INSERT IGNORE INTO race_waypoints (race_id, waypoint_id)
SELECT race_id, waypoint_id FROM temp_race_waypoints;

-- Migration section - only run if needed
-- Check if we need to migrate waypoints from data_buckets
SET @migration_flag = 0;
SELECT COUNT(*) INTO @migration_flag FROM account_waypoints LIMIT 1;

-- Create a flag to check if expanded waypoints need to be migrated
SET @expanded_migration_flag = 0;
SELECT COUNT(*) INTO @expanded_migration_flag FROM account_settings WHERE setting_name = 'expanded_waypoints' LIMIT 1;

-- If we have no account_waypoints data yet, do the migration
-- This ensures idempotence - only run migration once
DROP PROCEDURE IF EXISTS MigrateWaypoints;
DELIMITER //
CREATE PROCEDURE MigrateWaypoints()
BEGIN
    -- Only proceed if we need to migrate
    IF @migration_flag = 0 THEN
        -- Create a numbers table for splitting comma-separated values
        DROP TEMPORARY TABLE IF EXISTS numbers;
        CREATE TEMPORARY TABLE numbers (n INT);
        INSERT INTO numbers (n)
        SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5
        UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9 UNION SELECT 10
        UNION SELECT 11 UNION SELECT 12 UNION SELECT 13 UNION SELECT 14 UNION SELECT 15
        UNION SELECT 16 UNION SELECT 17 UNION SELECT 18 UNION SELECT 19 UNION SELECT 20
        UNION SELECT 21 UNION SELECT 22 UNION SELECT 23 UNION SELECT 24 UNION SELECT 25
        UNION SELECT 26 UNION SELECT 27 UNION SELECT 28 UNION SELECT 29 UNION SELECT 30
        UNION SELECT 31 UNION SELECT 32 UNION SELECT 33 UNION SELECT 34 UNION SELECT 35
        UNION SELECT 36 UNION SELECT 37 UNION SELECT 38 UNION SELECT 39 UNION SELECT 40
        UNION SELECT 41 UNION SELECT 42 UNION SELECT 43 UNION SELECT 44 UNION SELECT 45
        UNION SELECT 46 UNION SELECT 47 UNION SELECT 48 UNION SELECT 49 UNION SELECT 50;

        -- Migrate account waypoints from data_buckets
        INSERT IGNORE INTO account_waypoints (account_id, waypoint_id)
        SELECT DISTINCT
            SUBSTRING(db.key, 11) as account_id,
            w.id as waypoint_id
        FROM
            data_buckets db
            JOIN numbers n ON n.n <= CHAR_LENGTH(db.value) - CHAR_LENGTH(REPLACE(db.value, ',', '')) + 1
            JOIN waypoints w ON w.shortname = TRIM(SUBSTRING_INDEX(SUBSTRING_INDEX(db.value, ',', n.n), ',', -1))
        WHERE
            db.key LIKE 'Waypoints-%'
            AND db.account_id > 0
            AND db.character_id = 0;

        -- Clean up temporary table
        DROP TEMPORARY TABLE IF EXISTS numbers;

        -- Output migration status
        SELECT 'Account waypoints successfully migrated' AS migration_status;
    ELSE
        SELECT 'Account waypoints already migrated, skipping' AS migration_status;
    END IF;
END //
DELIMITER ;

-- Call the procedure to migrate waypoints
CALL MigrateWaypoints();
DROP PROCEDURE IF EXISTS MigrateWaypoints;

-- If we have no expanded waypoints settings yet, do the migration
DROP PROCEDURE IF EXISTS MigrateExpandedWaypoints;
DELIMITER //
CREATE PROCEDURE MigrateExpandedWaypoints()
BEGIN
    -- Only proceed if we need to migrate
    IF @expanded_migration_flag = 0 THEN
        -- Migrate expanded waypoints settings from data_buckets
        INSERT IGNORE INTO account_settings (account_id, setting_name, setting_value)
        SELECT
            DISTINCT CONVERT(SUBSTRING_INDEX(db.key, '-', 1), UNSIGNED) as account_id,
            'expanded_waypoints' as setting_name,
            'true' as setting_value
        FROM
            data_buckets db
        WHERE
            db.key LIKE '%-group-ports-enabled'
            AND db.value <> ''
            AND db.account_id > 0;

        -- Output migration status
        SELECT 'Expanded waypoints settings successfully migrated' AS migration_status;
    ELSE
        SELECT 'Expanded waypoints settings already migrated, skipping' AS migration_status;
    END IF;
END //
DELIMITER ;

-- Call the procedure to migrate expanded waypoints
CALL MigrateExpandedWaypoints();
DROP PROCEDURE IF EXISTS MigrateExpandedWaypoints;

-- Create a view to see the migration summary
CREATE OR REPLACE VIEW v_migration_summary AS
SELECT
    'Account Waypoints' as type,
    COUNT(*) as count
FROM account_waypoints
UNION ALL
SELECT
    'Race Waypoints' as type,
    COUNT(*) as count
FROM race_waypoints
UNION ALL
SELECT
    'Expanded Waypoints Settings' as type,
    COUNT(*) as count
FROM account_settings WHERE setting_name = 'expanded_waypoints'
UNION ALL
SELECT
    'Total Waypoints' as type,
    COUNT(*) as count
FROM waypoints;

-- View the migration summary
SELECT * FROM v_migration_summary;

*/

#include "../common/repositories/waypoints_repository.h"
#include "../common/repositories/waypoint_categories_repository.h"
#include "../common/repositories/race_waypoints_repository.h"
#include "../common/repositories/account_waypoints_repository.h"
#include "../common/repositories/character_waypoints_repository.h"

#include "client.h"
#include "zone.h"

#include "thj_waypoints.h"

std::vector<WaypointsRepository::Waypoints>& Zone::GetAllWaypoints(bool force_reload) {
    if (force_reload || m_all_waypoints.empty()) {
        m_all_waypoints = WaypointsRepository::All(database);
    }

    return m_all_waypoints;
}

std::vector<WaypointsRepository::Waypoints>& Client::GetUnlockedWaypoints(bool force_reload) {
	if (force_reload || m_unlocked_waypoints.empty()) {
		LogDebug("Querying Database for waypoints");
		m_unlocked_waypoints.clear();

		auto all_waypoints = WaypointsRepository::All(content_db);
		std::set<int32> unlocked_ids;

		if (AllowAccountWaypoints()) {
			auto account_waypoints = AccountWaypointsRepository::GetWaypointIds(database, AccountID());
			unlocked_ids.insert(account_waypoints.begin(), account_waypoints.end());
		} else {
			auto character_waypoints = CharacterWaypointsRepository::GetWaypointIds(database, CharacterID());
			unlocked_ids.insert(character_waypoints.begin(), character_waypoints.end());
		}

		auto race_waypoints = RaceWaypointsRepository::GetWaypointIds(content_db, GetBaseRace());
		unlocked_ids.insert(race_waypoints.begin(), race_waypoints.end());

		for (const auto& wp : all_waypoints) {
			if (unlocked_ids.find(wp.id) != unlocked_ids.end()) {
				m_unlocked_waypoints.push_back(wp);
			}
		}
	}

    return m_unlocked_waypoints;
}

bool Client::IsWaypointUnlocked(int32 waypoint_id) {
    auto& waypoints = GetUnlockedWaypoints();

    for (const auto& wp : waypoints) {
        if (wp.id == waypoint_id) {
            return true;
        }
    }

    return false;
}

bool Client::UnlockWaypoint(int32 waypoint_id) {
    bool added = false;

    if (AllowAccountWaypoints()) {
        added |= AccountWaypointsRepository::AddWaypoint(database, AccountID(), waypoint_id);
    } else {
        added |= CharacterWaypointsRepository::AddWaypoint(database, CharacterID(), waypoint_id);
    }

	if (added) {
		GetUnlockedWaypoints(true);
	}

    return added;
}

void Client::SendWaypointList() {
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

    wp_list->group_enabled = AllowExpandedWaypoints();
    wp_list->expedition_enabled = AllowExpandedWaypoints() && GetExpedition();
    wp_list->entry_count = entry_count;

    // Interleave all waypoints with unlocked status
    for (size_t i = 0; i < entry_count; i++) {
        const auto& wp = all_waypoints[i];
        WaypointListEntry_Struct& entry = wp_list->entries[i];

        entry.category_id = wp.category;
        entry.waypoint_id = wp.id;
        entry.enabled = (unlocked_ids.find(wp.id) != unlocked_ids.end()) ? 1 : 0;
        strncpy(entry.name, wp.long_name.c_str(), sizeof(entry.name) - 1);
        entry.name[sizeof(entry.name) - 1] = '\0';
    }

    QueuePacket(outapp);
    safe_delete(outapp);

    LogDebug("Sent {} waypoints to client {} (Group: {}, Expedition: {})",
             entry_count,
             GetName(),
             wp_list->group_enabled ? "Enabled" : "Disabled",
             wp_list->expedition_enabled ? "Enabled" : "Disabled");
}

bool Client::AllowAccountWaypoints() {
	// Can adjust this for Self-Found, Hardcore, etc.
	return true;
}

bool Client::AllowExpandedWaypoints() {
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

void Client::EnableExpandedWaypoints() {
	m_expanded_waypoints = 1;
	SetAccountBucket("expanded_waypoints", "true");
}