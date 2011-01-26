#include "DebuggerBase.h"

DebuggerBase::DebuggerBase()
{
	MainSplitter = (gcnew SplitContainer());
	CallStackSplitter = (gcnew SplitContainer());
	DebuggerToolbar = (gcnew ToolStrip());
	ToolbarResumeDebugger = (gcnew ToolStripButton());
	ToolbarExecuteLine = (gcnew ToolStripButton());
	Console = (gcnew ListBox());
	CallStackList = (gcnew ListView());
	CallStackLabel = (gcnew ColumnHeader());
	MainSplitter->Panel1->SuspendLayout();
	MainSplitter->Panel2->SuspendLayout();
	MainSplitter->SuspendLayout();
	CallStackSplitter->Panel1->SuspendLayout();
	CallStackSplitter->SuspendLayout();
	DebuggerToolbar->SuspendLayout();
	DebuggerBox->SuspendLayout();

	MainSplitter->Dock = DockStyle::Fill;
	MainSplitter->Location = System::Drawing::Point(0, 0);
	MainSplitter->Name = L"MainSplitter";
	MainSplitter->Orientation = Orientation::Horizontal;

	MainSplitter->Panel1->Controls->Add(DebuggerToolbar);
	MainSplitter->Panel1->Controls->Add(CallStackSplitter);

	MainSplitter->Panel2->Controls->Add(Console);
	MainSplitter->Size = System::Drawing::Size(995, 532);
	MainSplitter->SplitterDistance = 408;
	MainSplitter->TabIndex = 0;

	CallStackSplitter->Dock = DockStyle::Fill;
	CallStackSplitter->Location = System::Drawing::Point(0, 0);
	CallStackSplitter->Name = L"CallStackSplitter";

	CallStackSplitter->Panel1->Controls->Add(CallStackList);
	CallStackSplitter->Size = System::Drawing::Size(995, 408);
	CallStackSplitter->SplitterDistance = 331;
	CallStackSplitter->TabIndex = 0;

	DebuggerToolbar->AllowItemReorder = true;
	DebuggerToolbar->AllowMerge = false;
	DebuggerToolbar->Dock = DockStyle::Bottom;
	DebuggerToolbar->GripStyle = ToolStripGripStyle::Hidden;
	DebuggerToolbar->Items->AddRange(gcnew cli::array< ToolStripItem^  >(2) {ToolbarExecuteLine, 
		ToolbarResumeDebugger});
	DebuggerToolbar->Location = System::Drawing::Point(0, 383);
	DebuggerToolbar->Name = L"DebuggerToolbar";
	DebuggerToolbar->RightToLeft = RightToLeft::Yes;
	DebuggerToolbar->Size = System::Drawing::Size(995, 25);
	DebuggerToolbar->TabIndex = 1;
	DebuggerToolbar->Text = L"Toolbar";

	ToolbarResumeDebugger->ImageTransparentColor = System::Drawing::Color::Magenta;
	ToolbarResumeDebugger->Name = L"ToolbarResumeDebugger";
	ToolbarResumeDebugger->Size = System::Drawing::Size(108, 22);
	ToolbarResumeDebugger->Text = L"Resume Debugger";
	ToolbarResumeDebugger->ToolTipText = L"Resume Debugger";
	ToolbarResumeDebugger->Click += gcnew EventHandler(this, &DebuggerBase::ToolbarResumeDebugger_Click);

	ToolbarExecuteLine->ImageTransparentColor = System::Drawing::Color::Magenta;
	ToolbarExecuteLine->Name = L"ToolbarExecuteLine";
	ToolbarExecuteLine->Size = System::Drawing::Size(76, 22);
	ToolbarExecuteLine->Text = L"Execute Line";
	ToolbarExecuteLine->Click += gcnew EventHandler(this, &DebuggerBase::ToolbarExecuteLine_Click);

	Console->Dock = System::Windows::Forms::DockStyle::Fill;
	Console->Font = (gcnew System::Drawing::Font(L"Consolas", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
		static_cast<System::Byte>(0)));
	Console->FormattingEnabled = true;
	Console->ItemHeight = 19;
	Console->Location = System::Drawing::Point(0, 0);
	Console->Name = L"Console";
	Console->Size = System::Drawing::Size(995, 118);
	Console->TabIndex = 0;

	CallStackList->Columns->AddRange(gcnew cli::array< ColumnHeader^  >(1) {CallStackLabel});
	CallStackList->Dock = DockStyle::Fill;
	CallStackList->Font = (gcnew System::Drawing::Font(L"Consolas", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
		static_cast<System::Byte>(0)));
	CallStackList->HeaderStyle = ColumnHeaderStyle::Nonclickable;
	CallStackList->HideSelection = false;
	CallStackList->Location = System::Drawing::Point(0, 0);
	CallStackList->MultiSelect = false;
	CallStackList->Name = L"CallStack";
	CallStackList->ShowGroups = false;
	CallStackList->Size = System::Drawing::Size(331, 408);
	CallStackList->TabIndex = 0;
	CallStackList->UseCompatibleStateImageBehavior = false;
	CallStackList->View = View::Details;

	CallStackLabel->Text = L"Call Stack";
	CallStackLabel->Width = 350;

	DebuggerBox->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
	DebuggerBox->AutoScaleMode = AutoScaleMode::Font;
	DebuggerBox->ClientSize = System::Drawing::Size(995, 532);
	DebuggerBox->ControlBox = false;
	DebuggerBox->Controls->Add(MainSplitter);
	DebuggerBox->Name = L"Interface";
	DebuggerBox->SizeGripStyle = SizeGripStyle::Show;
	DebuggerBox->StartPosition = FormStartPosition::CenterParent;
	DebuggerBox->Text = L"Script Debugger";

	MainSplitter->Panel1->ResumeLayout(false);
	MainSplitter->Panel1->PerformLayout();
	MainSplitter->Panel2->ResumeLayout(false);
	MainSplitter->Panel2->PerformLayout();
	MainSplitter->ResumeLayout(false);
	CallStackSplitter->Panel1->ResumeLayout(false);
	CallStackSplitter->ResumeLayout(false);
	DebuggerToolbar->ResumeLayout(false);
	DebuggerToolbar->PerformLayout();

	DebuggerBox->ResumeLayout(false);
	DebuggerBox->Hide();

	CallStack = gcnew Stack<DebuggerContext^>();
}

