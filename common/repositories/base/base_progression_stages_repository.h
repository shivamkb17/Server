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

#ifndef EQEMU_BASE_PROGRESSION_STAGES_REPOSITORY_H
#define EQEMU_BASE_PROGRESSION_STAGES_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>

class BaseProgressionStagesRepository {
public:
	struct ProgressionStages {
		int32_t     stage_id;
		std::string stage_name;
		int32_t     flag_id;
	};

	static std::string PrimaryKey()
	{
		return std::string("stage_id");
	}

	static std::vector<std::string> Columns()
	{
		return {
			"stage_id",
			"stage_name",
			"flag_id",
		};
	}

	static std::vector<std::string> SelectColumns()
	{
		return {
			"stage_id",
			"stage_name",
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
		return std::string("progression_stages");
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

	static ProgressionStages NewEntity()
	{
		ProgressionStages e{};

		e.stage_id   = 0;
		e.stage_name = "";
		e.flag_id    = 0;

		return e;
	}

	static ProgressionStages GetProgressionStages(
		const std::vector<ProgressionStages> &progression_stagess,
		int progression_stages_id
	)
	{
		for (auto &progression_stages : progression_stagess) {
			if (progression_stages.stage_id == progression_stages_id) {
				return progression_stages;
			}
		}

		return NewEntity();
	}

	static ProgressionStages FindOne(
		Database& db,
		int progression_stages_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				progression_stages_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			ProgressionStages e{};

			e.stage_id   = row[0] ? static_cast<int32_t>(atoi(row[0])) : 0;
			e.stage_name = row[1] ? row[1] : "";
			e.flag_id    = row[2] ? static_cast<int32_t>(atoi(row[2])) : 0;

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int progression_stages_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				progression_stages_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const ProgressionStages &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[0] + " = " + std::to_string(e.stage_id));
		v.push_back(columns[1] + " = '" + Strings::Escape(e.stage_name) + "'");
		v.push_back(columns[2] + " = " + std::to_string(e.flag_id));

		auto results = db.QueryDatabase(
			fmt::format(
				"UPDATE {} SET {} WHERE {} = {}",
				TableName(),
				Strings::Implode(", ", v),
				PrimaryKey(),
				e.stage_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static ProgressionStages InsertOne(
		Database& db,
		ProgressionStages e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.stage_id));
		v.push_back("'" + Strings::Escape(e.stage_name) + "'");
		v.push_back(std::to_string(e.flag_id));

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES ({})",
				BaseInsert(),
				Strings::Implode(",", v)
			)
		);

		if (results.Success()) {
			e.stage_id = results.LastInsertedID();
			return e;
		}

		e = NewEntity();

		return e;
	}

	static int InsertMany(
		Database& db,
		const std::vector<ProgressionStages> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.stage_id));
			v.push_back("'" + Strings::Escape(e.stage_name) + "'");
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

	static std::vector<ProgressionStages> All(Database& db)
	{
		std::vector<ProgressionStages> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			ProgressionStages e{};

			e.stage_id   = row[0] ? static_cast<int32_t>(atoi(row[0])) : 0;
			e.stage_name = row[1] ? row[1] : "";
			e.flag_id    = row[2] ? static_cast<int32_t>(atoi(row[2])) : 0;

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<ProgressionStages> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<ProgressionStages> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			ProgressionStages e{};

			e.stage_id   = row[0] ? static_cast<int32_t>(atoi(row[0])) : 0;
			e.stage_name = row[1] ? row[1] : "";
			e.flag_id    = row[2] ? static_cast<int32_t>(atoi(row[2])) : 0;

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
		const ProgressionStages &e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.stage_id));
		v.push_back("'" + Strings::Escape(e.stage_name) + "'");
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
		const std::vector<ProgressionStages> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.stage_id));
			v.push_back("'" + Strings::Escape(e.stage_name) + "'");
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

#endif //EQEMU_BASE_PROGRESSION_STAGES_REPOSITORY_H
