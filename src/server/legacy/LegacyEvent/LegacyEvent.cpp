/*
* Copyright (C) 2012-2014 ProjectLegacy <http://msacn.org/>
* Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
*/

#include "LegacyEvent.h"
#include "ObjectMgr.h"
#include "ObjectAccessor.h"
#include "World.h"
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Player.h"
#include "WorldSession.h"

LegacyEventMgr::LegacyEventMgr() { }
LegacyEventMgr::~LegacyEventMgr() { }

LegacyEvent const* LegacyEventMgr::GetEvent(uint32 id)
{
	LegacyEvents::const_iterator itr = _legacyEvents.find(id);
	if (itr != _legacyEvents.end())
		return &(itr->second);

	return NULL;
}

void LegacyEventMgr::SetEventStage(uint32 id, int32 stage)
{
	LegacyEvents::iterator itr = _legacyEvents.find(id);
	if (itr != _legacyEvents.end() && itr->second.stage != stage)
	{
		itr->second.stage = stage;
		SaveEventStage(id, stage);
		ExecuteEventsOnStage(id, stage);
	}
}

int32 LegacyEventMgr::GetEventStage(uint32 id)
{
	LegacyEvents::const_iterator itr = _legacyEvents.find(id);
	if (itr != _legacyEvents.end())
		return itr->second.stage;
	return 0;
}

uint32 LegacyEventMgr::GetEventStageMisc(uint32 id)
{
	LegacyEvents::const_iterator itr = _legacyEvents.find(id);
	if (itr != _legacyEvents.end())
		return itr->second.misc;
	return 0;
}

bool LegacyEventMgr::IsEventValid(uint32 id)
{
	LegacyEventsDefine::const_iterator itr = _legacyEventsDefine.find(id);
	if (itr != _legacyEventsDefine.end())
		return true;
	return false;
}

void LegacyEventMgr::LoadEvents()
{
	_legacyGossips.clear();

	_legacyEventsDefine.clear();
	QueryResult result = WorldDatabase.Query("SELECT id, name FROM legacy_events");
	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			_legacyEventsDefine[fields[0].GetUInt32()] = fields[1].GetString();
		} while (result->NextRow());
	}

	_legacyEvents.clear();
	result = CharacterDatabase.Query("SELECT id, state, misc, invoker, invokerType, date FROM legacy_events_stage ORDER BY guid ASC");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			uint32 id = fields[0].GetUInt32();
			if (IsEventValid(id))
			{
				LegacyEvent legacyEvent;
				legacyEvent.stage = fields[1].GetInt32();
				legacyEvent.misc = fields[2].GetUInt32();
				legacyEvent.invoker = fields[3].GetUInt32();
				legacyEvent.invokerType = (LegacyEventInvokerType)fields[4].GetInt32();
				legacyEvent.date = fields[5].GetUInt32();
				_legacyEvents[id] = legacyEvent;
			}
			else
				TC_LOG_ERROR("server.loading", "Invalid LegacyEvent id: %u, skipped.", id);
		} while (result->NextRow());
	}

	_legacyEventsCreatureStateDefine.clear();
	result = WorldDatabase.Query("SELECT entry, spawnInfo, isDead, phase, model, gossip, questInfo, vendorInfo FROM legacy_events_creature_states");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			uint32 id = fields[0].GetUInt32();
			LegacyEventsCreatureState state;
			state.spawnInfo = fields[1].GetUInt32();
			state.isDead = fields[2].GetUInt8() == 0 ? false : true;
			state.phase = fields[3].GetUInt8();
			state.model = fields[4].GetUInt32();
			state.gossip = fields[5].GetUInt32();
			state.questInfo = fields[6].GetUInt32();
			state.vendorInfo = fields[7].GetUInt32();
			_legacyEventsCreatureStateDefine[id] = state;
		} while (result->NextRow());
	}

	_legacyEventsQuestInfo.clear();
	result = WorldDatabase.Query("SELECT entry, quest FROM legacy_events_quest_exchange");
	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			_legacyEventsQuestInfo.insert(std::pair<uint32, int32>(fields[0].GetUInt32(), fields[1].GetInt32()));
		} while (result->NextRow());
	}

	_legacyEventsVendorInfo.clear();
	result = WorldDatabase.Query("SELECT entry, item, maxCount, incrTime, extendedCost FROM legacy_events_vendor_exchange");
	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			LegacyEventsVendorInfo info;
			info.item = fields[1].GetInt32();
			info.maxCount = fields[2].GetUInt32();
			info.incrTime = fields[3].GetUInt32();
			info.extendedConst = fields[4].GetUInt32();
			_legacyEventsVendorInfo.insert(std::pair<uint32, LegacyEventsVendorInfo>(fields[0].GetUInt32(), info));
		} while (result->NextRow());
	}

	_legacyEventsSpawnInfo.clear();
	result = WorldDatabase.Query("SELECT entry, isSpawn, map, x, y, z, o FROM legacy_events_spawn_exchange");
	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			uint32 id = fields[0].GetUInt32();
			LegacyEventsCreatureSpawnInfo info;
			info.isSpawn = fields[1].GetUInt32() == 0 ? false : true;
			info.map = fields[2].GetUInt32();
			info.x = fields[3].GetFloat();
			info.y = fields[4].GetFloat();
			info.z = fields[5].GetFloat();
			info.o = fields[6].GetFloat();
			_legacyEventsSpawnInfo[id] = info;
		} while (result->NextRow());
	}

	_legacyEventsForCreature.clear();
	result = WorldDatabase.Query("SELECT event, eventState, creatureEntry, creatureGuid, creatureState FROM legacy_events_creature_exchange");
	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			LegacyEventsForCreature cEvents;
			cEvents.event = fields[0].GetUInt32();
			cEvents.eventState = fields[1].GetInt32();
			cEvents.creatureEntry = fields[2].GetUInt32();
			cEvents.creatureGuid = fields[3].GetUInt32();
			cEvents.creatureState = fields[4].GetInt32();
			_legacyEventsForCreature.push_back(cEvents);
		} while (result->NextRow());
	}

	TC_LOG_INFO("server.loading", ">> LegacyEvents Loaded.");
	TC_LOG_INFO("server.loading", ">> Rolling history events...");
	RollHistoryEvents();
	TC_LOG_INFO("server.loading", ">> Rolling history events complete.");
}

