#include "LegacyScript.h"
#include "LegacyScriptLoader.h"
#include <boost/spirit/include/classic.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <regex>

using namespace boost::spirit::classic;

void LegacyScriptLoader::LoadDefines()
{
    ActionDefines.clear();
    LoadActionDefines("SayLine", LEGACY_ACTION_SAY);
    LoadActionDefines("SetFaction", LEGACY_ACTION_SET_FACTION);
    LoadActionDefines("SetModel", LEGACY_ACTION_SET_MODEL);
    LoadActionDefines("PlaySound", LEGACY_ACTION_PLAY_SOUND);
    LoadActionDefines("Emote", LEGACY_ACTION_EMOTE);
    LoadActionDefines("FailQuest", LEGACY_ACTION_QUEST_FAIL);
    LoadActionDefines("AddQuest", LEGACY_ACTION_QUEST_ADD);
    LoadActionDefines("SetReactState", LEGACY_ACTION_SET_REACT_STATE);
    LoadActionDefines("ActiveGameObject", LEGACY_ACTION_ACTIVE_GO);
    LoadActionDefines("RandomEmote", LEGACY_ACTION_RANDOM_EMOTE);
    LoadActionDefines("CastSpell", LEGACY_ACTION_CAST_SPELL);
    LoadActionDefines("SummonCreature", LEGACY_ACTION_SUMMON_CREATURE);
    LoadActionDefines("ModThreat", LEGACY_ACTION_MOD_THREAT);
    LoadActionDefines("ModThreatAll", LEGACY_ACTION_MOD_THREAT_ALL);
    LoadActionDefines("QuestEventHappened", LEGACY_ACTION_QUEST_EVENT_HAPPENED);
    LoadActionDefines("EmoteState", LEGACY_ACTION_EMOTE_STATE);
    LoadActionDefines("SetUnitFlags", LEGACY_ACTION_SET_UNIT_FLAG);
    LoadActionDefines("RemoveUnitFlags", LEGACY_ACTION_REMOVE_UNIT_FLAG);
    LoadActionDefines("SetAutoAttack", LEGACY_ACTION_SET_AUTO_ATTACK);
    LoadActionDefines("AllowCombatMovement", LEGACY_ACTION_ALLOW_COMBAT_MOVEMENT);
    LoadActionDefines("SetEventPhase", LEGACY_ACTION_SET_EVENT_PHASE);
    LoadActionDefines("IncreaseEventPhase", LEGACY_ACTION_INCREASE_EVENT_PHASE);
    LoadActionDefines("Evade", LEGACY_ACTION_EVADE);
    LoadActionDefines("FleeForAssist", LEGACY_ACTION_FLEE_FOR_ASSIST);
    LoadActionDefines("QuestGroupEventHappened", LEGACY_ACTION_QUEST_GROUP_EVENT_HAPPENED);
    LoadActionDefines("RemoveAuraFromSpell", LEGACY_ACTION_REMOVE_AURA_FROM_SPELL);
    LoadActionDefines("Follow", LEGACY_ACTION_FOLLOW);
    LoadActionDefines("SetRandomPhase", LEGACY_ACTION_SET_RANDOM_PHASE);
    LoadActionDefines("Reset", LEGACY_ACTION_RESET);
    LoadActionDefines("CallKilledMonster", LEGACY_ACTION_CALL_KILLED_MONSTER);
    LoadActionDefines("SetInstanceData", LEGACY_ACTION_SET_INSTANCE_DATA);
    LoadActionDefines("SetInstanceData64", LEGACY_ACTION_SET_INSTANCE_DATA64);
    LoadActionDefines("UpdateTemplate", LEGACY_ACTION_UPDATE_TEMPLATE);
    LoadActionDefines("Die", LEGACY_ACTION_DIE);
    LoadActionDefines("DoZoneInCombat", LEGACY_ACTION_DO_ZONE_IN_COMBAT);
    LoadActionDefines("CallForHelp", LEGACY_ACTION_CALL_FOR_HELP);
    LoadActionDefines("SetSheath", LEGACY_ACTION_SET_SHEATH);
    LoadActionDefines("ForceDespawn", LEGACY_ACTION_FORCE_DESPAWN);
    LoadActionDefines("SetInvicibilityLevel", LEGACY_ACTION_SET_INVICIBILITY_LEVEL);
    LoadActionDefines("SetMount", LEGACY_ACTION_SET_MOUNT_MODEL);
    LoadActionDefines("SetPhaseMask", LEGACY_ACTION_SET_PHASE_MASK);
    LoadActionDefines("SetData", LEGACY_ACTION_SET_DATA);
    LoadActionDefines("MoveForward", LEGACY_ACTION_MOVE_FORWARD);
    LoadActionDefines("SetVisible", LEGACY_ACTION_SET_VISIBLE);
    LoadActionDefines("SetActive", LEGACY_ACTION_SET_ACTIVE);
    LoadActionDefines("AttackStart", LEGACY_ACTION_ATTACK_START);
    LoadActionDefines("SummonGameObject", LEGACY_ACTION_SUMMON_GAMEOBJECT);
    LoadActionDefines("Kill", LEGACY_ACTION_KILL);
    LoadActionDefines("ActiveTaxi", LEGACY_ACTION_ACTIVE_TAXI);
    LoadActionDefines("WayPointStart", LEGACY_ACTION_WAYPOINT_START);
    LoadActionDefines("WayPointPause", LEGACY_ACTION_WAYPOINT_PAUSE);
    LoadActionDefines("WayPointStop", LEGACY_ACTION_WAYPOINT_STOP);
    LoadActionDefines("AddItem", LEGACY_ACTION_ADD_ITEM);
    LoadActionDefines("RemoveItem", LEGACY_ACTION_REMOVE_ITEM);
    LoadActionDefines("SetAITemplate", LEGACY_ACTION_SET_AI_TEMPLATE);
    LoadActionDefines("SetRun", LEGACY_ACTION_SET_RUN);
    LoadActionDefines("SetFly", LEGACY_ACTION_SET_FLY);
    LoadActionDefines("SetSwim", LEGACY_ACTION_SET_SWIM);
    LoadActionDefines("Teleport", LEGACY_ACTION_TELEPORT);
    LoadActionDefines("StoreVarDec", LEGACY_ACTION_STORE_VAR_DEC);
    LoadActionDefines("StoreTargetList", LEGACY_ACTION_STORE_TARGET_LIST);
    LoadActionDefines("WayPointResume", LEGACY_ACTION_WAYPOINT_RESUME);
    LoadActionDefines("SetOrientation", LEGACY_ACTION_SET_ORIENTATION);
    LoadActionDefines("SetFaceTo", LEGACY_ACTION_SET_FACE_TO);
    LoadActionDefines("PlayMovie", LEGACY_ACTION_PLAY_MOVIE);
    LoadActionDefines("MoveTo", LEGACY_ACTION_MOVE_TO);
    LoadActionDefines("Respawn", LEGACY_ACTION_RESPAWN);
    LoadActionDefines("Equip", LEGACY_ACTION_EQUIP);
    LoadActionDefines("CloseGossip", LEGACY_ACTION_CLOSE_GOSSIP);
    LoadActionDefines("AddAura", LEGACY_ACTION_ADD_AURA);
    LoadActionDefines("InterrupSpell", LEGACY_ACTION_INTERRUPT_SPELL);
    LoadActionDefines("SetDynamicFlags", LEGACY_ACTION_SET_DYNAMIC_FLAGS);
    LoadActionDefines("RemoveDynamicFlags", LEGACY_ACTION_REMOVE_DYNAMIC_FLAGS);
    LoadActionDefines("JumpTo", LEGACY_ACTION_JUMP_TO);
    LoadActionDefines("SendGossipMenu", LEGACY_ACTION_SEND_GOSSIP_MENU);
    LoadActionDefines("SetGameObjectLootState", LEGACY_ACTION_SET_GO_LOOT_STATE);
    LoadActionDefines("SetHomePosition", LEGACY_ACTION_SET_HOME_POSITION);
    LoadActionDefines("SetHealthRegen", LEGACY_ACTION_SET_HEALTH_REGEN);
    LoadActionDefines("SetRoot", LEGACY_ACTION_SET_ROOT);
    LoadActionDefines("SetGameObjectFlags", LEGACY_ACTION_SET_GO_FLAGS);
    LoadActionDefines("RemoveGameObjectFlags", LEGACY_ACTION_REMOVE_GO_FLAGS);
    LoadActionDefines("SummonCreatureGroup", LEGACY_ACTION_SUMMON_CREATURE_GROUP);
    LoadActionDefines("SetPower", LEGACY_ACTION_SET_POWER);
    LoadActionDefines("AddPower", LEGACY_ACTION_ADD_POWER);
    LoadActionDefines("RemovePower", LEGACY_ACTION_REMOVE_POWER);
    LoadActionDefines("GameEventStart", LEGACY_ACTION_GAME_EVENT_START);
    LoadActionDefines("GameEventStop", LEGACY_ACTION_GAME_EVENT_STOP);
    LoadActionDefines("WorldEventStart", LEGACY_ACTION_WORLD_EVENT_START);
    LoadActionDefines("SetWorldEventStage", LEGACY_ACTION_SET_WORLD_EVENT_STAGE);
    LoadActionDefines("IncreaseWorldEventStage", LEGACY_ACTION_INCREASE_WORLD_EVENT_STAGE);
    LoadActionDefines("MorphTo", LEGACY_ACTION_MORPH_TO);
    LoadActionDefines("Talk", LEGACY_ACTION_TALK);

    TargetDefines.clear();
    LoadTargetDefines("Self", LEGACY_TARGET_SELF);
    LoadTargetDefines("Victim", LEGACY_TARGET_VICTIM);

    TriggerDefines.clear();
    LoadTriggerDefines("OnRespawn", LEGACY_TRIGGER_RESPAWN);
    LoadTriggerDefines("OnReachedHome", LEGACY_TRIGGER_REACHED_HOME);
    LoadTriggerDefines("OnAggro", LEGACY_TRIGGER_ENTER_COMBAT);
    LoadTriggerDefines("OnEvade", LEGACY_TRIGGER_ENTER_EVADE_MODE);
    LoadTriggerDefines("OnDeath", LEGACY_TRIGGER_DEAD);
    LoadTriggerDefines("OnKill", LEGACY_TRIGGER_KILLED);
    LoadTriggerDefines("OnSummonCreature", LEGACY_TRIGGER_SUMMONED_CREATURE);
    LoadTriggerDefines("OnAttackStart", LEGACY_TRIGGER_ATTACK_START);
    LoadTriggerDefines("OnMoveInLineOfSight", LEGACY_TRIGGER_MOVE_IN_LINE_OF_SIGHT);
    LoadTriggerDefines("OnSpellHit", LEGACY_TRIGGER_SPELL_HIT);
    LoadTriggerDefines("OnSpellHitTarget", LEGACY_TRIGGER_SPELL_HIT_TARGET);
    LoadTriggerDefines("OnTakenDamage", LEGACY_TRIGGER_TAKEN_DAMAGE);
    LoadTriggerDefines("OnReceiveHeal", LEGACY_TRIGGER_RECEIVE_HEAL);
    LoadTriggerDefines("OnUpdate", LEGACY_TRIGGER_UPDATE);
    LoadTriggerDefines("OnReceiveEmote", LEGACY_TRIGGER_RECEIVE_EMOTE);
    LoadTriggerDefines("OnMoveInForm", LEGACY_TRIGGER_MOVE_IN_FORM);
    LoadTriggerDefines("OnSummonBy", LEGACY_TRIGGER_SUMMONED_BY);
    LoadTriggerDefines("OnDealDamage", LEGACY_TRIGGER_DEAL_DAMAGE);
    LoadTriggerDefines("OnSummonedCreatureDespawn", LEGACY_TRIGGER_SUMMONED_CREATURE_DESPAWNED);
    LoadTriggerDefines("OnCorpseRemove", LEGACY_TRIGGER_CORPSE_REMOVED);
    LoadTriggerDefines("OnUpdateCharmed", LEGACY_TRIGGER_UPDATE_CHARMED);
    LoadTriggerDefines("OnPassengerBoarded", LEGACY_TRIGGER_PASSENGER_BOARDED);
    LoadTriggerDefines("OnAIInitialize", LEGACY_TRIGGER_AI_INITIALIZED);
    LoadTriggerDefines("OnCharmed", LEGACY_TRIGGER_CHARMED);
    LoadTriggerDefines("OnGossipHello", LEGACY_TRIGGER_GOSSIP_HELLO);
    LoadTriggerDefines("OnGossipSelect", LEGACY_TRIGGER_GOSSIP_SELECT);
    LoadTriggerDefines("OnGossipSelectCode", LEGACY_TRIGGER_GOSSIP_SELECT_CODE);
    LoadTriggerDefines("OnQuestAccepted", LEGACY_TRIGGER_QUEST_ACCEPTED);
    LoadTriggerDefines("OnDummyEffect", LEGACY_TRIGGER_DUMMY_EFFECT);
    LoadTriggerDefines("OnTriggerGameEvent", LEGACY_TRIGGER_GAME_EVENT);
    LoadTriggerDefines("OnSpellClick", LEGACY_TRIGGER_SPELL_CLICK);
    LoadTriggerDefines("WhileInCombat", LEGACY_TRIGGER_UPDATE_IN_COMBAT);
}

