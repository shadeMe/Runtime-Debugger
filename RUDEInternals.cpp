#include "RUDEInternals.h"

UInt32				kTESFile_GetChunkData = 0x00450C20;

TESGame**			g_TESGame = (TESGame**)0x00B33398;

MemHdlr				kScriptLoadText		(0x004FC136, ScriptLoadTextHook, 0, 0);


void PatchGameInitialization(void)
{
	kScriptLoadText.WriteJump();
}

void __stdcall DoScriptLoadTextHook(ModEntry::Data* Plugin, Script* LoadingScript)
{
	UInt32 Size = Plugin->formInfo.chunkInfo.length + 1;

	LoadingScript->text = (char*)FormHeap_Allocate(Size);
	memset(LoadingScript->text, 0, Size);

	thisCall(kTESFile_GetChunkData, Plugin, LoadingScript->text, Size - 1);
}

void __declspec(naked) ScriptLoadTextHook(void)
{
	static UInt32 kScriptLoadTextHookRetnAddr = 0x004FC13C;
	__asm
	{
		cmp		edi, 'XTCS'
		jz		SCTX
		cmp		edi, 'ORCS'
		jmp		[kScriptLoadTextHookRetnAddr]
	SCTX:
		pushad
		push	ebx
		push	esi
		call	DoScriptLoadTextHook
		popad

		mov		eax, 0x004FC2FD
		jmp		eax
	}
}