void LegacyEventMgr::SaveEventStage(uint32 id, int32 state)
{
	CharacterDatabase.PExecute("REPLACE INTO legacy_events_state (id, state, time) VALUES (%u, %d, %u)", id, state, time(NULL));
}

void LegacyEventMgr::RollHistoryEvents()
{
	for (LegacyEvents::const_iterator itr = _legacyEvents.begin(); itr != _legacyEvents.end(); ++itr)
		ExecuteEventsOnStage(itr->first, itr->second.stage);
}

void LegacyEventMgr::ExecuteEventsOnStage(uint32 id, int32 stage)
{
	for (LegacyEventsForCreatureList::const_iterator itr = _legacyEventsForCreature.begin(); itr != _legacyEventsForCreature.end(); ++itr)
	{
		if (itr->event == id && itr->eventState == stage)
			ExecuteEventsForCreature(itr->creatureEntry, itr->creatureGuid, itr->creatureState);
	}
}

void LegacyEventMgr::ExecuteEventsForCreature(uint32 entry, uint32 guid, uint32 creatureState)
{
	//if (guid) // if guid was defined, ignore entry define.
	//{
	//	CreatureData& data = sObjectMgr->NewOrExistCreatureData(guid);
	//	if (!data.id)
	//		return;

	//	Creature* creature = ObjectAccessor::GetObjectInWorld(MAKE_NEW_GUID(guid, data.id, HIGHGUID_UNIT), (Creature*)NULL);

	//	if (LegacyEventsCreatureState const* cState = GetEventCreatureState(creatureState))
	//	{
	//		if (cState->spawnInfo)
	//		{
	//			if (LegacyEventsCreatureSpawnInfo const* spawnInfo = GetSpawnInfo(cState->spawnInfo))
	//			{
	//				if (spawnInfo->isSpawn)
	//				{
	//					if (spawnInfo->map != -1)
	//					{
	//						data.mapid = spawnInfo->map;
	//						data.posX = spawnInfo->x;
	//						data.posY = spawnInfo->y;
	//						data.posZ = spawnInfo->z;
	//						data.orientation = spawnInfo->o;
	//						if (creature)
	//						{
	//							if (creature->GetMapId() == spawnInfo->map)
	//							{
	//								creature->Relocate(spawnInfo->x, spawnInfo->y, spawnInfo->z, spawnInfo->o);
	//								creature->Respawn();
	//							}
	//							else
	//							{
	//								creature->DespawnOrUnsummon();
	//								creature->SetVisible(false);
	//							}
	//						}
	//					}

	//					if (cState->model)
	//					{
	//						data.displayid = cState->model;
	//						if (creature)
	//							creature->SetDisplayId(cState->model);
	//					}

	//					if (cState->phase)
	//					{
	//						data.phaseMask = 1 << cState->phase;
	//						if (creature)
	//							creature->SetPhaseMask(1 << cState->phase, true);
	//					}

	//					if (cState->isDead)
	//					{
	//						/*if (creature)
	//						{
	//						creature->setDeathState(DEAD);
	//						creature->HandleEmoteCommand(EMOTE_STATE_DEAD);
	//						creature->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
	//						}*/
	//					}
	//				}
	//				else // if isspawn == 0
	//				{
	//					if (creature)
	//					{
	//						creature->SetRespawnTime(DAY);
	//						creature->DespawnOrUnsummon();
	//						creature->SetVisible(false);
	//					}
	//				}

	//				if (cState->questInfo) // affect all creature of this entry.
	//				{
	//					UpdateQuestRelations(entry, cState->questInfo);
	//					data.npcflag |= UNIT_NPC_FLAG_QUESTGIVER;
	//					if (creature)
	//						creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
	//				}

	//				if (cState->vendorInfo)
	//				{
	//					UpdateVentorInfos(entry, cState->vendorInfo);
	//					data.npcflag |= UNIT_NPC_FLAG_VENDOR;
	//					if (creature)
	//						creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR);
	//				}

	//				if (cState->gossip)
	//				{
	//					LegacyGossips::iterator itr = _legacyGossips.find(entry);
	//					if (itr != _legacyGossips.end()) // already exist something.
	//						itr->second = cState->gossip;
	//					else
	//						_legacyGossips[entry] = cState->gossip;

	//					data.npcflag |= UNIT_NPC_FLAG_GOSSIP;
	//					if (creature)
	//						creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
	//				}
	//			}
	//		}
	//	}
	//}
	//else if (entry)
	//{
	//	// do entry stuff.
	//}
}

