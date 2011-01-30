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
					ToolStripButton^					ToolbarQuitGame;
					ToolStripButton^					ToolbarResumeExecution;
					ToolStripButton^					ToolbarExecuteTillNextBreakpoint;
					ToolStripButton^					ToolbarExecuteTillNextLine;
					ToolStripButton^					ToolbarExecuteTillNextBlock;
					ToolStripButton^					ToolbarExecuteTillNextError;
					ToolStripButton^					ToolbarExecuteTillNextCommand;
					ToolStripButton^					ToolbarExecuteTillReturn;
					ToolStripButton^					ToolbarExecuteTillNextScript;
				ListView^						CallStackList;
					ColumnHeader^						CallStackLabel;
			ListView^							Console;


	Stack<DebuggerContext^>^			CallStack;	
	bool								Initialized;

	void								ToolbarQuitGame_Click(Object^ Sender, EventArgs^ E);
	void								ToolbarResumeExecution_Click(Object^ Sender, EventArgs^ E);
	void								ToolbarExecuteButtons_Click(Object^ Sender, EventArgs^ E);
	void								DebuggerBox_Cancel(Object^ Sender, FormClosingEventArgs^ E);
	void								CallStackList_ItemActivate(Object^ Sender, EventArgs^ E);

	void								SetupExecuteButton(ToolStripButton^ Button, String^ DisplayText, UInt8 State);

	void								UpdateCallStackList();
	DebuggerContext^					GetExecutingContext();
	void								Initialize();
	void								Deinitialize();

	void								Show();
	void								Hide() { DebuggerBox->Hide(); }
public:
	static DebuggerBase^%				GetSingleton();

	bool								GetIsInitialized() { return Initialized; }
	UInt8								GetState();
	void								SetState(UInt8 State);
	void								DebugScript(Script* WorkingScript, ScriptEventList* EventList, UInt32 CurrentLine, UInt16 CurrentOffset);
	UInt8								HandleScriptRunnerCallback(DebuggerMessage Message, UInt32* Data);
	void								PrintToConsole(String^ Message);
};

#define DEBUGGER						DebuggerBase::GetSingleton()