#pragma once

#include "obse/GameData.h"
#include "obse/Script.h"
#include "obse/PluginAPI.h"
#include "obse/GameObjects.h"
#include "obse/CommandTable.h"
#include "UtilityBox.h"

using namespace MemoryHandler;

extern MemHdlr kScriptLoadText;

void PatchScriptTextLoad(void);
void ScriptLoadTextHook(void);

extern UInt32		kTESFile_GetChunkData;

extern CommandInfo	kCommandInfo_DebugBreak;