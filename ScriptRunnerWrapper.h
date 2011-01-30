#pragma once

#include "RUDEInternals.h"
#include "[Common]\DebuggerMessagingInterface.h"

extern const UInt32		kScriptRunner_GetSingleton;
extern const UInt32		kScriptRunner_RunScript;
extern const UInt32		kScriptRunner_LookupCommandByOpcode;
extern const UInt32		kCommandTable_ScriptBlocks;

void PatchScriptRunnerMethods(void);

extern MemHdlr		kRerouteDebugMessages;
extern MemHdlr		kScriptRunnerNewInstance;
extern MemHdlr		kScriptRunnerNewLineExecutionStart;
extern MemHdlr		kScriptRunnerReferenceEvaluationFailed;
extern MemHdlr		kScriptRunnerEncounteredReturnCommand;	
extern MemHdlr		kScriptRunnerBlockHandlerCalled;
extern MemHdlr		kScriptRunnerCommandHandlerCalled;		
extern MemHdlr		kScriptRunnerCommandHandlerExecuted;	
extern MemHdlr		kScriptRunnerLineExecutionFailed;	
extern MemHdlr		kScriptRunnerExecutionComplete;

void ScriptRunnerNewInstanceHook(void);
void ScriptRunnerNewLineExecutionStartHook(void);
void ScriptRunnerReferenceEvaluationFailedHook(void);
void ScriptRunnerEncounteredReturnCommandHook(void);	
void ScriptRunnerBlockHandlerCalledHook(void);
void ScriptRunnerCommandHandlerCalledHook(void);
void ScriptRunnerCommandHandlerExecutedHook(void);
void ScriptRunnerLineExecutionFailedHook(void);	
void ScriptRunnerExecutionCompleteHook(void);

void __stdcall DebugTextDetour(const char* Message);
void __stdcall SendMessagePingback(DebuggerMessage Message, UInt32* Data);