LegacyEventsCreatureState const* LegacyEventMgr::GetEventCreatureState(uint32 id)
{
	LegacyEventsCreatureStateDefine::const_iterator itr = _legacyEventsCreatureStateDefine.find(id);
	if (itr != _legacyEventsCreatureStateDefine.end())
		return &(itr->second);
	return NULL;
}

LegacyEventsCreatureSpawnInfo const* LegacyEventMgr::GetSpawnInfo(uint32 id)
{
	LegacyEventsSpawnInfos::const_iterator itr = _legacyEventsSpawnInfo.find(id);
	if (itr != _legacyEventsSpawnInfo.end())
		return &(itr->second);
	return NULL;
}

void LegacyEventMgr::UpdateQuestRelations(uint32 creatureEntry, uint32 quests)
{
	// quest > 0 : add quest from creature
	// quest < 0 : remove quest from creature
	// Get creature entry
	CreatureTemplate const* data = sObjectMgr->GetCreatureTemplate(creatureEntry);
	if (!data) // creature info not found
		return;

	QuestRelations* CreatureQuestMap = sObjectMgr->GetCreatureQuestRelationMap();

	LegacyEventsInfoList::const_iterator bitr = _legacyEventsQuestInfo.find(quests);
	if (bitr == _legacyEventsQuestInfo.end()) // nothing found, return
		return;

	LegacyEventsInfoList::const_iterator eitr = _legacyEventsQuestInfo.upper_bound(quests);

	for (; bitr != eitr; ++bitr)
	{
		int32 quest = bitr->second;

		if (!quest) // typo maybe
			continue;;

		if (quest > 0)
		{
			CreatureQuestMap->insert(QuestRelations::value_type(creatureEntry, quest));
		}
		else
		{
			// Remove the pair(id, quest) from the multimap
			QuestRelations::iterator qitr = CreatureQuestMap->find(creatureEntry);
			if (qitr == CreatureQuestMap->end())
				continue;
			QuestRelations::iterator lastElement = CreatureQuestMap->upper_bound(creatureEntry);
			for (; qitr != lastElement; ++qitr)
			{
				if (qitr->second == -quest)
				{
					CreatureQuestMap->erase(qitr);          // iterator is now no more valid
					break;                                  // but we can exit loop since the element is found
				}
			}
		}
	}
}

