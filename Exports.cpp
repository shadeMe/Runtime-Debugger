#include "Exports.h"
#include "RUDEInternals.h"
#include "ScriptRunnerWrapper.h"
#include "obse/GameExtraData.h"

extern "C"
{

__declspec(dllexport) void _D_PRINT(const char* Message)
{
	DebugPrint(Message);
}

__declspec(dllexport) bool GetScriptDataForForm(bool* OperationResult, UInt32 FormID, Script** ResultScript, ScriptEventList** ResultEventList)
{
	bool Result = false;

	if (FormID)
	{
		TESForm* Form = LookupFormByID(FormID);
		if (Form)
		{
			TESQuest* Quest = OBLIVION_CAST(Form, TESForm, TESQuest);
			TESObjectREFR* Ref = OBLIVION_CAST(Form, TESForm, TESObjectREFR);

			if (Quest)
			{
				*ResultScript = Quest->scriptable.script;
				*ResultEventList = Quest->scriptEventList;
				Result = true;
			}
			else if (Ref)
			{
				BSExtraData* xData = Ref->baseExtraList.GetByType(kExtraData_Script);
				if (xData)
				{
					ExtraScript* xScript = OBLIVION_CAST(xData, BSExtraData, ExtraScript);
					*ResultScript = xScript->script;
					*ResultEventList = xScript->eventList;
					Result = true;
				}
			}
		}
	}

	*OperationResult = Result;
	return Result;
}

__declspec(dllexport) bool ToggleDebugBreakInvocationState()
{
	return DebugBreakInvocationState = (!DebugBreakInvocationState);
}

}