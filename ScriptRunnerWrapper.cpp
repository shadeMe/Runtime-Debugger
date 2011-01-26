#include "ScriptRunnerWrapper.h"
#include "[Common]\CLIWrapper.h"

const UInt32				kScriptRunner_GetSingleton = 0x00516970;
const UInt32				kScriptRunner_RunScript = 0x00517810;
const UInt32				kScriptRunner_LookupCommandByOpcode = 0x004FCA30;
const UInt32				kCommandTable_ScriptBlocks = 0x00B0AF48;


MemHdlr						kRerouteDebugMessages						(0x00A3DA08 + 4, (UInt32)0, 0, 0);

MemHdlr						kScriptRunnerNewInstance					(0x005174D4, ScriptRunnerNewInstanceHook, 0, 0);
MemHdlr						kScriptRunnerNewLineExecutionStart			(0x00517519, ScriptRunnerNewLineExecutionStartHook, 0, 0);
MemHdlr						kScriptRunnerReferenceEvaluationFailed		(0x00517571, ScriptRunnerReferenceEvaluationFailedHook, 0, 0);
MemHdlr						kScriptRunnerEncounteredReturnCommand		(0x00517520, ScriptRunnerEncounteredReturnCommandHook, 0, 0);	
MemHdlr						kScriptRunnerBlockHandlerCalled				(0x00516ADA, ScriptRunnerBlockHandlerCalledHook, 0, 0);
MemHdlr						kScriptRunnerCommandHandlerCalled			(0x005172AD, ScriptRunnerCommandHandlerCalledHook, 0, 0);
MemHdlr						kScriptRunnerLineExecutionFailed			(0x00517576, ScriptRunnerLineExecutionFailedHook, 0, 0);
MemHdlr						kScriptRunnerExecutionComplete				(0x00517617, ScriptRunnerExecutionCompleteHook, 0, 0);


static UInt32				CurrentLineBuffer = 0;			// passed by the Debugbreak handler, cached by command handler event


// ### i seeem to be passing opcodes instead of offsets!!

void PatchScriptRunnerMethods(void)
{
	kRerouteDebugMessages.WriteUInt32((UInt32)&DebugTextDetour);

	kScriptRunnerNewInstance.WriteJump();
	kScriptRunnerNewLineExecutionStart.WriteJump();
	kScriptRunnerReferenceEvaluationFailed.WriteJump();
	kScriptRunnerEncounteredReturnCommand.WriteJump();	
	kScriptRunnerBlockHandlerCalled.WriteJump();
	kScriptRunnerCommandHandlerCalled.WriteJump();
	kScriptRunnerLineExecutionFailed.WriteJump();	
	kScriptRunnerExecutionComplete.WriteJump();
}

void __stdcall DebugTextDetour(const char* Message)
{
	CLIWrapper::Debugger::PrintToConsole(Message);
}

void __stdcall SendMessagePingback(DebuggerMessage Message, UInt32* Data)
{
	if ((CLIWrapper::Debugger::GetIsInitialized() || Message == kDebuggerMessage_DebugBreakCalled) &&
		CLIWrapper::Debugger::ScriptRunnerCallbackWrapper(Message, Data) == kDebuggerState_Break)
	{
		do
		{
			Sleep(0x32);
		}
		while (CLIWrapper::Debugger::GetExecutingContextState() == kDebuggerState_Break);
	}

	delete [] Data;
}


void __stdcall HandleScriptRunnerEvent_NewInstance(Script* Script, ScriptEventList* EventList, UInt32 Line, UInt16 Offset)
{
	UInt32* Data = new UInt32[4];

	Data[0] = (UInt32)Line;
	Data[1] = (UInt32)Offset;
	Data[2] = (UInt32)Script;
	Data[3] = (UInt32)EventList;
	
	SendMessagePingback(kDebuggerMessage_NewInstance, Data);
}
void __declspec(naked) ScriptRunnerNewInstanceHook(void)
{
	__asm
	{
		jbe		JUMPL

		pushad
		push	0
		push	0
		push	[esi + 8]
		push	edi
		call	HandleScriptRunnerEvent_NewInstance
		popad

		mov		eax, 0x005174DA
		jmp		eax

	JUMPL:
		mov		eax, 0x0051757B
		jmp		eax
	}
}

