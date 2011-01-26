#include "ByteCodeParser.h"

UInt32 ByteCodeParser::Read16(Array^% Data, UInt32% CurrentOffset)
{
    UInt32 LoByte = (Byte)Data->GetValue((int)CurrentOffset++),
		   HiByte = (Byte)Data->GetValue((int)CurrentOffset++);
    return LoByte + (HiByte << 8);
}

bool ByteCodeParser::LineHasData(String^% Line)
{
	String^ Extract = Line->Split(';')[0];
	if (Extract == "")		return false;

	int Idx = 0;
	for each (char Itr in Extract) {
		if (Itr != '\t' && Itr != ' ')	break;
		Idx++;
	}

	Extract = Extract->Substring(Idx)->Split('\t', '\r', '\n', ' ')[0];
	if (Extract == "")		return false;

	if (!String::Compare(Extract, "ref", true) ||
		!String::Compare(Extract, "reference", true) ||
		!String::Compare(Extract, "string_var", true) ||
		!String::Compare(Extract, "array_var", true) ||
		!String::Compare(Extract, "long", true) ||
		!String::Compare(Extract, "short", true) ||
		!String::Compare(Extract, "float", true) ||
		!String::Compare(Extract, "int", true))

		return false;
	else
		return true;
}

UInt32 ByteCodeParser::GetOffsetForLine(String^% Line, Array^% Data, UInt32% CurrentOffset)
{
	if (!ByteCodeParser::LineHasData(Line))
		return 0xFFFFFFFF;

	UInt32 OpCode = Read16(Data, CurrentOffset);

	switch (OpCode)
	{
	case 0x1D:				 // scn
		ByteCodeParser::Read16(Data, CurrentOffset);
		return 0;
	case 0x1C:				 // dot operator
		ByteCodeParser::Read16(Data, CurrentOffset);
		return GetOffsetForLine(Line, Data, CurrentOffset) - 4;
	case 0x11:				 // end
		ByteCodeParser::Read16(Data, CurrentOffset);
		return CurrentOffset - 4;
	case 0x1E:				// return
		ByteCodeParser::Read16(Data, CurrentOffset);			 
		return CurrentOffset - 4;
	case 0x17:				 // else
		ByteCodeParser::Read16(Data, CurrentOffset);
		ByteCodeParser::Read16(Data, CurrentOffset);
		return CurrentOffset - 6;
	case 0x19:				 // endif
		ByteCodeParser::Read16(Data, CurrentOffset);
		return CurrentOffset - 4;
	default:
		UInt32 Len = ByteCodeParser::Read16(Data, CurrentOffset);
		UInt32 Cur = CurrentOffset - 4;
		CurrentOffset += Len;
		return Cur;
	}
}