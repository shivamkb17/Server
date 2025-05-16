/*

-- SECTION 1: DROP TABLES
DROP TABLE IF EXISTS thj_waypoints_account;
DROP TABLE IF EXISTS thj_waypoints_character;
DROP TABLE IF EXISTS thj_waypoints_default;
DROP TABLE IF EXISTS thj_waypoints;
DROP TABLE IF EXISTS thj_waypoints_categories;

-- SECTION 2: CREATE TABLES WITH NEW NAMES
CREATE TABLE thj_waypoints_categories (
    id INT PRIMARY KEY,
    name VARCHAR(32) NOT NULL UNIQUE
);

CREATE TABLE thj_waypoints (
    id INT AUTO_INCREMENT PRIMARY KEY,
    shortname VARCHAR(32) UNIQUE NOT NULL,
    long_name VARCHAR(64) NOT NULL,
    category INT NOT NULL,
    x FLOAT NOT NULL,
    y FLOAT NOT NULL,
    z FLOAT NOT NULL,
    heading FLOAT NOT NULL,
    INDEX idx_thj_waypoints_shortname (shortname),
    INDEX idx_thj_waypoints_category (category)
);

CREATE TABLE thj_waypoints_character (
    id INT AUTO_INCREMENT PRIMARY KEY,
    character_id BIGINT UNSIGNED NOT NULL,
    waypoint_id INT NOT NULL,
    unlock_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE KEY unique_character_waypoint (character_id, waypoint_id),
    INDEX idx_thj_waypoints_character_id (character_id)
);

CREATE TABLE thj_waypoints_account (
    id INT AUTO_INCREMENT PRIMARY KEY,
    account_id BIGINT UNSIGNED NOT NULL,
    waypoint_id INT NOT NULL,
    unlock_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE KEY unique_account_waypoint (account_id, waypoint_id),
    INDEX idx_thj_waypoints_account_id (account_id)
);

-- New more flexible default waypoints table
CREATE TABLE thj_waypoints_default (
    id INT AUTO_INCREMENT PRIMARY KEY,
    waypoint_id INT NOT NULL,
    race_id INT NOT NULL DEFAULT 0,           -- 0 means any race
    class_mask INT UNSIGNED NOT NULL DEFAULT 65535, -- 0xFFFF means all classes
    min_level INT NOT NULL DEFAULT 1,         -- Minimum level required (default 1)
    max_level INT NOT NULL DEFAULT 255,       -- Maximum level required (default 255, meaning no cap)
    UNIQUE KEY unique_default_waypoint (waypoint_id, race_id, class_mask, min_level),
    INDEX idx_thj_waypoints_default_race (race_id),
    INDEX idx_thj_waypoints_default_class (class_mask),
    INDEX idx_thj_waypoints_default_level (min_level, max_level)
);

-- SECTION 3: INSERT WAYPOINT CATEGORIES
INSERT INTO thj_waypoints_categories (id, name) VALUES
(0, 'Antonica'),
(1, 'Faydwer'),
(2, 'Odus'),
(3, 'Kunark'),
(4, 'Velious'),
(5, 'Luclin'),
(6, 'The Planes'),
(7, 'Taelosia'),
(8, 'Discord');

-- SECTION 4: INSERT WAYPOINTS (FIRST PART - ANTONICA)
INSERT INTO thj_waypoints (shortname, long_name, category, x, y, z, heading) VALUES
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
('southkarana', 'Southern Plains of Karana (Aviak Village)', 0, 1027, -6689, 0, 260);

-- SECTION 5: INSERT WAYPOINTS (SECOND PART - FAYDWER)
INSERT INTO thj_waypoints (shortname, long_name, category, x, y, z, heading) VALUES
('akanon', 'Ak\'anon', 1, -761, 1279, -24.25, 182.25),
('cauldron', 'Dagnor\'s Cauldron (Near Unrest)', 1, -700, -1790, 100, 11),
('felwithea', 'Northern Felwithe', 1, -626, 240, -10.25, 330),
('gfaydark', 'Greater Faydark (Druid Ring)', 1, -385, 458, 0, 0),
('kaladima', 'Southern Kaladim', 1, 197, 90, 3.75, 492),
('mistmoore', 'Castle of Mistmoore', 1, 122, -294, -179, 135);

-- SECTION 6: INSERT WAYPOINTS (THIRD PART - ODUS)
INSERT INTO thj_waypoints (shortname, long_name, category, x, y, z, heading) VALUES
('erudnext', 'Erudin', 2, -240, -1216, 52, 510),
('hole', 'The Ruins of Old Paineel', 2, -543, 287, -140, 125),
('paineel', 'Paineel', 2, 210, 839, 4, 275),
('tox', 'The Toxxulia Forest', 2, -916, -1510, -33, 0),
('stonebrunt', 'Stonebrunt Mountains', 2, 673, -4531, 0, 0),
('dulak', 'Dulak Harbor', 2, -1190, -190, 4, 128),
('gunthak', 'The Gulf of Gunthak', 2, -410, 1402, 3, 0);

-- SECTION 7: INSERT WAYPOINTS (FOURTH PART - KUNARK)
INSERT INTO thj_waypoints (shortname, long_name, category, x, y, z, heading) VALUES
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
('chardokb', 'The Halls of Betrayal', 3, -210, 315, 1.5, 200);

-- SECTION 8: INSERT WAYPOINTS (FIFTH PART - VELIOUS)
INSERT INTO thj_waypoints (shortname, long_name, category, x, y, z, heading) VALUES
('cobaltscar', 'Cobalt Scar', 4, -1633, -1064, 296, 115),
('eastwastes', 'Eastern Wastes (Crystal Caverns)', 4, 464, -4037, 144, 178),
('greatdivide', 'Great Divide (Velketor\'s Labyrinth)', 4, 3287, -6646, -35, 251),
('iceclad', 'The Iceclad Ocean (Tower of Frozen Shadow)', 4, 3127, 1300, 111, 500),
('wakening', 'The Wakening Land', 4, 4552, 1455, -60, 130),
('westwastes', 'The Western Wastes (Temple of Veeshan)', 4, 808, 1323, -196, 373),
('sirens', 'Siren\'s Grotto', 4, 20, -590, -93, 0);

-- SECTION 9: INSERT WAYPOINTS (SIXTH PART - LUCLIN)
INSERT INTO thj_waypoints (shortname, long_name, category, x, y, z, heading) VALUES
('dawnshroud', 'The Dawnshroud Peaks', 5, -1260, -280, 97, 500),
('fungusgrove', 'Fungus Grove', 5, 1359, 2398, -261, 256),
('sharvahl', 'The City of Shar Vahl', 5, 250, 55, -188, 400),
('ssratemple', 'Ssraeshza Temple', 5, -6.5, 0, 4, 0),
('tenebrous', 'Tenebrous Mountains', 5, -967, -1514, -56, 443),
('umbral', 'The Umbral Plains', 5, 1840, -640, 24, 0),
('twilight', 'The Twilight Sea', 5, -1028, 1338, 39, 0),
('scarlet', 'The Scarlet Desert', 5, -1777, -956, -99, 0),
('paludal', 'Paludal Caverns', 5, 220, -1175, -236, 0),
('bazaar', 'The Bazaar', 5, 105, -175, -15, 65);

-- SECTION 10: INSERT WAYPOINTS (SEVENTH PART - THE PLANES)
INSERT INTO thj_waypoints (shortname, long_name, category, x, y, z, heading) VALUES
('airplane', 'The Plane of Sky', 6, 700, 1560, -680, 300),
('fearplane', 'The Plane of Fear', 6, 1065, -1305, 3, 500),
('hateplaneb', 'The Plane of Hate', 6, -400, 680, 4, 330),
('poknowledge', 'The Plane of Knowledge', 6, -215, 50, -160, 128),
('potranquility', 'The Plane of Tranquility', 6, -8, -192, -628, 115),
('potimea', 'The Plane of Time', 6, 0, 110, 8, 0);

-- SECTION 11: INSERT WAYPOINTS (EIGHTH PART - TAELOSIA)
INSERT INTO thj_waypoints (shortname, long_name, category, x, y, z, heading) VALUES
('barindu', 'Barindu, Hanging Gardens', 7, 210, -515, -117, 510),
('kodtaz', 'Kod`Taz, Broken Trial Grounds', 7, 1536, -2422, -348, 4),
('natimbi', 'Natimbi, the Broken Shores', 7, -310, 125, 520, 70),
('qvic', 'Qvic, Prayer Grounds of Calling', 7, -1018, -1403, -490, 3),
('txevu', 'Txevu, Lair of the Elite', 7, -316, -20, -420, 430);

-- SECTION 12: INSERT WAYPOINTS (NINTH PART - DISCORD)
INSERT INTO thj_waypoints (shortname, long_name, category, x, y, z, heading) VALUES
('wallofslaughter', 'Wall of Slaughter', 8, -943, 13, 130, 0);

-- SECTION 13: INSERT DEFAULT WAYPOINTS
-- Class bitmask constants
-- 1: Warrior      = 1 << 0 = 1
-- 2: Cleric       = 1 << 1 = 2
-- 3: Paladin      = 1 << 2 = 4
-- 4: Ranger       = 1 << 3 = 8
-- 5: Shadowknight = 1 << 4 = 16
-- 6: Druid        = 1 << 5 = 32
-- 7: Monk         = 1 << 6 = 64
-- 8: Bard         = 1 << 7 = 128
-- 9: Rogue        = 1 << 8 = 256
-- 10: Shaman      = 1 << 9 = 512
-- 11: Necromancer = 1 << 10 = 1024
-- 12: Wizard      = 1 << 11 = 2048
-- 13: Magician    = 1 << 12 = 4096
-- 14: Enchanter   = 1 << 13 = 8192
-- 15: Beastlord   = 1 << 14 = 16384
-- 16: Berserker   = 1 << 15 = 32768
-- All Classes = 65535 (0xFFFF)

-- Insert race-specific default waypoints (converted from the old race_waypoints)
-- Human
INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 1, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'freportw';

INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 1, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'qeynos2';

-- Barbarian
INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 2, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'halas';

-- Erudite
INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 3, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'erudnext';

-- Wood Elf
INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 4, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'gfaydark';

-- High Elf
INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 5, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'felwithea';

-- Dark Elf
INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 6, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'neriakb';

-- Half Elf
INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 7, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'freportw';

INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 7, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'qeynos2';

INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 7, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'gfaydark';

-- Dwarf
INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 8, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'kaladima';

-- Troll
INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 9, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'grobb';

INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 9, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'neriakb';

-- Ogre
INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 10, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'oggok';

INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 10, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'neriakb';

-- Halfling
INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 11, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'rivervale';

-- Gnome
INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 12, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'akanon';

-- Iksar
INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 128, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'cabeast';

-- Vah Shir
INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 130, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'sharvahl';

-- Froglok
INSERT INTO thj_waypoints_default (waypoint_id, race_id, class_mask, min_level, max_level)
SELECT id, 330, 65535, 1, 255 FROM thj_waypoints WHERE shortname = 'qeynos2';

-- SECTION 14: MIGRATE ACCOUNT WAYPOINTS
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
INSERT INTO thj_waypoints_account (account_id, waypoint_id)
SELECT DISTINCT
    CONVERT(SUBSTRING(db.key, 11), UNSIGNED) as account_id,
    w.id as waypoint_id
FROM
    data_buckets db
    JOIN numbers n ON n.n <= CHAR_LENGTH(db.value) - CHAR_LENGTH(REPLACE(db.value, ',', '')) + 1
    JOIN thj_waypoints w ON w.shortname = TRIM(SUBSTRING_INDEX(SUBSTRING_INDEX(db.value, ',', n.n), ',', -1))
WHERE
    db.key LIKE 'Waypoints-%'
    AND db.account_id > 0
    AND db.character_id = 0;

-- SECTION 15: MIGRATE EXPANDED WAYPOINTS
-- Delete any existing 'expanded_waypoints' data_buckets entries
DELETE FROM data_buckets WHERE `key` = 'expanded_waypoints';

-- Migrate expanded waypoints from old format to new format
INSERT IGNORE INTO data_buckets (`key`, value, expires, account_id, character_id, npc_id, bot_id, zone_id, instance_id)
SELECT
    'expanded_waypoints' as `key`,
    'true' as value,
    0 as expires,
    CONVERT(SUBSTRING_INDEX(db.key, '-', 1), UNSIGNED) as account_id,
    0 as character_id, -- No character_id
    0 as npc_id,
    0 as bot_id,
    0 as zone_id,
    0 as instance_id
FROM
    data_buckets db
WHERE
    db.key LIKE '%-group-ports-enabled'
    AND db.value <> '';

-- Clean up temporary table
DROP TEMPORARY TABLE IF EXISTS numbers;

-- SECTION 16: CREATE SUMMARY VIEW
-- Create a view to see the migration summary
CREATE OR REPLACE VIEW v_thj_waypoints_summary AS
SELECT
    'Account Waypoints' as type,
    COUNT(*) as count
FROM thj_waypoints_account
UNION ALL
SELECT
    'Default Waypoints' as type,
    COUNT(*) as count
FROM thj_waypoints_default
UNION ALL
SELECT
    'Expanded Waypoints Settings' as type,
    COUNT(*) as count
FROM data_buckets WHERE `key` = 'expanded_waypoints'
UNION ALL
SELECT
    'Total Waypoints' as type,
    COUNT(*) as count
FROM thj_waypoints;

SELECT * FROM peq.v_thj_waypoints_summary;

*/