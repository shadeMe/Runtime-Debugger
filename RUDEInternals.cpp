#include "RUDEInternals.h"
#include "ScriptRunnerWrapper.h"
#include <string>

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

void OBSEMessageHandler(OBSEMessagingInterface::Message* Msg)
{
	switch (Msg->type)
	{
	case OBSEMessagingInterface::kMessage_RuntimeScriptError:
		std::string MessageText((const char*)Msg->data);

		size_t OffsetIdx = MessageText.find("0x");

		std::string ErrorMessage = MessageText.substr(0, MessageText.find_first_of("\n")), ErrorOffset(""), CommandName("");
		if (OffsetIdx != std::string::npos)
		{
			ErrorOffset = MessageText.substr(MessageText.find("0x") + 2, 4);
			CommandName = MessageText.substr(MessageText.find("0x") + 16, MessageText.length());
		}

		UInt32 Offset = 0;
		if (ErrorOffset != "")
		{
			sscanf_s(ErrorOffset.c_str(), "%04X", &Offset);
			Offset -= 4;
		}

		UInt32* Data = new UInt32[4];

		Data[0] = (UInt32)0;
		Data[1] = (UInt16)Offset;
		Data[2] = (UInt32)ErrorMessage.c_str();
		Data[3] = (UInt32)CommandName.c_str();
		
		SendMessagePingback(kDebuggerMessage_OBSEScriptErrorEncountered, Data);
		break;
	}
}
