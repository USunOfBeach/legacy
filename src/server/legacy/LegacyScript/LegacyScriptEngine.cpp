#include "LegacyScriptEngine.h"
#include "CreatureAI.h"
#include "Player.h"
#include "ObjectMgr.h"

void LegacyScriptEngine::UpdateAI(LegacyScriptData &data, uint32 diff)
{
    for (LegacyTriggerMap::iterator itr = data.triggerMap.begin(); itr != data.triggerMap.end(); ++itr)
    {
        if (itr->second.UpdateDelayTimer(diff))
        {
            for (int i = 0; i != itr->second.events.size(); ++i)
                ExecuteEvent(*(data.GetEvent(itr->second.events[i])), itr->second.me, itr->second.invoker);
        }
    }   

    for (LegacyEventMap::iterator itr = data.eventMap.begin(); itr != data.eventMap.end(); ++itr)
        itr->second.UpdateCooldown(diff);
}

void LegacyScriptEngine::ExecuteOn(LegacyTrigger trigger, LegacyScriptData &data, Unit* self /*= NULL*/, Unit* victim /* = NULL */)
{
    LegacyTriggerMap::iterator itr = data.triggerMap.find(trigger);
    if (itr != data.triggerMap.end())
    {
        if (!itr->second.counting && itr->second.delay != 0)
        {
            itr->second.delayTimer = itr->second.delay;
            itr->second.counting = true;
        }
        else
        {
            for (std::vector<uint32>::iterator itr_e = itr->second.events.begin(); itr_e != itr->second.events.end(); ++itr_e)
                ExecuteEvent(*(data.GetEvent(*itr_e)), self, victim);
        }
    }
}