DebuggerBase^% DebuggerBase::GetSingleton()
{
	if (Singleton == nullptr)
		Singleton = gcnew DebuggerBase();

	return Singleton;
}

DebuggerContext^ DebuggerBase::GetExecutingContext()
{
	if (CallStack->Count > 0)
		return CallStack->Peek();
	else
		return nullptr;
}

void DebuggerBase::UpdateCallStackList()
{
	CallStackList->BeginUpdate();

	CallStackList->Items->Clear();
	for each (DebuggerContext^% Itr in CallStack)
	{
		CallStackList->Items->Add(Itr->DescribeWorkingScript());
	}

	CallStackList->EndUpdate();
}

void DebuggerBase::Initialize()
{
	DebuggerBox->Show(); 
}

void DebuggerBase::Deinitialize()
{
	Console->Items->Clear();
	DebuggerBox->Hide(); 
}

void DebuggerBase::DebugScript(Script* WorkingScript, ScriptEventList* EventList, UInt32 CurrentLine, UInt16 CurrentOffset)
{
	CallStack->Push(gcnew DebuggerContext(WorkingScript, EventList, gcnew StandardErrorOutput(this, &DebuggerBase::PrintToConsole), CurrentLine, CurrentOffset));
	UpdateCallStackList();

	DebuggerContext^% CurrentContext = GetExecutingContext();
	CallStackSplitter->Panel2->Controls->Add(CurrentContext->GetContainer());
}

