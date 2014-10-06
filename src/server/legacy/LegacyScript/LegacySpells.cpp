#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "LegacyScriptLoader.h"

/************************************************************************/
/*                       WARRIOR SPELLS                                 */
/************************************************************************/
// 6343 - Thunder Clap
class legacy_spell_warr_thunderclap : public SpellScriptLoader
{
public:
    legacy_spell_warr_thunderclap() : SpellScriptLoader("legacy_spell_warr_thunderclap") { }

    class legacy_spell_warr_thunderclap_SpellScript : public SpellScript
    {
        PrepareSpellScript(legacy_spell_warr_thunderclap_SpellScript);

        void OnSpellHit()
        {
            Unit* victim = GetHitUnit();
            if (victim->HasAuraWithMechanic(1 << MECHANIC_BLEED))
                GetCaster()->CastSpell(victim, 80872);
        }

        void Register() override
        {
            OnHit += SpellHitFn(legacy_spell_warr_thunderclap_SpellScript::OnSpellHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new legacy_spell_warr_thunderclap_SpellScript;
    }
};

// -81257 - Bloodthirst
class legacy_spell_warr_bloodthirst : public SpellScriptLoader
{
public:
    legacy_spell_warr_bloodthirst() : SpellScriptLoader("legacy_spell_warr_bloodthirst") { }

    class legacy_spell_warr_bloodthirst_SpellScript : public SpellScript
    {
        PrepareSpellScript(legacy_spell_warr_bloodthirst_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            int32 damage = GetEffectValue();
            GetCaster()->CastCustomSpell(GetCaster(), 81266, &damage, NULL, NULL, true, NULL);
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(legacy_spell_warr_bloodthirst_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new legacy_spell_warr_bloodthirst_SpellScript();
    }
};

// 81266 - Bloodthirst Heal Trigger
class legacy_spell_warr_bloodthirst_heal_trigger : public SpellScriptLoader
{
public:
    legacy_spell_warr_bloodthirst_heal_trigger() : SpellScriptLoader("legacy_spell_warr_bloodthirst_heal_trigger") { }

    class legacy_spell_warr_bloodthirst_heal_trigger_AuraScript : public AuraScript
    {
        PrepareAuraScript(legacy_spell_warr_bloodthirst_heal_trigger_AuraScript);

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
        {
            PreventDefaultAction();
            uint32 dmg = procInfo.GetDamageInfo()->GetDamage();
            uint32 factor = aurEff->GetAmount();
            int32 heal = CalculatePct(dmg, factor);
            heal = std::max(heal, int32(GetCaster()->CountPctFromMaxHealth(1)));
            heal = std::min(heal, int32(GetCaster()->CountPctFromMaxHealth(5)));
            GetCaster()->CastCustomSpell(GetCaster(), 81267, &heal, NULL, NULL, true, NULL);
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(legacy_spell_warr_bloodthirst_heal_trigger_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new legacy_spell_warr_bloodthirst_heal_trigger_AuraScript();
    }
};

// -81413 - Execute
class legacy_spell_warr_execute : public SpellScriptLoader
{
public:
    legacy_spell_warr_execute() : SpellScriptLoader("legacy_spell_warr_execute") { }

    class legacy_spell_warr_execute_SpellScript : public SpellScript
    {
        PrepareSpellScript(legacy_spell_warr_execute_SpellScript);

