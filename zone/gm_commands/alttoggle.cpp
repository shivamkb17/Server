#include "../client.h"
#include "../../common/repositories/aa_ability_repository.h"

void command_alt_toggle(Client *c, const Seperator *sep) {
    if (!c) {
        return;
    }

    if (sep->argnum < 1 || !sep->IsNumber(1)) {
        c->Message(Chat::White, "Usage: #alt_toggle <ability_id> [toggle|enable|disable]");
        return;
    }

    int ability_id = Strings::ToInt(sep->arg[1]);

    auto aa_ability = AaAbilityRepository::FindOne(database, ability_id);
    if (aa_ability.id == 0) {
        c->Message(Chat::White, fmt::format("AA ability with ID {} not found.", ability_id).c_str());
        return;
    }

    std::string ability_name = aa_ability.name;

    bool new_status;
    std::string action = "toggle";

    if (sep->argnum >= 2) {
        std::string command = Strings::ToLower(sep->arg[2]);

        if (command == "enable" || command == "on" || command == "1") {
            new_status = true;
            action = "enable";
        }
        else if (command == "disable" || command == "off" || command == "0") {
            new_status = false;
            action = "disable";
        }
        else if (command == "toggle") {
            // Get current status and flip it
            new_status = !c->GetToggleAAStatus(ability_id);
            action = "toggle";
        }
        else {
            c->Message(Chat::White, "Invalid action. Use: toggle, enable, or disable");
            return;
        }
    }
    else {
        new_status = !c->GetToggleAAStatus(ability_id);
    }

    c->SetToggleAAStatus(ability_id, new_status);

    std::string status_text = new_status ? "enabled" : "disabled";
    if (action == "toggle") {
        c->Message(Chat::White, fmt::format("Toggled AA '{}' ({}) to: {}", ability_name, ability_id, status_text).c_str());
    }
    else {
        c->Message(Chat::White, fmt::format("AA '{}' ({}) {}", ability_name, ability_id, status_text).c_str());
    }
}