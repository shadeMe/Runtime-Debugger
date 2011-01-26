#pragma once

class ByteCodeParser
{
	static UInt32										Read16(Array^% Data, UInt32% CurrentOffset);
	static bool											LineHasData(String^% Line);
public:
	static UInt32										GetOffsetForLine(String^% Line, Array^% Data, UInt32% CurrentOffset);
};
