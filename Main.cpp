IDebugLog					gLog("RuntimeDebugger.log");

PluginHandle				g_pluginHandle = kPluginHandle_Invalid;


extern "C" {

bool OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "RuntimeDebugger";
	info->version = 1;

	if(!obse->isEditor)
	{
		if(obse->obseVersion < OBSE_VERSION_INTEGER)
		{
			_ERROR("OBSE version too old (got %08X expected at least %08X)", obse->obseVersion, OBSE_VERSION_INTEGER);
			return false;
		}

		if(obse->oblivionVersion != OBLIVION_VERSION)
		{
			_ERROR("incorrect Oblivion version (got %08X need %08X)", obse->oblivionVersion, OBLIVION_VERSION);
			return false;
		}
	}
	else

	return true;
}

bool OBSEPlugin_Load(const OBSEInterface * obse)
{
	g_pluginHandle = obse->GetPluginHandle();
	_MESSAGE("RuntimeDebugger Initializing...");
	gLog.Indent();

	//

	gLog.Outdent();
	_MESSAGE("RuntimeDebugger Initialized!\n\n");
	return true;
}

};
