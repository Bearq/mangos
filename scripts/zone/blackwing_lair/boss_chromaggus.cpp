/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Chromaggus
SD%Complete: 95
SDComment: Chromatic Mutation disabled due to lack of core support
SDCategory: Blackwing Lair
EndScriptData */

#include "../../sc_defines.h"

//These spells are actually called elemental shield
//What they do is decrease all damage by 75% then they increase
//One school of damage by 1100%
#define SPELL_FIRE_VURNALBILTY      22277
#define SPELL_FROST_VURNALBILTY     22278
#define SPELL_SHADOW_VURNALBILTY    22279
#define SPELL_NATURE_VURNALBILTY    22280
#define SPELL_ARCANE_VURNALBILTY    22281

#define EMOTE_FRENZY    "goes into a killing frenzy!"
#define EMOTE_SHIMMER   "flinches as its skin shimmers"

//Incinerate 23308,23309
#define SPELL_INCINERATE    23308
//Time lapse 23310, 23311(old threat mod that was removed in 2.01)
#define SPELL_TIMELAPSE     23310
//Corrosive Acid 23313, 23314
#define SPELL_CORROSIVEACID 23313
//Ignite Flesh 23315,23316
#define SPELL_IGNITEFLESH   23315
//Frost burn 23187, 23189
#define SPELL_FROSTBURN     23187

//Brood Affliction 23173 - Scripted Spell that cycles through all targets within 100 yards and has a chance to cast one of the afflictions on them
//Since Scripted spells arn't coded I'll just write a function that does the same thing

//Blue affliction 23153
#define SPELL_BROODAF_BLUE      23153
//Black affliction 23154
#define SPELL_BROODAF_BLACK     23154
//Red affliction 23155 (23168 on death)
#define SPELL_BROODAF_RED       23155
//Bronze Affliction  23170
#define SPELL_BROODAF_BRONZE    23170
//Brood Affliction Green 23169
#define SPELL_BROODAF_GREEN     23169

//Spell cast on player if they get all 5 debuffs
#define SPELL_CHROMATIC_MUT_1   23174

//The frenzy spell may be wrong
#define SPELL_FRENZY            19812
#define SPELL_ENRAGE            28747


struct MANGOS_DLL_DECL boss_chromaggusAI : public ScriptedAI
{
    boss_chromaggusAI(Creature *c) : ScriptedAI(c) 
    {
        //Select the 2 breaths that we are going to use until despawned
        //5 possiblities for the first breath, 4 for the second, 20 total possiblites
        //This way we don't end up casting 2 of the same breath
        //TL TL would be stupid
        srand(time(NULL));
        switch (rand()%20)
        {
            //B1 - Incin
        case 0:
            Breath1_Spell = SPELL_INCINERATE;
            Breath2_Spell = SPELL_TIMELAPSE;
            break;

        case 1:
            Breath1_Spell = SPELL_INCINERATE;
            Breath2_Spell = SPELL_CORROSIVEACID;
            break;

        case 2:
            Breath1_Spell = SPELL_INCINERATE;
            Breath2_Spell = SPELL_IGNITEFLESH;
            break;

        case 3:
            Breath1_Spell = SPELL_INCINERATE;
            Breath2_Spell = SPELL_FROSTBURN;
            break;

            //B1 - TL
        case 4:
            Breath1_Spell = SPELL_TIMELAPSE;
            Breath2_Spell = SPELL_INCINERATE;
            break;

        case 5:
            Breath1_Spell = SPELL_TIMELAPSE;
            Breath2_Spell = SPELL_CORROSIVEACID;
            break;

        case 6:
            Breath1_Spell = SPELL_TIMELAPSE;
            Breath2_Spell = SPELL_IGNITEFLESH;
            break;

        case 7:
            Breath1_Spell = SPELL_TIMELAPSE;
            Breath2_Spell = SPELL_FROSTBURN;
            break;

            //B1 - Acid
        case 8:
            Breath1_Spell = SPELL_CORROSIVEACID;
            Breath2_Spell = SPELL_INCINERATE;
            break;

        case 9:
            Breath1_Spell = SPELL_CORROSIVEACID;
            Breath2_Spell = SPELL_TIMELAPSE;
            break;

        case 10:
            Breath1_Spell = SPELL_CORROSIVEACID;
            Breath2_Spell = SPELL_IGNITEFLESH;
            break;

        case 11:
            Breath1_Spell = SPELL_CORROSIVEACID;
            Breath2_Spell = SPELL_FROSTBURN;
            break;

            //B1 - Ignite
        case 12:
            Breath1_Spell = SPELL_IGNITEFLESH;
            Breath2_Spell = SPELL_INCINERATE;
            break;

        case 13:
            Breath1_Spell = SPELL_IGNITEFLESH;
            Breath2_Spell = SPELL_CORROSIVEACID;
            break;

        case 14:
            Breath1_Spell = SPELL_IGNITEFLESH;
            Breath2_Spell = SPELL_TIMELAPSE;
            break;

        case 15:
            Breath1_Spell = SPELL_IGNITEFLESH;
            Breath2_Spell = SPELL_FROSTBURN;
            break;

            //B1 - Frost
        case 16:
            Breath1_Spell = SPELL_FROSTBURN;
            Breath2_Spell = SPELL_INCINERATE;
            break;

        case 17:
            Breath1_Spell = SPELL_FROSTBURN;
            Breath2_Spell = SPELL_TIMELAPSE;
            break;

        case 18:
            Breath1_Spell = SPELL_FROSTBURN;
            Breath2_Spell = SPELL_CORROSIVEACID;
            break;

        case 19:
            Breath1_Spell = SPELL_FROSTBURN;
            Breath2_Spell = SPELL_IGNITEFLESH;
            break;
        };

        EnterEvadeMode();
    }

