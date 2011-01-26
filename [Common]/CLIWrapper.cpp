#include "RUDEInternals.h"
#include "CLIWrapper.h"

namespace CLIWrapper
{
	namespace Debugger
	{
		_GetIsInitialized					GetIsInitialized;
		_GetExecutingContextState			GetExecutingContextState;
		_ScriptRunnerCallbackWrapper		ScriptRunnerCallbackWrapper;
		_PrintToConsole						PrintToConsole;
	}

	bool CLIWrapper::Import(const OBSEInterface * obse)
	{
		SetErrorMode(0);
		HMODULE hMod = LoadLibrary(std::string(std::string(obse->GetOblivionDirectory()) + "Data\\OBSE\\Plugins\\ComponentDLLs\\RUDE\\Debugger.dll").c_str());

		if (hMod == NULL)
		{
			DebugPrint("Couldn't load Debugger.dll");
			LogWinAPIErrorMessage(GetLastError());
			return false;
		}

		CLIWrapper::Debugger::GetIsInitialized = (CLIWrapper::Debugger::_GetIsInitialized)GetProcAddress(hMod, "GetIsInitialized");
		CLIWrapper::Debugger::GetExecutingContextState = (CLIWrapper::Debugger::_GetExecutingContextState)GetProcAddress(hMod, "GetExecutingContextState");
		CLIWrapper::Debugger::ScriptRunnerCallbackWrapper = (CLIWrapper::Debugger::_ScriptRunnerCallbackWrapper)GetProcAddress(hMod, "ScriptRunnerCallbackWrapper");
		CLIWrapper::Debugger::PrintToConsole = (CLIWrapper::Debugger::_PrintToConsole)GetProcAddress(hMod, "PrintToConsole");

		if (!CLIWrapper::Debugger::GetIsInitialized || 
			!CLIWrapper::Debugger::GetExecutingContextState || 
			!CLIWrapper::Debugger::ScriptRunnerCallbackWrapper ||
			!CLIWrapper::Debugger::PrintToConsole)
		{	
			LogWinAPIErrorMessage(GetLastError());
			return false;
		}

		return true;
	}
}