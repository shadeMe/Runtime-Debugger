#pragma once

#include "obse/GameData.h"
#include "obse/Script.h"
#include "obse/PluginAPI.h"
#include "obse/GameObjects.h"
#include "obse/CommandTable.h"
#include "UtilityBox.h"

using namespace MemoryHandler;

extern MemHdlr kScriptLoadText;

void PatchGameInitialization(void);

void ScriptLoadTextHook(void);

extern UInt32		kTESFile_GetChunkData;

extern CommandInfo	kCommandInfo_DebugBreak;

// 28
class TESGame
{
public:
	TESGame();
	~TESGame();

	UInt8			quitGameFlag;				// 00
	UInt8			quitToMainMenuFlag;			// 01
	UInt8			unk02;						// 02
	UInt8			unk03;						// 03
	UInt8			unk04;						// 04
	UInt8			pad05[3];					// 05
	HWND			mainWindowHandle;			// 08
	HINSTANCE		processInstance;			// 0C
	DWORD			mainThreadID;				// 10
	HANDLE			mainThreadHandle;			// 14
	UInt32			unk18;						// 18
	UInt32			unk1C;						// 1C
	void*			directInputStruct;			// 20	size = 0x1BD8
	void*			directSoundStruct;			// 24	size = 0x328
};

extern TESGame**		g_TESGame;