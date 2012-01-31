/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __BATTLEGROUNDWG_H
#define __BATTLEGROUNDWG_H

class BattleGround;

#include "BattleGround.h"

enum BG_WG_WorldState
{
    BG_WG_WORLD_STATE_VEHICLE_H         = 3490,
    BG_WG_WORLD_STATE_MAX_VEHICLE_H     = 3491,
    BG_WG_WORLD_STATE_VEHICLE_A         = 3680,
    BG_WG_WORLD_STATE_MAX_VEHICLE_A     = 3681,
    BG_WG_WORLD_STATE_ACTIVE            = 3801,
    BG_WG_WORLD_STATE_DEFENDER          = 3802,
    BG_WG_WORLD_STATE_ATTACKER          = 3803,
};

enum BG_WG_Items
{
    BG_WG_MARK_OF_HONOR                                = 43589,
};

enum BG_WG_Sounds
{
    BG_WG_HORDE_WINS                                   = 8454,
    BG_WG_ALLIANCE_WINS                                = 8455,
    BG_WG_START                                        = 3439,
};

enum BG_WG_Achievements
{
    BG_WG_ACHIEVEMENTS_WIN_WG                          = 1717,
    BG_WG_ACHIEVEMENTS_WIN_WG_100                      = 1718, // todo
    BG_WG_ACHIEVEMENTS_WG_GNOMESLAUGHTER               = 1723, // todo
    BG_WG_ACHIEVEMENTS_WG_TOWER_DESTROY                = 1727,
    BG_WG_ACHIEVEMENTS_DESTRUCTION_DERBY_A             = 1737, // todo
    BG_WG_ACHIEVEMENTS_WG_TOWER_CANNON_KILL            = 1751, // todo
    BG_WG_ACHIEVEMENTS_WG_MASTER_A                     = 1752, // todo
    BG_WG_ACHIEVEMENTS_WIN_WG_TIMER_10                 = 1755,
    BG_WG_ACHIEVEMENTS_STONE_KEEPER_50                 = 2085, // todo
    BG_WG_ACHIEVEMENTS_STONE_KEEPER_100                = 2086, // todo
    BG_WG_ACHIEVEMENTS_STONE_KEEPER_250                = 2087, // todo
    BG_WG_ACHIEVEMENTS_STONE_KEEPER_500                = 2088, // todo
    BG_WG_ACHIEVEMENTS_STONE_KEEPER_1000               = 2089, // todo
    BG_WG_ACHIEVEMENTS_WG_RANGER                       = 2199, // todo
    BG_WG_ACHIEVEMENTS_DESTRUCTION_DERBY_H             = 2476, // todo
    BG_WG_ACHIEVEMENTS_WG_MASTER_H                     = 2776, // todo
};


enum BG_WG_SpellId
{
    // AWartime auras
    BG_WG_SPELL_RECRUIT                               = 37795,
    BG_WG_SPELL_CORPORAL                              = 33280,
    BG_WG_SPELL_LIEUTENANT                            = 55629,
    BG_WG_SPELL_TENACITY                              = 58549,
    BG_WG_SPELL_TENACITY_VEHICLE                      = 59911,
    BG_WG_SPELL_TOWER_CONTROL                         = 62064,
    BG_WG_SPELL_SPIRITUAL_IMMUNITY                    = 58729,
    BG_WG_SPELL_GREAT_HONOR                           = 58555,
    BG_WG_SPELL_GREATER_HONOR                         = 58556,
    BG_WG_SPELL_GREATEST_HONOR                        = 58557,
    BG_WG_SPELL_ALLIANCE_FLAG                         = 14268,
    BG_WG_SPELL_HORDE_FLAG                            = 14267,

    // Reward spells
    BG_WG_SPELL_VICTORY_REWARD                        = 56902,
    BG_WG_SPELL_DEFEAT_REWARD                         = 58494,
    BG_WG_SPELL_DAMAGED_TOWER                         = 59135,
    BG_WG_SPELL_DESTROYED_TOWER                       = 59136,
    BG_WG_SPELL_DAMAGED_BUILDING                      = 59201,
    BG_WG_SPELL_INTACT_BUILDING                       = 59203,

    BG_WG_SPELL_TELEPORT_BRIDGE                       = 59096,
    BG_WG_SPELL_TELEPORT_FORTRESS                     = 60035,

    BG_WG_SPELL_TELEPORT_DALARAN                      = 53360,
    BG_WG_SPELL_VICTORY_AURA                          = 60044,

    // Other spells
    BG_WG_SPELL_WINTERGRASP_WATER                     = 36444,
    BG_WG_SPELL_ESSENCE_OF_WINTERGRASP                = 58045,
    BG_WG_SPELL_WINTERGRASP_RESTRICTED_FLIGHT_AREA    = 58730,

    // Phasing spells
    BG_WG_SPELL_HORDE_CONTROLS_FACTORY_PHASE_SHIFT    = 56618,// ADDS PHASE 16
    BG_WG_SPELL_ALLIANCE_CONTROLS_FACTORY_PHASE_SHIFT = 56617,// ADDS PHASE 32

    BG_WG_SPELL_HORDE_CONTROL_PHASE_SHIFT             = 55773,// ADDS PHASE 64
    BG_WG_SPELL_ALLIANCE_CONTROL_PHASE_SHIFT          = 55774,// ADDS PHASE 128
};

enum BG_WG_Npc
{
    BG_WG_NPC_GUARD_H                      = 30739,
    BG_WG_NPC_GUARD_A                      = 30740,
    BG_WG_NPC_STALKER                      = 00000,

