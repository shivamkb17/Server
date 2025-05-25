#include "../client.h"

void command_petcmd(Client *c, const Seperator *sep) {
    // Basic null pointer checks
    if (!c || !sep) {
        return;
    }

    const std::string usage = "Usage: #petcmd [attack, qattack, follow, guard, sit, stop, taunt (on\\off), hold (on\\off), ghold (on\\off), spellhold (on\\off), focus (on\\off), back, regroup (on\\off), assist (on\\off), health, leader, feign, leave] [all, swarm, mag, bst, nec, enc, shm, dru, brd, shd]. Verbs do not need to be applied in any specific order.";

    // Safely build a vector of args
    std::vector<std::string> args;
    // argnum is the index of the last argument, not the count
    for (uint16 i = 0; i <= sep->argnum && i < sep->GetMaxArgNum(); ++i) {
        if (sep->arg[i] != nullptr) {
            args.push_back(sep->arg[i]);
        }
    }

    // Check if we have at least one argument
    if (args.empty()) {
        c->Message(Chat::White, usage.c_str());
        return;
    }

    // Track commands and targets
    bool all_classes = false;
    bool swarm_only = false;
    std::vector<int> command_codes;
    std::vector<uint8> class_targets;

    // Define mappings for class targets
    std::map<std::string, uint8> class_map = {
        {"mag", Class::Magician}, {"mage", Class::Magician}, {"magician", Class::Magician},
        {"bst", Class::Beastlord}, {"bl", Class::Beastlord}, {"beast", Class::Beastlord}, {"beastlord", Class::Beastlord},
        {"nec", Class::Necromancer}, {"necro", Class::Necromancer}, {"necromancer", Class::Necromancer},
        {"enc", Class::Enchanter}, {"ench", Class::Enchanter}, {"enchanter", Class::Enchanter},
        {"shm", Class::Shaman}, {"shaman", Class::Shaman},
        {"dru", Class::Druid}, {"druid", Class::Druid},
        {"brd", Class::Bard}, {"bard", Class::Bard},
        {"shd", Class::ShadowKnight}, {"sk", Class::ShadowKnight}, {"shadowknight", Class::ShadowKnight},
        {"war", Class::Warrior}, {"warrior", Class::Warrior},
        {"clr", Class::Cleric}, {"cler", Class::Cleric}, {"cleric", Class::Cleric},
        {"pal", Class::Paladin}, {"paladin", Class::Paladin},
        {"rng", Class::Ranger}, {"ranger", Class::Ranger},
        {"mnk", Class::Monk}, {"monk", Class::Monk},
        {"rog", Class::Rogue}, {"rogue", Class::Rogue},
        {"wiz", Class::Wizard}, {"wizard", Class::Wizard},
        {"ber", Class::Berserker}, {"berserker", Class::Berserker}
    };

    // Define mappings for normal commands
    std::map<std::string, int> command_map = {
        {"attack", PET_ATTACK},
        {"qattack", PET_QATTACK},
        {"follow", PET_FOLLOWME}, {"followme", PET_FOLLOWME},
        {"guard", PET_GUARDHERE},
        {"sit", PET_SIT},
        {"stop", PET_STOP}, {"freeze", PET_STOP},
        {"back", PET_BACKOFF}, {"backoff", PET_BACKOFF},
        {"leave", PET_GETLOST}, {"dismiss", PET_GETLOST}, {"getlost", PET_GETLOST},
        {"health", PET_HEALTHREPORT}, {"healthreport", PET_HEALTHREPORT}, {"hp", PET_HEALTHREPORT}, {"stats", PET_HEALTHREPORT}, {"inventory", PET_HEALTHREPORT}, {"inv", PET_HEALTHREPORT},
        {"leader", PET_LEADER}, {"master", PET_LEADER},
        {"feign", PET_FEIGN}, {"fd", PET_FEIGN}, {"playdead", PET_FEIGN}
    };

    // Define mappings for toggle commands with their on/off variants
    struct ToggleCommand {
        int base;
        int on;
        int off;
    };

    std::map<std::string, ToggleCommand> toggle_command_map = {
        {"taunt", {PET_TAUNT, PET_TAUNT_ON, PET_TAUNT_OFF}},
        {"hold", {PET_HOLD, PET_HOLD_ON, PET_HOLD_OFF}},
        {"ghold", {PET_GHOLD, PET_GHOLD_ON, PET_GHOLD_OFF}},
        {"spellhold", {PET_SPELLHOLD, PET_SPELLHOLD_ON, PET_SPELLHOLD_OFF}},
        {"nocast", {PET_SPELLHOLD, PET_SPELLHOLD_ON, PET_SPELLHOLD_OFF}},
        {"focus", {PET_FOCUS, PET_FOCUS_ON, PET_FOCUS_OFF}},
        {"regroup", {PET_REGROUP, PET_REGROUP_ON, PET_REGROUP_OFF}},
        {"assist", {CUSTOM_PET_ASSIST, CUSTOM_PET_ASSIST_ON, CUSTOM_PET_ASSIST_OFF}},
        {"assistme", {CUSTOM_PET_ASSIST, CUSTOM_PET_ASSIST_ON, CUSTOM_PET_ASSIST_OFF}}
    };

	// Define multi-word commands and their corresponding codes
	std::map<std::pair<std::string, std::string>, int> multi_word_commands = {
		{{"follow", "me"}, PET_FOLLOWME},
		{{"guard", "here"}, PET_GUARDHERE},
		{{"get", "lost"}, PET_GETLOST},
		{{"assist", "me"}, CUSTOM_PET_ASSIST},
		{{"back", "off"}, PET_BACKOFF},
		{{"health", "report"}, PET_HEALTHREPORT},
		{{"play", "dead"}, PET_FEIGN}
	};

	// Process multi-word commands first
	std::vector<bool> arg_processed(args.size(), false);
	for (size_t i = 0; i < args.size() - 1; ++i) {
		if (arg_processed[i]) continue;

		std::string first_word = Strings::ToLower(args[i]);
		std::string second_word = Strings::ToLower(args[i+1]);

		auto it = multi_word_commands.find({first_word, second_word});
		if (it != multi_word_commands.end()) {
			command_codes.push_back(it->second);
			arg_processed[i] = true;
			arg_processed[i+1] = true;
		}
	}

	// Process each remaining argument
	for (size_t i = 0; i < args.size(); ++i) {
		// Skip already processed arguments
		if (arg_processed[i]) {
			continue;
		}

		const std::string& arg = Strings::ToLower(args[i]);

        // Check if it's "all"
        if (arg == "all") {
            all_classes = true;
            continue;
        }

        // Check if it's "swarm"
        if (arg == "swarm") {
            swarm_only = true;
            continue;
        }

        // Check if it's a class target
        auto class_it = class_map.find(arg);
        if (class_it != class_map.end()) {
            class_targets.push_back(class_it->second);
            continue;
        }

        // Check if it's a normal command
        auto cmd_it = command_map.find(arg);
        if (cmd_it != command_map.end()) {
            command_codes.push_back(cmd_it->second);
            continue;
        }

        // Check if it's a toggle command
        auto toggle_it = toggle_command_map.find(arg);
        if (toggle_it != toggle_command_map.end()) {
            // Check for on/off modifier
            int cmd_code = toggle_it->second.base; // Default to base command

            // Look ahead for on/off
            if (i + 1 < args.size()) {
                const std::string& next_arg = Strings::ToLower(args[i + 1]);
                if (next_arg == "on") {
                    cmd_code = toggle_it->second.on;
                    i++; // Skip the modifier
                }
                else if (next_arg == "off") {
                    cmd_code = toggle_it->second.off;
                    i++; // Skip the modifier
                }
            }

            command_codes.push_back(cmd_code);
            continue;
        }

        // Unrecognized argument - silently ignore
    }

    // If no commands specified, show usage
    if (command_codes.empty()) {
        c->Message(Chat::White, usage.c_str());
        return;
    }

    // Define commands that are not supported by swarm pets
    std::set<int> swarm_unsupported_commands = {
        PET_FOLLOWME,
        PET_GUARDHERE,
        PET_SIT,
        PET_HEALTHREPORT,
        PET_FEIGN,
        PET_REGROUP, PET_REGROUP_ON, PET_REGROUP_OFF,
        PET_SPELLHOLD, PET_SPELLHOLD_ON, PET_SPELLHOLD_OFF,
        PET_TAUNT, PET_TAUNT_ON, PET_TAUNT_OFF
    };

    // Check if swarm_only is used with unsupported commands
    if (swarm_only) {
        std::vector<std::string> unsupported_found;
        for (int cmd : command_codes) {
            if (swarm_unsupported_commands.find(cmd) != swarm_unsupported_commands.end()) {
                // Map command back to user-friendly name for error message
                std::string cmd_name = "unknown";
                if (cmd == PET_FOLLOWME) cmd_name = "follow";
                else if (cmd == PET_GUARDHERE) cmd_name = "guard";
                else if (cmd == PET_SIT) cmd_name = "sit";
                else if (cmd == PET_HEALTHREPORT) cmd_name = "health";
                else if (cmd == PET_FEIGN) cmd_name = "feign";
                else if (cmd == PET_REGROUP || cmd == PET_REGROUP_ON || cmd == PET_REGROUP_OFF) cmd_name = "regroup";
                else if (cmd == PET_SPELLHOLD || cmd == PET_SPELLHOLD_ON || cmd == PET_SPELLHOLD_OFF) cmd_name = "spellhold";
                else if (cmd == PET_TAUNT || cmd == PET_TAUNT_ON || cmd == PET_TAUNT_OFF) cmd_name = "taunt";

                unsupported_found.push_back(cmd_name);
            }
        }

        if (!unsupported_found.empty()) {
            std::string msg = "The following commands are not supported by swarm pets: ";
            for (size_t i = 0; i < unsupported_found.size(); ++i) {
                if (i > 0) msg += ", ";
                msg += unsupported_found[i];
            }
            c->Message(Chat::White, msg.c_str());
            return;
        }
    }

    // Default behavior: if no target specified, target all regular pets only
    if (class_targets.empty() && !all_classes && !swarm_only) {
        all_classes = true;
    }

    // Get target for commands
    Mob* target = c->GetTarget();

    // Handle swarm pets (only when explicitly targeted with 'swarm')
    if (swarm_only) {
        // Check if we have any persistent toggle commands that don't require active swarm pets
        std::vector<int> persistent_commands;
        std::vector<int> immediate_commands;

        for (int cmd : command_codes) {
            if (cmd == PET_HOLD || cmd == PET_HOLD_ON || cmd == PET_HOLD_OFF ||
                cmd == PET_GHOLD || cmd == PET_GHOLD_ON || cmd == PET_GHOLD_OFF ||
                cmd == PET_FOCUS || cmd == PET_FOCUS_ON || cmd == PET_FOCUS_OFF ||
                cmd == CUSTOM_PET_ASSIST || cmd == CUSTOM_PET_ASSIST_ON || cmd == CUSTOM_PET_ASSIST_OFF) {
                persistent_commands.push_back(cmd);
            } else {
                immediate_commands.push_back(cmd);
            }
        }

        // Process persistent settings first (these work without active swarm pets)
        for (int cmd : persistent_commands) {
            switch (cmd) {
                case PET_HOLD:
                    // Toggle current hold setting
                    {
                        std::string current = c->GetBucket("pet_settings.swarm.hold");
                        std::string new_setting = (current == "on") ? "off" : "on";
                        c->SetBucket("pet_settings.swarm.hold", new_setting);
                        c->Message(Chat::White, fmt::format("Swarm pet hold setting: {}", (new_setting == "on") ? "ON" : "OFF").c_str());
                    }
                    break;
                case PET_HOLD_ON:
                    c->SetBucket("pet_settings.swarm.hold", "on");
                    c->Message(Chat::White, "Swarm pet hold setting: ON");
                    break;
                case PET_HOLD_OFF:
                    c->SetBucket("pet_settings.swarm.hold", "off");
                    c->Message(Chat::White, "Swarm pet hold setting: OFF");
                    break;
                case PET_GHOLD:
                    // Toggle current ghold setting
                    {
                        std::string current = c->GetBucket("pet_settings.swarm.ghold");
                        std::string new_setting = (current == "on") ? "off" : "on";
                        c->SetBucket("pet_settings.swarm.ghold", new_setting);
                        c->Message(Chat::White, fmt::format("Swarm pet ghold setting: {}", (new_setting == "on") ? "ON" : "OFF").c_str());
                    }
                    break;
                case PET_GHOLD_ON:
                    c->SetBucket("pet_settings.swarm.ghold", "on");
                    c->Message(Chat::White, "Swarm pet ghold setting: ON");
                    break;
                case PET_GHOLD_OFF:
                    c->SetBucket("pet_settings.swarm.ghold", "off");
                    c->Message(Chat::White, "Swarm pet ghold setting: OFF");
                    break;
                case PET_FOCUS:
                    // Toggle current focus setting
                    {
                        std::string current = c->GetBucket("pet_settings.swarm.focus");
                        std::string new_setting = (current == "on") ? "off" : "on";
                        c->SetBucket("pet_settings.swarm.focus", new_setting);
                        c->Message(Chat::White, fmt::format("Swarm pet focus setting: {}", (new_setting == "on") ? "ON" : "OFF").c_str());
                    }
                    break;
                case PET_FOCUS_ON:
                    c->SetBucket("pet_settings.swarm.focus", "on");
                    c->Message(Chat::White, "Swarm pet focus setting: ON");
                    break;
                case PET_FOCUS_OFF:
                    c->SetBucket("pet_settings.swarm.focus", "off");
                    c->Message(Chat::White, "Swarm pet focus setting: OFF");
                    break;
                case CUSTOM_PET_ASSIST:
                    // Toggle current assist setting
                    {
                        std::string current = c->GetBucket("pet_settings.swarm.assist");
                        std::string new_setting = (current == "on") ? "off" : "on";
                        c->SetBucket("pet_settings.swarm.assist", new_setting);
                        c->Message(Chat::White, fmt::format("Swarm pet assist setting: {}", (new_setting == "on") ? "ON" : "OFF").c_str());
                    }
                    break;
                case CUSTOM_PET_ASSIST_ON:
                    c->SetBucket("pet_settings.swarm.assist", "on");
                    c->Message(Chat::White, "Swarm pet assist setting: ON");
                    break;
                case CUSTOM_PET_ASSIST_OFF:
                    c->SetBucket("pet_settings.swarm.assist", "off");
                    c->Message(Chat::White, "Swarm pet assist setting: OFF");
                    break;
            }
        }

        // If we only had persistent commands and no active swarm pets, we're done
        if (immediate_commands.empty()) {
            if (!persistent_commands.empty()) {
                c->Message(Chat::White, "Settings saved for future swarm pets.");
            }
            return;
        }

        // Handle immediate commands (these require active swarm pets)
        auto swarm_pets = c->GetAllSwarmPets();
        if (swarm_pets.empty()) {
            c->Message(Chat::White, "You don't have any swarm pets under your control.");
            return;
        }

        // Execute all commands (both persistent and immediate) on existing swarm pets
        for (auto swarm_pet : swarm_pets) {
            if (!swarm_pet || !swarm_pet->IsNPC()) {
                continue;
            }

            for (int cmd : command_codes) {
                swarm_pet->CastToNPC()->DoPetCommand(cmd, target);
            }
        }
    }

    // Handle regular pets (skip if swarm_only is true)
    if (!swarm_only) {
        auto pets = c->GetAllPets();
        if (pets.empty()) {
            c->Message(Chat::White, "You don't have any pets under your control.");
            return;
        }

        // Execute commands on regular pets
        for (auto pet : pets) {
            // Skip invalid pets
            if (!pet || !pet->IsNPC()) {
                continue;
            }

            auto pet_class_id = pet->CastToNPC()->GetPetOriginClass();

            // Check if this pet's class matches our targets
            if (all_classes || std::find(class_targets.begin(), class_targets.end(), pet_class_id) != class_targets.end()) {
                // Execute all commands on this pet
                for (int cmd : command_codes) {
                    pet->CastToNPC()->DoPetCommand(cmd, target);
                }
            }
        }
    }
}