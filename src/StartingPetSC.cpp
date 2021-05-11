/*
 *  Module for AzerothCore by Gozzim (https://github.com/Gozzim)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StartingPet.h"
#include "Player.h"
#include "Chat.h"
#include "WorldSession.h"
#include "ScriptMgr.h"
#include "Config.h"

static bool StartingPetEnable;
static bool StartingPetAnnounce;
static bool StartingPetHunter;
static bool StartingPetName;
static uint32 StartingMinion;

class StartingPetBeforeConfigLoad : public WorldScript {
public:
    StartingPetBeforeConfigLoad() : WorldScript("StartingPetBeforeConfigLoad") { }

    void OnBeforeConfigLoad(bool /*reload*/) override {
        StartingPetEnable = sConfigMgr->GetBoolDefault("StartingPet.Enable", 1);
        StartingPetAnnounce = sConfigMgr->GetBoolDefault("StartingPet.Announce", 1);
        StartingPetHunter = sConfigMgr->GetBoolDefault("StartingPet.HunterPet", 1);
        StartingPetName = sConfigMgr->GetBoolDefault("StartingPet.RandName", 1);
        StartingMinion = sConfigMgr->GetIntDefault("StartingPet.WarlockMinion", 47);
    }
};

class StartingPetScripts : public PlayerScript {
public:
    StartingPetScripts() : PlayerScript("StartingPetScripts") { }

    void OnFirstLogin(Player* player) override {
        if (StartingPetEnable)
        {
            if (StartingPetAnnounce)
                ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00StartingPet |rmodule.");

            if (StartingPetHunter && player->getClass() == CLASS_HUNTER)
            {
                sStartingPet->CreateRandomPet(player, StartingPetName);
                sStartingPet->LearnPetSpells(player);
            }

            if (StartingMinion > 0 && player->getClass() == CLASS_WARLOCK)
            {
                sStartingPet->LearnWarlockSpells(player, StartingMinion);
                sStartingPet->SummonWarlockMinion(player, StartingMinion);
            }
        }
    }
};

void AddStartingPetScripts() {
    new StartingPetBeforeConfigLoad();
    new StartingPetScripts();
}