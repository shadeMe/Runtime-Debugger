#include "Exports.h"
#include "DebuggerBase.h"

extern "C"
{

__declspec(dllexport) bool GetIsInitialized(void)
{
	return DEBUGGER->GetIsInitialized();
}

__declspec(dllexport) UInt8 GetExecutingContextState(void)
{
	return DEBUGGER->GetState();
}

__declspec(dllexport) UInt8 ScriptRunnerCallbackWrapper(DebuggerMessage Message, UInt32* Data)
{
	return DEBUGGER->HandleScriptRunnerCallback(Message, Data);
}

__declspec(dllexport) void PrintToConsole(const char* Message)
{
	DEBUGGER->PrintToConsole(gcnew String(Message));
}

}