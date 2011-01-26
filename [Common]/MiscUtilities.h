#pragma once

#include "Includes.h"

public ref class CStringWrapper
{
	IntPtr												P;
	const char*											CString;

	void												Free() { Marshal::FreeHGlobal(P); }
public:
	CStringWrapper(String^% Source);
	~CStringWrapper()									{ this->!CStringWrapper(); }
	!CStringWrapper()									{ this->Free(); }

	const char*											String() { return CString; }
};

public ref class DebuggerGeneralException : public Exception
{
public:
	DebuggerGeneralException(String^ Message) : Exception(Message) {};
};

public ref class ImageResourceManager
{
	ResourceManager^					Manager;
public:
	ImageResourceManager(String^ BaseName);

	Image^								CreateImageFromResource(String^ ResourceIdentifier);
	void								SetupImageForToolStripButton(ToolStripButton^ Control);
};

void DebugPrint(String^ Message, bool Achtung = false);	