UInt8 DebuggerBase::HandleScriptRunnerCallback(DebuggerMessage Message, UInt32* Data)
{
	DebuggerContext^ CurrentContext = GetExecutingContext();// ### pass offset during each message

	switch (Message)
	{
	case kDebuggerMessage_DebugBreakCalled:
		{
			UInt32 Line = Data[0];
			UInt32 Offset = Data[1];
			Script* WorkingScript = (Script*)Data[2];
			ScriptEventList* WorkingEventList = (ScriptEventList*)Data[3];

			if (GetIsInitialized() == 0)
			{
				Initialize();
				DebugScript(WorkingScript, WorkingEventList, Line, Offset);
			}

			if (GetExecutingContext()->GetEventList() != WorkingEventList)
			{
				MessageBox::Show("ASSERTION FAILURE!");
				Debugger::Break();
			}

			GetExecutingContext()->UpdateContext(Line, Offset);
			SetState(kDebuggerState_Break);		
			PrintToConsole("Encountered DebugBreak at line " + Line + ", offset " + Offset);
			break;
		}
	case kDebuggerMessage_NewInstance:
		{
			Script* WorkingScript = (Script*)Data[0];
			ScriptEventList* WorkingEventList = (ScriptEventList*)Data[1];

			DebugScript(WorkingScript, WorkingEventList, 0, 0);
			SetState(kDebuggerState_Break);
			PrintToConsole("New ScriptRunner instance started - Script " + WorkingScript->refID.ToString("x8"));
			break;
		}
	case kDebuggerMessage_NewLineExecutionStart:
		{
			UInt32 Line = Data[0];
			UInt32 Offset = Data[1];

			if (GetState() == kDebuggerState_DebugTillNextLine || CurrentContext->GetIsLineBreakPoint(Line))
			{
				CurrentContext->UpdateContext(Line, Offset);
				SetState(kDebuggerState_Break);
			}
			break;
		}
	case kDebuggerMessage_ReferenceEvaluationFailed:
		{
			UInt32 Line = Data[0];
			UInt32 Offset = Data[1];

			if (GetState() == kDebuggerState_DebugTillNextError)
			{
				CurrentContext->UpdateContext(Line, Offset);
				SetState(kDebuggerState_Break);
			}
			PrintToConsole("Reference evaluation failed at line " + Line + ", offset " + Offset.ToString("x8") + " - Current script blacklisted!");
			break;
		}
	case kDebuggerMessage_EncounteredReturnCommand:
		{
			UInt32 Line = Data[0];
			UInt32 Offset = Data[1];

			if (GetState() == kDebuggerState_DebugTillReturn)
			{
				CurrentContext->UpdateContext(Line, Offset);
				SetState(kDebuggerState_Break);
			}
			PrintToConsole("Encountered Return command");
			break;
		}
	case kDebuggerMessage_BlockHandlerCalled:
		{
			UInt32 Line = Data[0];
			UInt32 Offset = Data[1];

			const char* CommandName = (const char*)Data[2];
			if (GetState() == kDebuggerState_DebugTillNextBlock)
			{
				CurrentContext->UpdateContext(Line, Offset);
				SetState(kDebuggerState_Break);
			}
			PrintToConsole("Entering script block " + gcnew String(CommandName));		
			break;
		}
	case kDebuggerMessage_CommandHandlerCalled:
		{
			UInt32 Line = Data[0];
			UInt32 Offset = Data[1];

			const char* CommandName = (const char*)Data[2];
			if (GetState() == kDebuggerState_DebugTillNextCommand)
			{
				CurrentContext->UpdateContext(Line, Offset);
				SetState(kDebuggerState_Break);
			}
			PrintToConsole("Executing script command " + gcnew String(CommandName) + " at offset " + Offset.ToString("x8"));				
			break;
		}
	case kDebuggerMessage_LineExecutionFailed:
		{
			UInt32 Line = Data[0];
			UInt32 Offset = Data[1];

			if (GetState() == kDebuggerState_DebugTillNextError)
			{
				CurrentContext->UpdateContext(Line, Offset);
				SetState(kDebuggerState_Break);
			}
			PrintToConsole("Bytecode evaluation failed at line " + Line + ", offset " + Offset.ToString("x8") + "!");		
			break;
		}
	case kDebuggerMessage_ExecutionComplete:
		{
			UInt32 Result = Data[0];
			DebuggerContext^ ExecutedContext = CallStack->Pop();
			CallStackSplitter->Panel2->Controls->Remove(ExecutedContext->GetContainer());
			UpdateCallStackList();

			PrintToConsole("Execution of script " + CurrentContext->DescribeWorkingScript() + " completed with result " + Result);	
			if (CallStack->Count == 0)
			{
				Deinitialize();
				return kDebuggerState_Invalid;
			}

			break;
		}
	}
	return GetState();
}

void DebuggerBase::PrintToConsole(String^ Message)
{
	Console->Items->Add(Message);
	DebugPrint(Message);
}

void DebuggerBase::ToolbarResumeDebugger_Click(Object^ Sender, EventArgs^ E)
{
	GetExecutingContext()->SetState(kDebuggerState_Debugging);
}

void DebuggerBase::ToolbarExecuteLine_Click(Object^ Sender, EventArgs^ E)
{
	GetExecutingContext()->SetState(kDebuggerState_DebugTillNextLine);
}
