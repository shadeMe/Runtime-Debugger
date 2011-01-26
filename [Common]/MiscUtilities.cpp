#include "MiscUtilities.h"
#include "NativeWrapper.h"

CStringWrapper::CStringWrapper(System::String^% Source)
{
	P = Marshal::StringToHGlobalAnsi(Source);
	CString = static_cast<char*>(P.ToPointer());
}

ImageResourceManager::ImageResourceManager(String^ BaseName)
{
	Manager = gcnew ResourceManager(BaseName, Assembly::GetExecutingAssembly());
}

Image^ ImageResourceManager::CreateImageFromResource(String^ ResourceIdentifier)
{
	try
	{
		return dynamic_cast<Image^>(Manager->GetObject(ResourceIdentifier));
	}
	catch (...)
	{
		return nullptr;
	}
}

void ImageResourceManager::SetupImageForToolStripButton(ToolStripButton^ Control)
{
	Control->Image = CreateImageFromResource(Control->Name);
}

void DebugPrint(String^ Message, bool Achtung)	
{
	if (Achtung) {
		Media::SystemSounds::Hand->Play();
	}

	NativeWrapper::_D_PRINT((gcnew CStringWrapper(Message))->String());
}