void LegacyScriptEngine::ExecuteEvent(LegacyEventData &data, Unit* self, Unit* victim /* = NULL */)
{
    //if (!self)
    //    return;

    //// check cooldown
    //if (data.cooldown > 0)
    //    return;

    //Creature* me = self->ToCreature();
    //Unit* target = victim;
    //Player* p = NULL;
    //if (victim)
    //{
    //    p = victim->ToPlayer();
    //}
    ////GameObject* g;

    //// set cooldown
    //if (data.cooldownMax)
    //    data.cooldown = urand(data.cooldownMin, data.cooldownMax);

    //// process actions
    //for (LegacyActionMap::const_iterator itr = data.actions.begin(); itr != data.actions.end(); ++itr)
    //{
    //    // should i do these stuff here?
    //    switch (itr->action)
    //    {
    //        case LEGACY_ACTION_SAY:
    //            // check target define
    //            if (itr->actor == LEGACY_TARGET_SELF)
    //            {
    //                if (me)
    //                    me->AI()->Talk(itr->p1, victim);
    //            }
    //            break;
    //        case LEGACY_ACTION_SET_FACTION:
    //            if (itr->actor == LEGACY_TARGET_SELF)
    //                me->setFaction(itr->p1);
    //            else if (itr->actor == LEGACY_TARGET_VICTIM)
    //                target->setFaction(itr->p1);
    //            break;
    //        case LEGACY_ACTION_SET_MODEL:
    //            if (itr->actor == LEGACY_TARGET_SELF)
    //                me->SetDisplayId(itr->p1);
    //            else if (itr->actor == LEGACY_TARGET_VICTIM)
    //                target->SetDisplayId(itr->p1);
    //            break;
    //        case LEGACY_ACTION_PLAY_SOUND:
    //            break;
    //        case LEGACY_ACTION_EMOTE:
    //            if (itr->actor == LEGACY_TARGET_SELF)
    //                me->HandleEmoteCommand(itr->p1);
    //            else if (itr->target == LEGACY_TARGET_VICTIM)
    //                target->HandleEmoteCommand(itr->p1);
    //            break;
    //        case LEGACY_ACTION_QUEST_FAIL:
    //            if (itr->actor == LEGACY_TARGET_VICTIM && p)
    //                p->FailQuest(itr->p1);
    //            break;
    //        case LEGACY_ACTION_QUEST_ADD:
    //            if (itr->actor == LEGACY_TARGET_VICTIM && p)
    //            {
    //                if (const Quest* q = sObjectMgr->GetQuestTemplate(itr->p1))
    //                    p->AddQuest(q, me);
    //            }
    //            break;
    //        case LEGACY_ACTION_SET_REACT_STATE:
    //            if (itr->actor == LEGACY_TARGET_SELF)
    //                me->SetReactState((ReactStates)itr->p1);
    //            break;
    //        case LEGACY_ACTION_ACTIVE_GO:
    //            if (GameObject* g = sObjectAccessor->GetGameObject(*me, itr->p1))
    //                g->setActive(true);
    //            break;
    //        case LEGACY_ACTION_RANDOM_EMOTE:
    //            break;
    //        case LEGACY_ACTION_CAST_SPELL:
    //            if (itr->actor == LEGACY_TARGET_SELF)
    //            {
    //                if (me)
    //                {
    //                    if (itr->target == LEGACY_TARGET_SELF)
    //                        me->CastSpell(me, itr->p1, false);
    //                    else if (itr->target == LEGACY_TARGET_VICTIM)
    //                    {
    //                        if (victim)
    //                            me->CastSpell(victim, itr->p1, false);
    //                    }
    //                }
    //            }
    //            else if (itr->actor == LEGACY_TARGET_VICTIM)
    //            {
    //                if (victim)
    //                {
    //                    if (itr->target == LEGACY_TARGET_SELF)
    //                    {
    //                        if (me)
    //                            victim->CastSpell(me, itr->p1, false);
    //                    }
    //                    else if (itr->target == LEGACY_TARGET_VICTIM)
    //                        victim->CastSpell(victim, itr->p1, false);
    //                }
    //            }
    //            break;
    //        case LEGACY_ACTION_SUMMON_CREATURE:
    //            me->SummonCreature(itr->p1, me->GetPosition());
    //            break;
    //        case LEGACY_ACTION_MOD_THREAT:
    //            break;
    //        case LEGACY_ACTION_MOD_THREAT_ALL:
    //            break;
    //        case LEGACY_ACTION_QUEST_EVENT_HAPPENED:
    //            break;
    //        case LEGACY_ACTION_EMOTE_STATE:
    //            break;
    //        case LEGACY_ACTION_SET_UNIT_FLAG:
    //            break;
    //        case LEGACY_ACTION_REMOVE_UNIT_FLAG:
    //            break;
    //        case LEGACY_ACTION_SET_AUTO_ATTACK:
    //            break;
    //        case LEGACY_ACTION_ALLOW_COMBAT_MOVEMENT:
    //            break;
    //        case LEGACY_ACTION_SET_EVENT_PHASE:
    //            break;
    //        case LEGACY_ACTION_INCREASE_EVENT_PHASE:
    //            break;
    //        case LEGACY_ACTION_EVADE:
    //            break;
    //        case LEGACY_ACTION_FLEE_FOR_ASSIST:
    //            break;
    //        case LEGACY_ACTION_QUEST_GROUP_EVENT_HAPPENED:
    //            break;
    //        case LEGACY_ACTION_REMOVE_AURA_FROM_SPELL:
    //            break;
    //        case LEGACY_ACTION_FOLLOW:
    //            break;
    //        case LEGACY_ACTION_SET_RANDOM_PHASE:
    //            break;
    //        case LEGACY_ACTION_RESET:
    //            break;
    //        case LEGACY_ACTION_CALL_KILLED_MONSTER:
    //            break;
    //        case LEGACY_ACTION_SET_INSTANCE_DATA:
    //            break;
    //        case LEGACY_ACTION_SET_INSTANCE_DATA64:
    //            break;
    //        case LEGACY_ACTION_UPDATE_TEMPLATE:
    //            break;
    //        case LEGACY_ACTION_DIE:
    //            break;
    //        case LEGACY_ACTION_DO_ZONE_IN_COMBAT:
    //            break;
    //        case LEGACY_ACTION_CALL_FOR_HELP:
    //            break;
    //        case LEGACY_ACTION_SET_SHEATH:
    //            break;
    //        case LEGACY_ACTION_FORCE_DESPAWN:
    //            break;
    //        case LEGACY_ACTION_SET_INVICIBILITY_LEVEL:
    //            break;
    //        case LEGACY_ACTION_SET_MOUNT_MODEL:
    //            break;
    //        case LEGACY_ACTION_SET_PHASE_MASK:
    //            break;
    //        case LEGACY_ACTION_SET_DATA:
    //            break;
    //        case LEGACY_ACTION_MOVE_FORWARD:
    //            break;
    //        case LEGACY_ACTION_SET_VISIBLE:
    //            break;
    //        case LEGACY_ACTION_SET_ACTIVE:
    //            break;
    //        case LEGACY_ACTION_ATTACK_START:
    //            break;
    //        case LEGACY_ACTION_SUMMON_GAMEOBJECT:
    //            break;
    //        case LEGACY_ACTION_KILL:
    //            break;
    //        case LEGACY_ACTION_ACTIVE_TAXI:
    //            break;
    //        case LEGACY_ACTION_WAYPOINT_START:
    //            break;
    //        case LEGACY_ACTION_WAYPOINT_PAUSE:
    //            break;
    //        case LEGACY_ACTION_WAYPOINT_STOP:
    //            break;
    //        case LEGACY_ACTION_ADD_ITEM:
    //            break;
    //        case LEGACY_ACTION_REMOVE_ITEM:
    //            break;
    //        case LEGACY_ACTION_SET_AI_TEMPLATE:
    //            break;
    //        case LEGACY_ACTION_SET_RUN:
    //            break;
    //        case LEGACY_ACTION_SET_FLY:
    //            break;
    //        case LEGACY_ACTION_SET_SWIM:
    //            break;
    //        case LEGACY_ACTION_TELEPORT:
    //            break;
    //        case LEGACY_ACTION_STORE_VAR_DEC:
    //            break;
    //        case LEGACY_ACTION_STORE_TARGET_LIST:
    //            break;
    //        case LEGACY_ACTION_WAYPOINT_RESUME:
    //            break;
    //        case LEGACY_ACTION_SET_ORIENTATION:
    //            break;
    //        case LEGACY_ACTION_SET_FACE_TO:
    //            break;
    //        case LEGACY_ACTION_PLAY_MOVIE:
    //            break;
    //        case LEGACY_ACTION_MOVE_TO:
    //            break;
    //        case LEGACY_ACTION_RESPAWN:
    //            break;
    //        case LEGACY_ACTION_EQUIP:
    //            break;
    //        case LEGACY_ACTION_CLOSE_GOSSIP:
    //            break;
    //        case LEGACY_ACTION_ADD_AURA:
    //            break;
    //        case LEGACY_ACTION_INTERRUPT_SPELL:
    //            break;
    //        case LEGACY_ACTION_SET_DYNAMIC_FLAGS:
    //            break;
    //        case LEGACY_ACTION_REMOVE_DYNAMIC_FLAGS:
    //            break;
    //        case LEGACY_ACTION_JUMP_TO:
    //            break;
    //        case LEGACY_ACTION_SEND_GOSSIP_MENU:
    //            break;
    //        case LEGACY_ACTION_SET_GO_LOOT_STATE:
    //            break;
    //        case LEGACY_ACTION_SET_HOME_POSITION:
    //            break;
    //        case LEGACY_ACTION_SET_HEALTH_REGEN:
    //            break;
    //        case LEGACY_ACTION_SET_ROOT:
    //            break;
    //        case LEGACY_ACTION_SET_GO_FLAGS:
    //            break;
    //        case LEGACY_ACTION_REMOVE_GO_FLAGS:
    //            break;
    //        case LEGACY_ACTION_SUMMON_CREATURE_GROUP:
    //            break;
    //        case LEGACY_ACTION_SET_POWER:
    //            break;
    //        case LEGACY_ACTION_ADD_POWER:
    //            break;
    //        case LEGACY_ACTION_REMOVE_POWER:
    //            break;
    //        case LEGACY_ACTION_GAME_EVENT_START:
    //            break;
    //        case LEGACY_ACTION_GAME_EVENT_STOP:
    //            break;
    //        case LEGACY_ACTION_WORLD_EVENT_START:
    //            break;
    //        case LEGACY_ACTION_SET_WORLD_EVENT_STAGE:
    //            break;
    //        case LEGACY_ACTION_INCREASE_WORLD_EVENT_STAGE:
    //            break;
    //        case LEGACY_ACTION_MORPH_TO:
    //            break;
    //        default:
    //            break;
    //    }
    //}

}
