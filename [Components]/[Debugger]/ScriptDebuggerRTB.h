#pragma once

#include "NumberedRichTextBox.h"

public ref class ScriptDebuggerRTB : public OffsetRichTextBox
{
protected:
	Color							LineHighlightColor;
	Color							BreakpointColor;
	UInt32							CurrentLine;

	List<UInt32>^					Breakpoints;

	void							UpdateLineHighlights(void);
	void							HighlightLineHandler(int Line, Color% LineColor);
	void							ToggleBreakPoint(int Line);

	virtual void					LineField_MouseDown(Object^ Sender, MouseEventArgs^ E) override;
public:
	ScriptDebuggerRTB(UInt32 LinesToScroll, Font^ Font, Color ForegroundColor, Color BackgroundColor, Color HighlightColor, Color LineHighlightColor, Color BreakpointColor);

	void							HighlightNewLine(int Line) { CurrentLine = Line, UpdateLineHighlights(); }
	bool							GetIsLineBreakPoint(int Line);
};