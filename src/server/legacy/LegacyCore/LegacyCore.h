/*
* Copyright (C) 2012-2014 ProjectLegacy <http://msacn.org/>
* Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
*/

#ifndef _LEGACYCORE_H
#define _LEGACYCORE_H

#include "LegacyDefines.h"
#include "LegacyEvent.h"
#include "LegacyScript.h"
#include "LegacyScriptLoader.h"
#include "LegacyScriptEngine.h"
#include "Player.h"

struct LegacyComprehendSpell
{
    uint32 spell;
    uint32 set;
    uint32 data;
};

typedef std::unordered_map<uint32, uint32> LegacyPlayerDataSet;
typedef std::unordered_map<uint32, LegacyPlayerDataSet> LegacyPlayerData;
typedef std::multimap<uint32, LegacyComprehendSpell> LegacyComprehendSpells;

class LegacyCore
{
public:
    static LegacyCore* instance()
    {
        static LegacyCore instance;
        return &instance;
    }

    // Stat System
    float CalcXDiminish(float value, float interval) const;
    float CalcAgility2WeaponCrit(float agility) const;
    float CalcAgility2Dodge(float agility) const;
    float CalcSpirit2SpellCrit(float spirit) const;
    float CalcRatingMultiplier(CombatRating cr) const;
    
    // Content Enablility
    bool IsEnabled(LegacyEnableType type, uint32 entry) const;

    // Player Data System
    uint32 GetPlayerData(Player* p, uint32 set) const;
    bool PlayerDataMeets(Player* p, uint32 set, uint32 data) const;
    void SetPlayerData(Player* p, uint32 set, uint32 data);
    void RemovePlayerData(Player* p, uint32 set);
    
    // Spell Comprehension
    void ComprehendSpell(Player* p, uint32 entry);

    // SkillLine System
    uint32 SkillValue(Player* p, uint32 skillline);
    void StepSkill(Player* p, uint32 skillline, uint32 step = 1);
    void UpdateSkills(Player* p, LegacySkillUpdateAction action);
    void UpdateMeleeSkills(Player* p);
    void UpdateRangedSkills(Player* p);

    // Talent
    bool IsTalentUnlocked(Player* p, const TalentEntry* talent);

    // Load
    void Load();
private:
    LegacyCore();
    ~LegacyCore();

    LegacyEnableContainer _enableContainer;
    LegacyPlayerData _playerData;
    LegacyComprehendSpells _comprehendSpells;
};

#define xCore LegacyCore::instance()

#endif // ! _LEGACYCORE_H
