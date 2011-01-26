#pragma once

#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "windows.h"
#include "resource.h"
#include "[Libraries]\MemoryHandler\MemoryHandler.h"

using namespace SME;

void DebugPrint(const char* fmt, ...);
void WaitUntilDebuggerAttached();
void __stdcall ToggleFlag(UInt32* Flag, UInt32 Mask, bool State);		// state = 1 [ON], 0 [OFF]
void LogWinAPIErrorMessage(DWORD ErrorID);
