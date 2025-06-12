/**
 * DO NOT MODIFY THIS FILE
 *
 * This repository was automatically generated and is NOT to be modified directly.
 * Any repository modifications are meant to be made to the repository extending the base.
 * Any modifications to base repositories are to be made by the generator only
 *
 * @generator ./utils/scripts/generators/repository-generator.pl
 * @docs https://docs.eqemu.io/developer/repositories
 */

#ifndef EQEMU_BASE_PROGRESSION_ATLAS_REPOSITORY_H
#define EQEMU_BASE_PROGRESSION_ATLAS_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>

class BaseProgressionAtlasRepository {
public:
	struct ProgressionAtlas {
		int32_t zone_id;
		int32_t flag_id;
	};

	static std::string PrimaryKey()
	{
		return std::string("zone_id");
	}

	static std::vector<std::string> Columns()
	{
		return {
			"zone_id",
			"flag_id",
		};
	}

	static std::vector<std::string> SelectColumns()
	{
		return {
			"zone_id",
			"flag_id",
		};
	}

	static std::string ColumnsRaw()
	{
		return std::string(Strings::Implode(", ", Columns()));
	}

	static std::string SelectColumnsRaw()
	{
		return std::string(Strings::Implode(", ", SelectColumns()));
	}

	static std::string TableName()
	{
		return std::string("progression_atlas");
	}

	static std::string BaseSelect()
	{
		return fmt::format(
			"SELECT {} FROM {}",
			SelectColumnsRaw(),
			TableName()
		);
	}

	static std::string BaseInsert()
	{
		return fmt::format(
			"INSERT INTO {} ({}) ",
			TableName(),
			ColumnsRaw()
		);
	}

	static ProgressionAtlas NewEntity()
	{
		ProgressionAtlas e{};

		e.zone_id = 0;
		e.flag_id = 0;

		return e;
	}

	static ProgressionAtlas GetProgressionAtlas(
		const std::vector<ProgressionAtlas> &progression_atlass,
		int progression_atlas_id
	)
	{
		for (auto &progression_atlas : progression_atlass) {
			if (progression_atlas.zone_id == progression_atlas_id) {
				return progression_atlas;
			}
		}

		return NewEntity();
	}

	static ProgressionAtlas FindOne(
		Database& db,
		int progression_atlas_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				progression_atlas_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			ProgressionAtlas e{};

			e.zone_id = row[0] ? static_cast<int32_t>(atoi(row[0])) : 0;
			e.flag_id = row[1] ? static_cast<int32_t>(atoi(row[1])) : 0;

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int progression_atlas_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				progression_atlas_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const ProgressionAtlas &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[0] + " = " + std::to_string(e.zone_id));
		v.push_back(columns[1] + " = " + std::to_string(e.flag_id));

		auto results = db.QueryDatabase(
			fmt::format(
				"UPDATE {} SET {} WHERE {} = {}",
				TableName(),
				Strings::Implode(", ", v),
				PrimaryKey(),
				e.zone_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static ProgressionAtlas InsertOne(
		Database& db,
		ProgressionAtlas e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.zone_id));
		v.push_back(std::to_string(e.flag_id));

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES ({})",
				BaseInsert(),
				Strings::Implode(",", v)
			)
		);

		if (results.Success()) {
			e.zone_id = results.LastInsertedID();
			return e;
		}

		e = NewEntity();

		return e;
	}

	static int InsertMany(
		Database& db,
		const std::vector<ProgressionAtlas> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.zone_id));
			v.push_back(std::to_string(e.flag_id));

			insert_chunks.push_back("(" + Strings::Implode(",", v) + ")");
		}

		std::vector<std::string> v;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES {}",
				BaseInsert(),
				Strings::Implode(",", insert_chunks)
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static std::vector<ProgressionAtlas> All(Database& db)
	{
		std::vector<ProgressionAtlas> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			ProgressionAtlas e{};

			e.zone_id = row[0] ? static_cast<int32_t>(atoi(row[0])) : 0;
			e.flag_id = row[1] ? static_cast<int32_t>(atoi(row[1])) : 0;

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<ProgressionAtlas> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<ProgressionAtlas> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			ProgressionAtlas e{};

			e.zone_id = row[0] ? static_cast<int32_t>(atoi(row[0])) : 0;
			e.flag_id = row[1] ? static_cast<int32_t>(atoi(row[1])) : 0;

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static int DeleteWhere(Database& db, const std::string &where_filter)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {}",
				TableName(),
				where_filter
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int Truncate(Database& db)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"TRUNCATE TABLE {}",
				TableName()
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int64 GetMaxId(Database& db)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"SELECT COALESCE(MAX({}), 0) FROM {}",
				PrimaryKey(),
				TableName()
			)
		);

		return (results.Success() && results.begin()[0] ? strtoll(results.begin()[0], nullptr, 10) : 0);
	}

	static int64 Count(Database& db, const std::string &where_filter = "")
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"SELECT COUNT(*) FROM {} {}",
				TableName(),
				(where_filter.empty() ? "" : "WHERE " + where_filter)
			)
		);

		return (results.Success() && results.begin()[0] ? strtoll(results.begin()[0], nullptr, 10) : 0);
	}

	static std::string BaseReplace()
	{
		return fmt::format(
			"REPLACE INTO {} ({}) ",
			TableName(),
			ColumnsRaw()
		);
	}

	static int ReplaceOne(
		Database& db,
		const ProgressionAtlas &e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.zone_id));
		v.push_back(std::to_string(e.flag_id));

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES ({})",
				BaseReplace(),
				Strings::Implode(",", v)
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int ReplaceMany(
		Database& db,
		const std::vector<ProgressionAtlas> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.zone_id));
			v.push_back(std::to_string(e.flag_id));

			insert_chunks.push_back("(" + Strings::Implode(",", v) + ")");
		}

		std::vector<std::string> v;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES {}",
				BaseReplace(),
				Strings::Implode(",", insert_chunks)
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}
};

#endif //EQEMU_BASE_PROGRESSION_ATLAS_REPOSITORY_H
