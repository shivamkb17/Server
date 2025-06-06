#ifndef EQEMU_ACCOUNT_CHARACTER_SET_MEMBERS_REPOSITORY_H
#define EQEMU_ACCOUNT_CHARACTER_SET_MEMBERS_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_account_character_set_members_repository.h"

class AccountCharacterSetMembersRepository: public BaseAccountCharacterSetMembersRepository {
public:

    static std::vector<uint32> GetCharacterIDsForSet(Database& db, uint32 set_id) {
        auto m = GetWhere(db, fmt::format("set_id = {}", set_id));
        std::vector<uint32> character_ids;

        for (const auto& member : m) {
            character_ids.emplace_back(member.character_id);
        }

        return character_ids;
    }

    static bool AddCharacterToSet(
        Database& db,
        int32_t set_id,
        int32_t character_id
    )
    {

		auto e = GetWhere(
			db,
			fmt::format("set_id = {} AND character_id = {}", set_id, character_id)
		);
		auto m = GetCharacterIDsForSet(db, set_id);

        if (!e.empty() || m.size() >= 12) {
            return false;
        }

        auto entity = NewEntity();
        entity.set_id = set_id;
        entity.character_id = character_id;

        auto result = InsertOne(db, entity);
        return result.set_id != 0;
    }

     static bool RemoveCharacterFromSet(
        Database& db,
        int32_t set_id,
        int32_t character_id
    )
    {
        int rows_affected = DeleteWhere(
            db,
            fmt::format("set_id = {} AND character_id = {}", set_id, character_id)
        );

        return rows_affected > 0;
    }

    static bool MoveCharacterBetweenSets(
        Database& db,
        int32_t character_id,
        int32_t from_set_id,
        int32_t to_set_id
    )
    {
        if (!RemoveCharacterFromSet(db, from_set_id, character_id)) {
            return false;
        }

        if (!AddCharacterToSet(db, to_set_id, character_id)) {
            AddCharacterToSet(db, from_set_id, character_id);
            return false;
        }

        return true;
    }

    static int32_t GetCharacterSetId(
        Database& db,
        int32_t character_id
    )
    {
		auto m = GetWhere(
			db,
			fmt::format("character_id = {} LIMIT 1", character_id)
        );

        if (m.empty()) {
            return 0;
        }

        return m[0].set_id;
    }

static std::vector<uint32> GetCharacterSetIds(
    Database& db,
    int32_t character_id
)
{
    auto m = GetWhere(
        db,
        fmt::format("character_id = {} ORDER BY set_id", character_id)
    );

    std::vector<uint32> set_ids;
    for (auto& member : m) {
        set_ids.push_back(member.set_id);
    }

    return set_ids;
}


    static int ClearCharacterSet(
        Database& db,
        int32_t set_id
    )
    {
        return DeleteWhere(db, fmt::format("set_id = {}", set_id));
    }

    static bool IsCharacterInSet(
        Database& db,
        int32_t set_id,
        int32_t character_id
    )
    {
        auto count = Count(
            db,
            fmt::format("set_id = {} AND character_id = {}", set_id, character_id)
        );

        return count > 0;
    }

	static int RemoveCharacterFromAllSets(
		Database& db,
		int32_t character_id
	)
	{
		return DeleteWhere(db, fmt::format("character_id = {}", character_id));
	}

};

#endif //EQEMU_ACCOUNT_CHARACTER_SET_MEMBERS_REPOSITORY_H