void __stdcall HandleScriptRunnerEvent_NewLineExecutionStart(UInt32 Line, UInt16 Offset)
{
	UInt32* Data = new UInt32[2];

	Data[0] = (UInt32)Line;
	Data[1] = (UInt32)Offset;
	
	SendMessagePingback(kDebuggerMessage_NewLineExecutionStart, Data);
}
void __declspec(naked) ScriptRunnerNewLineExecutionStartHook(void)
{
	__asm
	{
		mov		eax, [esp + 0x28]	
		mov     ecx, [esp + 0x38]	
		pushad
		push	ecx
		push	eax
		call	HandleScriptRunnerEvent_NewLineExecutionStart
		popad

		mov		eax, 0x00517520
		cmp     ecx, 0x1E
		jmp		eax
	}
}

void __stdcall HandleScriptRunnerEvent_ReferenceEvaluationFailed(UInt32 Line, UInt16 Offset)
{
	UInt32* Data = new UInt32[2];

	Data[0] = (UInt32)Line;
	Data[1] = (UInt32)Offset;
	
	SendMessagePingback(kDebuggerMessage_ReferenceEvaluationFailed, Data);
}
void __declspec(naked) ScriptRunnerReferenceEvaluationFailedHook(void)
{
	__asm
	{
		mov		eax, [esp + 0x28]	
		mov     ecx, [esp + 0x38]
		pushad
		push	ecx
		push	eax
		call	HandleScriptRunnerEvent_ReferenceEvaluationFailed
		popad

		mov     [edi + 0x20], ebx	
		mov		eax, 0x0051757B
		jmp		eax
	}
}

void __stdcall HandleScriptRunnerEvent_EncounteredReturnCommand(UInt32 Line, UInt16 Offset)
{
	UInt32* Data = new UInt32[2];

	Data[0] = (UInt32)Line;
	Data[1] = (UInt32)Offset;
	
	SendMessagePingback(kDebuggerMessage_EncounteredReturnCommand, Data);
}
void __declspec(naked) ScriptRunnerEncounteredReturnCommandHook(void)
{
	__asm
	{
		jz		RETURN
		mov		eax, 0x00517522
		jmp		eax
	RETURN:
		mov		eax, [esp + 0x28]	
		mov     ecx, [esp + 0x38]
		pushad
		push	ecx
		push	eax
		call	HandleScriptRunnerEvent_EncounteredReturnCommand
		popad		

		mov		eax, 0x0051757B
		jmp		eax
	}
}	

void __stdcall HandleScriptRunnerEvent_BlockHandlerCalled(UInt32 Line, UInt16 Offset, UInt32 CommandTableOffset)
{
	CommandInfo* Command = &((CommandInfo*)kCommandTable_ScriptBlocks)[CommandTableOffset];
	UInt32* Data = new UInt32[3];

	Data[0] = (UInt32)Line;
	Data[1] = (UInt32)Offset;
	Data[2] = (UInt32)Command->longName;
	
	SendMessagePingback(kDebuggerMessage_BlockHandlerCalled, Data);
	CurrentLineBuffer = Line;
}
void __declspec(naked) ScriptRunnerBlockHandlerCalledHook(void)
{
	__asm
	{
		mov		eax, [esp + 0x770]			// opcode offset
		movsx   edx, word ptr [edi + eax]	// command table offset

		push	edx
		mov		eax, [esp + 0x77C]			// line  ### will need correction according to previous push (+/-4) ; org 0x770, 0x14
		mov		edx, [esp + 0x18]			// offset
		push	edx
		push	eax
		call	HandleScriptRunnerEvent_BlockHandlerCalled

		lea     eax, [esp + 0x14]
		push    eax
		mov     eax, [esi + 0x8]
		mov		edx, 0x00516AE2
		jmp		edx
	}
}

