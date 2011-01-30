#pragma once

class Script;
struct ScriptEventList;

extern "C"
{

__declspec(dllexport) void _D_PRINT(const char* Message);
__declspec(dllexport) bool GetScriptDataForForm(UInt32 FormID, Script** ResultScript, ScriptEventList** ResultEventList);

}