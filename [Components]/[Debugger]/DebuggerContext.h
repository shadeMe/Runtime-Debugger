#pragma once

#include "ScriptDebuggerRTB.h"
#include "[Common]\DebuggerMessagingInterface.h"

using namespace System::Threading;

public delegate void StandardErrorOutput(String^ Message);

public ref class DebuggerContext
{
	static ImageList^						VariableIconList = gcnew ImageList();

	ScriptDebuggerRTB^						TextViewer;
	SplitContainer^							MainSplitter;
	ToolStrip^								ComponentToolbar;
		ToolStripButton^						ToolbarShowLocals;
		ToolStripButton^						ToolbarShowGlobals;
	ListView^								Locals;
	ListView^								Globals;
		TextBox^								ValueEditBox;

	Script*									WorkingScript;
	ScriptEventList*						WorkingEventList;

	UInt16									CurrentOffset;
	UInt8									State;

	StandardErrorOutput^					ErrorOutput;

	void									ToolbarShowLocals_Click(Object^ Sender, EventArgs^ E);
	void									ToolbarShowGlobals_Click(Object^ Sender, EventArgs^ E);
	void									AutosList_DoubleClick(Object^ Sender, EventArgs^ E);
		void									ValueEditBox_LostFocus(Object^ Sender, EventArgs^ E);
		void									ValueEditBox_KeyDown(Object^ Sender, KeyEventArgs^ E);

	void									ParseEventListForEnumeration(Script* SourceScript, ScriptEventList* SourceEventList, ListView^% DestinationListView, ListViewGroup^ GlobalFormGroup);

	void									UpdateAutos();
public:
	DebuggerContext(Script* WorkingScript, ScriptEventList* WorkingEventList, StandardErrorOutput^ ErrorOutput, UInt16 CurrentOffset);

	Control^								GetContainer() { return MainSplitter; }
	String^									DescribeWorkingScript();

	void									UpdateExecutingOffset(UInt16 CurrentOffset);
	bool									GetIsOffsetBreakPoint(UInt32 Offset) { return TextViewer->GetIsOffsetBreakPoint(Offset); }

	UInt16									GetCurrentOffset() { return CurrentOffset; }

	UInt8									GetState();
	void									SetState(UInt8 State);

	const ScriptEventList*					GetEventList() { return WorkingEventList; }
	void									UpdateUI() { TextViewer->HighlightNewOffset(CurrentOffset), UpdateAutos(); }
};