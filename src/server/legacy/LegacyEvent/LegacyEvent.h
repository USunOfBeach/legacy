/*
* Copyright (C) 2012-2014 ProjectLegacy <http://msacn.org/>
* Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
*/

#ifndef _LEGACYEVENTMGR_H
#define _LEGACYEVENTMGR_H

#include "LegacyDefines.h"

typedef std::unordered_map<uint32, uint32> LegacyWorldStates;

class LegacyEventMgr
{
public:
    static LegacyEventMgr* instance()
    {
        static LegacyEventMgr instance;
        return &instance;
    }

    // Note: All events are pre-defined in world database, this class (or the game design itself) won't generate any NEW events.
    void LoadEvents();
    LegacyEvent const* GetEvent(uint32 id);
    void SetEventStage(uint32 id, int32 stage);
    void SaveEventStage(uint32 id, int32 state);
    int32 GetEventStage(uint32 id);
    uint32 GetEventStageMisc(uint32 id);
    bool IsEventValid(uint32 id);
    void ExecuteEventsOnStage(uint32 id, int32 stage);
    void ExecuteEventsForCreature(uint32 entry, uint32 guid, uint32 creatureState);
    LegacyEventsCreatureState const* GetEventCreatureState(uint32 id);
    LegacyEventsCreatureSpawnInfo const* GetSpawnInfo(uint32 id);
    void RollHistoryEvents();
    void RollHistoryEventsLite(); // lite version, for faster & safer loading. unfinished.
    void UpdateQuestRelations(uint32 creatureEntry, uint32 quests);
    void UpdateVentorInfos(uint32 creatureEntry, uint32 items);
    uint32 GetGossipMenuId(Creature* c);
    uint32 GetLastGossipMenuForCreature(uint32 entry);

    // WorldState
    void BroadcastWorldState(Map* map, uint32 field, uint32 data);
    void FillZoneStatePocket(WorldPacket* data, uint32 zone_id);

private:
    LegacyEventMgr();
    ~LegacyEventMgr();
    LegacyEvents _legacyEvents;
    LegacyEventsDefine _legacyEventsDefine;
    LegacyEventsForCreatureList _legacyEventsForCreature;
    LegacyEventsCreatureStateDefine _legacyEventsCreatureStateDefine;
    LegacyEventsInfoList _legacyEventsQuestInfo;
    LegacyEventsVendorInfoList _legacyEventsVendorInfo;
    LegacyEventsSpawnInfos _legacyEventsSpawnInfo;
    LegacyGossips _legacyGossips;
};

#define xEventMgr LegacyEventMgr::instance()

#endif
