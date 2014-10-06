#ifndef _LEGACYSCRIPT_H
#define _LEGACYSCRIPT_H

#include "LegacyDefines.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "LegacyScriptEngine.h"
#include "LegacyScriptLoader.h"

class LegacyCreatureScript : public CreatureScript
{
public:
    LegacyCreatureScript(const char* name, LegacyScriptData _data) : CreatureScript(name), ScriptData(_data) { }
    ~LegacyCreatureScript() { }

    struct LegacyAI : public ScriptedAI
    {
        LegacyAI(Creature* c, LegacyScriptData _Data) : ScriptedAI(c), _Self(c), _ScriptData(_Data) { }

        void EnterCombat(Unit* who) override
        {
            xEngine->ExecuteOn(LEGACY_TRIGGER_ENTER_COMBAT, _ScriptData, _Self, who);
        }

        void UpdateAI(uint32 diff) override
        {
            xEngine->ExecuteOn(LEGACY_TRIGGER_UPDATE, _ScriptData, _Self);
            xEngine->UpdateAI(_ScriptData, diff); // for delayed event triggers
            if (_Self->IsInCombat())
                xEngine->ExecuteOn(LEGACY_TRIGGER_UPDATE_IN_COMBAT, _ScriptData, _Self);
        }

        // Called at any Damage from any attacker (before damage apply)
        void DamageTaken(Unit* attacker, uint32& damage) override { }

        //Called at creature death
        void JustDied(Unit* killer) override 
        {
            xEngine->ExecuteOn(LEGACY_TRIGGER_DEAD, _ScriptData, _Self, killer);
        }

        //Called at creature killing another unit
        void KilledUnit(Unit* victim) override 
        {
            xEngine->ExecuteOn(LEGACY_TRIGGER_KILLED, _ScriptData, _Self, victim);
        }

        // Called when the creature summon successfully other creature
        void JustSummoned(Creature* summon) override 
        {
            xEngine->ExecuteOn(LEGACY_TRIGGER_SUMMONED_CREATURE, _ScriptData, _Self, summon);
        }

        // Called when a summoned creature is despawned
        void SummonedCreatureDespawn(Creature* summon) override 
        {
            xEngine->ExecuteOn(LEGACY_TRIGGER_SUMMONED_CREATURE_DESPAWNED, _ScriptData, _Self, summon);
        }

        // Called when hit by a spell
        void SpellHit(Unit* caster, SpellInfo const* spell) override 
        {
            // we need some condition implements here.
        }

        // Called when spell hits a target
        void SpellHitTarget(Unit* target, SpellInfo const* spell) override 
        {

        }

        //Called at waypoint reached or PointMovement end
        void MovementInform(uint32 type, uint32 id) override 
        {

        }

        // Called when AI is temporarily replaced or put back when possess is applied or removed
        void OnPossess(bool apply) 
        {

        }

        LegacyScriptData _ScriptData;
        Creature* _Self;
    };

    CreatureAI* GetAI(Creature* c) const override
    {
        LegacyAI *ai = new LegacyAI(c, ScriptData);
        return ai;
    }
private:
    LegacyScriptData ScriptData;
};

class LegacyScriptMgr
{
public:
    static LegacyScriptMgr* instance()
    {
        static LegacyScriptMgr instance;
        return &instance;
    }
private:
    LegacyScriptMgr();
    ~LegacyScriptMgr();
};

#define xScriptMgr LegacyScriptMgr::instance()

#endif // !_LEGACYSCRIPT_H
