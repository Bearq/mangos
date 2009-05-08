/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Tirisfal_Glades
SD%Complete: 100
SDComment: Quest support: 590, 1819
SDCategory: Tirisfal Glades
EndScriptData */

/* ContentData
go_mausoleum_door
go_mausoleum_trigger
npc_calvin_montague
EndContentData */

#include "precompiled.h"

/*######
## go_mausoleum_door
## go_mausoleum_trigger
######*/

#define QUEST_ULAG      1819
#define C_ULAG          6390
#define GO_TRIGGER      104593
#define GO_DOOR         176594

GameObject* SearchMausoleumGo(Unit *source, uint32 entry, float range)
{
    GameObject* pGo = NULL;

    CellPair pair(MaNGOS::ComputeCellPair(source->GetPositionX(), source->GetPositionY()));
    Cell cell(pair);
    cell.data.Part.reserved = ALL_DISTRICT;
    cell.SetNoCreate();

    MaNGOS::NearestGameObjectEntryInObjectRangeCheck go_check(*source, entry, range);
    MaNGOS::GameObjectLastSearcher<MaNGOS::NearestGameObjectEntryInObjectRangeCheck> searcher(source, pGo, go_check);

    TypeContainerVisitor<MaNGOS::GameObjectLastSearcher<MaNGOS::NearestGameObjectEntryInObjectRangeCheck>, GridTypeMapContainer> go_searcher(searcher);

    CellLock<GridReadGuard> cell_lock(cell, pair);
    cell_lock->Visit(cell_lock, go_searcher,*(source->GetMap()));

    return pGo;
}

bool GOHello_go_mausoleum_door(Player* pPlayer, GameObject* _GO)
{
    if (pPlayer->GetQuestStatus(QUEST_ULAG) != QUEST_STATUS_INCOMPLETE)
        return false;

    if (GameObject *trigger = SearchMausoleumGo(pPlayer, GO_TRIGGER, 30))
    {
        trigger->SetGoState(GO_STATE_READY);
        pPlayer->SummonCreature(C_ULAG, 2390.26, 336.47, 40.01, 2.26, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
        return false;
    }

    return false;
}

bool GOHello_go_mausoleum_trigger(Player* pPlayer, GameObject* _GO)
{
    if (pPlayer->GetQuestStatus(QUEST_ULAG) != QUEST_STATUS_INCOMPLETE)
        return false;

    if (GameObject *door = SearchMausoleumGo(pPlayer, GO_DOOR, 30))
    {
        _GO->SetGoState(GO_STATE_ACTIVE);
        door->RemoveFlag(GAMEOBJECT_FLAGS,GO_FLAG_INTERACT_COND);
        return true;
    }

    return false;
}

/*######
## npc_calvin_montague
######*/

#define QUEST_590           590
#define FACTION_FRIENDLY    68
#define FACTION_HOSTILE     16

struct MANGOS_DLL_DECL npc_calvin_montagueAI : public ScriptedAI
{
    npc_calvin_montagueAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    void Reset()
    {
        m_creature->setFaction(FACTION_FRIENDLY);
    }

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
            if (((Player*)Killer)->GetQuestStatus(QUEST_590) == QUEST_STATUS_INCOMPLETE)
                ((Player*)Killer)->AreaExploredOrEventHappens(QUEST_590);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_calvin_montague(Creature* pCreature)
{
    return new npc_calvin_montagueAI(pCreature);
}

bool QuestAccept_npc_calvin_montague(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_590)
    {
        pCreature->setFaction(FACTION_HOSTILE);
        ((npc_calvin_montagueAI*)pCreature->AI())->AttackStart(pPlayer);
    }
    return true;
}

void AddSC_tirisfal_glades()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "go_mausoleum_door";
    newscript->pGOHello = &GOHello_go_mausoleum_door;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_mausoleum_trigger";
    newscript->pGOHello = &GOHello_go_mausoleum_trigger;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_calvin_montague";
    newscript->GetAI = &GetAI_npc_calvin_montague;
    newscript->pQuestAccept = &QuestAccept_npc_calvin_montague;
    newscript->RegisterSelf();
}
