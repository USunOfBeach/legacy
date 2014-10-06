#ifndef _LEGACYDEFINES_H_
#define _LEGACYDEFINES_H_

#include "SharedDefines.h"
#include "Creature.h"

/************************************************************************/
/*                           Enable System                              */
/************************************************************************/

enum LegacyEnableType
{
    LEGACY_ENABLE_TYPE_CREATURE_LOOT    = 1,
    LEGACY_ENABLE_TYPE_FISHING_LOOT     = 2,
    LEGACY_ENABLE_TYPE_QUEST            = 3,
};

struct LegacyEnableInfo
{
    uint32 entry;
    bool enabled;
};

typedef std::map<LegacyEnableType, LegacyEnableInfo> LegacyEnableContainer;

/************************************************************************/
/*                           Event System                               */
/************************************************************************/ 

enum LegacyEventInvokerType // LEIT
{
    LEIT_UNDEFINED = -1,
    LEIT_STARTER = 0, // not for use
    LEIT_PLAYER = 1,
    LEIT_CREATURE = 2
};

struct LegacyEvent
{
    int32 stage;
    uint32 misc;
    LegacyEventInvokerType invokerType;
    uint32 invoker;
    uint32 date;
};

struct LegacyEventsCreatureState
{
    uint32 spawnInfo;
    bool isDead;
    uint32 phase;
    uint32 model;
    uint32 gossip;
    uint32 questInfo;
    uint32 vendorInfo;
};

struct LegacyEventsCreatureSpawnInfo
{
    bool isSpawn;
    int32 map;
    float x;
    float y;
    float z;
    float o;
};

struct LegacyEventsForCreature
{
    uint32 event;
    int32 eventState;
    uint32 creatureEntry;
    uint32 creatureGuid;
    int32 creatureState;
};

struct LegacyEventsInfo
{
    uint32 entry;
    int32 id;
};

struct LegacyEventsVendorInfo
{
    int32 item;
    uint32 maxCount;
    uint32 incrTime;
    uint32 extendedConst;
};

typedef std::unordered_map<uint32, std::string> LegacyEventsDefine;
typedef std::map<uint32, LegacyEvent> LegacyEvents;
typedef std::unordered_map<uint32, LegacyEventsCreatureState> LegacyEventsCreatureStateDefine;
typedef std::unordered_map<uint32, LegacyEventsCreatureSpawnInfo> LegacyEventsSpawnInfos;
typedef std::vector<LegacyEventsForCreature> LegacyEventsForCreatureList;
typedef std::multimap<uint32, int32> LegacyEventsInfoList;
typedef std::multimap<uint32, LegacyEventsVendorInfo> LegacyEventsVendorInfoList;
typedef std::unordered_map<uint32, uint32> LegacyGossips;

/************************************************************************/
/*                        LegacyScript System                           */
/************************************************************************/

enum LegacyTrigger
{
    LEGACY_TRIGGER_NONE,
    LEGACY_TRIGGER_RESPAWN,
    LEGACY_TRIGGER_REACHED_HOME,
    LEGACY_TRIGGER_ENTER_COMBAT,
    LEGACY_TRIGGER_ENTER_EVADE_MODE,
    LEGACY_TRIGGER_DEAD,
    LEGACY_TRIGGER_KILLED,
    LEGACY_TRIGGER_SUMMONED_CREATURE,
    LEGACY_TRIGGER_ATTACK_START,
    LEGACY_TRIGGER_MOVE_IN_LINE_OF_SIGHT,
    LEGACY_TRIGGER_SPELL_HIT,
    LEGACY_TRIGGER_SPELL_HIT_TARGET,
    LEGACY_TRIGGER_TAKEN_DAMAGE,
    LEGACY_TRIGGER_RECEIVE_HEAL,
    LEGACY_TRIGGER_UPDATE,
    LEGACY_TRIGGER_RECEIVE_EMOTE,
    LEGACY_TRIGGER_MOVE_IN_FORM,
    LEGACY_TRIGGER_SUMMONED_BY,
    LEGACY_TRIGGER_DEAL_DAMAGE,
    LEGACY_TRIGGER_SUMMONED_CREATURE_DESPAWNED,
    LEGACY_TRIGGER_CORPSE_REMOVED,
    LEGACY_TRIGGER_UPDATE_CHARMED,
    LEGACY_TRIGGER_PASSENGER_BOARDED,
    LEGACY_TRIGGER_AI_INITIALIZED,
    LEGACY_TRIGGER_CHARMED,
    LEGACY_TRIGGER_GOSSIP_HELLO,
    LEGACY_TRIGGER_GOSSIP_SELECT,
    LEGACY_TRIGGER_GOSSIP_SELECT_CODE,
    LEGACY_TRIGGER_QUEST_ACCEPTED,
    LEGACY_TRIGGER_DUMMY_EFFECT,
    LEGACY_TRIGGER_GAME_EVENT,
    LEGACY_TRIGGER_SPELL_CLICK,
    LEGACY_TRIGGER_UPDATE_IN_COMBAT,
    LEGACY_TRIGGER_MAX
};

