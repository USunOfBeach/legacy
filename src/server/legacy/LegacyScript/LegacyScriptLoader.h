#ifndef _LEGACYSCRIPTLOADER_H_
#define _LEGACYSCRIPTLOADER_H_

#include "LegacyDefines.h"
#include <fstream>
#include "ScriptMgr.h"
#include <boost/spirit/include/classic.hpp>

using namespace boost::spirit::classic;

enum LegacyScriptType
{
    LEGACY_SCRIPT_TYPE_INVALID = 0,
    LEGACY_SCRIPT_TYPE_UNITSCRIPT = 1
};

enum LegacyParameterType
{
    PARAM_TYPE_INTEGER,
    PARAM_TYPE_FLOAT,
    PARAM_TYPE_BOOL
};

typedef std::unordered_map<std::string, LegacyScriptData> LegacyScriptDataMap;
typedef std::vector<std::string> ScriptStringHolder;
typedef std::map<std::string, LegacyEventData> EventIndexHolder;

struct ScriptParser : public grammar<ScriptParser>
{
    ScriptParser(LegacyScriptDataMap* d) { scriptMap = d; }

    LegacyScriptDataMap* scriptMap;
    std::string scriptName;
    void GetScriptName(std::string s)
    {
        scriptName = s;
    }

    template <typename ScannerT>
    struct definition
    {
        rule<ScannerT> script, triggerMap, trigger, triggerMethod, triggerAction, event, action, method, target, attribute, parameters, parameter;

        definition(ScriptParser const& self)
        {
            parameters = (parameter >> *(*space_p >> ',' >> *space_p >> parameter));
            parameter = (+real_p | "true" | "false");
            attribute = (str_p("Chance") | "Cooldown" | "Phase") >> *space_p >> '(' >> *space_p >> parameters >> *space_p >> ')';
            target = str_p("Self") | "Victim" | "SecondThreat";
            method = str_p("CastSpell") | "Talk" | "Kill" | "SayLine";
            action = *(target >> '.') >> method >> '(' >> parameter >> ')' >> *space_p >> *(str_p("To") >> *space_p >> target) >> ch_p(';');
            event = str_p("Event") >> *space_p >> +alnum_p >> *space_p >> *(ch_p('[') >> *(attribute >> *(*space_p >> ',' >> *space_p >> attribute)) >> ']') >> *space_p >> '{' >> *space_p >> *(*space_p >> action >> *space_p) >> '}';
            triggerMethod = str_p("ExecuteEvent");
            triggerAction = triggerMethod >> '(' >> *alpha_p >> ')' >> ch_p(';');
            trigger = (str_p("OnAggro") | "OnEnterCombat" | "OnKill") >> ':' >> *space_p >> *('(' >> *space_p >> parameter >> *space_p >> ')') >> *space_p >> '{' >> *(*space_p >> triggerAction >> *space_p) >> '}';
            triggerMap = str_p("EventBinding") >> *space_p >> '{' >> *space_p >> *trigger >> *space_p >> '}';
            script = str_p("UnitScript") >> *space_p >> +alnum_p[&GetScriptName] >> *space_p >> '{' >> *(*space_p >> event >> *space_p) >> *space_p >> +triggerMap >> *space_p >> '}';
        }

        rule<ScannerT> const& start() const { return script; }
    };
};

typedef std::map<std::string, LegacyAction> ActionDefine;
typedef std::map<std::string, LegacyTarget> TargetDefine;
typedef std::map<std::string, LegacyTrigger> TriggerDefine;
typedef std::unordered_map<uint32, LegacyTriggerMap> LegacyTriggerMaps;

class LegacyScriptLoader
{
public:
    static LegacyScriptLoader* instance()
    {
        static LegacyScriptLoader instance;
        return &instance;
    }

