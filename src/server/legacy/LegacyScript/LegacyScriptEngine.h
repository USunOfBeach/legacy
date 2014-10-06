#ifndef _LEGACYSCRIPTENGINE_H_
#define _LEGACYSCRIPTENGINE_H_

#include "LegacyDefines.h"

class LegacyScriptEngine
{
public:
    LegacyScriptEngine() { }
    ~LegacyScriptEngine() { }

    static LegacyScriptEngine* instance()
    {
        static LegacyScriptEngine instance;
        return &instance;
    }

    void ExecuteEvent(LegacyEventData &data, Unit* self, Unit* victim = NULL);
    void ExecuteOn(LegacyTrigger trigger, LegacyScriptData &data, Unit* self = NULL, Unit* victim = NULL);
    void UpdateAI(LegacyScriptData &data, uint32 diff);
};

#define xEngine LegacyScriptEngine::instance()

#endif // !_LEGACYSCRIPTENGINE_H_