        void HandleEffect(SpellEffIndex effIndex)
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetHitUnit())
            {
                SpellInfo const* spellInfo = GetSpellInfo();
                int32 powerUsed = std::min<int32>(caster->GetMaxPower(POWER_MANA) - spellInfo->CalcPowerCost(caster, SpellSchoolMask(spellInfo->SchoolMask)), caster->GetPower(POWER_MANA));
                int32 newPower = std::max<int32>(0, caster->GetPower(POWER_MANA) - powerUsed);
                caster->SetPower(POWER_MANA, uint32(newPower));
                int32 bp = caster->GetTotalAttackPowerValue(BASE_ATTACK) * spellInfo->Effects[0].CalcValue() * CalculatePct(powerUsed, spellInfo->Effects[1].CalcValue());
                caster->CastCustomSpell(target, 81422, &bp, NULL, NULL, true, NULL, NULL, GetOriginalCaster()->GetGUID());
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(legacy_spell_warr_execute_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new legacy_spell_warr_execute_SpellScript();
    }
};

// -81450 - Deep Wounds
class legacy_spell_warr_deep_wounds : public SpellScriptLoader
{
public:
    legacy_spell_warr_deep_wounds() : SpellScriptLoader("legacy_spell_warr_deep_wounds") { }

    class legacy_spell_warr_deep_wounds_AuraScript : public AuraScript
    {
        PrepareAuraScript(legacy_spell_warr_deep_wounds_AuraScript);

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& procInfo)
        {
            int32 damage = procInfo.GetDamageInfo()->GetDamage();
            Unit* caster = GetCaster();
            if (Unit* target = procInfo.GetActionTarget())
            {
                ApplyPct(damage, aurEff->GetAmount());
                caster->CastCustomSpell(target, 81459, &damage, NULL, NULL, true);
            }
        }

        void Register() override
        {
            OnEffectProc += AuraEffectProcFn(legacy_spell_warr_deep_wounds_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new legacy_spell_warr_deep_wounds_AuraScript();
    }
};

// -81460 - Sweeping Strikes
class legacy_spell_warr_sweeping_strikes : public SpellScriptLoader
{
public:
    legacy_spell_warr_sweeping_strikes() : SpellScriptLoader("legacy_spell_warr_sweeping_strikes") { }

    class legacy_spell_warr_sweeping_strikes_AuraScript : public AuraScript
    {
        PrepareAuraScript(legacy_spell_warr_sweeping_strikes_AuraScript);

        bool Load() override
        {
            _procTarget = NULL;
            return true;
        }

        bool CheckProc(ProcEventInfo& eventInfo)
        {
            _procTarget = eventInfo.GetActor()->SelectNearbyTarget(eventInfo.GetProcTarget());
            return _procTarget != nullptr;
        }

        void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            int32 damage = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
            GetTarget()->CastCustomSpell(81469, SPELLVALUE_BASE_POINT0, damage, _procTarget, true, NULL, aurEff);
        }

        void Register() override
        {
            DoCheckProc += AuraCheckProcFn(legacy_spell_warr_sweeping_strikes_AuraScript::CheckProc);
            OnEffectProc += AuraEffectProcFn(legacy_spell_warr_sweeping_strikes_AuraScript::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        }

    private:
        Unit* _procTarget;
    };

    AuraScript* GetAuraScript() const override
    {
        return new legacy_spell_warr_sweeping_strikes_AuraScript();
    }
};

// -81653 - Rend
class legacy_spell_warr_rend : public SpellScriptLoader
{
public:
    legacy_spell_warr_rend() : SpellScriptLoader("legacy_spell_warr_rend") { }

    class legacy_spell_warr_rend_AuraScript : public AuraScript
    {
        PrepareAuraScript(legacy_spell_warr_rend_AuraScript);

