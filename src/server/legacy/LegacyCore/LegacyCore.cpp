/*
* Copyright (C) 2012-2014 ProjectLegacy <http://msacn.org/>
* Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
*/

#include "LegacyCore.h"

LegacyCore::LegacyCore() { }
LegacyCore::~LegacyCore() { }

float LegacyCore::CalcXDiminish(float value, float interval) const
{
    if (value <= interval)
        return value;
    else if (value <= interval * 2.0f)
        return (interval + value) * 0.5f;
    else if (value <= interval * 3.0f)
        return interval + value * 0.25f;
    else
        return interval * 1.375f + value * 0.125f;
}

float LegacyCore::CalcAgility2WeaponCrit(float agility) const
{
    return agility / (100.0f + agility) * 40.0f;
}

float LegacyCore::CalcAgility2Dodge(float agility) const
{
    return agility / (100.0f + agility) * 30.0f;
}

float LegacyCore::CalcSpirit2SpellCrit(float spirit) const
{
    return spirit / (100.0f + spirit) * 40.0f;
}

float LegacyCore::CalcRatingMultiplier(CombatRating cr) const
{
    return 1.0f;
}

bool LegacyCore::IsEnabled(LegacyEnableType type, uint32 entry) const
{
    LegacyEnableContainer::const_iterator itr = _enableContainer.find(type);
    while (itr != _enableContainer.end())
    {
        if (itr->second.entry == entry)
            return itr->second.enabled;
        ++itr;
    }
    return false;
}

void LegacyCore::Load()
{
    _enableContainer.clear();
    QueryResult result = WorldDatabase.Query("SELECT type, id, enabled FROM legacy_enables");
    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            LegacyEnableType type = (LegacyEnableType)fields[0].GetUInt8();
            LegacyEnableInfo info;
            info.entry = fields[1].GetUInt32();
            info.enabled = fields[2].GetUInt8() == 0 ? false : true;
            _enableContainer[type] = info;
        } while (result->NextRow());
    }

    _playerData.clear();
    result = CharacterDatabase.Query("SELECT `guid`, `set`, `data` FROM legacy_character_data");
    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            uint32 guid = fields[0].GetUInt32();
            uint32 set = fields[1].GetUInt32();
            uint32 data = fields[2].GetUInt32();

            LegacyPlayerData::iterator itr = _playerData.find(guid);
            if (itr == _playerData.end())
            {
                LegacyPlayerDataSet ds;
                ds[set] = data;
                _playerData[guid] = ds;
            }
            else
                (itr->second)[set] = data;
        } while (result->NextRow());
    }

    _comprehendSpells.clear();
    //                                   0   1       2       3       4     5     6     7      8      9
    result = WorldDatabase.Query("SELECT id, spell1, spell2, spell3, set1, set2, set3, data1, data2, data3 FROM legacy_comprehend_spells");
    if (result)
    {
        do 
        {
            Field* fields = result->Fetch();
            LegacyComprehendSpell info;
            if (fields[1].GetUInt32())
            {
                info.spell = fields[1].GetUInt32();
                info.set = fields[4].GetUInt32();
                info.data = fields[7].GetUInt32();
                _comprehendSpells.insert(std::pair<uint32, LegacyComprehendSpell>(fields[0].GetUInt32(), info));
            }
            
            if (fields[2].GetUInt32())
            {
                info.spell = fields[2].GetUInt32();
                info.set = fields[5].GetUInt32();
                info.data = fields[8].GetUInt32();
                _comprehendSpells.insert(std::pair<uint32, LegacyComprehendSpell>(fields[0].GetUInt32(), info));
            }
            
            if (fields[3].GetUInt32())
            {
                info.spell = fields[3].GetUInt32();
                info.set = fields[6].GetUInt32();
                info.data = fields[9].GetUInt32();
                _comprehendSpells.insert(std::pair<uint32, LegacyComprehendSpell>(fields[0].GetUInt32(), info));
            }
        } while (result->NextRow());
    }
}

uint32 LegacyCore::GetPlayerData(Player* p, uint32 set) const
{
    if (!p || !set)
        return 0;

    uint32 guid = p->GetGUID();

    LegacyPlayerData::const_iterator itr = _playerData.find(guid);
    if (itr == _playerData.end())
        return 0;

    LegacyPlayerDataSet::const_iterator itr_s = (itr->second).find(set);
    if (itr_s == (itr->second).end())
        return 0;

    return itr_s->second;
}