LegacyScriptData LegacyScriptLoader::LoadScriptData(Unit* u, uint32 specifiedId /* = 0 */)
{
    LegacyScriptData data;
    return data;
}

LegacyTarget LegacyScriptLoader::GetTarget(std::string s)
{
    TargetDefine::const_iterator itr = TargetDefines.find(s);
    if (itr == TargetDefines.end())
        return LEGACY_TARGET_NONE;
    return itr->second;
}

LegacyAction LegacyScriptLoader::GetAction(std::string s)
{
    ActionDefine::const_iterator itr = ActionDefines.find(s);
    if (itr == ActionDefines.end())
        return LEGACY_ACTION_NONE;
    return itr->second;
}

LegacyTrigger LegacyScriptLoader::GetTrigger(std::string s)
{
    TriggerDefine::const_iterator itr = TriggerDefines.find(s);
    if (itr == TriggerDefines.end())
        return LEGACY_TRIGGER_NONE;
    return itr->second;
}

double LegacyScriptLoader::GetParam(std::string s, uint32 pos /* = 1 */)
{
    std::vector<std::string> list;
    rule<> parameter = *real_p | "true" | "false";
    parse_info<> r = parse(s.c_str(), parameter[push_back_a(list)] >> *(*space_p >> ',' >> *space_p >> parameter[push_back_a(list)]));
    if (list.size() >= pos)
    {
        std::string s = list[pos - 1];
        if (s == "true")
            return 1;
        else if (s == "false")
            return 0;
        else
            return strtod(list[pos - 1].c_str(), NULL);
    }
    else
        return 0;
}

