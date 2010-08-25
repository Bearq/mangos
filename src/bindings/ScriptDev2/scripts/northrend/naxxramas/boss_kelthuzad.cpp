/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_KelThuzad
SD%Complete: 75
SDComment: Timers will need adjustments, along with tweaking positions and amounts
SDCategory: Naxxramas
EndScriptData */

// some not answered questions:
// - will intro mobs, not sent to center, despawn when phase 2 start?
// - what happens if raid fail, can they start the event as soon after as they want?

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    //when shappiron dies. dialog between kel and lich king (in this order)
    SAY_SAPP_DIALOG1                    = -1533084,
    SAY_SAPP_DIALOG2_LICH               = -1533085,
    SAY_SAPP_DIALOG3                    = -1533086,
    SAY_SAPP_DIALOG4_LICH               = -1533087,
    SAY_SAPP_DIALOG5                    = -1533088,

    //when cat dies
    SAY_CAT_DIED                        = -1533089,

    SAY_SUMMON_MINIONS                  = -1533105,         //start of phase 1

    EMOTE_PHASE2                        = -1533135,         //start of phase 2
    SAY_AGGRO1                          = -1533094,
    SAY_AGGRO2                          = -1533095,
    SAY_AGGRO3                          = -1533096,

    SAY_SLAY1                           = -1533097,
    SAY_SLAY2                           = -1533098,

    SAY_DEATH                           = -1533099,

    SAY_CHAIN1                          = -1533100,
    SAY_CHAIN2                          = -1533101,
    SAY_FROST_BLAST                     = -1533102,

    SAY_REQUEST_AID                     = -1533103,         //start of phase 3
    SAY_ANSWER_REQUEST                  = -1533104,         //lich king answer

    SAY_SPECIAL1_MANA_DET               = -1533106,
    SAY_SPECIAL3_MANA_DET               = -1533107,
    SAY_SPECIAL2_DISPELL                = -1533108,

    EMOTE_GUARDIAN                      = -1533134,         // at each guardian summon

    //spells to be casted
    SPELL_FROST_BOLT                    = 28478,
    SPELL_FROST_BOLT_H                  = 55802,
    SPELL_FROST_BOLT_NOVA               = 28479,
    SPELL_FROST_BOLT_NOVA_H             = 55807,

    SPELL_CHAINS_OF_KELTHUZAD           = 28408,            // 3.x, heroic only
    SPELL_CHAINS_OF_KELTHUZAD_TARGET    = 28410,

    SPELL_MANA_DETONATION               = 27819,
    SPELL_SHADOW_FISSURE                = 27810,
    SPELL_FROST_BLAST                   = 27808,

    SPELL_CHANNEL_VISUAL                = 29423,

    MAX_SOLDIER_COUNT                   = 71,
    MAX_ABOMINATION_COUNT               = 8,
    MAX_BANSHEE_COUNT                   = 8,
};

static float M_F_ANGLE = 0.2f;                              // to adjust for map rotation
static float M_F_HEIGHT = 2.0f;                             // adjust for height difference
static float M_F_RANGE = 55.0f;                             // ~ range from center of chamber to center of alcove

enum Phase
{
    PHASE_INTRO,
    PHASE_NORMAL,
    PHASE_GUARDIANS,
};

struct MANGOS_DLL_DECL boss_kelthuzadAI : public ScriptedAI
{
    boss_kelthuzadAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

        m_uiGuardiansCountMax = m_bIsRegularMode ? 2 : 4;
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiGuardiansCount;
    uint32 m_uiGuardiansCountMax;
    uint32 m_uiGuardiansTimer;
    uint32 m_uiFrostBoltTimer;
    uint32 m_uiFrostBoltNovaTimer;
    uint32 m_uiChainsTimer;
    uint32 m_uiManaDetonationTimer;
    uint32 m_uiShadowFissureTimer;
    uint32 m_uiFrostBlastTimer;

    uint32 m_uiPhase1Timer;
    uint32 m_uiSoldierTimer;
    uint32 m_uiBansheeTimer;
    uint32 m_uiAbominationTimer;
    uint8  m_uiPhase;
    uint32 m_uiSoldierCount;
    uint32 m_uiBansheeCount;
    uint32 m_uiAbominationCount;
    uint32 m_uiSummonIntroTimer;
    uint32 m_uiIntroPackCount;

    std::set<uint64> m_lIntroMobsSet;
    std::set<uint64> m_lAddsSet;