void LegacyCore::SetPlayerData(Player* p, uint32 set, uint32 data)
{
    if (!p || !set || !data)
        return;

    uint32 guid = p->GetGUID();

    LegacyPlayerData::iterator itr = _playerData.find(guid);
    if (itr == _playerData.end())
    {
        LegacyPlayerDataSet dataset;
        dataset[set] = data;
        _playerData[guid] = dataset;
    }
    else
        (itr->second)[set] = data;

    p->UpdateForQuestWorldObjects();

    CharacterDatabase.PExecute("REPLACE INTO legacy_character_data (`guid`, `set`, `data`) VALUES (%u, %u, %u)", guid, set, data);
}

void LegacyCore::RemovePlayerData(Player* p, uint32 set)
{
    if (!p || !set)
        return;

    uint32 guid = p->GetGUID();

    LegacyPlayerData::iterator itr = _playerData.find(guid);
    if (itr == _playerData.end())
        return;
    else
    {
        LegacyPlayerDataSet::const_iterator itr_s = (itr->second).find(set);
        if (itr_s == (itr->second).end())
            return;
        else
            (itr->second).erase(set);
    }

    p->UpdateForQuestWorldObjects();

    CharacterDatabase.PExecute("DELETE FROM legacy_character_data WHERE `guid` = %u AND `set` = %u", guid, set);
}

bool LegacyCore::PlayerDataMeets(Player* p, uint32 set, uint32 data) const
{
    if (GetPlayerData(p, set) == data)
        return true;
    else
        return false;
}

void LegacyCore::ComprehendSpell(Player* p, uint32 entry)
{
    if (!p)
        return;

    LegacyComprehendSpells::const_iterator i, b, e;
    b = _comprehendSpells.lower_bound(entry);
    e = _comprehendSpells.upper_bound(entry);
    for (i = b; i != e; ++i)
    {
        if (i->second.spell != 0 && PlayerDataMeets(p, i->second.set, i->second.data))
            p->LearnSpell(i->second.spell, false);
    }
}

uint32 LegacyCore::SkillValue(Player* p, uint32 skillline)
{
    if (!p || !skillline)
        return 0;

    return p->GetSkillValue(skillline);
}

void LegacyCore::StepSkill(Player* p, uint32 skillline, uint32 step /* = 1 */)
{
    if (!p || !skillline)
        return;

    p->UpdateSkill(skillline, step);
}

void LegacyCore::UpdateSkills(Player* p, LegacySkillUpdateAction action)
{
    if (!p)
        return;

    switch (action)
    {
        case LSUA_ON_MELEE_DAMEGE:
            //UpdateMeleeSkills(p);
            break;
        case LSUA_ON_RANGED_DAMAGE:
            //UpdateRangedSkills(p);
            break;
        default:
            break;
    }
}

bool LegacyCore::IsTalentUnlocked(Player* p, const TalentEntry* talent)
{
    if (!p || !talent)
        return false;

    uint8 row = talent->Row;

    if (row == 0)
        return true;

    uint8 column = talent->Col;
    TalentTabEntry const* talentTabInfo = sTalentTabStore.LookupEntry(talent->TalentTab);

    if ((p->getClassMask() & talentTabInfo->ClassMask) == 0)
        return false;

    for (uint32 i = 0; i != sTalentStore.GetNumRows(); ++i)
    {
        if (TalentEntry const* depTalentInfo = sTalentStore.LookupEntry(i+1))
        {
            if (depTalentInfo->TalentTab == talent->TalentTab)
            {
                int32 x = std::abs((int32)(depTalentInfo->Col - talent->Col));
                int32 y = std::abs((int32)(depTalentInfo->Row - talent->Row));
                if ((x == 1 && y == 0) || (x == 0 && y == 1))
                {
                    for (uint8 j = 1; j != MAX_TALENT_RANK; ++j)
                    {
                        if (depTalentInfo->RankID[j] != 0 && p->HasSpell(depTalentInfo->RankID[j]))
                            return true;
                    }
                }
            }
        }
    }

    return false;
}