void LegacyScriptLoader::LoadTriggerMaps()
{
}

uint32 LegacyScriptLoader::GetLink(LegacyLinker links, std::string name) const
{
    LegacyLinker::const_iterator itr = links.find(name);
    if (itr != links.end())
        return itr->second;
    return 0;
}

void LegacyScriptLoader::LoadScripts()
{
    TC_LOG_INFO("server.loading", "Loading legacy script for creature...");

    LoadDefines();

    namespace fs = boost::filesystem;
    fs::path dir = "scripts";

    if (fs::is_directory(dir))
    {
        fs::recursive_directory_iterator end_itr;
        for (fs::recursive_directory_iterator dir_itr(dir); dir_itr != end_itr; ++dir_itr) {
            if (fs::is_directory(dir_itr->status())) {
                //std::cout << "directory: " << dir_itr->path().filename() << std::endl;
            }
            else if (fs::is_regular_file(dir_itr->status())) {
                //std::cout << "          " << dir_itr->path().filename() << std::endl;
                std::string script_file_name = fs::basename(dir_itr->path().filename());
                //TC_LOG_INFO("server.loading", ">> Loading script file: %s", script_file_name);
                if (fs::extension(dir_itr->path().filename()) == ".ls")
                {
                    std::string path = dir_itr->path().filename().string();
                    fs::ifstream file(dir / dir_itr->path().filename());
                    if (file.is_open())
                    {
                        std::string script;
                        char* buffer = new char[4096];
                        while (!file.eof())
                        {
                            file >> buffer;
                            script += buffer;
                            script += ' ';
                        }
                        // remove new lines.
                        std::regex nl_ex("\\n");
                        script = std::regex_replace(script, nl_ex, " ");
                        // remove comments.
                        std::regex comment_ex("\\/\\#.*?\\#\\/|\\#.*?\\#");
                        script = std::regex_replace(script, comment_ex, " ");
                        // do things here
                        std::string scriptString;
                        if (parse(script.c_str(), rScript[assign_a(scriptString)]).hit)
                        {
                            // UnitScript Part
                            std::string scriptName;
                            ScriptStringHolder eventString;
                            std::string triggerString;
                            parse(scriptString.c_str(), str_p("UnitScript") >> *space_p >> (+alnum_p)[assign_a(scriptName)] >> *space_p >> '{' >> *(*space_p >> rEvent[push_back_a(eventString)] >> *space_p) >> *space_p >> rTriggerMap[assign_a(triggerString)] >> *space_p >> '}');

                            if (scriptName != "" && eventString.size() != 0)
                            {
                                LegacyScriptData scriptData;
                                if (ParseScript(script, scriptData))
                                {
                                    new LegacyCreatureScript(scriptName.c_str(), scriptData);
                                }
                            }
                            // Other Parts
                            // ...
                        }
                        file.close();
                    }
                }
            }
        }
    }
}

