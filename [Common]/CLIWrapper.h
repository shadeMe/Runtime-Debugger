#pragma once

#include "[Common]\DebuggerMessagingInterface.h"

struct OBSEInterface;

namespace CLIWrapper
{
	namespace Debugger
	{
		typedef bool							(*_GetIsInitialized)(); 
		typedef UInt8							(*_GetExecutingContextState)(); 
		typedef UInt8							(*_ScriptRunnerCallbackWrapper)(DebuggerMessage, UInt32*); 
		typedef void							(*_PrintToConsole)(const char*);

		extern _GetIsInitialized				GetIsInitialized;
		extern _GetExecutingContextState		GetExecutingContextState;
		extern _ScriptRunnerCallbackWrapper		ScriptRunnerCallbackWrapper;
		extern _PrintToConsole					PrintToConsole;
	}

	bool										Import(const OBSEInterface * obse);
}