    uint32 Breath1_Spell;
    uint32 Breath2_Spell;
    uint32 CurrentVurln_Spell;

    uint32 Shimmer_Timer;
    uint32 Breath1_Timer;
    uint32 Breath2_Timer;
    uint32 Affliction_Timer;
    uint32 Frenzy_Timer;
    bool Enraged;
    bool InCombat;

    void EnterEvadeMode()
    {
        CurrentVurln_Spell = 0;     //We use this to store our last vurlnability spell so we can remove it later

        Shimmer_Timer = 0;          //Time till we change vurlnerabilites
        Breath1_Timer = 30000;      //First breath is 30 seconds
        Breath2_Timer = 60000;      //Second is 1 minute so that we can alternate
        Affliction_Timer = 10000;   //This is special - 5 seconds means that we cast this on 1 player every 5 sconds
        Frenzy_Timer = 15000;

        Enraged = false;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);

            InCombat = true;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                DoStartMeleeAttack(who);

                InCombat = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        //Shimmer_Timer Timer
        if (Shimmer_Timer < diff)
        {
            //Remove old vurlnability spell
            if (CurrentVurln_Spell)
                m_creature->RemoveAurasDueToSpell(CurrentVurln_Spell);

            //Cast new random vurlnabilty on self
            uint32 spell;
            switch (rand()%5)
            {
            case 0:
                spell = SPELL_FIRE_VURNALBILTY;
                break;

            case 1:
                spell = SPELL_FROST_VURNALBILTY;
                break;

            case 2:
                spell = SPELL_SHADOW_VURNALBILTY;
                break;

            case 3:
                spell = SPELL_NATURE_VURNALBILTY;
                break;

            case 4:
                spell = SPELL_ARCANE_VURNALBILTY;
                break;
            }

            DoCast(m_creature,spell);
            CurrentVurln_Spell = spell;

            DoTextEmote(EMOTE_SHIMMER, NULL);

            //45 seconds until we should cast this agian
            Shimmer_Timer = 45000;
        }else Shimmer_Timer -= diff;

        //Breath1_Timer
        if (Breath1_Timer < diff)
        {
            //Cast Breath1_Spell
            DoCast(m_creature->getVictim(),Breath1_Spell);

            //30 seconds until we should cast this agian
            Breath1_Timer = 60000;
        }else Breath1_Timer -= diff;

        //Breath2_Timer
        if (Breath2_Timer < diff)
        {
            //Cast Breath2_Spell
            DoCast(m_creature->getVictim(),Breath2_Spell);

            //30 seconds until we should cast this agian
            Breath2_Timer = 60000;
        }else Breath2_Timer -= diff;

        //Affliction_Timer
        if (Affliction_Timer < diff)
        {
            //Pick a random target then cast a random debuff
            //Currently there is no ability to select a random target
            //So we will just leave this code out
            //DoCast(m_creature->getVictim(),SPELL_SHADOWFLAME);
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);

            if (target)
            {
                switch (rand()%5)
                {
                case 0:
                    DoCast(target,SPELL_BROODAF_BLUE);
                    break;

                case 1:
                    DoCast(target,SPELL_BROODAF_BLACK);
                    break;

                case 2:
                    DoCast(target,SPELL_BROODAF_RED);
                    break;

                case 3:
                    DoCast(target,SPELL_BROODAF_BRONZE);
                    break;

                case 4:
                    DoCast(target,SPELL_BROODAF_GREEN);
                    break;
                }

                //Chromatic mutation if target is effected by all afflictions
                if (target->HasAura(SPELL_BROODAF_BLUE,0) 
                    && target->HasAura(SPELL_BROODAF_BLACK,0) 
                    && target->HasAura(SPELL_BROODAF_RED,0) 
                    && target->HasAura(SPELL_BROODAF_BRONZE,0) 
                    && target->HasAura(SPELL_BROODAF_GREEN,0))
                {
                    //target->RemoveAllAuras();

                    //Chromatic mutation is causing issues
                    //Assuming it is caused by a lack of core support for Charm
                    //So instead we instant kill our target
                    //DoCast(target,SPELL_CHROMATIC_MUT_1);
                    //target->DealDamage(target, target->GetHealth(), DIRECT_DAMAGE, 0, NULL, 0, false);
                }

            }

            //10 seconds until we should cast this agian
            Affliction_Timer = 10000;
        }else Affliction_Timer -= diff;

        //Frenzy_Timer
        if (Frenzy_Timer < diff)
        {
            //Cast Frenzy_
            DoCast(m_creature,SPELL_FRENZY);
            DoTextEmote(EMOTE_FRENZY,NULL);

            //10-15 seconds until we should cast this agian
            Frenzy_Timer = 10000 + (rand() % 5000);
        }else Frenzy_Timer -= diff;

        //Enrage if not already enraged and below 20%
        if (!Enraged && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 20)
        {
            DoCast(m_creature,SPELL_ENRAGE);
            Enraged = true;
        }

        DoMeleeAttackIfReady();
    }

};
CreatureAI* GetAI_boss_chromaggus(Creature *_Creature)
{
    return new boss_chromaggusAI (_Creature);
}


void AddSC_boss_chromaggus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_chromaggus";
    newscript->GetAI = GetAI_boss_chromaggus;
    m_scripts[nrscripts++] = newscript;
}