// note: this function won't handle script name parsing. do this in the outer circle.
bool LegacyScriptLoader::ParseScript(std::string scripts, LegacyScriptData &data)
{
    LegacyLinker eventLinks;
    // retrive events.
    ScriptStringHolder eventScripts;
    std::string triggerScripts;
    EventIndexHolder eventHolder;
    boost::spirit::classic::parse(scripts.c_str(), str_p("UnitScript") >> *space_p >> +alnum_p >> *space_p >> '{' >> *(*space_p >> rEvent[push_back_a(eventScripts)] >> *space_p) >> *space_p >> rTriggerMap[assign_a(triggerScripts)] >> *space_p >> '}');
    if (eventScripts.size() == 0) // no event defines
        return false;
    for (ScriptStringHolder::iterator itr = eventScripts.begin(); itr != eventScripts.end(); ++itr)
    {
        if (!ParseEvents(*itr, eventHolder)) // something bad happened during event parsing.
            return false;
    }
    // index events
    LegacyEventMap eventMap;
    EventIndexHolder::iterator itr_e = eventHolder.begin();
    uint32 _eventIndex = 1;
    while (itr_e != eventHolder.end())
    {
        eventMap[_eventIndex] = itr_e->second;
        eventLinks[itr_e->first] = _eventIndex;
        ++itr_e;
        ++_eventIndex;
    }
    // events parsing done. now deal with the triggers.
    LegacyTriggerMap triggerMap;
    if (!ParseTrigger(triggerScripts, triggerMap, eventLinks)) // something goes wrong while parsing triggers
        return false;
    // assign data
    data.eventMap = eventMap;
    data.triggerMap = triggerMap;
    return true;
}

