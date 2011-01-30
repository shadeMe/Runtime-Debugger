#pragma once

#include "[Common]\Includes.h"
#include "[Common]\MiscUtilities.h"

public ref class Globals
{
public:
	static ImageResourceManager^					DebuggerImageManager = gcnew ImageResourceManager("Debugger.Images");
};

#define SetupControlImage(Identifier)							Identifier##->Name = #Identifier;	\
																Identifier##->Image = Globals::DebuggerImageManager->CreateImageFromResource(#Identifier);	\
																Identifier##->ImageTransparentColor = Color::White