void LegacyEventMgr::UpdateVentorInfos(uint32 creatureEntry, uint32 items)
{
	// item > 0 : add item from vendorlist
	// item < 0 : remove item from vendorlist
	// Get creature entry
	CreatureTemplate const* data = sObjectMgr->GetCreatureTemplate(creatureEntry);
	if (!data) // creature info not found
		return;

	LegacyEventsVendorInfoList::const_iterator bitr = _legacyEventsVendorInfo.find(items);
	if (bitr == _legacyEventsVendorInfo.end()) // nothing found, return
		return;

	LegacyEventsVendorInfoList::const_iterator eitr = _legacyEventsVendorInfo.upper_bound(items);

	for (; bitr != eitr; ++bitr)
	{
		int32 item = bitr->second.item;

		if (!item)
			continue;

		if (item > 0)
		{
			sObjectMgr->AddVendorItem(creatureEntry, item, bitr->second.maxCount, bitr->second.incrTime, bitr->second.extendedConst, false);
		}
		else
		{
			sObjectMgr->RemoveVendorItem(creatureEntry, -item, false);
		}
	}
}

uint32 LegacyEventMgr::GetGossipMenuId(Creature* c)
{
	if (uint32 gossip = GetLastGossipMenuForCreature(c->GetCreatureTemplate()->Entry))
		return gossip;
	else
		return c->GetCreatureTemplate()->GossipMenuId;
}

uint32 LegacyEventMgr::GetLastGossipMenuForCreature(uint32 entry)
{
	LegacyGossips::iterator itr = _legacyGossips.find(entry);
	if (itr != _legacyGossips.end())
		return itr->second;
	else
		return 0;
}

void LegacyEventMgr::BroadcastWorldState(Map* map, uint32 field, uint32 data)
{
    Map::PlayerList const& players = map->GetPlayers();

    if (!players.isEmpty())
    {
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* player = itr->GetSource())
                player->SendUpdateWorldState(field, data);
        }
    }
}

//Excludes: (Check Player::SendInitWorldStates)
//139 : Eastern Pleaguelands
//1377 : Silithus
//2597 : Alterac Vallay
//3277 : Warsong Gulch
//3358 : Arathi Basin
//3820 : Eye of Storm
//3483 : Hellfire Peninsula
//3518 : Nagrand
//3519 : Terokka Forest
//3521 : Zangarmarsh
//3698 : Nagrand Arena
//3702 : Blade's Edge Arena
//3968 : Ruins of Lordaeron
//4378 : Dalaran Sewers
//4384 : Strand of the Ancients
//4406 : Ring of Valor
//4710 : Isle of Conquest
//4987 : Ruby Sanctum
//4812 : Icecrown Citadel
//4100 : Culling of Stratholme
//4228 : The Oculus
//4273 : Ulduar
//4820 : Halls of Reflection
//4197 : Wintergrasp

enum LegacyZoneId
{
    ZONE_ELWYNN_FOREST         = 12
};
void LegacyEventMgr::FillZoneStatePocket(WorldPacket* data, uint32 zone_id)
{
    switch (zone_id)
    {
        case ZONE_ELWYNN_FOREST: // Elwynn Forest
            *data << uint32(6000) << uint32(1);
            *data << uint32(6001) << uint32(50);
            *data << uint32(6002) << uint32(50);
            break;
        default:
            break;
    }
}