bool LegacyScriptLoader::ParseEvents(std::string scripts, EventIndexHolder &holder)
{
    // Event EventName[Attributes(Parameters)] { Actions }
    const char* eventString = scripts.c_str();
    std::string eventName;
    LegacyEventData data;
    uint32 eventIndex = 0;
    ScriptStringHolder actionString;
    ScriptStringHolder attrString;
    // chance, cooldown, cooldownmin, cooldownmax, phase, flags, p1, p2, p3, actions
    if (parse(eventString, str_p("Event") >> +space_p >> (+alnum_p)[assign_a(eventName)] >> *space_p >> *(ch_p('[') >> rAttribute[push_back_a(attrString)] >> *(*space_p >> ',' >> *space_p >> rAttribute[push_back_a(attrString)])) >> *space_p >> ']' >> *space_p >> '{' >> *space_p >> *(*space_p >> rAction[push_back_a(actionString)] >> *space_p) >> '}').hit)
    {
        for (ScriptStringHolder::iterator itr = attrString.begin(); itr != attrString.end(); ++itr)
        {
            std::string temp;
            std::string temp2;
            ScriptStringHolder temp3;
            if (parse((*itr).c_str(), str_p("Chance") >> '(' >> *space_p >> (+digit_p)[assign_a(temp)] >> *space_p >> ')').full)
                data.chance = GetParam(temp);
            else if (parse((*itr).c_str(), str_p("Cooldown") >> '(' >> *space_p >> (+digit_p)[assign_a(temp)] >> *space_p >> ',' >> *space_p >> (+digit_p)[assign_a(temp2)] >> *space_p >> ')').full)
            {
                data.cooldownMin = GetParam(temp);
                data.cooldownMax = GetParam(temp2);
            }
            else if (parse((*itr).c_str(), str_p("Phase") >> '(' >> *space_p >> (+digit_p)[push_back_a(temp3)] >> *(*space_p >> ',' >> *space_p >> (+digit_p)[push_back_a(temp3)]) >> *space_p >> ')').full)
            {
                data.phase.clear();
                for (ScriptStringHolder::iterator itr_temp3 = temp3.begin(); itr_temp3 != temp3.end(); ++itr_temp3)
                    data.phase.push_back(GetParam(*itr_temp3));
            }
        }
        // flags - nyi
        // p1, p2, p3 - nyi
        // actions
        LegacyActionMap actionMap;
        for (ScriptStringHolder::iterator itr_a = actionString.begin(); itr_a != actionString.end(); ++itr_a)
        {
            // Target1.Action(Parameter) To Target2
            LegacyActionData actionData;
            if (ParseAction((*itr_a), actionData))
                actionMap.push_back(actionData);
        }
        data.actions = actionMap;
        // check and save event index
        EventIndexHolder::iterator itr_i = holder.find(eventName);
        if (itr_i != holder.end()) // name duplicated, treat this script as invalid, so return false.
        {
            // @todo: output some error info.
            return false;
        }
        holder[eventName] = data;
        // all done.
        return true;
    }
    return false;
}