void __stdcall HandleScriptRunnerEvent_CommandHandlerCalledHook(UInt32 Line, UInt16 Offset, UInt32 CommandOpcode)
{
	CommandInfo* Command = ((CommandInfo* (__cdecl *)(UInt16))kScriptRunner_LookupCommandByOpcode)(CommandOpcode);
	UInt32* Data = new UInt32[3];

	Data[0] = (UInt32)Line;
	Data[1] = (UInt32)Offset;
	Data[2] = (UInt32)Command->longName;
	
	SendMessagePingback(kDebuggerMessage_CommandHandlerCalled, Data);
}
void __declspec(naked) ScriptRunnerCommandHandlerCalledHook(void)
{
	__asm
	{
		mov     eax, [esp + 0x764]			// opcode

		push	eax
		mov		eax, [esp + 0x77C]			// line  ### will need correction according to previous push (+/-4) ; org 0x770, 0x14
		mov		edx, [esp + 0x18]			// offset
		push	edx
		push	eax
		call	HandleScriptRunnerEvent_CommandHandlerCalledHook

		lea     eax, [esp + 0x14]
		push    eax
		mov     eax, [esi + 0x8]
		mov		edx, 0x005172B5
		jmp		edx
	}
}

void __stdcall HandleScriptRunnerEvent_LineExecutionFailed(UInt32 Line, UInt16 Offset)
{
	UInt32* Data = new UInt32[2];

	Data[0] = (UInt32)Line;
	Data[1] = (UInt32)Offset;
	
	SendMessagePingback(kDebuggerMessage_LineExecutionFailed, Data);
}
void __declspec(naked) ScriptRunnerLineExecutionFailedHook(void)
{
	__asm
	{
		mov		eax, [esp + 0x28]	
		mov     ecx, [esp + 0x38]
		pushad
		push	ecx
		push	eax
		call	HandleScriptRunnerEvent_LineExecutionFailed
		popad		

		mov     byte ptr [esp + 0x16], 1
		mov		ecx, 0x0051757B
		jmp		ecx
	}
}

void __stdcall HandleScriptRunnerEvent_ExecutionComplete(UInt32 Result)
{
	UInt32* Data = new UInt32[1];

	Data[0] = (UInt32)Result;
	
	SendMessagePingback(kDebuggerMessage_ExecutionComplete, Data);
}
void __declspec(naked) ScriptRunnerExecutionCompleteHook(void)
{
	__asm
	{
		pushad
		test	al, al
		jz		FAIL
		push	1
	CALLEVENT:
		call	HandleScriptRunnerEvent_ExecutionComplete
		popad

		pop     ecx
		mov		ecx, 0x0051761F
		jmp		ecx
	FAIL:
		push	0
		jmp		CALLEVENT
	}
}

bool Cmd_DebugBreak_Execute(COMMAND_ARGS)
{
	*result = 1;

	UInt32* Data = new UInt32[4];

	Data[0] = (UInt32)Line;
	Data[1] = (UInt32)Offset;
	Data[2] = (UInt32)Script;
	Data[3] = (UInt32)EventList;
	
	SendMessagePingback(kDebuggerMessage_DebugBreakCalled, Data);
	CurrentLineBuffer
	return true;
}

static CommandInfo kCommandInfo_DebugBreak =
{
	"DebugBreak",
	"",
	0,
	"Invokes the runtime debugger",
	0,		// requires parent obj
	0,		// doesn't have params
	NULL,	// no param table

	HANDLER(Cmd_DebugBreak_Execute)
};
