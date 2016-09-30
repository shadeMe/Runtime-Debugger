#include "RUDEInternals.h"
#include "ScriptRunnerWrapper.h"
#include "[Common]\CLIWrapper.h"

IDebugLog					gLog("RuntimeDebugger.log");

PluginHandle				g_pluginHandle = kPluginHandle_Invalid;
OBSEMessagingInterface*		g_msgIntfc = NULL;

extern "C"
{

bool OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "RUDE";
	info->version = 1;

	if(!obse->isEditor)
	{
		if(obse->obseVersion < OBSE_VERSION_INTEGER)
		{
			DebugPrint("OBSE version too old (got %08X expected at least %08X)", obse->obseVersion, OBSE_VERSION_INTEGER);
			return false;
		}

		if(obse->oblivionVersion != OBLIVION_VERSION)
		{
			DebugPrint("incorrect Oblivion version (got %08X need %08X)", obse->oblivionVersion, OBLIVION_VERSION);
			return false;
		}
	}

	return true;
}

bool OBSEPlugin_Load(const OBSEInterface * obse)
{
	g_pluginHandle = obse->GetPluginHandle();
	DebugPrint("RuntimeDebugger Initializing...");
	gLog.Indent();

	WaitUntilDebuggerAttached();

	if (!CLIWrapper::Import(obse))
		return false;
	
	obse->SetOpcodeBase(0x2710);						// 0x2710 - 0x272F
	obse->RegisterCommand(&kCommandInfo_DebugBreak);
	obse->RegisterCommand(&kCommandInfo_ToggleDebugBreaking);
	

	if (!obse->isEditor)
	{
		PatchGameInitialization();
		PatchScriptRunnerMethods();

		g_msgIntfc = (OBSEMessagingInterface*)obse->QueryInterface(kInterface_Messaging);
		g_msgIntfc->RegisterListener(g_pluginHandle, "OBSE", OBSEMessageHandler);
	}

	gLog.Outdent();
	DebugPrint("RuntimeDebugger Initialized!\n\n");
	return true;
}

};