bool LegacyScriptLoader::ParseAction(std::string scripts, LegacyActionData &actionData)
{
    const char* _actionString = scripts.c_str();
    std::string target1, target2, _action, _parameters;
    parse(_actionString, *(rTarget[assign_a(target1)] >> '.') >> rMethod[assign_a(_action)] >> '(' >> *space_p >> rParameters[assign_a(_parameters)] >> *space_p >> ')' >> *space_p >> *(("To") >> +space_p >> rTarget[assign_a(target2)]) >> ';');

    if (ValidateTarget(target1) && _action != "")
    {
        actionData.action = GetAction(_action);
        actionData.actor = GetTarget(target1);
        actionData.target = GetTarget(target2);
        actionData.p1 = GetParam(_parameters);
        actionData.p2 = GetParam(_parameters, 2);
        actionData.p3 = GetParam(_parameters, 3);
        actionData.p4 = GetParam(_parameters, 4);
        actionData.p5 = GetParam(_parameters, 5);
        actionData.p6 = GetParam(_parameters, 6);
        //actionData.t1 = 0;
        //actionData.t2 = 0;
        //actionData.t3 = 0;
        //actionData.x = 0;
        //actionData.y = 0;
        //actionData.z = 0;
        //actionData.o = 0;
        switch (actionData.action)
        {
            case LEGACY_ACTION_NONE:
                break;
            case LEGACY_ACTION_SAY:
                break;
            case LEGACY_ACTION_SET_FACTION:
                break;
            case LEGACY_ACTION_SET_MODEL:
                break;
            case LEGACY_ACTION_PLAY_SOUND:
                break;
            case LEGACY_ACTION_EMOTE:
                break;
            case LEGACY_ACTION_QUEST_FAIL:
                break;
            case LEGACY_ACTION_QUEST_ADD:
                break;
            case LEGACY_ACTION_SET_REACT_STATE:
                break;
            case LEGACY_ACTION_ACTIVE_GO:
                break;
            case LEGACY_ACTION_RANDOM_EMOTE:
                break;
            case LEGACY_ACTION_CAST_SPELL:
                break;
            case LEGACY_ACTION_SUMMON_CREATURE:
                break;
            case LEGACY_ACTION_MOD_THREAT:
                break;
            case LEGACY_ACTION_MOD_THREAT_ALL:
                break;
            case LEGACY_ACTION_QUEST_EVENT_HAPPENED:
                break;
            case LEGACY_ACTION_EMOTE_STATE:
                break;
            case LEGACY_ACTION_SET_UNIT_FLAG:
                break;
            case LEGACY_ACTION_REMOVE_UNIT_FLAG:
                break;
            case LEGACY_ACTION_SET_AUTO_ATTACK:
                break;
            case LEGACY_ACTION_ALLOW_COMBAT_MOVEMENT:
                break;
            case LEGACY_ACTION_SET_EVENT_PHASE:
                break;
            case LEGACY_ACTION_INCREASE_EVENT_PHASE:
                break;
            case LEGACY_ACTION_EVADE:
                break;
            case LEGACY_ACTION_FLEE_FOR_ASSIST:
                break;
            case LEGACY_ACTION_QUEST_GROUP_EVENT_HAPPENED:
                break;
            case LEGACY_ACTION_REMOVE_AURA_FROM_SPELL:
                break;
            case LEGACY_ACTION_FOLLOW:
                break;
            case LEGACY_ACTION_SET_RANDOM_PHASE:
                break;
            case LEGACY_ACTION_RESET:
                break;
            case LEGACY_ACTION_CALL_KILLED_MONSTER:
                break;
            case LEGACY_ACTION_SET_INSTANCE_DATA:
                break;
            case LEGACY_ACTION_SET_INSTANCE_DATA64:
                break;
            case LEGACY_ACTION_UPDATE_TEMPLATE:
                break;
            case LEGACY_ACTION_DIE:
                break;
            case LEGACY_ACTION_DO_ZONE_IN_COMBAT:
                break;
            case LEGACY_ACTION_CALL_FOR_HELP:
                break;
            case LEGACY_ACTION_SET_SHEATH:
                break;
            case LEGACY_ACTION_FORCE_DESPAWN:
                break;
            case LEGACY_ACTION_SET_INVICIBILITY_LEVEL:
                break;
            case LEGACY_ACTION_SET_MOUNT_MODEL:
                break;
            case LEGACY_ACTION_SET_PHASE_MASK:
                break;
            case LEGACY_ACTION_SET_DATA:
                break;
            case LEGACY_ACTION_MOVE_FORWARD:
                break;
            case LEGACY_ACTION_SET_VISIBLE:
                break;
            case LEGACY_ACTION_SET_ACTIVE:
                break;
            case LEGACY_ACTION_ATTACK_START:
                break;
            case LEGACY_ACTION_SUMMON_GAMEOBJECT:
                break;
            case LEGACY_ACTION_KILL:
                break;
            case LEGACY_ACTION_ACTIVE_TAXI:
                break;
            case LEGACY_ACTION_WAYPOINT_START:
                break;
            case LEGACY_ACTION_WAYPOINT_PAUSE:
                break;
            case LEGACY_ACTION_WAYPOINT_STOP:
                break;
            case LEGACY_ACTION_ADD_ITEM:
                break;
            case LEGACY_ACTION_REMOVE_ITEM:
                break;
            case LEGACY_ACTION_SET_AI_TEMPLATE:
                break;
            case LEGACY_ACTION_SET_RUN:
                break;
            case LEGACY_ACTION_SET_FLY:
                break;
            case LEGACY_ACTION_SET_SWIM:
                break;
            case LEGACY_ACTION_TELEPORT:
                break;
            case LEGACY_ACTION_STORE_VAR_DEC:
                break;
            case LEGACY_ACTION_STORE_TARGET_LIST:
                break;
            case LEGACY_ACTION_WAYPOINT_RESUME:
                break;
            case LEGACY_ACTION_SET_ORIENTATION:
                break;
            case LEGACY_ACTION_SET_FACE_TO:
                break;
            case LEGACY_ACTION_PLAY_MOVIE:
                break;
            case LEGACY_ACTION_MOVE_TO:
                break;
            case LEGACY_ACTION_RESPAWN:
                break;
            case LEGACY_ACTION_EQUIP:
                break;
            case LEGACY_ACTION_CLOSE_GOSSIP:
                break;
            case LEGACY_ACTION_ADD_AURA:
                break;
            case LEGACY_ACTION_INTERRUPT_SPELL:
                break;
            case LEGACY_ACTION_SET_DYNAMIC_FLAGS:
                break;
            case LEGACY_ACTION_REMOVE_DYNAMIC_FLAGS:
                break;
            case LEGACY_ACTION_JUMP_TO:
                break;
            case LEGACY_ACTION_SEND_GOSSIP_MENU:
                break;
            case LEGACY_ACTION_SET_GO_LOOT_STATE:
                break;
            case LEGACY_ACTION_SET_HOME_POSITION:
                break;
            case LEGACY_ACTION_SET_HEALTH_REGEN:
                break;
            case LEGACY_ACTION_SET_ROOT:
                break;
            case LEGACY_ACTION_SET_GO_FLAGS:
                break;
            case LEGACY_ACTION_REMOVE_GO_FLAGS:
                break;
            case LEGACY_ACTION_SUMMON_CREATURE_GROUP:
                break;
            case LEGACY_ACTION_SET_POWER:
                break;
            case LEGACY_ACTION_ADD_POWER:
                break;
            case LEGACY_ACTION_REMOVE_POWER:
                break;
            case LEGACY_ACTION_GAME_EVENT_START:
                break;
            case LEGACY_ACTION_GAME_EVENT_STOP:
                break;
            case LEGACY_ACTION_WORLD_EVENT_START:
                break;
            case LEGACY_ACTION_SET_WORLD_EVENT_STAGE:
                break;
            case LEGACY_ACTION_INCREASE_WORLD_EVENT_STAGE:
                break;
            case LEGACY_ACTION_MORPH_TO:
                break;
            case LEGACY_ACTION_MAX:
                break;
            default:
                break;
        }
        return true;
    }
    return false;
}