enum LegacyTriggerAction
{
    LEGACY_TRIGGER_ACTION_NONE,
    LEGACY_TRIGGER_ACTION_TRIGGER_EVENT,
    LEGACY_TRIGGER_ACTION_MAX
};

enum LegacyAction
{
    LEGACY_ACTION_NONE,
    LEGACY_ACTION_SAY,
    LEGACY_ACTION_SET_FACTION,
    LEGACY_ACTION_SET_MODEL,
    LEGACY_ACTION_PLAY_SOUND,
    LEGACY_ACTION_EMOTE,
    LEGACY_ACTION_QUEST_FAIL,
    LEGACY_ACTION_QUEST_ADD,
    LEGACY_ACTION_SET_REACT_STATE,
    LEGACY_ACTION_ACTIVE_GO,
    LEGACY_ACTION_RANDOM_EMOTE,
    LEGACY_ACTION_CAST_SPELL,
    LEGACY_ACTION_SUMMON_CREATURE,
    LEGACY_ACTION_MOD_THREAT,
    LEGACY_ACTION_MOD_THREAT_ALL,
    LEGACY_ACTION_QUEST_EVENT_HAPPENED,
    LEGACY_ACTION_EMOTE_STATE,
    LEGACY_ACTION_SET_UNIT_FLAG,
    LEGACY_ACTION_REMOVE_UNIT_FLAG,
    LEGACY_ACTION_SET_AUTO_ATTACK,
    LEGACY_ACTION_ALLOW_COMBAT_MOVEMENT,
    LEGACY_ACTION_SET_EVENT_PHASE,
    LEGACY_ACTION_INCREASE_EVENT_PHASE,
    LEGACY_ACTION_EVADE,
    LEGACY_ACTION_FLEE_FOR_ASSIST,
    LEGACY_ACTION_QUEST_GROUP_EVENT_HAPPENED,
    LEGACY_ACTION_REMOVE_AURA_FROM_SPELL,
    LEGACY_ACTION_FOLLOW,
    LEGACY_ACTION_SET_RANDOM_PHASE,
    LEGACY_ACTION_RESET,
    LEGACY_ACTION_CALL_KILLED_MONSTER,
    LEGACY_ACTION_SET_INSTANCE_DATA,
    LEGACY_ACTION_SET_INSTANCE_DATA64,
    LEGACY_ACTION_UPDATE_TEMPLATE,
    LEGACY_ACTION_DIE,
    LEGACY_ACTION_DO_ZONE_IN_COMBAT,
    LEGACY_ACTION_CALL_FOR_HELP,
    LEGACY_ACTION_SET_SHEATH,
    LEGACY_ACTION_FORCE_DESPAWN,
    LEGACY_ACTION_SET_INVICIBILITY_LEVEL,
    LEGACY_ACTION_SET_MOUNT_MODEL,
    LEGACY_ACTION_SET_PHASE_MASK,
    LEGACY_ACTION_SET_DATA,
    LEGACY_ACTION_MOVE_FORWARD,
    LEGACY_ACTION_SET_VISIBLE,
    LEGACY_ACTION_SET_ACTIVE,
    LEGACY_ACTION_ATTACK_START,
    LEGACY_ACTION_SUMMON_GAMEOBJECT,
    LEGACY_ACTION_KILL,
    LEGACY_ACTION_ACTIVE_TAXI,
    LEGACY_ACTION_WAYPOINT_START,
    LEGACY_ACTION_WAYPOINT_PAUSE,
    LEGACY_ACTION_WAYPOINT_STOP,
    LEGACY_ACTION_ADD_ITEM,
    LEGACY_ACTION_REMOVE_ITEM,
    LEGACY_ACTION_SET_AI_TEMPLATE,
    LEGACY_ACTION_SET_RUN,
    LEGACY_ACTION_SET_FLY,
    LEGACY_ACTION_SET_SWIM,
    LEGACY_ACTION_TELEPORT,
    LEGACY_ACTION_STORE_VAR_DEC,
    LEGACY_ACTION_STORE_TARGET_LIST,
    LEGACY_ACTION_WAYPOINT_RESUME,
    LEGACY_ACTION_SET_ORIENTATION,
    LEGACY_ACTION_SET_FACE_TO,
    LEGACY_ACTION_PLAY_MOVIE,
    LEGACY_ACTION_MOVE_TO,
    LEGACY_ACTION_RESPAWN,
    LEGACY_ACTION_EQUIP,
    LEGACY_ACTION_CLOSE_GOSSIP,
    LEGACY_ACTION_ADD_AURA,
    LEGACY_ACTION_INTERRUPT_SPELL,
    LEGACY_ACTION_SET_DYNAMIC_FLAGS,
    LEGACY_ACTION_REMOVE_DYNAMIC_FLAGS,
    LEGACY_ACTION_JUMP_TO,
    LEGACY_ACTION_SEND_GOSSIP_MENU,
    LEGACY_ACTION_SET_GO_LOOT_STATE,
    LEGACY_ACTION_SET_HOME_POSITION,
    LEGACY_ACTION_SET_HEALTH_REGEN,
    LEGACY_ACTION_SET_ROOT,
    LEGACY_ACTION_SET_GO_FLAGS,
    LEGACY_ACTION_REMOVE_GO_FLAGS,
    LEGACY_ACTION_SUMMON_CREATURE_GROUP,
    LEGACY_ACTION_SET_POWER,
    LEGACY_ACTION_ADD_POWER,
    LEGACY_ACTION_REMOVE_POWER,
    LEGACY_ACTION_GAME_EVENT_START,
    LEGACY_ACTION_GAME_EVENT_STOP,
    LEGACY_ACTION_WORLD_EVENT_START,
    LEGACY_ACTION_SET_WORLD_EVENT_STAGE,
    LEGACY_ACTION_INCREASE_WORLD_EVENT_STAGE,
    LEGACY_ACTION_MORPH_TO,
    LEGACY_ACTION_TALK,
    LEGACY_ACTION_MAX
};

