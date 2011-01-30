#include "Exports.h"
#include "RUDEInternals.h"
#include "obse/GameExtraData.h"

extern "C"
{

__declspec(dllexport) void _D_PRINT(const char* Message)
{
	DebugPrint(Message);
}

__declspec(dllexport) bool GetScriptDataForForm(UInt32 FormID, Script** ResultScript, ScriptEventList** ResultEventList)
{
	bool Result = false;

	if (FormID)
	{
		TESForm* Form = LookupFormByID(FormID);
		if (Form)
		{
			if (Form->IsReference() == 0)		// if not a ref, a quest
			{
				TESQuest* Quest = OBLIVION_CAST(Form, TESForm, TESQuest);
				*ResultScript = Quest->scriptable.script;
				*ResultEventList = Quest->scriptEventList;
				Result = true;
			}
			else
			{
				TESObjectREFR* Ref = OBLIVION_CAST(Form, TESForm, TESObjectREFR);

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

	return Result;
}

}