bool LegacyScriptLoader::ValidateTarget(std::string script)
{
    TargetDefine::const_iterator itr = TargetDefines.find(script);
    if (itr != TargetDefines.end())
        return true;
    else
        return false;
}

bool LegacyScriptLoader::ParseTrigger(std::string scripts, LegacyTriggerMap &triggerMap, LegacyLinker linker)
{
    // Trigger:(Parameter) { Actions }
    // for now we only got 1 action: ExecuteEvent(EventName).
    ScriptStringHolder triggers;
    parse(scripts.c_str(), str_p("EventBinding") >> *space_p >> '{' >> *space_p >> *rTrigger[push_back_a(triggers)] >> *space_p >> '}');
    for (ScriptStringHolder::iterator itr = triggers.begin(); itr != triggers.end(); ++itr)
    {
        LegacyTriggerData data;
        // retrive delay info and events index.
        std::string triggerString;
        std::string delayString;
        ScriptStringHolder eventIndexString;

        parse((*itr).c_str(), rTriggerName[assign_a(triggerString)] >> *space_p >> ':' >> *space_p >> *('(' >> *space_p >> (+real_p)[assign_a(delayString)] >> *space_p >> ')') >> *space_p >> '{' >> *space_p >> *(rTriggerEvent >> '(' >> *space_p >> (*alnum_p)[push_back_a(eventIndexString)] >> *space_p >> ')' >> *space_p >> ';') >> *space_p >> '}');
        data.delay = GetParam(delayString);
        // index events
        data.events.clear();
        for (ScriptStringHolder::iterator itr_ei = eventIndexString.begin(); itr_ei != eventIndexString.end(); ++itr_ei)
        {
            uint32 e_index = GetLink(linker, *itr_ei);
            if (e_index)
                data.events.push_back(e_index);
        }
        LegacyTrigger t = GetTrigger(triggerString);
        if (t != LEGACY_TRIGGER_NONE)
            triggerMap[t] = data;
        else
            return false;
    }
    return true;
}