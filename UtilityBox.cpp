#include "RUDEInternals.h"

void __stdcall ToggleFlag(UInt32* Flag, UInt32 Mask, bool State)
{
	if (State)	*Flag |= Mask;
	else		*Flag &= ~Mask;
}

void LogWinAPIErrorMessage(DWORD ErrorID)
{
	LPVOID ErrorMsg;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		ErrorID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &ErrorMsg,
		0, NULL );

	DebugPrint("\tError Message: %s", (LPSTR)ErrorMsg); 
	LocalFree(ErrorMsg);
}

void DebugPrint(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	_MESSAGE(fmt, args);
	va_end(args);
}

void WaitUntilDebuggerAttached()
{
	DebugPrint("----> Waiting For Debugger <----");
	while (IsDebuggerPresent() == FALSE){
		Sleep(5000);
	}
	DebugPrint("----> Debugger Attached <----");
}