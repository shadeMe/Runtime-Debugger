#pragma once

#include "[Common]\DebuggerMessagingInterface.h"

extern "C"
{

__declspec(dllexport) bool GetIsInitialized(void);
__declspec(dllexport) UInt8 GetExecutingContextState(void);
__declspec(dllexport) UInt8 ScriptRunnerCallbackWrapper(DebuggerMessage Message, UInt32* Data);
__declspec(dllexport) void PrintToConsole(const char* Message);

}