        void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
        {
            if (Unit* caster = GetCaster())
            {
                canBeRecalculated = false;
                float ap = caster->GetTotalAttackPowerValue(BASE_ATTACK);
                float weapon_dmg = caster->GetWeaponDamageRange(BASE_ATTACK, MAXDAMAGE);
                int32 weapon_speed = caster->GetAttackTime(BASE_ATTACK);
                weapon_dmg += ap * weapon_speed / 10000;
                amount = CalculatePct(weapon_dmg, GetSpellInfo()->Effects[EFFECT_1].CalcValue());
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(legacy_spell_warr_rend_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new legacy_spell_warr_rend_AuraScript();
    }
};

// -81716 - Last Stand
class legacy_spell_warr_last_stand : public SpellScriptLoader
{
public:
    legacy_spell_warr_last_stand() : SpellScriptLoader("legacy_spell_warr_last_stand") { }

    class legacy_spell_warr_last_stand_SpellScript : public SpellScript
    {
        PrepareSpellScript(legacy_spell_warr_last_stand_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            int32 healthModSpellBasePoints0 = int32(caster->CountPctFromMaxHealth(GetEffectValue()));
            caster->CastCustomSpell(caster, 12976, &healthModSpellBasePoints0, NULL, NULL, true, NULL);
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(legacy_spell_warr_last_stand_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new legacy_spell_warr_last_stand_SpellScript();
    }
};

// -81507 - Intimidating Shout
class legacy_spell_warr_intimidating_shout : public SpellScriptLoader
{
public:
    legacy_spell_warr_intimidating_shout() : SpellScriptLoader("legacy_spell_warr_intimidating_shout") { }

    class legacy_spell_warr_intimidating_shout_SpellScript : public SpellScript
    {
        PrepareSpellScript(legacy_spell_warr_intimidating_shout_SpellScript);

        void FilterTargets(std::list<WorldObject*>& unitList)
        {
            unitList.remove(GetExplTargetWorldObject());
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(legacy_spell_warr_intimidating_shout_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(legacy_spell_warr_intimidating_shout_SpellScript::FilterTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new legacy_spell_warr_intimidating_shout_SpellScript();
    }
};

/************************************************************************/
/*                       WARLOCK SPELLS                                 */
/************************************************************************/

// 1949 - Hellflame
class legacy_spell_warl_hellflame : public SpellScriptLoader
{
public:
    legacy_spell_warl_hellflame() : SpellScriptLoader("legacy_spell_warl_hellflame") { }

    class legacy_spell_warl_hellflame_AuraScript : public AuraScript
    {
        PrepareAuraScript(legacy_spell_warl_hellflame_AuraScript);

        void OnPeriodic(AuraEffect const* aurEff)
        {
            PreventDefaultAction();
            int32 self_damage = CalculatePct(GetCaster()->GetMaxHealth(), GetSpellInfo()->Effects[0].CalcValue(GetCaster()));
            int32 aoe_damage = self_damage;
            GetCaster()->CastCustomSpell(GetCaster(), 5857, &self_damage, &aoe_damage, NULL, true);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(legacy_spell_warl_hellflame_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new legacy_spell_warl_hellflame_AuraScript();
    }
};

// 27243 - Seed of Corruption
class legacy_spell_warl_seed_of_corruption_dot : public SpellScriptLoader
{
public:
    legacy_spell_warl_seed_of_corruption_dot() : SpellScriptLoader("legacy_spell_warl_seed_of_corruption_dot") { }

    class legacy_spell_warl_seed_of_corruption_dot_AuraScript : public AuraScript
    {
        PrepareAuraScript(legacy_spell_warl_seed_of_corruption_dot_AuraScript);

        void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            canBeRecalculated = false;

            if (Unit* target = GetTarget())
            {
                amount = target->CountPctFromCurHealth(20);
            }
        }

        void Register() override
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(legacy_spell_warl_seed_of_corruption_dot_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new legacy_spell_warl_seed_of_corruption_dot_AuraScript();
    }
};

/************************************************************************/
/*                         ROGUE SPELLS                                 */
/************************************************************************/
// 80906
class legacy_spell_rog_relentless_strike : public SpellScriptLoader
{
public:
    legacy_spell_rog_relentless_strike() : SpellScriptLoader("legacy_spell_rog_relentless_strike") { }

    class legacy_spell_rog_relentless_strike_SpellScript : public SpellScript
    {
        PrepareSpellScript(legacy_spell_rog_relentless_strike_SpellScript);

        void OnSpellCast()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                uint8 cp = player->GetComboPoints();
                int32 chance = 100;
                int32 dmg = (int32)cp + 3;
                player->CastCustomSpell(80913, SPELLVALUE_BASE_POINT1, dmg, player, TRIGGERED_FULL_MASK);
            }
        }

        void Register() override
        {
            OnCast += SpellCastFn(legacy_spell_rog_relentless_strike_SpellScript::OnSpellCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new legacy_spell_rog_relentless_strike_SpellScript();
    }
};

// 13750 Impulse
class legacy_spell_rog_impulse : public SpellScriptLoader
{
public:
    legacy_spell_rog_impulse() : SpellScriptLoader("legacy_spell_rog_impulse") { }

    class legacy_spell_rog_impulse_SpellScript : public SpellScript
    {
        PrepareSpellScript(legacy_spell_rog_impulse_SpellScript);

        void AfterSpellCast()
        {
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (Aura* aura = player->GetAura(80914))
                {
                    if (aura->GetStackAmount() < 3) // clear cooldown
                    {
                        player->RemoveSpellCooldown(13750, true);
                        player->AddSpellCooldown(13750, 0, time(NULL) + 1);
                    }
                    else
                        aura->Remove();
                }
            }
        }

        void Register() override
        {
            AfterCast += SpellCastFn(legacy_spell_rog_impulse_SpellScript::AfterSpellCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new legacy_spell_rog_impulse_SpellScript();
    }
};

/************************************************************************/
/*                         PRIEST SPELLS                                */
/************************************************************************/
// 17 - Power Word: Shield (legacy version)
class legacy_spell_pri_power_word_shield : public SpellScriptLoader
{
public:
    legacy_spell_pri_power_word_shield() : SpellScriptLoader("legacy_spell_pri_power_word_shield") { }

    class legacy_spell_pri_power_word_shield_AuraScript : public AuraScript
    {
        PrepareAuraScript(legacy_spell_pri_power_word_shield_AuraScript);

        void OnAbsorb(AuraEffect* /*aurEff*/, DamageInfo & /*dmgInfo*/, uint32 & /*absorbAmount*/)
        {
            if (GetCaster()->HasAura(81085))
            {
                if (Player* p = GetCaster()->ToPlayer())
                {
                    if (!p->HasSpellCooldown(81084))
                    {
                        p->CastSpell(p, 81084, true);
                        p->AddSpellCooldown(81084, 0, time(NULL) + 6);
                    }
                }
            }
        }

        void Register() override
        {
            OnEffectAbsorb += AuraEffectAbsorbFn(legacy_spell_pri_power_word_shield_AuraScript::OnAbsorb, EFFECT_0);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new legacy_spell_pri_power_word_shield_AuraScript();
    }
};

// itemset 929 - defendant's faith
class legacy_spell_pri_defendant_faith : public SpellScriptLoader
{
public:
    legacy_spell_pri_defendant_faith() : SpellScriptLoader("legacy_spell_pri_defendant_faith") { }

    class legacy_spell_pri_defendant_faith_AuraScript : public AuraScript
    {
        PrepareAuraScript(legacy_spell_pri_defendant_faith_AuraScript);

        void OnTrigger(ProcEventInfo& eventInfo)
        {
            PreventDefaultAction();
            // check aura state - 81087-flag 81088-effect
            Unit* caster = GetCaster();
            if (caster->HasAura(81088))
                return;

            if (caster->HasAura(81087))
            {
                if (Aura* aura = caster->GetAura(81087))
                {
                    if (aura->GetStackAmount() >= 3)
                    {
                        aura->Remove();
                        caster->AddAura(81088, caster);
                    }
                    else
                        aura->SetStackAmount(aura->GetStackAmount() + 1);
                }
            }
            else
                caster->AddAura(81087, caster);
        }

        void Register() override
        {
            OnProc += AuraProcFn(legacy_spell_pri_defendant_faith_AuraScript::OnTrigger);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new legacy_spell_pri_defendant_faith_AuraScript();
    }
};

/************************************************************************/
/*                        PALADIN SPELLS                                */
/************************************************************************/

// 80926 - Judgement
enum JudgementSpells
{
    SEAL_COMMAND = 80927,
    SEAL_VENGENCE = 80930,
    SEAL_LIGHT = 80932,
    SEAL_WISDOM = 80934,
    SEAL_JUSTICE = 80936,
    SEAL_MARTYR = 80946,
    JUDGEMENT_COMMAND = 80929,
    JUDGEMENT_VENGENCE = 80931,
    JUDGEMENT_LIGHT = 80933,
    JUDGEMENT_WISDOM = 80935,
    JUDGEMENT_JUSTICE = 80937,
    JUDGEMENT_MARTYR = 80939
};

class legacy_spell_pal_judgement : public SpellScriptLoader
{
public:
    legacy_spell_pal_judgement() : SpellScriptLoader("legacy_spell_pal_judgement") { }

    class legacy_spell_pal_judgement_SpellScript : public SpellScript
    {
        PrepareSpellScript(legacy_spell_pal_judgement_SpellScript);

        void OnHitTarget()
        {
            Unit* caster = GetCaster();
            Unit* victim = GetHitUnit();

            if (victim && victim->IsAlive())
            {
                if (caster->HasAura(SEAL_COMMAND)) // seal of command
                {
                    caster->RemoveAurasDueToSpell(SEAL_COMMAND, caster->GetGUID()); // currrent seal

                    victim->RemoveAurasDueToSpell(JUDGEMENT_VENGENCE, caster->GetGUID());
                    victim->RemoveAurasDueToSpell(JUDGEMENT_LIGHT, caster->GetGUID());
                    victim->RemoveAurasDueToSpell(JUDGEMENT_WISDOM, caster->GetGUID());
                    victim->RemoveAurasDueToSpell(JUDGEMENT_JUSTICE, caster->GetGUID());
                    caster->CastSpell(victim, JUDGEMENT_COMMAND);
                    caster->RemoveAurasDueToSpell(SEAL_COMMAND);
                }
                else if (caster->HasAura(SEAL_VENGENCE)) // seal of vengence
                {
                    caster->RemoveAurasDueToSpell(SEAL_VENGENCE, caster->GetGUID()); // currrent seal

                    victim->RemoveAurasDueToSpell(JUDGEMENT_LIGHT, caster->GetGUID());
                    victim->RemoveAurasDueToSpell(JUDGEMENT_WISDOM, caster->GetGUID());
                    victim->RemoveAurasDueToSpell(JUDGEMENT_JUSTICE, caster->GetGUID());
                    caster->CastSpell(victim, JUDGEMENT_VENGENCE);
                    caster->RemoveAurasDueToSpell(SEAL_VENGENCE);
                }
                else if (caster->HasAura(SEAL_LIGHT)) // seal of light
                {
                    caster->RemoveAurasDueToSpell(SEAL_LIGHT, caster->GetGUID()); // currrent seal

                    victim->RemoveAurasDueToSpell(JUDGEMENT_VENGENCE, caster->GetGUID());
                    victim->RemoveAurasDueToSpell(JUDGEMENT_WISDOM, caster->GetGUID());
                    victim->RemoveAurasDueToSpell(JUDGEMENT_JUSTICE, caster->GetGUID());
                    caster->CastSpell(victim, JUDGEMENT_LIGHT);
                    caster->RemoveAurasDueToSpell(SEAL_LIGHT);
                }
                else if (caster->HasAura(SEAL_WISDOM)) // seal of wisdom
                {
                    caster->RemoveAurasDueToSpell(SEAL_WISDOM, caster->GetGUID()); // currrent seal

                    victim->RemoveAurasDueToSpell(JUDGEMENT_VENGENCE, caster->GetGUID());
                    victim->RemoveAurasDueToSpell(JUDGEMENT_LIGHT, caster->GetGUID());
                    victim->RemoveAurasDueToSpell(JUDGEMENT_JUSTICE, caster->GetGUID());
                    caster->CastSpell(victim, JUDGEMENT_WISDOM);
                    caster->RemoveAurasDueToSpell(SEAL_WISDOM);
                }
                else if (caster->HasAura(SEAL_JUSTICE)) // seal of justice
                {
                    caster->RemoveAurasDueToSpell(SEAL_JUSTICE, caster->GetGUID()); // currrent seal

                    victim->RemoveAurasDueToSpell(JUDGEMENT_VENGENCE, caster->GetGUID());
                    victim->RemoveAurasDueToSpell(JUDGEMENT_LIGHT, caster->GetGUID());
                    victim->RemoveAurasDueToSpell(JUDGEMENT_WISDOM, caster->GetGUID());
                    caster->CastSpell(victim, JUDGEMENT_JUSTICE);
                    caster->RemoveAurasDueToSpell(SEAL_JUSTICE);
                }
                else if (caster->HasAura(SEAL_MARTYR)) // seal of martyr
                {
                    caster->RemoveAurasDueToSpell(SEAL_MARTYR, caster->GetGUID()); // currrent seal

                    victim->RemoveAurasDueToSpell(JUDGEMENT_VENGENCE, caster->GetGUID());
                    victim->RemoveAurasDueToSpell(JUDGEMENT_LIGHT, caster->GetGUID());
                    victim->RemoveAurasDueToSpell(JUDGEMENT_WISDOM, caster->GetGUID());
                    victim->RemoveAurasDueToSpell(JUDGEMENT_JUSTICE, caster->GetGUID());
                    caster->CastSpell(caster, JUDGEMENT_MARTYR); // to self.
                    caster->RemoveAurasDueToSpell(SEAL_MARTYR);
                }
            }
        }

        void Register() override
        {
            AfterHit += SpellHitFn(legacy_spell_pal_judgement_SpellScript::OnHitTarget);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new legacy_spell_pal_judgement_SpellScript();
    }
};

// 80938 - Martyrize
class legacy_spell_pal_martyrize : public SpellScriptLoader
{
public:
    legacy_spell_pal_martyrize() : SpellScriptLoader("legacy_spell_pal_martyrize") { }

    class legacy_spell_pal_martyrize_SpellScript : public SpellScript
    {
        PrepareSpellScript(legacy_spell_pal_martyrize_SpellScript);

        SpellCastResult CheckCast()
        {
            Unit* caster = GetCaster();
            bool has_martyr_seal = GetCaster()->HasAura(80939);
            bool can_use_martyrize = false;
            max_duration = 0;

            if (has_martyr_seal)
            {
                Unit::AuraEffectList const& stun_auras = GetCaster()->GetAuraEffectsByType(SPELL_AURA_MOD_STUN);
                for (Unit::AuraEffectList::const_iterator i = stun_auras.begin(); i != stun_auras.end(); ++i)
                {
                    if ((*i)->GetBase()->GetMaxDuration() - (*i)->GetBase()->GetDuration() > 1000)
                    {
                        max_duration = std::max((*i)->GetBase()->GetDuration(), max_duration);
                        can_use_martyrize = true;
                    }
                }

                Unit::AuraEffectList const& fear_auras = GetCaster()->GetAuraEffectsByType(SPELL_AURA_MOD_FEAR);
                for (Unit::AuraEffectList::const_iterator i = fear_auras.begin(); i != fear_auras.end(); ++i)
                {
                    if ((*i)->GetBase()->GetMaxDuration() - (*i)->GetBase()->GetDuration() > 1000)
                    {
                        max_duration = std::max((*i)->GetBase()->GetDuration(), max_duration);
                        can_use_martyrize = true;
                    }
                }

                Unit::AuraEffectList const& confuse_auras = GetCaster()->GetAuraEffectsByType(SPELL_AURA_MOD_CONFUSE);
                for (Unit::AuraEffectList::const_iterator i = confuse_auras.begin(); i != confuse_auras.end(); ++i)
                {
                    if ((*i)->GetBase()->GetMaxDuration() - (*i)->GetBase()->GetDuration() > 1000)
                    {
                        max_duration = std::max((*i)->GetBase()->GetDuration(), max_duration);
                        can_use_martyrize = true;
                    }
                }
            }

            uint32 caster_unitflag = caster->GetUInt32Value(UNIT_FIELD_FLAGS);
            if (caster_unitflag & UNIT_FLAG_STUNNED)
            {
                if (can_use_martyrize)
                    return SPELL_CAST_OK;
                else
                    return SPELL_FAILED_STUNNED;
            }
            else if (caster_unitflag & UNIT_FLAG_CONFUSED)
            {
                if (can_use_martyrize)
                    return SPELL_CAST_OK;
                else
                    return SPELL_FAILED_CONFUSED;
            }
            else if (caster_unitflag & UNIT_FLAG_FLEEING)
            {
                if (can_use_martyrize)
                    return SPELL_CAST_OK;
                else
                    return SPELL_FAILED_FLEEING;
            }

            return SPELL_CAST_OK;
        }

        void OnSpellCast()
        {
            Unit* caster = GetCaster();
            // normal cast
            if (!(caster->GetUInt32Value(UNIT_FIELD_FLAGS) & UNIT_FLAG_STUNNED) &&
                !(caster->GetUInt32Value(UNIT_FIELD_FLAGS) & UNIT_FLAG_FLEEING) &&
                !(caster->GetUInt32Value(UNIT_FIELD_FLAGS) & UNIT_FLAG_CONFUSED))
                caster->CastSpell(caster, 80946);
            else if (max_duration > 0)
            {
                // damage = 5% + life * duration sec * 2%
                int32 damage = std::min(CalculatePct(caster->GetMaxHealth(), 5 + max_duration / 500), caster->GetHealth() - caster->CountPctFromMaxHealth(10));
                caster->RemoveAurasByType(SPELL_AURA_MOD_CONFUSE);
                caster->RemoveAurasByType(SPELL_AURA_MOD_STUN);
                caster->RemoveAurasByType(SPELL_AURA_MOD_FEAR);
                if (damage > 0)
                    caster->CastCustomSpell(caster, 80945, &damage, NULL, NULL, true);
                caster->CastSpell(caster, 80946);
            }
            else
                return;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(legacy_spell_pal_martyrize_SpellScript::CheckCast);
            OnCast += SpellCastFn(legacy_spell_pal_martyrize_SpellScript::OnSpellCast);
        }

    private:
        int32 max_duration;
    };

    SpellScript* GetSpellScript() const override
    {
        return new legacy_spell_pal_martyrize_SpellScript();
    }
};

void LegacyScriptLoader::LoadSpellScripts()
{
    new legacy_spell_warr_thunderclap();
    new legacy_spell_warr_bloodthirst();
    new legacy_spell_warr_bloodthirst_heal_trigger();
    new legacy_spell_warr_execute();
    new legacy_spell_warr_deep_wounds();
    new legacy_spell_warr_sweeping_strikes();
    new legacy_spell_warr_rend();
    new legacy_spell_warr_last_stand();
    new legacy_spell_warr_intimidating_shout();

    new legacy_spell_warl_hellflame();
    new legacy_spell_warl_seed_of_corruption_dot();

    new legacy_spell_rog_relentless_strike();
    new legacy_spell_rog_impulse();

    new legacy_spell_pri_power_word_shield();
    new legacy_spell_pri_defendant_faith();

    new legacy_spell_pal_judgement();
    new legacy_spell_pal_martyrize();
}