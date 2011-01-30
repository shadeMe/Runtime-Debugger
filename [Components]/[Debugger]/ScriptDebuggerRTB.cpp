#include "ScriptDebuggerRTB.h"
#include "[Common]\NativeWrapper.h"

ScriptDebuggerRTB::ScriptDebuggerRTB(UInt32 LinesToScroll, Font^ Font, Color ForegroundColor, Color BackgroundColor, Color HighlightColor, Color LineHighlightColor, Color BreakpointColor) : OffsetRichTextBox(LinesToScroll, Font, ForegroundColor, BackgroundColor, HighlightColor)
{
	this->LineHighlightColor = LineHighlightColor;
	this->BreakpointColor = BreakpointColor;

	CurrentOffset = 0;
	Breakpoints = gcnew List<UInt32>();
	OffsetFlag = true;
}

void ScriptDebuggerRTB::LineField_MouseDown(Object^ Sender, MouseEventArgs^ E)
{
	TextField->Focus();
	int BaseOffset = 0, SelStart = LineField->SelectionStart; 

	try 
	{ 
		if (SelStart != -1 &&
			LineField->GetLineFromCharIndex(SelStart) < LineField->Lines->Length && LineField->Lines->Length > 0)
		{
			if (OffsetFlag)
			{
				UInt16 Offset = int::Parse(LineField->Lines[LineField->GetLineFromCharIndex(SelStart)], System::Globalization::NumberStyles::AllowHexSpecifier);
				BaseOffset = Offset;
			}
		}
		else
			return;

		ToggleBreakPoint(BaseOffset);
	}
	catch (...)
	{
		return;
	}	
}

void ScriptDebuggerRTB::UpdateLineHighlights(void)
{
	try
	{
		NativeWrapper::LockWindowUpdate(TextField->Handle);
		UInt32 CaretPos = TextField->SelectionStart;

		TextField->SelectAll();
		TextField->SelectionBackColor = TextField->BackColor;
		TextField->SelectionLength = 0;	

		for each (UInt32 Itr in Breakpoints)
			HighlightLineHandler(Itr, BreakpointColor);

		if (GetLineFromOffset(CurrentOffset) != -1)
			HighlightLineHandler(GetLineFromOffset(CurrentOffset), LineHighlightColor);

		TextField->SelectionStart = CaretPos;
		TextField->SelectionLength = 0;	
	}
	finally
	{
		NativeWrapper::LockWindowUpdate(IntPtr::Zero);
	}
}

void ScriptDebuggerRTB::HighlightLineHandler(int Line, Color% LineColor)
{
	UInt32 CaretPos = TextField->SelectionStart;

	int SelStart = TextField->GetFirstCharIndexFromLine(Line), SelEnd = TextField->GetFirstCharIndexFromLine(Line + 1);
	if (SelStart == -1)
		return;
	else if (SelEnd == -1)
		SelEnd = TextField->Text->Length - 1;

	TextField->SelectionStart = SelStart;
	TextField->SelectionLength = SelEnd - SelStart;

	TextField->SelectionBackColor = LineColor;
	TextField->SelectionStart = CaretPos;
}

void ScriptDebuggerRTB::ToggleBreakPoint(UInt32 Offset)
{
	int Line = GetLineFromOffset(Offset);
	if (Line != -1)
	{
		if (GetIsOffsetBreakPoint(Offset))
			Breakpoints->Remove(Line);
		else
			Breakpoints->Add(Line);

		UpdateLineHighlights();
	}
}

bool ScriptDebuggerRTB::GetIsOffsetBreakPoint(UInt32 Offset)
{
	bool Result = false;
	int Line = GetLineFromOffset(Offset);
	if (Line != -1)
	{
		for each (UInt32 Itr in Breakpoints)
		{
			if (Itr == Line)
			{
				Result = true;
				break;
			}
		}
	}
	return Result;
}

int ScriptDebuggerRTB::GetLineFromOffset(UInt32 Offset)
{
	int Result = GetIndexOfOffset(Offset);
	if (Result == -1)
	{
		while (Offset-- && Result == -1)
		{
			Result = GetIndexOfOffset(Offset);
		}
	}
	return Result;
}

void ScriptDebuggerRTB::HighlightNewOffset(UInt32 Offset)
{
	CurrentOffset = Offset;
	UpdateLineHighlights();

	int Line = GetLineFromOffset(Offset);
	if (Line != -1)
	{
		int CaretPos = TextField->GetFirstCharIndexFromLine(Line);
		if (CaretPos != -1)
		{
			TextField->SelectionStart = CaretPos;
			TextField->SelectionLength = 0;
			TextField->ScrollToCaret();
		}
	}
}