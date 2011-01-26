#include "ScriptDebuggerRTB.h"

ScriptDebuggerRTB::ScriptDebuggerRTB(UInt32 LinesToScroll, Font^ Font, Color ForegroundColor, Color BackgroundColor, Color HighlightColor, Color LineHighlightColor, Color BreakpointColor) : OffsetRichTextBox(LinesToScroll, Font, ForegroundColor, BackgroundColor, HighlightColor)
{
	this->LineHighlightColor = LineHighlightColor;
	this->BreakpointColor = BreakpointColor;

	CurrentLine = 0;
	Breakpoints = gcnew List<UInt32>();
}

void ScriptDebuggerRTB::LineField_MouseDown(Object^ Sender, MouseEventArgs^ E)
{
	TextField->Focus();
	int LineNo = 0, SelStart = LineField->SelectionStart; 

	try 
	{ 
		if (SelStart != -1 &&
			LineField->GetLineFromCharIndex(SelStart) < LineField->Lines->Length && LineField->Lines->Length > 0)
		{
			if (OffsetFlag)
			{
				UInt16 Offset = int::Parse(LineField->Lines[LineField->GetLineFromCharIndex(SelStart)], System::Globalization::NumberStyles::AllowHexSpecifier);
				LineNo = GetIndexOfOffset(Offset);
			}
			else
			{
				String^ Selection = LineField->Lines[LineField->GetLineFromCharIndex(SelStart)]->Replace(" ", "");;
				LineNo = int::Parse(Selection) - 1;
			}
		}
		else
			return;

		ToggleBreakPoint(LineNo);
	}
	catch (...)
	{
		return;
	}	
}

void ScriptDebuggerRTB::UpdateLineHighlights(void)
{
	TextField->SelectAll();
	TextField->BackColor = Color::White;
	TextField->SelectionLength = 0;	

	for each (UInt32 Itr in Breakpoints)
		HighlightLineHandler(Itr, BreakpointColor);

	HighlightLineHandler(CurrentLine, LineHighlightColor);
}

void ScriptDebuggerRTB::HighlightLineHandler(int Line, Color% LineColor)
{
	UInt32 CaretPos = TextField->SelectionStart;

	int SelStart = TextField->GetFirstCharIndexFromLine(Line), SelEnd = TextField->GetFirstCharIndexFromLine(Line + 1);
	if (SelEnd == -1)
		SelEnd = TextField->Text->Length - 1;

	TextField->SelectionStart = SelStart;
	TextField->SelectionLength = SelEnd - SelStart;

	TextField->SelectionBackColor = LineColor;
	TextField->SelectionStart = CaretPos;
}

void ScriptDebuggerRTB::ToggleBreakPoint(int Line)
{
	if (GetIsLineBreakPoint(Line))
		Breakpoints->Remove(Line);
	else
		Breakpoints->Add(Line);

	UpdateLineHighlights();
}

bool ScriptDebuggerRTB::GetIsLineBreakPoint(int Line)
{
	bool Result = false;
	for each (UInt32 Itr in Breakpoints)
	{
		if (Itr == Line)
		{
			Result = true;
			break;
		}
	}
	return Result;
}