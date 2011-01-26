#pragma once

#include "DebuggerContext.h"

public ref class DebuggerBase
{
	static DebuggerBase^				Singleton;

	DebuggerBase();

	Form^								DebuggerBox;
		SplitContainer^						MainSplitter;
			SplitContainer^						CallStackSplitter;
				ToolStrip^							DebuggerToolbar;
					ToolStripButton^					ToolbarResumeDebugger;
					ToolStripButton^					ToolbarExecuteLine;
				ListView^						CallStackList;
					ColumnHeader^						CallStackLabel;
			ListBox^							Console;


	Stack<DebuggerContext^>^			CallStack;	

	void								ToolbarResumeDebugger_Click(Object^ Sender, EventArgs^ E);
	void								ToolbarExecuteLine_Click(Object^ Sender, EventArgs^ E);

	void								UpdateCallStackList();
	DebuggerContext^					GetExecutingContext();
	void								Initialize();
	void								Deinitialize();
public:
	static DebuggerBase^%				GetSingleton();

	bool								GetIsInitialized() { return DebuggerBox->Visible; }
	UInt8								GetState() { return GetExecutingContext()->GetState(); }
	void								SetState(UInt8 State) { GetExecutingContext()->SetState(State); }
	void								DebugScript(Script* WorkingScript, ScriptEventList* EventList, UInt32 CurrentLine, UInt16 CurrentOffset);
	UInt8								HandleScriptRunnerCallback(DebuggerMessage Message, UInt32* Data);
	void								PrintToConsole(String^ Message);
};

#define DEBUGGER						DebuggerBase::GetSingleton()