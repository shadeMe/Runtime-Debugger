#pragma once

#include "NumberedRichTextBox.h"

public ref class ScriptDebuggerRTB : public OffsetRichTextBox
{
protected:
	Color							LineHighlightColor;
	Color							BreakpointColor;
	UInt32							CurrentOffset;

	List<UInt32>^					Breakpoints;

	void							UpdateLineHighlights(void);
	void							HighlightLineHandler(int Line, Color% LineColor);
	void							ToggleBreakPoint(UInt32 Offset);

	virtual void					LineField_MouseDown(Object^ Sender, MouseEventArgs^ E) override;
public:
	ScriptDebuggerRTB(UInt32 LinesToScroll, Font^ Font, Color ForegroundColor, Color BackgroundColor, Color HighlightColor, Color LineHighlightColor, Color BreakpointColor);

	int								GetLineFromOffset(UInt32 Offset);
	void							HighlightNewOffset(UInt32 Offset);
	bool							GetIsOffsetBreakPoint(UInt32 Offset);
};