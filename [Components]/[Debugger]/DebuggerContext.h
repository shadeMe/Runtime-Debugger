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

	Script*									WorkingScript;
	ScriptEventList*						WorkingEventList;

	UInt16									CurrentOffset;
	UInt32									CurrentLine;

	ref class ContextState
	{
		UInt8								State;
	public:
		ContextState() : State(kDebuggerState_Invalid) {}

		UInt8								Get() { return State; }
		void								Set(UInt8 New) { State = New; }
	};

	ContextState^							State;

	StandardErrorOutput^					ErrorOutput;

	void									ToolbarShowLocals_Click(Object^ Sender, EventArgs^ E);
	void									ToolbarShowGlobals_Click(Object^ Sender, EventArgs^ E);

	void									ParseEventListForEnumeration(Script* SourceScript, ScriptEventList* SourceEventList, ListView^% DestinationListView, UInt32 GlobalFormID);

	void									UpdateAutos();
	void									UpdateCurrentLine(int Line) { return TextViewer->HighlightNewLine(Line); }
public:
	DebuggerContext(Script* WorkingScript, ScriptEventList* WorkingEventList, StandardErrorOutput^ ErrorOutput, UInt32 CurrentLine, UInt16 CurrentOffset);

	Control^								GetContainer() { return MainSplitter; }
	String^									DescribeWorkingScript();

	void									UpdateContext(UInt32 CurrentLine, UInt16 CurrentOffset);
	bool									GetIsLineBreakPoint(int Line) { return TextViewer->GetIsLineBreakPoint(Line); }

	UInt32									GetCurrentLine() { return CurrentLine; }
	UInt16									GetCurrentOffset() { return CurrentOffset; }

	UInt8									GetState();
	void									SetState(UInt8 State);

	const ScriptEventList*					GetEventList() { return WorkingEventList; }
};