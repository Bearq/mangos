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

#include "ConfusedMovementGenerator.h"
#include "MapManager.h"
#include "Creature.h"
#include "Player.h"
#include "movement/MoveSplineInit.h"
#include "movement/MoveSpline.h"
#include "PathFinder.h"

template<class T>
void ConfusedMovementGenerator<T>::Initialize(T &unit)
{
    const float wander_distance=11;
    float x,y,z;
    x = unit.GetPositionX();
    y = unit.GetPositionY();
    z = unit.GetPositionZ();

    TerrainInfo const* map = unit.GetTerrain();

    i_nextMove = 1;

    bool is_water_ok, is_land_ok;
    _InitSpecific(unit, is_water_ok, is_land_ok);

    for(unsigned int idx=0; idx < MAX_CONF_WAYPOINTS+1; ++idx)
    {
        const float wanderX=wander_distance*rand_norm_f() - wander_distance/2;
        const float wanderY=wander_distance*rand_norm_f() - wander_distance/2;

        i_waypoints[idx][0] = x + wanderX;
        i_waypoints[idx][1] = y + wanderY;

        // prevent invalid coordinates generation
        MaNGOS::NormalizeMapCoord(i_waypoints[idx][0]);
        MaNGOS::NormalizeMapCoord(i_waypoints[idx][1]);

        // check LOS
        if(!unit.IsWithinLOS(i_waypoints[idx][0], i_waypoints[idx][1], z))
        {
            i_waypoints[idx][0] = idx > 0 ? i_waypoints[idx-1][0] : x;
            i_waypoints[idx][1] = idx > 0 ? i_waypoints[idx-1][1] : y;
        }

        bool is_water = map->IsInWater(i_waypoints[idx][0],i_waypoints[idx][1],z);
        // if generated wrong path just ignore
        if ((is_water && !is_water_ok) || (!is_water && !is_land_ok))
        {
            i_waypoints[idx][0] = idx > 0 ? i_waypoints[idx-1][0] : x;
            i_waypoints[idx][1] = idx > 0 ? i_waypoints[idx-1][1] : y;
        }

        unit.UpdateAllowedPositionZ(i_waypoints[idx][0],i_waypoints[idx][1],z);
        i_waypoints[idx][2] =  z;
    }

    unit.StopMoving();
    unit.addUnitState(UNIT_STAT_CONFUSED|UNIT_STAT_CONFUSED_MOVE);
}

template<class T>
void ConfusedMovementGenerator<T>::Interrupt(T &unit)
{
    // confused state still applied while movegen disabled
    unit.clearUnitState(UNIT_STAT_CONFUSED_MOVE);
}

template<class T>
void ConfusedMovementGenerator<T>::Reset(T &unit)
{
    i_nextMoveTime.Reset(0);
    unit.StopMoving();
    unit.addUnitState(UNIT_STAT_CONFUSED|UNIT_STAT_CONFUSED_MOVE);
}

template<class T>
bool ConfusedMovementGenerator<T>::Update(T &unit, const uint32 &diff)
{
    // ignore in case other no reaction state
    if (unit.hasUnitState(UNIT_STAT_CAN_NOT_REACT & ~UNIT_STAT_CONFUSED))
        return true;

    if (i_nextMoveTime.Passed())
    {
        // currently moving, update location
        unit.addUnitState(UNIT_STAT_CONFUSED_MOVE);

        if (unit.movespline->Finalized())
            i_nextMoveTime.Reset(urand(800, 1500));
    }
    else
    {
        // waiting for next move
        i_nextMoveTime.Update(diff);
        if(i_nextMoveTime.Passed() )
        {
            // start moving
            unit.addUnitState(UNIT_STAT_CONFUSED_MOVE);

            float x = i_x + 10.0f*(rand_norm_f() - 0.5f);
            float y = i_y + 10.0f*(rand_norm_f() - 0.5f);
            float z = i_z;

            unit.UpdateAllowedPositionZ(x, y, z);

            PathFinder path(&unit);
            path.setPathLengthLimit(30.0f);
            path.calculate(x, y, z);
            if(!(path.getPathType() & PATHFIND_NORMAL))
            {
                i_nextMoveTime.Reset(urand(800, 1000));
                return true;
            }

            Movement::MoveSplineInit init(unit);
            init.MovebyPath(path.getPath());
            init.SetWalk(true);
            init.Launch();
        }
    }

    return true;
}

template<>
void ConfusedMovementGenerator<Player>::Finalize(Player &unit)
{
    unit.clearUnitState(UNIT_STAT_CONFUSED|UNIT_STAT_CONFUSED_MOVE);
    unit.StopMoving();
}

template<>
void ConfusedMovementGenerator<Creature>::Finalize(Creature &unit)
{
    unit.clearUnitState(UNIT_STAT_CONFUSED|UNIT_STAT_CONFUSED_MOVE);
}

template void ConfusedMovementGenerator<Player>::Initialize(Player &player);
template void ConfusedMovementGenerator<Creature>::Initialize(Creature &creature);
template void ConfusedMovementGenerator<Player>::Interrupt(Player &player);
template void ConfusedMovementGenerator<Creature>::Interrupt(Creature &creature);
template void ConfusedMovementGenerator<Player>::Reset(Player &player);
template void ConfusedMovementGenerator<Creature>::Reset(Creature &creature);
template bool ConfusedMovementGenerator<Player>::Update(Player &player, const uint32 &diff);
template bool ConfusedMovementGenerator<Creature>::Update(Creature &creature, const uint32 &diff);