    bool ParseAction(std::string scripts, LegacyActionData &data);
    bool ParseEvents(std::string scripts, EventIndexHolder &holder);
    bool ParseEvent(std::string scripts, LegacyEventData &data);
    bool ParseTrigger(std::string scripts, LegacyTriggerMap &data, LegacyLinker linker);
    bool ParseScript(std::string scripts, LegacyScriptData &data);
    LegacyTarget GetTarget(std::string s);
    LegacyAction GetAction(std::string s);
    LegacyTrigger GetTrigger(std::string s);
    double GetParam(std::string s, uint32 pos = 1);

    // there is a chance that we will implement the dynamical trigger map switch functionality so we should not constant the pointer or the function itself.
    LegacyScriptData LoadScriptData(Unit* u, uint32 specifiedId = 0);

    void LoadTriggerMaps();
    std::string LoadScriptFile(std::string file);
    void LoadScripts();

    bool ValidateTarget(std::string script);

    void LoadSpellScripts();
private:
    ActionDefine ActionDefines;
    TargetDefine TargetDefines;
    TriggerDefine TriggerDefines;
    LegacyLinker EventLink; // format: TriggerName::EventName
    LegacyLinker TriggerLink; // format: TriggerName
    LegacyTriggerMaps TriggerMaps;
    LegacyEventMap EventMaps;

    void LoadDefines();
    void LoadActionDefines(std::string s, LegacyAction action) { ActionDefines[s] = action; }
    void LoadTargetDefines(std::string s, LegacyTarget target) { TargetDefines[s] = target; }
    void LoadTriggerDefines(std::string s, LegacyTrigger trigger) { TriggerDefines[s] = trigger; }
    uint32 GetLink(LegacyLinker links, std::string name) const;

    // parsing rules
    rule<> rParameters = (rParameter >> *(*space_p >> ',' >> *space_p >> rParameter));
    rule<> rParameter = (+real_p | "true" | "false");
    rule<> rAttribute = (str_p("Chance") | "Cooldown" | "Phase") >> *space_p >> '(' >> *space_p >> rParameters >> *space_p >> ')';
    rule<> rTarget = str_p("Self") | "Victim" | "SecondThreat";
    rule<> rMethod = str_p("CastSpell") | "Talk" | "Kill" | "SayLine";
    rule<> rAction = *(rTarget >> '.') >> rMethod >> '(' >> rParameter >> ')' >> *space_p >> *(str_p("To") >> *space_p >> rTarget) >> ch_p(';');
    rule<> rEvent = str_p("Event") >> +space_p >> +alnum_p >> *space_p >> ch_p('[') >> *(rAttribute >> *(*space_p >> ',' >> *space_p >> rAttribute)) >> *space_p >> ']' >> *space_p >> '{' >> *space_p >> *(*space_p >> rAction >> *space_p) >> '}';
    rule<> rTriggerMethod = str_p("ExecuteEvent");
    rule<> rTriggerEvent = str_p("ExecuteEvent");
    rule<> rTriggerAction = rTriggerMethod >> '(' >> *alpha_p >> ')' >> ch_p(';');
    rule<> rTrigger = rTriggerName >> ':' >> *space_p >> *('(' >> *space_p >> rParameter >> *space_p >> ')') >> *space_p >> '{' >> *(*space_p >> rTriggerAction >> *space_p) >> '}';
    rule<> rTriggerName = str_p("OnAggro") | "OnEnterCombat" | "OnKill" | "OnUpdate";
    rule<> rTriggerMap = str_p("EventBinding") >> *space_p >> '{' >> *space_p >> *rTrigger >> *space_p >> '}';
    rule<> rScript = str_p("UnitScript") >> *space_p >> +alnum_p >> *space_p >> '{' >> *(*space_p >> rEvent >> *space_p) >> *space_p >> +rTriggerMap >> *space_p >> '}';
};

#define xScriptLoader LegacyScriptLoader::instance()

#endif // !_LEGACYSCRIPTLOADER_H_