    BG_WG_NPC_VIERON_BLAZEFEATHER          = 31102,
    BG_WG_NPC_STONE_GUARD_MUKAR            = 32296,// <WINTERGRASP QUARTERMASTER>
    BG_WG_NPC_HOODOO_MASTER_FU_JIN         = 31101,// <MASTER HEXXER>
    BG_WG_NPC_CHAMPION_ROS_SLAI            = 39173,// <WINTERGRASP QUARTERMASTER>
    BG_WG_NPC_COMMANDER_DARDOSH            = 31091,
    BG_WG_NPC_TACTICAL_OFFICER_KILRATH     = 31151,
    BG_WG_NPC_SIEGESMITH_STRONGHOOF        = 31106,
    BG_WG_NPC_PRIMALIST_MULFORT            = 31053,
    BG_WG_NPC_LIEUTENANT_MURP              = 31107,

    BG_WG_NPC_BOWYER_RANDOLPH              = 31052,
    BG_WG_NPC_KNIGHT_DAMERON               = 32294,// <WINTERGRASP QUARTERMASTER>
    BG_WG_NPC_SORCERESS_KAYLANA            = 31051,// <ENCHANTRESS>
    BG_WG_NPC_MARSHAL_MAGRUDER             = 39172,// <WINTERGRASP QUARTERMASTER>
    BG_WG_NPC_COMMANDER_ZANNETH            = 31036,
    BG_WG_NPC_TACTICAL_OFFICER_AHBRAMIS    = 31153,
    BG_WG_NPC_SIEGE_MASTER_STOUTHANDLE     = 31108,
    BG_WG_NPC_ANCHORITE_TESSA              = 31054,
    BG_WG_NPC_SENIOR_DEMOLITIONIST_LEGOSO  = 31109,

    BG_WG_NPC_TAUNKA_SPIRIT_GUIDE                         = 31841,    // Horde spirit guide for Wintergrasp
    BG_WG_NPC_DWARVEN_SPIRIT_GUIDE                        = 31842,    // Alliance spirit guide for Wintergrasp
};

enum BG_WG_GoType
{
    BG_WG_OBJECTTYPE_DOOR = 0,
    BG_WG_OBJECTTYPE_TITANRELIC = 1,
    BG_WG_OBJECTTYPE_WALL = 2,
    BG_WG_OBJECTTYPE_DOOR_LAST = 3,
    BG_WG_OBJECTTYPE_KEEP_TOWER = 4,
    BG_WG_OBJECTTYPE_TOWER = 5,
};

//Need Id
/*enum BG_WG_GoId
{
    BG_WG_OBJECTTYPE_DOOR               = 
    BG_WG_OBJECTTYPE_TITANRELIC         =
    BG_WG_OBJECTTYPE_WALL               =
    BG_WG_OBJECTTYPE_DOOR_LAST          =
    BG_WG_OBJECTTYPE_KEEP_TOWER         =
    BG_WG_OBJECTTYPE_TOWER              =
};*/

enum BG_WG_GraveYard
{
    BG_WG_GY_WORKSHOP_NE = 0,
    BG_WG_GY_WORKSHOP_NW = 1,
    BG_WG_GY_WORKSHOP_SE = 2,
    BG_WG_GY_WORKSHOP_SW = 3,
    BG_WG_GY_KEEP = 4,
    BG_WG_GY_HORDE = 5,
    BG_WG_GY_ALLIANCE = 6,
    BG_WG_GY_MAX= 7,
};

class BattleGroundWG : public BattleGround
{
    friend class BattleGroundMgr;

    public:
        BattleGroundWG();
        ~BattleGroundWG();

        void Update(uint32 uiDiff);

        /* inherited from BattleGroundClass */
        virtual void AddPlayer(Player *plr);
        virtual void EventPlayerDamageGO(Player *player, GameObject* target_obj, uint32 eventId, uint32 doneBy = 0);
        virtual void FillInitialWorldStates(WorldPacket& data, uint32& count);
        virtual void HandleKillUnit(Creature *unit, Player *killer);
        virtual void HandleKillPlayer(Player* player, Player* killer);
        virtual void Reset();

        virtual WorldSafeLocsEntry const* GetClosestGraveYard(Player* player);

        void RemovePlayer(Player* plr);
        void HandleAreaTrigger(Player *Source, uint32 Trigger);
        bool SetupBattleGround();
        void EndBattleGround(Team winner);
        void EventPlayerClickedOnFlag(Player *Source, GameObject* target_obj);
        void UpdateScore(BattleGroundTeamIndex teamIdx, int32 points);
        void SpawnGates();

        /* Scorekeeping */
        void UpdatePlayerScore(Player *Source, uint32 type, uint32 value, bool doAddHonor = true);

        static BattleGroundTeamIndex GetTeamIndexByTeamId(Team team) { return team == ALLIANCE ? BG_TEAM_ALLIANCE : BG_TEAM_HORDE; }
        uint32 GetVehicleFaction(uint8 vehicleType) const { return GetCorrectFactionIC(vehicleType); }
        uint32 GetCorrectFactionIC(uint8 vehicleType) const;

    private:
        uint32 m_resource_Timer[BG_WG_MAX_RESOURCE_NODES];

        // Transports
        Transport* gunshipAlliance;
        Transport* gunshipHorde;
        Transport* CreateTransport(uint32 goEntry,uint32 period);
        void SendTransportInit(Player* player);

        void HandleBuffs();

        void _CreateBanner(uint8 node, uint8 type, uint8 teamIndex, bool delay);
};
#endif