    void Reset()
    {
        m_uiFrostBoltTimer      = urand(1000, 60000);       //It won't be more than a minute without cast it
        m_uiFrostBoltNovaTimer  = 15000;                    //Cast every 15 seconds
        m_uiChainsTimer         = urand(30000, 60000);      //Cast no sooner than once every 30 seconds
        m_uiManaDetonationTimer = 20000;                    //Seems to cast about every 20 seconds
        m_uiShadowFissureTimer  = 25000;                    //25 seconds
        m_uiFrostBlastTimer     = urand(30000, 60000);      //Random time between 30-60 seconds
        m_uiGuardiansTimer      = 5000;                     //5 seconds for summoning each Guardian of Icecrown in phase 3
        m_uiGuardiansCount      = 0;
        m_uiSummonIntroTimer    = 0;
        m_uiIntroPackCount      = 0;

        m_uiPhase1Timer         = 228000;                   //Phase 1 lasts "3 minutes and 48 seconds"
        m_uiSoldierTimer        = 5000;
        m_uiBansheeTimer        = 5000;
        m_uiAbominationTimer    = 5000;
        m_uiSoldierCount        = 0;
        m_uiBansheeCount        = 0;
        m_uiAbominationCount    = 0;
        m_uiPhase               = PHASE_INTRO;

        // it may be some spell should be used instead, to control the intro phase
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (urand(0, 1))
            DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        DespawnAdds();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELTHUZAD, DONE);
    }

    void JustReachedHome()
    {
        DespawnIntroCreatures();
        DespawnAdds();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELTHUZAD, NOT_STARTED);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_KELTHUZAD) != IN_PROGRESS)
            return;

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void DespawnIntroCreatures()
    {
        if (m_pInstance)
        {
            for(std::set<uint64>::const_iterator itr = m_lIntroMobsSet.begin(); itr != m_lIntroMobsSet.end(); ++itr)
            {
                if (Creature* pCreature = m_pInstance->instance->GetCreature(*itr))
                    pCreature->ForcedDespawn();
            }
        }

        m_lIntroMobsSet.clear();
    }

    void DespawnAdds()
    {
        if (m_pInstance)
        {
            for(std::set<uint64>::const_iterator itr = m_lAddsSet.begin(); itr != m_lAddsSet.end(); ++itr)
            {
                if (Creature* pCreature = m_pInstance->instance->GetCreature(*itr))
                {
                    if (pCreature->isAlive())
                    {
                        pCreature->AI()->EnterEvadeMode();
                        pCreature->ForcedDespawn(15000);
                    }
                }
            }
        }

        m_lAddsSet.clear();
    }

    float GetLocationAngle(uint32 uiId)
    {
        switch(uiId)
        {
            case 1: return M_PI_F - M_F_ANGLE;              // south
            case 2: return M_PI_F / 2 * 3 - M_F_ANGLE;      // east
            case 3: return M_PI_F / 2 - M_F_ANGLE;          // west
            case 4: return M_PI_F / 4 - M_F_ANGLE;          // north-west
            case 5: return M_PI_F / 4 * 7 - M_F_ANGLE;      // north-east
            case 6: return M_PI_F / 4 * 5 - M_F_ANGLE;      // south-east
            case 7: return M_PI_F / 4 * 3 - M_F_ANGLE;      // south-west
        }

        return M_F_ANGLE;
    }

    void SummonIntroCreatures(uint32 packId)
    {
        if (!m_pInstance)
            return;

        float fAngle = GetLocationAngle(packId+1);

        float fX, fY, fZ;
        m_pInstance->GetChamberCenterCoords(fX, fY, fZ);

        fX += M_F_RANGE * cos(fAngle);
        fY += M_F_RANGE * sin(fAngle);
        fZ += M_F_HEIGHT;

        MaNGOS::NormalizeMapCoord(fX);
        MaNGOS::NormalizeMapCoord(fY);

        uint32 uiNpcEntry = NPC_SOUL_WEAVER;
        
        for(uint8 uiI = 0; uiI < 14; ++uiI)
        {
            if (uiI > 0)
            {
                if (uiI < 4)
                    uiNpcEntry = NPC_UNSTOPPABLE_ABOM;
                else
                    uiNpcEntry = NPC_SOLDIER_FROZEN;
            }

            float fNewX, fNewY, fNewZ;
            m_creature->GetRandomPoint(fX, fY, fZ, 12.0f, fNewX, fNewY, fNewZ);

            m_creature->SummonCreature(uiNpcEntry, fNewX, fNewY, fNewZ, fAngle + M_PI_F, TEMPSUMMON_CORPSE_DESPAWN, 5000);
        }
    }

    void SummonMob(uint32 uiType)
    {
        if (!m_pInstance)
            return;

        float fAngle = GetLocationAngle(urand(1, 7));

        float fX, fY, fZ;
        m_pInstance->GetChamberCenterCoords(fX, fY, fZ);

        fX += M_F_RANGE * cos(fAngle);
        fY += M_F_RANGE * sin(fAngle);
        fZ += M_F_HEIGHT;

        MaNGOS::NormalizeMapCoord(fX);
        MaNGOS::NormalizeMapCoord(fY);

        m_creature->SummonCreature(uiType, fX, fY, fZ, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 5000);
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
            case NPC_GUARDIAN:
            {
                DoScriptText(EMOTE_GUARDIAN, m_creature);

                m_lAddsSet.insert(pSummoned->GetGUID());
                ++m_uiGuardiansCount;

                pSummoned->SetInCombatWithZone();
                break;
            }
            case NPC_SOLDIER_FROZEN:
            case NPC_UNSTOPPABLE_ABOM:
            case NPC_SOUL_WEAVER:
            {
                if (m_uiIntroPackCount < 7)
                    m_lIntroMobsSet.insert(pSummoned->GetGUID());
                else
                {
                    m_lAddsSet.insert(pSummoned->GetGUID());

                    if(m_pInstance)
                    {
                        float fX, fY, fZ;
                        m_pInstance->GetChamberCenterCoords(fX, fY, fZ);
                        pSummoned->GetMotionMaster()->MovePoint(0, fX, fY, fZ);
                    }
                }

                break;
            }
        }
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
            case NPC_GUARDIAN:
            case NPC_SOLDIER_FROZEN:
            case NPC_UNSTOPPABLE_ABOM:
            case NPC_SOUL_WEAVER:
                m_lAddsSet.erase(pSummoned->GetGUID());
                break;
        }
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiMotionType, uint32 uiPointId)
    {
        if (uiMotionType == POINT_MOTION_TYPE && uiPointId == 0)
            pSummoned->SetInCombatWithZone();
    }

    Unit* SelectTargetWithMana()
    {
        ThreatList const& m_threatlist = m_creature->getThreatManager().getThreatList();

        if (m_threatlist.empty())
            return NULL;

        std::list<uint64> manaPositive;
        for (ThreatList::const_iterator itr = m_threatlist.begin(); itr!= m_threatlist.end(); ++itr)
        {
            if (Unit* pTemp = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid()))
            {
                //player and has mana
                if ((pTemp->GetTypeId() == TYPEID_PLAYER) && (pTemp->getPowerType() == POWER_MANA))
                    manaPositive.push_back(pTemp->GetGUID());
            }
        }

        if (!manaPositive.empty())
        {
            std::list<uint64>::iterator m_uiPlayerGUID = manaPositive.begin();
            advance(m_uiPlayerGUID, (rand()%manaPositive.size()));

            if (Player* pTemp = m_creature->GetMap()->GetPlayer(*m_uiPlayerGUID))
                return pTemp;
        }
        return NULL;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_pInstance || m_pInstance->GetData(TYPE_KELTHUZAD) != IN_PROGRESS)
            return;

        if (m_uiPhase == PHASE_INTRO)
        {
            if (m_uiIntroPackCount < 7)
            {
                if (m_uiSummonIntroTimer < uiDiff)
                {
                    if (!m_uiIntroPackCount)
                        DoScriptText(SAY_SUMMON_MINIONS, m_creature);

                    SummonIntroCreatures(m_uiIntroPackCount);
                    ++m_uiIntroPackCount;
                    m_uiSummonIntroTimer = 2000;
                }
                else
                    m_uiSummonIntroTimer -= uiDiff;
            }
            else
            {
                if (m_uiPhase1Timer < uiDiff)
                {
                    m_uiPhase = PHASE_NORMAL;
                    DespawnIntroCreatures();

                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    SetCombatMovement(true);
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());

                    DoScriptText(EMOTE_PHASE2, m_creature);

                    switch(urand(0, 2))
                    {
                        case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
                        case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
                        case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
                    };
                }
                else
                    m_uiPhase1Timer -= uiDiff;

                if (m_uiSoldierCount < MAX_SOLDIER_COUNT)
                {
                    if (m_uiSoldierTimer < uiDiff)
                    {
                        SummonMob(NPC_SOLDIER_FROZEN);
                        ++m_uiSoldierCount;
                        m_uiSoldierTimer = 3000;
                    }
                    else
                        m_uiSoldierTimer -= uiDiff;
                }

                if (m_uiAbominationCount < MAX_ABOMINATION_COUNT)
                {
                    if (m_uiAbominationTimer < uiDiff)
                    {
                        SummonMob(NPC_UNSTOPPABLE_ABOM);
                        ++m_uiAbominationCount;
                        m_uiAbominationTimer = 25000;
                    }
                    else
                        m_uiAbominationTimer -= uiDiff;
                }

                if (m_uiBansheeCount < MAX_BANSHEE_COUNT)
                {
                    if (m_uiBansheeTimer < uiDiff)
                    {
                        SummonMob(NPC_SOUL_WEAVER);
                        ++m_uiBansheeCount;
                        m_uiBansheeTimer = 25000;
                    }
                    else
                        m_uiBansheeTimer -= uiDiff;
                }
            }
        }
        else // normal or guardian phase
        {
            if (m_uiFrostBoltTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FROST_BOLT : SPELL_FROST_BOLT_H) == CAST_OK)
                    m_uiFrostBoltTimer = urand(1000, 60000);
            }
            else
                m_uiFrostBoltTimer -= uiDiff;

            if (m_uiFrostBoltNovaTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FROST_BOLT_NOVA : SPELL_FROST_BOLT_NOVA_H) == CAST_OK)
                    m_uiFrostBoltNovaTimer = 15000;
            }
            else
                m_uiFrostBoltNovaTimer -= uiDiff;

            if (m_uiManaDetonationTimer < uiDiff)
            {
                if (Unit* pTarget = SelectTargetWithMana())
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_MANA_DETONATION) == CAST_OK)
                    {
                        if (urand(0, 1))
                            DoScriptText(SAY_SPECIAL1_MANA_DET, m_creature);

                        m_uiManaDetonationTimer = 20000;
                    }
                }
            }
            else
                m_uiManaDetonationTimer -= uiDiff;

            if (m_uiShadowFissureTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_SHADOW_FISSURE) == CAST_OK)
                    {
                        if (urand(0, 1))
                            DoScriptText(SAY_SPECIAL3_MANA_DET, m_creature);

                        m_uiShadowFissureTimer = 25000;
                    }
                }
            }
            else
                m_uiShadowFissureTimer -= uiDiff;

            if (m_uiFrostBlastTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, (m_bIsRegularMode ? 1 : 0)))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_FROST_BLAST) == CAST_OK)
                    {
                        if (urand(0, 1))
                            DoScriptText(SAY_FROST_BLAST, m_creature);

                        m_uiFrostBlastTimer = urand(30000, 60000);
                    }
                }
            }
            else
                m_uiFrostBlastTimer -= uiDiff;

            if (!m_bIsRegularMode)
            {
                if (m_uiChainsTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CHAINS_OF_KELTHUZAD) == CAST_OK)
                    {
                        DoScriptText(urand(0, 1) ? SAY_CHAIN1 : SAY_CHAIN2, m_creature);

                        m_uiChainsTimer = urand(30000, 60000);
                    }
                }
                else
                    m_uiChainsTimer -= uiDiff;
            }

            if (m_uiPhase == PHASE_NORMAL)
            {
                if (m_creature->GetHealthPercent() < 45.0f)
                {
                    m_uiPhase = PHASE_GUARDIANS;
                    DoScriptText(SAY_REQUEST_AID, m_creature);

                    // here Lich King should respond to Kel'Thuzad but I don't know which creature to make talk
                    // so for now just make Kel'Thuzad says it.
                    DoScriptText(SAY_ANSWER_REQUEST, m_creature);
                }
            }
            else if (m_uiPhase == PHASE_GUARDIANS && m_uiGuardiansCount < m_uiGuardiansCountMax)
            {
                if (m_uiGuardiansTimer < uiDiff)
                {
                    // Summon a Guardian of Icecrown in a random alcove
                    SummonMob(NPC_GUARDIAN);
                    m_uiGuardiansTimer = 5000;
                }
                else
                    m_uiGuardiansTimer -= uiDiff;
            }

            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_boss_kelthuzad(Creature* pCreature)
{
    return new boss_kelthuzadAI(pCreature);
}

void AddSC_boss_kelthuzad()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_kelthuzad";
    NewScript->GetAI = &GetAI_boss_kelthuzad;
    NewScript->RegisterSelf();
}