enum LegacyTarget
{
    LEGACY_TARGET_NONE,
    LEGACY_TARGET_SELF,
    LEGACY_TARGET_VICTIM,
    LEGACY_TARGET_MAX
};

struct LegacyActionData
{
    LegacyAction action;
    int32 p1, p2, p3, p4, p5, p6 = 0;
    LegacyTarget actor, target = LEGACY_TARGET_NONE;
    uint32 t1, t2, t3 = 0;
    float x, y, z, o = 0;
};

typedef std::vector<LegacyActionData> LegacyActionMap;

struct LegacyEventData
{
    uint32 chance = 0;
    uint32 cooldownMin = 0;
    uint32 cooldownMax = 0;
    uint32 cooldown = 0;
    std::vector<uint32> phase;
    uint32 flags = 0;
    uint32 p1, p2, p3 = 0;
    LegacyActionMap actions;

    void UpdateCooldown(uint32 diff)
    {
        cooldown = diff >= cooldown ? 0 : cooldown - diff;
    }
};

typedef std::unordered_map<uint32, LegacyEventData> LegacyEventMap;

struct LegacyTriggerData
{
    uint32 delay = 0;
    uint32 delayTimer = 0;
    bool counting = false;
    std::vector<uint32> events;
    Creature* me;
    Unit* invoker;

    bool UpdateDelayTimer(uint32 diff)
    {
        if (!counting)
            return false;

        delayTimer = diff >= delayTimer ? 0 : delayTimer - diff;

        counting = !(delayTimer == 0);
        return !counting;
    }
};

typedef std::unordered_map<LegacyTrigger, LegacyTriggerData> LegacyTriggerMap;

typedef std::unordered_map<std::string, uint32> LegacyLinker;

struct LegacyScriptData
{
    LegacyTriggerMap triggerMap;
    LegacyEventMap eventMap;

    LegacyEventData* GetEvent(uint32 id)
    {
        LegacyEventMap::iterator itr = eventMap.find(id);
        if (itr != eventMap.end())
            return &(itr->second);
        else
            return NULL;
    }
};

/************************************************************************/
/*                       SkillLine System                               */
/************************************************************************/
enum LegacySkillUpdateAction
{
    LSUA_ON_MELEE_DAMEGE,
    LSUA_ON_RANGED_DAMAGE
};

#endif // !_LEGACYDEFINES_H_
